/*
 * Copyright 2006-2018, Haiku, Inc. All Rights Reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Axel Dörfler, axeld@pinc-software.de
 *		Alexander von Gluck IV, kallisti5@unixzen.com
 *		Adrien Destugues, pulkomandy@pulkomandy.tk
 */


#include "pll.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

#include <Debug.h>

#include <create_display_modes.h>
#include <ddc.h>
#include <edid.h>
#include <validate_display_mode.h>

#include "accelerant_protos.h"
#include "accelerant.h"
#include "utility.h"


#undef TRACE
#define TRACE_MODE
#ifdef TRACE_MODE
#	define TRACE(x...) _sPrintf("intel_extreme: " x)
#else
#	define TRACE(x...)
#endif

#define ERROR(x...) _sPrintf("intel_extreme: " x)
#define CALLED(x...) TRACE("CALLED %s\n", __PRETTY_FUNCTION__)


// PLL limits, taken from programming manual when available, and from Linux KMS
// drivers otherwise. However, note that we use the values of N+2, M1+2 and
// M2+2 here, the - 2 being applied when we write the values to the registers.

static pll_limits kLimits85x = {
	// p, p1, p2,  n,   m, m1, m2
	{  4,  2,  2,  5,  96, 20,  8},
	{128, 33,  4, 18, 140, 28, 18},
	165000, 930000, 1400000
};

// TODO according to the docs, the limits for 9xx anf G45 should be the same.
// For Iron Lake, a new set of timings is introduced along with the FDI system,
// and carried on to later cards with just one further change (to the P2 cutoff
// frequency) in Sandy Bridge.
//
// So, it makes no sense to have separate limits and algorithm for 9xx and G45.

static pll_limits kLimits9xxSdvo = {
	// p, p1, p2,  n,   m, m1, m2
	{  5,  1,  5,  5,  70, 12,  7},	// min
	{ 80,  8, 10, 10, 120, 22, 11},	// max
	200000, 1400000, 2800000
};

static pll_limits kLimits9xxLvds = {
	// p, p1, p2,  n,   m, m1, m2
	{  7,  1,  7,  1,  70,  8,  3},	// min
	{ 98,  8, 14,  6, 120, 18,  7},	// max
	112000, 1400000, 2800000
};

// Limits for G45 cards taken from g45_vlo3_register_0_0_0.pdf, page 46
// Note that n here is actually n+2, but m1 and m2 are as in the datasheet.

static pll_limits kLimitsG4xSdvo = {
	// p, p1, p2,  n,   m, m1, m2
	{  5,  1,  5,  5,  70, 12,  7},	// min
	{ 80,  8, 10, 10, 120, 22, 11},	// max
	270000, 1400000, 2800000
};

#if 0
static pll_limits kLimitsG4xHdmi = {
	// p, p1, p2,  n,   m, m1, m2
	{  5,  1,  5,  5,  70, 12,  7},	// min
	{ 80,  8, 10, 10, 120, 22, 11},	// max
	165000, 1400000, 2800000
};
#endif

static pll_limits kLimitsG4xLvdsSingle = {
	// p, p1, p2,  n,   m, m1, m2
	{  7,  1, 14,  5,  70, 12,  7},	// min
	{ 98,  8, 14, 10, 120, 22, 11},	// max
	0, 1400000, 2800000
};

static pll_limits kLimitsG4xLvdsDual = {
	// p, p1, p2,  n,   m, m1, m2
	{ 14,  2,  7,  5,  70, 12,  7},	// min
	{ 42,  6,  7, 10, 120, 22, 11},	// max
	0, 1400000, 2800000
};

static pll_limits kLimitsIlkDac = {
	// p, p1, p2, n,   m, m1, m2
	{  5,  1,  5, 3,  79, 14,  7}, // min
	{ 80,  8, 10, 8, 118, 24, 11}, // max
	225000, 1760000, 3510000
};

static pll_limits kLimitsIlkLvdsSingle = {
	// p, p1, p2, n,   m, m1, m2
	{ 28,  2, 14, 3,  79, 14,  7}, // min
	{112,  8, 14, 8, 118, 24, 11}, // max
	225000, 1760000, 3510000
};

static pll_limits kLimitsIlkLvdsDual = {
	// p, p1, p2, n,   m, m1, m2
	{ 14,  2,  7, 3,  79, 14,  7}, // min
	{ 56,  8,  7, 8, 127, 24, 11}, // max
	225000, 1760000, 3510000
};

// 100Mhz RefClock
static pll_limits kLimitsIlkLvdsSingle100 = {
	// p, p1, p2, n,   m, m1, m2
	{ 28,  2, 14, 3,  79, 14,  7}, // min
	{112,  8, 14, 8, 126, 24, 11}, // max
	225000, 1760000, 3510000
};

static pll_limits kLimitsIlkLvdsDual100 = {
	// p, p1, p2, n,   m, m1, m2
	{ 14,  2,  7, 3,  79, 14,  7}, // min
	{ 42,  6,  7, 8, 126, 24, 11}, // max
	225000, 1760000, 3510000
};

// TODO From haswell onwards, a completely different PLL design is used
// (intel_gfx-prm-osrc-hsw-display_0.pdf, page 268 for VGA). It uses a "virtual
// root frequency" and one just has to set a single divider (integer and
// fractional parts), so it makes no sense to reuse the same code and limit
// structures there.
//
// For other display connections, the clock is handled differently, as there is
// no need for a precise timing to send things in sync with the display.
#if 0
static pll_limits kLimitsChv = {
	// p, p1, p2, n,   m, m1, m2
	{  0,  2,  1, 1,  79, 2,   24 << 22}, // min
	{  0,  4, 14, 1, 127, 2,  175 << 22}, // max
	0, 4800000, 6480000
};

static pll_limits kLimitsVlv = {
	// p, p1, p2, n,   m, m1, m2
	{  0,  2,  2, 1,  79, 2,   11},	// min
	{  0,  3, 20, 7, 127, 3,  156},	// max
	0, 4000000, 6000000
};

static pll_limits kLimitsBxt = {
	// p, p1, p2, n,  m, m1, m2
	{  0,  2,  1, 1,  0,  2,   2 << 22}, // min
	{  0,  4, 20, 1,  0,  2, 255 << 22}, // max
	0, 4800000, 6700000
};
#endif

static pll_limits kLimitsPinSdvo = {
	// p, p1, p2, n,   m, m1,  m2
	{  5,  1,  5, 3,   2,  0,   0},	// min
	{ 80,  8, 10, 6, 256,  0, 254},	// max
	200000, 1700000, 3500000
};

static pll_limits kLimitsPinLvds = {
	// p, p1, p2, n,   m, m1,  m2
	{  7,  1, 14, 3,   2,  0,   0},	// min
	{112,  8, 14, 6, 256,  0, 254},	// max
	112000, 1700000, 3500000
};


static bool
lvds_dual_link(display_mode* current)
{
	float requestedPixelClock = current->timing.pixel_clock / 1000.0f;
	if (requestedPixelClock > 112.999)
		return true;

	// TODO: Force dual link on MacBookPro6,2  MacBookPro8,2  MacBookPro9,1

	return ((read32(INTEL_DIGITAL_LVDS_PORT) & LVDS_CLKB_POWER_MASK)
		== LVDS_CLKB_POWER_UP);
}


bool
valid_pll_divisors(pll_divisors* divisors, pll_limits* limits)
{
	pll_info &info = gInfo->shared_info->pll_info;
	uint32 vco = info.reference_frequency * divisors->m / divisors->n;
	uint32 frequency = vco / divisors->p;

	if (divisors->p < limits->min.p || divisors->p > limits->max.p
		|| divisors->m < limits->min.m || divisors->m > limits->max.m
		|| vco < limits->min_vco || vco > limits->max_vco
		|| frequency < info.min_frequency || frequency > info.max_frequency)
		return false;

	return true;
}


static void
compute_pll_p2(display_mode* current, pll_divisors* divisors,
	pll_limits* limits, bool isLVDS)
{
	if (isLVDS) {
		if (lvds_dual_link(current)) {
			// fast DAC timing via 2 channels (dual link LVDS)
			divisors->p2 = limits->min.p2;
		} else {
			// slow DAC timing
			divisors->p2 = limits->max.p2;
		}
	} else {
		if (current->timing.pixel_clock < limits->dot_limit) {
			// slow DAC timing
			divisors->p2 = limits->max.p2;
		} else {
			// fast DAC timing
			divisors->p2 = limits->min.p2;
		}
	}
}


// TODO we can simplify this computation, with the way the dividers are set, we
// know that all values in the valid range for M are reachable. M1 allows to
// generate any multiple of 5 in the range and M2 allows to reach the 4 next
// values. Therefore, we don't need to loop over the range of values for M1 and
// M2 separately, we could instead just loop over possible values for M.
// For this to work, the logic of this function must be reversed: for a given M,
// it should give the resulting M1 and M2 values for programming the registers.
static uint32
compute_pll_m(pll_divisors* divisors)
{
	if (gInfo->shared_info->device_type.InGroup(INTEL_GROUP_CHV)
		|| gInfo->shared_info->device_type.InGroup(INTEL_GROUP_VLV)) {
		return divisors->m1 * divisors->m2;
	}

	// Pineview, m1 is reserved
	if (gInfo->shared_info->device_type.InGroup(INTEL_GROUP_PIN))
		return divisors->m2;

	return 5 * divisors->m1 + divisors->m2;
}


static uint32
compute_pll_p(pll_divisors* divisors)
{
	return divisors->p1 * divisors->p2;
}


static void
compute_dpll_g4x(display_mode* current, pll_divisors* divisors, bool isLVDS)
{
	float requestedPixelClock = current->timing.pixel_clock / 1000.0f;
	float referenceClock
		= gInfo->shared_info->pll_info.reference_frequency / 1000.0f;

	TRACE("%s: required MHz: %g, reference clock: %g\n", __func__,
		requestedPixelClock, referenceClock);

	pll_limits limits;
	if (gInfo->shared_info->device_type.InGroup(INTEL_GROUP_G4x)) {
		// TODO: Pass port type via video_configuration
		if (isLVDS) {
			if (lvds_dual_link(current))
				memcpy(&limits, &kLimitsG4xLvdsDual, sizeof(pll_limits));
			else
				memcpy(&limits, &kLimitsG4xLvdsSingle, sizeof(pll_limits));
		//} else if (type == INTEL_PORT_TYPE_HDMI) {
		//	memcpy(&limits, &kLimitsG4xHdmi, sizeof(pll_limits));
		} else
			memcpy(&limits, &kLimitsG4xSdvo, sizeof(pll_limits));
	} else {
		// There must be a PCH, so this is ivy bridge or later
		if (isLVDS) {
			if (lvds_dual_link(current)) {
				if (referenceClock == 100.0)
					memcpy(&limits, &kLimitsIlkLvdsDual100, sizeof(pll_limits));
				else
					memcpy(&limits, &kLimitsIlkLvdsDual, sizeof(pll_limits));
			} else {
				if (referenceClock == 100.0) {
					memcpy(&limits, &kLimitsIlkLvdsSingle100,
						sizeof(pll_limits));
				} else {
					memcpy(&limits, &kLimitsIlkLvdsSingle, sizeof(pll_limits));
				}
			}
		} else {
			memcpy(&limits, &kLimitsIlkDac, sizeof(pll_limits));
		}
	}

	compute_pll_p2(current, divisors, &limits, isLVDS);

	TRACE("PLL limits, min: p %" B_PRId32 " (p1 %" B_PRId32 ", "
		"p2 %" B_PRId32 "), n %" B_PRId32 ", m %" B_PRId32 " "
		"(m1 %" B_PRId32 ", m2 %" B_PRId32 ")\n", limits.min.p,
		limits.min.p1, limits.min.p2, limits.min.n, limits.min.m,
		limits.min.m1, limits.min.m2);
	TRACE("PLL limits, max: p %" B_PRId32 " (p1 %" B_PRId32 ", "
		"p2 %" B_PRId32 "), n %" B_PRId32 ", m %" B_PRId32 " "
		"(m1 %" B_PRId32 ", m2 %" B_PRId32 ")\n", limits.max.p,
		limits.max.p1, limits.max.p2, limits.max.n, limits.max.m,
		limits.max.m1, limits.max.m2);

	float best = requestedPixelClock;
	pll_divisors bestDivisors;

	for (divisors->n = limits.min.n; divisors->n <= limits.max.n;
			divisors->n++) {
		for (divisors->m1 = limits.max.m1; divisors->m1 >= limits.min.m1;
				divisors->m1--) {
			for (divisors->m2 = limits.max.m2; divisors->m2 >= limits.min.m2;
					divisors->m2--) {
				for (divisors->p1 = limits.max.p1;
						divisors->p1 >= limits.min.p1; divisors->p1--) {
					divisors->m = compute_pll_m(divisors);
					divisors->p = compute_pll_p(divisors);

					if (!valid_pll_divisors(divisors, &limits))
						continue;

					float error = fabs(requestedPixelClock
						- ((referenceClock * divisors->m) / divisors->n)
						/ divisors->p);
					if (error < best) {
						best = error;
						bestDivisors = *divisors;

						if (error == 0)
							break;
					}
				}
			}
		}
	}
	*divisors = bestDivisors;
	TRACE("%s: best MHz: %g (error: %g)\n", __func__,
		((referenceClock * divisors->m) / divisors->n) / divisors->p,
		best);
}


static void
compute_dpll_9xx(display_mode* current, pll_divisors* divisors, bool isLVDS)
{
	float requestedPixelClock = current->timing.pixel_clock / 1000.0f;
	float referenceClock
		= gInfo->shared_info->pll_info.reference_frequency / 1000.0f;

	TRACE("%s: required MHz: %g\n", __func__, requestedPixelClock);

	pll_limits limits;
	if (gInfo->shared_info->device_type.InGroup(INTEL_GROUP_PIN)) {
		if (isLVDS)
			memcpy(&limits, &kLimitsPinLvds, sizeof(pll_limits));
		else
			memcpy(&limits, &kLimitsPinSdvo, sizeof(pll_limits));
	} else if (gInfo->shared_info->device_type.InGroup(INTEL_GROUP_85x)) {
		memcpy(&limits, &kLimits85x, sizeof(pll_limits));
	} else {
		if (isLVDS)
			memcpy(&limits, &kLimits9xxLvds, sizeof(pll_limits));
		else
			memcpy(&limits, &kLimits9xxSdvo, sizeof(pll_limits));
	}

	compute_pll_p2(current, divisors, &limits, isLVDS);

	TRACE("PLL limits, min: p %" B_PRId32 " (p1 %" B_PRId32 ", "
		"p2 %" B_PRId32 "), n %" B_PRId32 ", m %" B_PRId32 " "
		"(m1 %" B_PRId32 ", m2 %" B_PRId32 ")\n", limits.min.p,
		limits.min.p1, limits.min.p2, limits.min.n, limits.min.m,
		limits.min.m1, limits.min.m2);
	TRACE("PLL limits, max: p %" B_PRId32 " (p1 %" B_PRId32 ", "
		"p2 %" B_PRId32 "), n %" B_PRId32 ", m %" B_PRId32 " "
		"(m1 %" B_PRId32 ", m2 %" B_PRId32 ")\n", limits.max.p,
		limits.max.p1, limits.max.p2, limits.max.n, limits.max.m,
		limits.max.m1, limits.max.m2);

	bool is_pine = gInfo->shared_info->device_type.InGroup(INTEL_GROUP_PIN);

	float best = requestedPixelClock;
	pll_divisors bestDivisors;
	memset(&bestDivisors, 0, sizeof(bestDivisors));

	for (divisors->m1 = limits.min.m1; divisors->m1 <= limits.max.m1;
			divisors->m1++) {
		for (divisors->m2 = limits.min.m2; divisors->m2 <= limits.max.m2
				&& ((divisors->m2 < divisors->m1) || is_pine); divisors->m2++) {
			for (divisors->n = limits.min.n; divisors->n <= limits.max.n;
					divisors->n++) {
				for (divisors->p1 = limits.min.p1;
						divisors->p1 <= limits.max.p1; divisors->p1++) {
					divisors->m = compute_pll_m(divisors);
					divisors->p = compute_pll_p(divisors);

					if (!valid_pll_divisors(divisors, &limits))
						continue;

					float error = fabs(requestedPixelClock
						- ((referenceClock * divisors->m) / divisors->n)
						/ divisors->p);
					if (error < best) {
						best = error;
						bestDivisors = *divisors;

						if (error == 0)
							break;
					}
				}
			}
		}
	}

	*divisors = bestDivisors;

	if (best == requestedPixelClock)
		debugger("No valid PLL configuration found");
	else {
		TRACE("%s: best MHz: %g (error: %g)\n", __func__,
			((referenceClock * divisors->m) / divisors->n) / divisors->p,
			best);
	}
}


void
compute_pll_divisors(display_mode* current, pll_divisors* divisors, bool isLVDS)
{
	if (gInfo->shared_info->device_type.InGroup(INTEL_GROUP_G4x)
		|| (gInfo->shared_info->pch_info != INTEL_PCH_NONE)) {
		compute_dpll_g4x(current, divisors, isLVDS);
	} else if (gInfo->shared_info->device_type.InGroup(INTEL_GROUP_CHV)) {
		ERROR("%s: TODO: CherryView\n", __func__);
	} else if (gInfo->shared_info->device_type.InGroup(INTEL_GROUP_VLV)) {
		ERROR("%s: TODO: VallyView\n", __func__);
	} else
		compute_dpll_9xx(current, divisors, isLVDS);

	TRACE("%s: found: p = %" B_PRId32 " (p1 = %" B_PRId32 ", "
		"p2 = %" B_PRId32 "), n = %" B_PRId32 ", m = %" B_PRId32 " "
		"(m1 = %" B_PRId32 ", m2 = %" B_PRId32 ")\n", __func__,
		divisors->p, divisors->p1, divisors->p2, divisors->n,
		divisors->m, divisors->m1, divisors->m2);
}


void
refclk_activate_ilk(bool hasPanel)
{
	CALLED();

	// aka, our engineers hate you

	bool wantsSSC;
	bool hasCK505;
	if (gInfo->shared_info->pch_info == INTEL_PCH_IBX) {
		//XXX: This should be == vbt display_clock_mode
		hasCK505 = true;
		wantsSSC = hasCK505;
	} else {
		hasCK505 = false;
		wantsSSC = true;
	}

	uint32 clkRef = read32(PCH_DREF_CONTROL);
	uint32 newRef = clkRef;

	newRef &= ~DREF_NONSPREAD_SOURCE_MASK;

	if (hasCK505)
		newRef |= DREF_NONSPREAD_CK505_ENABLE;
	else
		newRef |= DREF_NONSPREAD_SOURCE_ENABLE;

	newRef &= ~DREF_SSC_SOURCE_MASK;
	newRef &= ~DREF_CPU_SOURCE_OUTPUT_MASK;
	newRef &= ~DREF_SSC1_ENABLE;

	if (newRef == clkRef) {
		TRACE("%s: No changes to reference clock.\n", __func__);
		return;
	}

	if (hasPanel) {
		newRef &= ~DREF_SSC_SOURCE_MASK;
		newRef |= DREF_SSC_SOURCE_ENABLE;

		if (wantsSSC)
			newRef |= DREF_SSC1_ENABLE;
		else
			newRef &= ~DREF_SSC1_ENABLE;

		// Power up SSC before enabling outputs
		write32(PCH_DREF_CONTROL, newRef);
		read32(PCH_DREF_CONTROL);
		spin(200);

		newRef &= ~DREF_CPU_SOURCE_OUTPUT_MASK;

		bool hasEDP = true;
		if (hasEDP) {
			if (wantsSSC)
				newRef |= DREF_CPU_SOURCE_OUTPUT_DOWNSPREAD;
			else
				newRef |= DREF_CPU_SOURCE_OUTPUT_NONSPREAD;
		} else
			newRef |= DREF_CPU_SOURCE_OUTPUT_DISABLE;

		write32(PCH_DREF_CONTROL, newRef);
		read32(PCH_DREF_CONTROL);
		spin(200);
	} else {
		newRef &= ~DREF_CPU_SOURCE_OUTPUT_MASK;
		newRef |= DREF_CPU_SOURCE_OUTPUT_DISABLE;

		write32(PCH_DREF_CONTROL, newRef);
		read32(PCH_DREF_CONTROL);
		spin(200);

		if (!wantsSSC) {
			newRef &= ~DREF_SSC_SOURCE_MASK;
			newRef |= DREF_SSC_SOURCE_DISABLE;
			newRef &= ~DREF_SSC1_ENABLE;

			write32(PCH_DREF_CONTROL, newRef);
			read32(PCH_DREF_CONTROL);
			spin(200);
		}
	}
}

