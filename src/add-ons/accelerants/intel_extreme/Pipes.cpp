/*
 * Copyright 2011-2015, Haiku, Inc. All Rights Reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Michael Lotz, mmlr@mlotz.ch
 *		Alexander von Gluck IV, kallisti5@unixzen.com
 */
#include "Pipes.h"

#include "accelerant.h"
#include "intel_extreme.h"
#include <KernelExport.h>

#include <stdlib.h>
#include <string.h>

#include <new>


#define TRACE_PIPE
#ifdef TRACE_PIPE
extern "C" void _sPrintf(const char* format, ...);
#	define TRACE(x...) _sPrintf("intel_extreme: " x)
#else
#	define TRACE(x...) ;
#endif

#define ERROR(x...) _sPrintf("intel_extreme: " x)
#define CALLED(x...) TRACE("CALLED %s\n", __PRETTY_FUNCTION__)


// PIPE: 6
// PLANE: 7


void
program_pipe_color_modes(uint32 colorMode)
{
	// All pipes get the same color mode
	write32(INTEL_DISPLAY_A_CONTROL, (read32(INTEL_DISPLAY_A_CONTROL)
			& ~(DISPLAY_CONTROL_COLOR_MASK | DISPLAY_CONTROL_GAMMA))
        | colorMode);
	write32(INTEL_DISPLAY_B_CONTROL, (read32(INTEL_DISPLAY_B_CONTROL)
			& ~(DISPLAY_CONTROL_COLOR_MASK | DISPLAY_CONTROL_GAMMA))
		| colorMode);
}


// #pragma mark - Pipe


Pipe::Pipe(pipe_index pipeIndex)
	:
	fHasTranscoder(false),
	fFDILink(NULL),
//	fPanelFitter(NULL),
	fPipeIndex(pipeIndex),
	fPipeOffset(0),
	fPlaneOffset(0)
{
	if (pipeIndex == INTEL_PIPE_B) {
		fPipeOffset = INTEL_DISPLAY_OFFSET;
		fPlaneOffset = INTEL_PLANE_OFFSET;
	}

	// IvyBridge: Analog + Digital Ports behind FDI (on northbridge)
	// Haswell: Only VGA behind FDI (on northbridge)
	// SkyLake: FDI gone. No more northbridge video.
	if (gInfo->shared_info->pch_info != INTEL_PCH_NONE) {
		TRACE("%s: Pipe %s routed through FDI\n", __func__,
			(pipeIndex == INTEL_PIPE_A) ? "A" : "B");

		fHasTranscoder = true;

		// Program FDILink if PCH
		fFDILink = new(std::nothrow) FDILink(pipeIndex);
	}

	TRACE("Pipe %s. Pipe Base: 0x%" B_PRIxADDR
		" Plane Base: 0x% " B_PRIxADDR "\n", (pipeIndex == INTEL_PIPE_A)
			? "A" : "B", fPipeOffset, fPlaneOffset);
}


Pipe::~Pipe()
{
}


bool
Pipe::IsEnabled()
{
	CALLED();

	return (read32(INTEL_DISPLAY_A_PIPE_CONTROL + fPipeOffset)
		& INTEL_PIPE_ENABLED) != 0;
}


void
Pipe::Configure(display_mode* mode)
{
	uint32 pipeControl = read32(INTEL_DISPLAY_A_PIPE_CONTROL + fPipeOffset);

	// TODO: Haswell+ dithering changes.
	if (gInfo->shared_info->device_type.Generation() >= 4) {
		pipeControl |= (INTEL_PIPE_DITHER_EN | INTEL_PIPE_DITHER_TYPE_SP);
		switch (mode->space) {
			case B_CMAP8:
			case B_RGB15_LITTLE:
			case B_RGB16_LITTLE:
				pipeControl |= INTEL_PIPE_6BPC;
				break;
			case B_RGB24_LITTLE:
				pipeControl |= INTEL_PIPE_8BPC;
				break;
			case B_RGB32_LITTLE:
			default:
				pipeControl |= INTEL_PIPE_10BPC;
				break;
		}
	}

	// TODO: CxSR downclocking?

	// TODO: Interlaced modes
	pipeControl |= INTEL_PIPE_PROGRESSIVE;

	write32(INTEL_DISPLAY_A_PIPE_CONTROL + fPipeOffset, pipeControl);
	read32(INTEL_DISPLAY_A_PIPE_CONTROL + fPipeOffset);
}


void
Pipe::_ConfigureTranscoder(display_mode* target)
{
	CALLED();

	TRACE("%s: fPipeOffset: 0x%" B_PRIx32"\n", __func__, fPipeOffset);

	// update timing (fPipeOffset bumps the DISPLAY_A to B when needed)
	write32(INTEL_TRANSCODER_A_HTOTAL + fPipeOffset,
		((uint32)(target->timing.h_total - 1) << 16)
		| ((uint32)target->timing.h_display - 1));
	write32(INTEL_TRANSCODER_A_HBLANK + fPipeOffset,
		((uint32)(target->timing.h_total - 1) << 16)
		| ((uint32)target->timing.h_display - 1));
	write32(INTEL_TRANSCODER_A_HSYNC + fPipeOffset,
		((uint32)(target->timing.h_sync_end - 1) << 16)
		| ((uint32)target->timing.h_sync_start - 1));

	write32(INTEL_TRANSCODER_A_VTOTAL + fPipeOffset,
		((uint32)(target->timing.v_total - 1) << 16)
		| ((uint32)target->timing.v_display - 1));
	write32(INTEL_TRANSCODER_A_VBLANK + fPipeOffset,
		((uint32)(target->timing.v_total - 1) << 16)
		| ((uint32)target->timing.v_display - 1));
	write32(INTEL_TRANSCODER_A_VSYNC + fPipeOffset,
		((uint32)(target->timing.v_sync_end - 1) << 16)
		| ((uint32)target->timing.v_sync_start - 1));

	#if 0
	// XXX: Is it ok to do these on non-digital?
	write32(INTEL_TRANSCODER_A_POS + fPipeOffset, 0);
	write32(INTEL_TRANSCODER_A_IMAGE_SIZE + fPipeOffset,
		((uint32)(target->virtual_width - 1) << 16)
			| ((uint32)target->virtual_height - 1));
	#endif
}


void
Pipe::ConfigureTimings(display_mode* target)
{
	CALLED();

	TRACE("%s: fPipeOffset: 0x%" B_PRIx32"\n", __func__, fPipeOffset);

	if (target == NULL) {
		ERROR("%s: Invalid display mode!\n", __func__);
		return;
	}

	/* If there is a transcoder, leave the display at its native resolution,
	 * and configure only the transcoder (panel fitting will match them
	 * together). */
	if (!fHasTranscoder)
	{
		// update timing (fPipeOffset bumps the DISPLAY_A to B when needed)
		write32(INTEL_DISPLAY_A_HTOTAL + fPipeOffset,
			((uint32)(target->timing.h_total - 1) << 16)
			| ((uint32)target->timing.h_display - 1));
		write32(INTEL_DISPLAY_A_HBLANK + fPipeOffset,
			((uint32)(target->timing.h_total - 1) << 16)
			| ((uint32)target->timing.h_display - 1));
		write32(INTEL_DISPLAY_A_HSYNC + fPipeOffset,
			((uint32)(target->timing.h_sync_end - 1) << 16)
			| ((uint32)target->timing.h_sync_start - 1));

		write32(INTEL_DISPLAY_A_VTOTAL + fPipeOffset,
			((uint32)(target->timing.v_total - 1) << 16)
			| ((uint32)target->timing.v_display - 1));
		write32(INTEL_DISPLAY_A_VBLANK + fPipeOffset,
			((uint32)(target->timing.v_total - 1) << 16)
			| ((uint32)target->timing.v_display - 1));
		write32(INTEL_DISPLAY_A_VSYNC + fPipeOffset,
			((uint32)(target->timing.v_sync_end - 1) << 16)
			| ((uint32)target->timing.v_sync_start - 1));
	}

	// XXX: Is it ok to do these on non-digital?

	write32(INTEL_DISPLAY_A_POS + fPipeOffset, 0);

	// Set the image size for both pipes, just in case.
	write32(INTEL_DISPLAY_A_IMAGE_SIZE,
		((uint32)(target->virtual_width - 1) << 16)
			| ((uint32)target->virtual_height - 1));
	write32(INTEL_DISPLAY_B_IMAGE_SIZE,
		((uint32)(target->virtual_width - 1) << 16)
			| ((uint32)target->virtual_height - 1));

	write32(INTEL_DISPLAY_A_PIPE_SIZE + fPipeOffset,
		((uint32)(target->timing.v_display - 1) << 16)
			| ((uint32)target->timing.h_display - 1));

	// This is useful for debugging: it sets the border to red, so you
	// can see what is border and what is porch (black area around the
	// sync)
	//write32(INTEL_DISPLAY_A_RED + fPipeOffset, 0x00FF0000);

	if (fHasTranscoder)
		_ConfigureTranscoder(target);
}


void
Pipe::ConfigureClocks(const pll_divisors& divisors, uint32 pixelClock,
	uint32 extraFlags)
{
	CALLED();

	addr_t pllDivisorA = INTEL_DISPLAY_A_PLL_DIVISOR_0;
	addr_t pllDivisorB = INTEL_DISPLAY_A_PLL_DIVISOR_1;
	addr_t pllControl = INTEL_DISPLAY_A_PLL;
	addr_t pllMD = INTEL_DISPLAY_A_PLL_MD;

	if (fPipeIndex == INTEL_PIPE_B) {
		pllDivisorA = INTEL_DISPLAY_B_PLL_DIVISOR_0;
		pllDivisorB = INTEL_DISPLAY_B_PLL_DIVISOR_1;
		pllControl = INTEL_DISPLAY_B_PLL;
		pllMD = INTEL_DISPLAY_B_PLL_MD;
	}

	float refFreq = gInfo->shared_info->pll_info.reference_frequency / 1000.0f;

	if (gInfo->shared_info->device_type.InGroup(INTEL_GROUP_96x)) {
		float adjusted = ((refFreq * divisors.m) / divisors.n) / divisors.p;
		uint32 pixelMultiply = uint32(adjusted / (pixelClock / 1000.0f));
		write32(pllMD, (0 << 24) | ((pixelMultiply - 1) << 8));
	}

	// XXX: For now we assume no LVDS downclocking and program the same divisor
	// value to both divisor 0 (standard) and 1 (reduced divisor)
	if (gInfo->shared_info->device_type.InGroup(INTEL_GROUP_PIN)) {
		write32(pllDivisorA, (((1 << divisors.n) << DISPLAY_PLL_N_DIVISOR_SHIFT)
				& DISPLAY_PLL_IGD_N_DIVISOR_MASK)
			| (((divisors.m2 - 2) << DISPLAY_PLL_M2_DIVISOR_SHIFT)
				& DISPLAY_PLL_IGD_M2_DIVISOR_MASK));
		write32(pllDivisorB, (((1 << divisors.n) << DISPLAY_PLL_N_DIVISOR_SHIFT)
				& DISPLAY_PLL_IGD_N_DIVISOR_MASK)
			| (((divisors.m2 - 2) << DISPLAY_PLL_M2_DIVISOR_SHIFT)
				& DISPLAY_PLL_IGD_M2_DIVISOR_MASK));
	} else {
		write32(pllDivisorA, (((divisors.n - 2) << DISPLAY_PLL_N_DIVISOR_SHIFT)
				& DISPLAY_PLL_N_DIVISOR_MASK)
			| (((divisors.m1 - 2) << DISPLAY_PLL_M1_DIVISOR_SHIFT)
				& DISPLAY_PLL_M1_DIVISOR_MASK)
			| (((divisors.m2 - 2) << DISPLAY_PLL_M2_DIVISOR_SHIFT)
				& DISPLAY_PLL_M2_DIVISOR_MASK));
		write32(pllDivisorB, (((divisors.n - 2) << DISPLAY_PLL_N_DIVISOR_SHIFT)
				& DISPLAY_PLL_N_DIVISOR_MASK)
			| (((divisors.m1 - 2) << DISPLAY_PLL_M1_DIVISOR_SHIFT)
				& DISPLAY_PLL_M1_DIVISOR_MASK)
			| (((divisors.m2 - 2) << DISPLAY_PLL_M2_DIVISOR_SHIFT)
				& DISPLAY_PLL_M2_DIVISOR_MASK));
	}

	uint32 pll = DISPLAY_PLL_ENABLED | DISPLAY_PLL_NO_VGA_CONTROL | extraFlags;

	if (gInfo->shared_info->device_type.Generation() >= 3) {
		// p1 divisor << 1 , 1-8
		if (gInfo->shared_info->device_type.InGroup(INTEL_GROUP_PIN)) {
			pll |= ((1 << (divisors.p1 - 1))
					<< DISPLAY_PLL_IGD_POST1_DIVISOR_SHIFT)
				& DISPLAY_PLL_IGD_POST1_DIVISOR_MASK;
		} else {
			pll |= ((1 << (divisors.p1 - 1))
					<< DISPLAY_PLL_POST1_DIVISOR_SHIFT)
				& DISPLAY_PLL_9xx_POST1_DIVISOR_MASK;
		//	pll |= ((divisors.p1 - 1) << DISPLAY_PLL_POST1_DIVISOR_SHIFT)
		//		& DISPLAY_PLL_9xx_POST1_DIVISOR_MASK;
		}

		if (divisors.p2 == 5 || divisors.p2 == 7)
			pll |= DISPLAY_PLL_DIVIDE_HIGH;

		if (gInfo->shared_info->device_type.InGroup(INTEL_GROUP_96x))
			pll |= 6 << DISPLAY_PLL_PULSE_PHASE_SHIFT;
	} else {
		if (divisors.p2 != 5 && divisors.p2 != 7)
			pll |= DISPLAY_PLL_DIVIDE_4X;

		pll |= DISPLAY_PLL_2X_CLOCK;

		// TODO: Is this supposed to be DISPLAY_PLL_IGD_POST1_DIVISOR_MASK??
		if (divisors.p1 > 2) {
			pll |= ((divisors.p1 - 2) << DISPLAY_PLL_POST1_DIVISOR_SHIFT)
				& DISPLAY_PLL_POST1_DIVISOR_MASK;
		} else
			pll |= DISPLAY_PLL_POST1_DIVIDE_2;
	}

	// Allow the PLL to warm up by masking its bit.
	write32(pllControl, pll & ~DISPLAY_PLL_NO_VGA_CONTROL);
	read32(pllControl);
	spin(150);
	write32(pllControl, pll);
	read32(pllControl);
	spin(150);
}


void
Pipe::Enable(bool enable)
{
	CALLED();

	addr_t pipeReg = INTEL_DISPLAY_A_PIPE_CONTROL + fPipeOffset;
	addr_t planeReg = INTEL_DISPLAY_A_CONTROL + fPlaneOffset;

	// Planes always have to operate on an enabled pipe

	if (enable) {
		write32(pipeReg, read32(pipeReg) | INTEL_PIPE_ENABLED);
		wait_for_vblank();
		write32(planeReg, read32(planeReg) | DISPLAY_CONTROL_ENABLED);
	} else {
		write32(planeReg, read32(planeReg) & ~DISPLAY_CONTROL_ENABLED);
		wait_for_vblank();
		write32(pipeReg, read32(pipeReg) & ~INTEL_PIPE_ENABLED);
	}

	read32(INTEL_DISPLAY_A_BASE);
		// flush the eventually cached PCI bus writes
}
