/*
 * Copyright 2012-2016, Haiku, Inc. All Rights Reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *      Alexander von Gluck, kallisti5@unixzen.com
 */


#include "dp.h"


#define TRACE_DISPLAY
#ifdef TRACE_DISPLAY
extern "C" void _sPrintf(const char* format, ...);
#   define TRACE(x...) _sPrintf("dp_common: " x)
#else
#   define TRACE(x...) ;
#endif

#define ERROR(x...) _sPrintf("dp_common: " x)


uint32
dp_encode_link_rate(uint32 linkRate)
{
	switch (linkRate) {
		case 162000:
			// 1.62 Ghz
			return DP_LINK_RATE_162;
		case 270000:
			// 2.7 Ghz
			return DP_LINK_RATE_270;
		case 540000:
			// 5.4 Ghz
			return DP_LINK_RATE_540;
	}

	ERROR("%s: Unknown DisplayPort Link Rate! (0x%" B_PRIX32 ")\n",
		__func__, linkRate);
	return DP_LINK_RATE_162;
}


uint32
dp_decode_link_rate(uint32 rawLinkRate)
{
	switch (rawLinkRate) {
		case DP_LINK_RATE_162:
			return 162000;
		case DP_LINK_RATE_270:
			return 270000;
		case DP_LINK_RATE_540:
			return 540000;
	}
	ERROR("%s: Unknown DisplayPort Link Rate! (0x%" B_PRIX32 ")\n",
		__func__, rawLinkRate);
	return 162000;
}


uint32
dp_get_pixel_clock_max(int linkRate, int laneCount, int bpp)
{
	return (linkRate * laneCount * 8) / bpp;
}
