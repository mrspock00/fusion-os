/*
	Copyright (c) 2002, Thomas Kurschel
	

	Part of Radeon kernel driver
		
	Memory controller setup.
	
	The memory controller of the Radeon provides a universal mapping
	from addresses generated by the different DMA units within the
	graphics chip to addresses in local/PCI/AGP memory. Here, we
	set this mapping up.
	
	Further we initialize some bus controller registers here (where bus
	means non-local memory interface).
*/

#include "radeon_driver.h"
#include "buscntrl_regs.h"
#include "config_regs.h"
#include "mmio.h"
#include "memcntrl_regs.h"


// get last RAM address + 1, i.e. first unused physical address
static uint32 getTopOfRam()
{
	system_info info;

	// there is no function to really get this info;
	// as a hack, we ask for number of physical RAM pages and hope
	// they are contigous, starting at address 0	
	get_system_info( &info );
	
	return info.max_pages * 4096;
}

// graphics card addresses correspond to physical CPU addresses as much as possible
static void Radeon_SetupMCAddresses_Direct( device_info *di )
{
	shared_info *si = di->si;
	uint32 aper0 = INREG( di->regs, RADEON_CONFIG_APER_0_BASE );
	
	// bug in asics mean memory must be aligned to memory size... 
	if ( IS_DI_R300_VARIANT || di->asic == rt_rv280 ) {
		aper0 &= ~( di->local_mem_size - 1 );
	}
	
	// set address range of video memory;
	// use the same addresses the CPU sees
	si->memory[mt_local].virtual_addr_start = aper0;
	si->memory[mt_local].virtual_size = di->local_mem_size;

	// PCI GART has no corresponding CPU address space, so we must find an unused
	// one; we assume that the addresses directly after physical RAM are
	// not in use as the BIOS should have allocated address for PCI devices
	// starting with highest address possible. 
	// no problem in terms of alignment: it must be a multiple 4K only
	si->memory[mt_PCI].virtual_addr_start = (getTopOfRam() + 4095) & ~4095;
	si->memory[mt_PCI].virtual_size = ATI_MAX_PCIGART_PAGES * ATI_PCIGART_PAGE_SIZE;

	// similar problem with AGP: though there _is_ a corresponding CPU address space,
	// we don't know it (this would require finding the AGP bridge and
	// getting info from there, which would be a dangerous hack);
	// solution is to locate AGP aperture directly after PCI GART;
	// we define a 4 MB aperture to meet any possible alignment restrictions
	si->memory[mt_AGP].virtual_addr_start = 
		(si->memory[mt_PCI].virtual_addr_start + si->memory[mt_PCI].virtual_size
		+ 0x3fffff) & ~0x3fffff;
	si->memory[mt_AGP].virtual_size = 0x400000;

}


#if 0
// graphics card addresses are mapped in a way to restrict direct main memory access
static void Radeon_SetupMCAddresses_Safe( device_info *di )
{
	shared_info *si = di->si;

	// any address not covered by frame buffer, PCI GART or AGP aperture
	// leads to a direct memory access
	// -> this is dangerous, so we make sure entire address space is mapped
	
	// locate PCI GART at top of address space
	// warning about size: there are quite strong alignment restrictions,
	// so we better obey!
	si->memory[mt_PCI].virtual_size = ATI_MAX_PCIGART_PAGES * ATI_PCIGART_PAGE_SIZE;
	si->memory[mt_PCI].virtual_addr_start = 0 - si->memory[mt_PCI].virtual_size;	

	// let AGP range overlap with frame buffer to hide it;
	// according to spec, frame buffer should win but we better
	// choose an unused-area to avoid trouble
	// (specs don't talk about overlapping area, let's hope
	// the memory controller won't choke if we ever access it)
	si->memory[mt_AGP].virtual_size = 0x400000;
	si->memory[mt_AGP].virtual_addr_start = 
		si->memory[mt_PCI].virtual_addr_start - 
		si->memory[mt_AGP].virtual_size;

	// set address range of video memory
	// let it cover all remaining addresses; 
	// addresses are wrapped 
	si->memory[mt_local].virtual_addr_start = 0;
	si->memory[mt_local].virtual_size = 
		si->memory[mt_AGP].virtual_addr_start -
		si->memory[mt_local].virtual_addr_start;
}
#endif

// graphics cards addresses are mapped IGP compliantly
static void Radeon_SetupMCAddresses_IGP( device_info *di )
{
	shared_info *si = di->si;
	uint32 tom;
	
	// the frame buffer memory address range is read from TOM register
	// it located at end of physical RAM (at least it seems so)
	tom = INREG( di->regs, RADEON_NB_TOM );
	si->memory[mt_local].virtual_addr_start = (tom & 0xffff) << 16;
	si->memory[mt_local].virtual_size =
		(((tom >> 16) + 1) << 16) -
		si->memory[mt_local].virtual_addr_start;

	// after the frame buffer, physical memory should end and unused
	// physical addresses start - good location to put the PCI GART to
	si->memory[mt_PCI].virtual_addr_start = ((((tom >> 16) + 1) << 16) + 4095) & ~4095;
	si->memory[mt_PCI].virtual_size = ATI_MAX_PCIGART_PAGES * ATI_PCIGART_PAGE_SIZE;

	// locate AGP aperture after PCI GART
	si->memory[mt_AGP].virtual_addr_start = 
		(si->memory[mt_PCI].virtual_addr_start + 
		si->memory[mt_PCI].virtual_size + 0x3fffff) & ~0x3fffff;
	si->memory[mt_AGP].virtual_size = 0x400000;

}

void Radeon_InitMemController( device_info *di )
{
	vuint8 *regs = di->regs;
	shared_info *si = di->si;

	if( di->is_igp )
		Radeon_SetupMCAddresses_IGP( di );
	else
		Radeon_SetupMCAddresses_Direct/*Radeon_SetupMCAddresses_Safe*/( di );

	SHOW_INFO0( 3, "Graphics card address mapping:" );
	SHOW_INFO( 3, " local memory 0x%" B_PRIx32 "@0x%" B_PRIx32,
		si->memory[mt_local].virtual_size, si->memory[mt_local].virtual_addr_start );
	SHOW_INFO( 3, " PCI GART 0x%" B_PRIx32 "@0x%" B_PRIx32,
		si->memory[mt_PCI].virtual_size, si->memory[mt_PCI].virtual_addr_start );
	SHOW_INFO( 3, " disabled AGP GART 0x%" B_PRIx32 "@0x%" B_PRIx32,
		si->memory[mt_AGP].virtual_size, si->memory[mt_AGP].virtual_addr_start );
	
	//si->nonlocal_mem = di->DMABuffer.ptr;
	
	// Turn on PCI GART
	OUTREGP( regs, RADEON_AIC_CNTL, RADEON_PCIGART_TRANSLATE_EN, 
		~RADEON_PCIGART_TRANSLATE_EN );
		
	// set PCI GART page-table base address
	OUTREG( regs, RADEON_AIC_PT_BASE, di->pci_gart.GATT.phys );

	// set address range for PCI address translation
	// we must restrict range to the actually used GART size here!
	OUTREG( regs, RADEON_AIC_LO_ADDR, si->memory[mt_PCI].virtual_addr_start );
	OUTREG( regs, RADEON_AIC_HI_ADDR, si->memory[mt_PCI].virtual_addr_start + 
		si->memory[mt_PCI].virtual_size/*di->pci_gart.buffer.size*/ - 1 );

	// set AGP address range
	OUTREG( regs, RADEON_MC_AGP_LOCATION, 0xffffffc0 /* EK magic numbers from X.org
		(si->memory[mt_AGP].virtual_addr_start >> 16) |
		((si->memory[mt_AGP].virtual_addr_start + si->memory[mt_AGP].virtual_size - 1) & 0xffff0000 )*/);

	// disable AGP
	OUTREG( regs, RADEON_AGP_COMMAND, 0 );
	
	// set address range of video memory
	// (lower word = begin >> 16
	//  upper word = end >> 16)
	OUTREG( regs, RADEON_MC_FB_LOCATION,
		((si->memory[mt_local].virtual_addr_start + si->memory[mt_local].virtual_size - 1) & 0xffff0000) |
		 (si->memory[mt_local].virtual_addr_start >> 16) );
		 
	// base address of CRTC and others must be same as frame buffer address
	// (we could specify any address too, but local memory is of course first choice)
	OUTREG( regs, RADEON_DISPLAY_BASE_ADDRESS, si->memory[mt_local].virtual_addr_start );
	OUTREG( regs, RADEON_CRTC2_DISPLAY_BASE_ADDRESS, si->memory[mt_local].virtual_addr_start );
	OUTREG( regs, RADEON_OV0_BASE_ADDRESS, si->memory[mt_local].virtual_addr_start );
		 
	// fix some bus controller setting
	// I reckon this takes care of discarding data unnecessarily read 
	// during a burst; let's hope this will fix the nasty CP crashing problem
	// EK this seems unecessary. OUTREGP( regs, RADEON_BUS_CNTL, RADEON_BUS_RD_DISCARD_EN, ~RADEON_BUS_RD_DISCARD_EN );
	
//	SHOW_FLOW0( 3, "done" );
}
