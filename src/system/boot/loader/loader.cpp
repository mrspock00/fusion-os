/*
 * Copyright 2003-2009, Axel Dörfler, axeld@pinc-software.de.
 * Distributed under the terms of the MIT License.
 */


#include "loader.h"
#include "elf.h"
#include "RootFileSystem.h"

#include <directories.h>
#include <OS.h>
#include <util/list.h>
#include <boot/stage2.h>
#include <boot/vfs.h>
#include <boot/platform.h>
#include <boot/stdio.h>
#include <boot/partitions.h>
#include <Catalog.h>
#include <Locale.h>
#include <unistd.h>
#include <string.h>
#include "zarar.h"

#ifndef BOOT_ARCH
#	error BOOT_ARCH has to be defined to differentiate the kernel per platform
#endif

#define SYSTEM_DIRECTORY_PREFIX	"system/"
#define KERNEL_IMAGE	"kernel_" BOOT_ARCH
#define KERNEL_PATH		SYSTEM_DIRECTORY_PREFIX KERNEL_IMAGE

#ifdef ALTERNATE_BOOT_ARCH
# define ALTERNATE_KERNEL_IMAGE	"kernel_" ALTERNATE_BOOT_ARCH
# define ALTERNATE_KERNEL_PATH	"system/" ALTERNATE_KERNEL_IMAGE
#endif


static const char* const kSystemDirectoryPrefix = SYSTEM_DIRECTORY_PREFIX;

static const char *sKernelPaths[][2] = {
	{ KERNEL_PATH, KERNEL_IMAGE },
#ifdef ALTERNATE_BOOT_ARCH
	{ ALTERNATE_KERNEL_PATH, ALTERNATE_KERNEL_IMAGE },
#endif
	{ NULL, NULL },
};

static const char *sAddonPaths[] = {
	kVolumeLocalSystemKernelAddonsDirectory,
	kVolumeLocalCommonNonpackagedKernelAddonsDirectory,
	kVolumeLocalCommonKernelAddonsDirectory,
	kVolumeLocalUserNonpackagedKernelAddonsDirectory,
	kVolumeLocalUserKernelAddonsDirectory,
	NULL
};


static int
open_maybe_packaged(BootVolume& volume, const char* path, int openMode)
{
	if (strncmp(path, kSystemDirectoryPrefix, strlen(kSystemDirectoryPrefix))
			== 0) {
		path += strlen(kSystemDirectoryPrefix);
		return open_from(volume.SystemDirectory(), path, openMode);
	}

	return open_from(volume.RootDirectory(), path, openMode);
}


static int
find_kernel(BootVolume& volume, const char** name = NULL)
{
	for (int32 i = 0; sKernelPaths[i][0] != NULL; i++) {
		int fd = open_maybe_packaged(volume, sKernelPaths[i][0], O_RDONLY);
		if (fd >= 0) {
			if (name)
				*name = sKernelPaths[i][1];

			return fd;
		}
	}

	return B_ENTRY_NOT_FOUND;
}



bool
is_bootable(Directory *volume)
{
	if (volume->IsEmpty())
		return false;

	BootVolume bootVolume;
	if (bootVolume.SetTo(volume) != B_OK)
		return false;

	// check for the existance of a kernel (for our platform)
	int fd = find_kernel(bootVolume);
	if (fd < 0)
		return false;

	close(fd);

	return true;
}

status_t
load_kernel(stage2_args* args, BootVolume& volume)
{

	const char *name;
	int fd = find_kernel(volume, &name);
	if (fd < B_OK)
		return fd;

	dprintf("load kernel %s...\n", name);
	// kernel yuklenir.
	elf_init();
	preloaded_image *image;
	status_t status = elf_load_image(fd, &image);
	
	close(fd);
	if (status < B_OK) {
		dprintf("loading kernel failed: %" B_PRIx32 "!\n", status);
		return status;
	}
	
	gKernelArgs.kernel_image = image;

	status = elf_relocate_image(gKernelArgs.kernel_image);
	if (status < B_OK) {
		dprintf("relocating kernel failed: %" B_PRIx32 "!\n", status);
		return status;
	}

	gKernelArgs.kernel_image->name = kernel_args_strdup(name);
	
	return B_OK;
}


static status_t
load_modules_from(BootVolume& volume, const char* path)
{
	// we don't have readdir() & co. (yet?)...

	int fd = open_maybe_packaged(volume, path, O_RDONLY);
	if (fd < B_OK)
		return fd;

	Directory *modules = (Directory *)get_node_from(fd);
	if (modules == NULL)
		return B_ENTRY_NOT_FOUND;

	void *cookie;
	if (modules->Open(&cookie, O_RDONLY) == B_OK) {
		char name[B_FILE_NAME_LENGTH];
		while (modules->GetNextEntry(cookie, name, sizeof(name)) == B_OK) {
			if (!strcmp(name, ".") || !strcmp(name, ".."))
				continue;

			status_t status = elf_load_image(modules, name);
			if (status != B_OK)
				dprintf("Could not load \"%s\" error %" B_PRIx32 "\n", name, status);
		}

		modules->Close(cookie);
	}

	return B_OK;
}


/*status_t
add_de_in_ram(bool load)
{	
	if(load == true)
	{	
				
	} else {
	dprintf("This is not loading or RAM is full");
	}

	
}	*/

struct Operation {
	bool boolean;
	bool stage1;
	bool stage2;
	bool stage3;
};


status_t
load_modules(stage2_args* args, BootVolume& volume)
{
	Operation as;
	// ToDo: this should be mostly replaced by a hardware oriented detection mechanism

	int32 i = 0;
	for (; sAddonPaths[i]; i++) {
		char path[B_FILE_NAME_LENGTH];
		snprintf(path, sizeof(path), "%s/boot", sAddonPaths[i]);

		if (load_modules_from(volume, path) != B_OK)
			failed++;
	}

	if (failed == i) {
		// couldn't load any boot modules
		// fall back to load all modules (currently needed by the boot floppy)
		const char *paths[] = { "bus_managers", "busses/ide", "busses/scsi",
			"generic", "partitioning_systems", "drivers/bin", NULL};

		for (int32 i = 0; paths[i]; i++) {
			char path[B_FILE_NAME_LENGTH];
			snprintf(path, sizeof(path), "%s/%s", sAddonPaths[0], paths[i]);
			load_modules_from(volume, path);
			as.stage1 = true;
		}
	}

	// and now load all partitioning and file system modules
	char path[B_FILE_NAME_LENGTH];
	snprintf(path, sizeof(path), "%s/%s", sAddonPaths[0], "file_systems");
	load_modules_from(volume, path);
	snprintf(path, sizeof(path), "%s/%s", sAddonPaths[0], "partitioning_systems");
	load_modules_from(volume, path);
	//as.stage1 = as.stage2; this code its code given an error
	return B_OK;
}
void op(bool iz_n)
{
	Operation as;
	const char *pth[] = {"true","false", NULL};
	for(int32 i = 0; pth[i]; i++)
	{
		if(strcmp(pth[i], "true") == 1)
		{
			iz_n = true;
			as.boolean = iz_n;

		} else {
			if(strcmp(pth[i], "false") == 1)
			{
				iz_n = false;
				as.boolean = iz_n;
			}
		}

	} 
}



void
Bootloader_s::BootLoader_stage3(BootVolume& volume, stage3_t* n, bool boolean)
{
	
	Operation as;
	const char *path_izin[] = {"izin", NULL};
	boolean = true;
	if(boolean == true)
	{
		for(int32 i = 0; path_izin[i]; i++) 
		{
			char paths_f[B_FILE_NAME_LENGTH];
			load_modules_from(volume, paths_f);
			boolean = true;	
			as.stage3 = true;
		}
	} else {
		//n = false;
		boolean = true;
	}
	op(true);
}
// not typedef struct this is only struct because not use a_t
