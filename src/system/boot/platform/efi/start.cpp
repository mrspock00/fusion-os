/*
 * Copyright 2014-2016 Haiku, Inc. All rights reserved.
 * Copyright 2013-2014, Fredrik Holmqvist, fredrik.holmqvist@gmail.com.
 * Copyright 2014, Henry Harrington, henry.harrington@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */


#include <string.h>

#include <KernelExport.h>

#include <arch/cpu.h>
#include <boot/kernel_args.h>
#include <boot/platform.h>
#include <boot/stage2.h>
#include <boot/stdio.h>
#include <kernel.h>

#include "arch_mmu.h"
#include "acpi.h"
#include "console.h"
#include "efi_platform.h"
#include "cpu.h"
#include "mmu.h"
#include "serial.h"
#include "smp.h"
#include "timer.h"


extern void (*__ctor_list)(void);
extern void (*__ctor_end)(void);


const efi_system_table		*kSystemTable;
const efi_boot_services		*kBootServices;
const efi_runtime_services	*kRuntimeServices;
efi_handle kImage;


static uint32 sBootOptions;
static uint64 gLongKernelEntry;


extern "C" int main(stage2_args *args);
extern "C" void _start(void);
extern "C" void efi_enter_kernel(uint64 pml4, uint64 entry_point, uint64 stack);


static void
call_ctors(void)
{
	void (**f)(void);

	for (f = &__ctor_list; f < &__ctor_end; f++)
		(**f)();
}


extern "C" uint32
platform_boot_options()
{
	return sBootOptions;
}


static void
convert_preloaded_image(preloaded_elf64_image* image)
{
	fix_address(image->next);
	fix_address(image->name);
	fix_address(image->debug_string_table);
	fix_address(image->syms);
	fix_address(image->rel);
	fix_address(image->rela);
	fix_address(image->pltrel);
	fix_address(image->debug_symbols);
}


/*!	Convert all addresses in kernel_args to 64-bit addresses. */
static void
convert_kernel_args()
{
	fix_address(gKernelArgs.boot_volume);
	fix_address(gKernelArgs.vesa_modes);
	fix_address(gKernelArgs.edid_info);
	fix_address(gKernelArgs.debug_output);
	fix_address(gKernelArgs.boot_splash);
	#if defined(__x86_64__) || defined(__x86__)
	fix_address(gKernelArgs.arch_args.apic);
	fix_address(gKernelArgs.arch_args.hpet);
	#endif

	convert_preloaded_image(static_cast<preloaded_elf64_image*>(
		gKernelArgs.kernel_image.Pointer()));
	fix_address(gKernelArgs.kernel_image);

	// Iterate over the preloaded images. Must save the next address before
	// converting, as the next pointer will be converted.
	preloaded_image* image = gKernelArgs.preloaded_images;
	fix_address(gKernelArgs.preloaded_images);
	while (image != NULL) {
		preloaded_image* next = image->next;
		convert_preloaded_image(static_cast<preloaded_elf64_image*>(image));
		image = next;
	}

	// Fix driver settings files.
	driver_settings_file* file = gKernelArgs.driver_settings;
	fix_address(gKernelArgs.driver_settings);
	while (file != NULL) {
		driver_settings_file* next = file->next;
		fix_address(file->next);
		fix_address(file->buffer);
		file = next;
	}
}


extern "C" void
platform_start_kernel(void)
{
	if (gKernelArgs.kernel_image->elf_class != ELFCLASS64)
		panic("32-bit kernels not supported with EFI");

	smp_init_other_cpus();

	preloaded_elf64_image *image = static_cast<preloaded_elf64_image *>(
		gKernelArgs.kernel_image.Pointer());

	arch_mmu_init();
	convert_kernel_args();

	// Save the kernel entry point address.
	gLongKernelEntry = image->elf_header.e_entry;
	dprintf("kernel entry at %#lx\n", gLongKernelEntry);

	// map in a kernel stack
	void *stack_address = NULL;
	if (platform_allocate_region(&stack_address,
		KERNEL_STACK_SIZE + KERNEL_STACK_GUARD_PAGES * B_PAGE_SIZE, 0, false)
		!= B_OK) {
		panic("Unabled to allocate a stack");
	}
	gKernelArgs.cpu_kstack[0].start = fix_address((uint64_t)stack_address);
	gKernelArgs.cpu_kstack[0].size = KERNEL_STACK_SIZE
		+ KERNEL_STACK_GUARD_PAGES * B_PAGE_SIZE;
	dprintf("Kernel stack at %#lx\n", gKernelArgs.cpu_kstack[0].start);

	// Prepare to exit EFI boot services.
	// Read the memory map.
	// First call is to determine the buffer size.
	size_t memory_map_size = 0;
	efi_memory_descriptor dummy;
	efi_memory_descriptor *memory_map;
	size_t map_key;
	size_t descriptor_size;
	uint32_t descriptor_version;
	if (kBootServices->GetMemoryMap(&memory_map_size, &dummy, &map_key,
		&descriptor_size, &descriptor_version) != EFI_BUFFER_TOO_SMALL) {
		panic("Unable to determine size of system memory map");
	}

	// Allocate a buffer twice as large as needed just in case it gets bigger between
	// calls to ExitBootServices.
	size_t actual_memory_map_size = memory_map_size * 2;
	memory_map
		= (efi_memory_descriptor *)kernel_args_malloc(actual_memory_map_size);

	if (memory_map == NULL)
		panic("Unable to allocate memory map.");

	// Read (and print) the memory map.
	memory_map_size = actual_memory_map_size;
	if (kBootServices->GetMemoryMap(&memory_map_size, memory_map, &map_key,
		&descriptor_size, &descriptor_version) != EFI_SUCCESS) {
		panic("Unable to fetch system memory map.");
	}

	addr_t addr = (addr_t)memory_map;
	dprintf("System provided memory map:\n");
	for (size_t i = 0; i < memory_map_size / descriptor_size; ++i) {
		efi_memory_descriptor *entry
			= (efi_memory_descriptor *)(addr + i * descriptor_size);
		dprintf("  %#lx-%#lx  %#lx %#x %#lx\n", entry->PhysicalStart,
			entry->PhysicalStart + entry->NumberOfPages * 4096,
			entry->VirtualStart, entry->Type, entry->Attribute);
	}

	// Generate page tables for use after ExitBootServices.
	uint64_t final_pml4 = mmu_generate_post_efi_page_tables(memory_map_size,
		memory_map, descriptor_size, descriptor_version);
	dprintf("Final PML4 at %#lx\n", final_pml4);

	// Attempt to fetch the memory map and exit boot services.
	// This needs to be done in a loop, as ExitBootServices can change the
	// memory map.
	// Even better: Only GetMemoryMap and ExitBootServices can be called after
	// the first call to ExitBootServices, as the firmware is permitted to
	// partially exit. This is why twice as much space was allocated for the
	// memory map, as it's impossible to allocate more now.
	// A changing memory map shouldn't affect the generated page tables, as
	// they only needed to know about the maximum address, not any specific
	// entry.
	dprintf("Calling ExitBootServices. So long, EFI!\n");
	while (true) {
		if (kBootServices->ExitBootServices(kImage, map_key) == EFI_SUCCESS) {
			// The console was provided by boot services, disable it.
			stdout = NULL;
			stderr = NULL;
			// Also switch to legacy serial output (may not work on all systems)
			serial_switch_to_legacy();
			dprintf("Switched to legacy serial output\n");
			break;
		}

		memory_map_size = actual_memory_map_size;
		if (kBootServices->GetMemoryMap(&memory_map_size, memory_map, &map_key,
			&descriptor_size, &descriptor_version) != EFI_SUCCESS) {
			panic("Unable to fetch system memory map.");
		}
	}

	// Update EFI, generate final kernel physical memory map, etc.
	mmu_post_efi_setup(memory_map_size, memory_map,
		descriptor_size, descriptor_version);

	smp_boot_other_cpus(final_pml4, gLongKernelEntry);

	// Enter the kernel!
	efi_enter_kernel(final_pml4, gLongKernelEntry,
		gKernelArgs.cpu_kstack[0].start + gKernelArgs.cpu_kstack[0].size);

	panic("Shouldn't get here");
}


extern "C" void
platform_exit(void)
{
	return;
}


/**
 * efi_main - The entry point for the EFI application
 * @image: firmware-allocated handle that identifies the image
 * @systemTable: EFI system table
 */
extern "C" efi_status
efi_main(efi_handle image, efi_system_table *systemTable)
{
	stage2_args args;

	memset(&args, 0, sizeof(stage2_args));

	kImage = image;
	kSystemTable = systemTable;
	kBootServices = systemTable->BootServices;
	kRuntimeServices = systemTable->RuntimeServices;

	call_ctors();

	console_init();
	serial_init();
	serial_enable();

	sBootOptions = console_check_boot_keys();

	// disable apm in case we ever load a 32-bit kernel...
	gKernelArgs.platform_args.apm.version = 0;

	gKernelArgs.num_cpus = 1;
	#if defined(__x86_64__) || defined(__x86__)
	gKernelArgs.arch_args.hpet_phys = 0;
	gKernelArgs.arch_args.hpet = NULL;
	#endif

	cpu_init();
	acpi_init();
	timer_init();
	smp_init();

	main(&args);

	return EFI_SUCCESS;
}
