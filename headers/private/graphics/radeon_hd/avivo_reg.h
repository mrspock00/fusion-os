/*
 * Copyright 2009 Advanced Micro Devices, Inc.
 * Copyright 2009 Red Hat Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Authors: Dave Airlie
 *          Alex Deucher
 *          Jerome Glisse
 *          Alexander von Gluck <kallisti5@unixzen.com>
 */
#ifndef __AVIVO_REG_H__
#define __AVIVO_REG_H__


#define	AVIVO_D1CRTC_UPDATE_LOCK					0x60E8
#define AVIVO_D1GRPH_SWAP_CNTL						0x610C
#define	AVIVO_D1GRPH_PRIMARY_SURFACE_ADDRESS		0x6110
#define	AVIVO_D1GRPH_SECONDARY_SURFACE_ADDRESS		0x6118

#define	AVIVO_D1VGA_CONTROL							0x0330
#define		AVIVO_DVGA_CONTROL_MODE_ENABLE			(1 << 0)
#define		AVIVO_DVGA_CONTROL_TIMING_SELECT		(1 << 8)
#define		AVIVO_DVGA_CONTROL_SYNC_POLARITY_SELECT	(1 << 9)
#define		AVIVO_DVGA_CONTROL_OVERSCAN_TIMING_SELECT (1 << 10)
#define		AVIVO_DVGA_CONTROL_OVERSCAN_COLOR_EN	(1 << 16)
#define		AVIVO_DVGA_CONTROL_ROTATE				(1 << 24)
#define AVIVO_D2VGA_CONTROL							0x0338

#define	AVIVO_VGA_HDP_CONTROL						0x328
#define		AVIVO_VGA_MEM_PAGE_SELECT_EN			(1 << 0)
#define		AVIVO_VGA_MEMORY_DISABLE				(1 << 4)
#define		AVIVO_VGA_RBBM_LOCK_DISABLE				(1 << 8)
#define		AVIVO_VGA_SOFT_RESET					(1 << 16)
#define	AVIVO_VGA_MEMORY_BASE_ADDRESS				0x0310
#define	AVIVO_VGA_RENDER_CONTROL					0x0300
#define		AVIVO_VGA_VSTATUS_CNTL_MASK				(3 << 16)


#define AVIVO_MC_INDEX								0x0070
#define AVIVO_MC_DATA								0x0074

#define AVIVO_CP_DYN_CNTL							0x000f /* PLL */
#define 	AVIVO_CP_FORCEON						(1 << 0)
#define AVIVO_E2_DYN_CNTL							0x0011 /* PLL */
#define 	AVIVO_E2_FORCEON						(1 << 0)
#define AVIVO_IDCT_DYN_CNTL							0x0013 /* PLL */
#define 	AVIVO_IDCT_FORCEON						(1 << 0)

#define AVIVO_HDP_FB_LOCATION						0x134
      
#define AVIVO_EXT1_PPLL_REF_DIV_SRC					0x400
#define AVIVO_EXT1_PPLL_REF_DIV						0x404
#define AVIVO_EXT1_PPLL_UPDATE_LOCK					0x408
#define AVIVO_EXT1_PPLL_UPDATE_CNTL					0x40c

#define AVIVO_EXT2_PPLL_REF_DIV_SRC					0x410
#define AVIVO_EXT2_PPLL_REF_DIV						0x414
#define AVIVO_EXT2_PPLL_UPDATE_LOCK					0x418
#define AVIVO_EXT2_PPLL_UPDATE_CNTL					0x41c

#define AVIVO_EXT1_PPLL_FB_DIV						0x430
#define AVIVO_EXT2_PPLL_FB_DIV						0x434

#define AVIVO_EXT1_PPLL_POST_DIV_SRC				0x438
#define AVIVO_EXT1_PPLL_POST_DIV					0x43c

#define AVIVO_EXT2_PPLL_POST_DIV_SRC				0x440
#define AVIVO_EXT2_PPLL_POST_DIV					0x444

#define AVIVO_EXT1_PPLL_CNTL						0x448
#define AVIVO_EXT2_PPLL_CNTL						0x44c

#define AVIVO_P1PLL_CNTL							0x450
#define AVIVO_P2PLL_CNTL							0x454
#define AVIVO_P1PLL_INT_SS_CNTL						0x458
#define AVIVO_P2PLL_INT_SS_CNTL						0x45c
#define AVIVO_P1PLL_TMDSA_CNTL						0x460
#define AVIVO_P2PLL_LVTMA_CNTL						0x464

#define AVIVO_PCLK_CRTC1_CNTL						0x480
#define AVIVO_PCLK_CRTC2_CNTL						0x484

/* first crtc */
#define AVIVO_D1CRTC_H_TOTAL						0x6000
#define AVIVO_D1CRTC_H_BLANK_START_END				0x6004
#define AVIVO_D1CRTC_H_SYNC_A						0x6008
#define AVIVO_D1CRTC_H_SYNC_A_CNTL					0x600C
#define AVIVO_D1CRTC_H_SYNC_B						0x6010
#define AVIVO_D1CRTC_H_SYNC_B_CNTL					0x6014

#define AVIVO_D1CRTC_V_TOTAL						0x6020
#define AVIVO_D1CRTC_V_BLANK_START_END				0x6024
#define AVIVO_D1CRTC_V_SYNC_A						0x6028
#define AVIVO_D1CRTC_V_SYNC_A_CNTL					0x602C
#define AVIVO_D1CRTC_V_SYNC_B						0x6030
#define AVIVO_D1CRTC_V_SYNC_B_CNTL					0x6034

#define AVIVO_D1CRTC_CONTROL						0x6080
#define 	AVIVO_CRTC_EN							(1 << 0)
#define 	AVIVO_CRTC_DISP_READ_REQUEST_DISABLE	(1 << 24)
#define AVIVO_D1CRTC_BLANK_CONTROL					0x6084
#define AVIVO_D1CRTC_INTERLACE_CONTROL				0x6088
#define AVIVO_D1CRTC_INTERLACE_STATUS				0x608C
#define AVIVO_D1CRTC_STATUS							0x609C
#define AVIVO_D1CRTC_STATUS_POSITION				0x60A0
#define AVIVO_D1CRTC_FRAME_COUNT					0x60A4
#define AVIVO_D1CRTC_STEREO_CONTROL					0x60C4

#define AVIVO_D1MODE_MASTER_UPDATE_MODE				0x60e4

/* master controls */
#define AVIVO_DC_CRTC_MASTER_EN						0x60f8
#define AVIVO_DC_CRTC_TV_CONTROL					0x60fc

#define AVIVO_D1GRPH_ENABLE							0x6100
#define AVIVO_D1GRPH_CONTROL						0x6104
#define 	AVIVO_D1GRPH_CONTROL_DEPTH_8BPP			(0 << 0)
#define 	AVIVO_D1GRPH_CONTROL_DEPTH_16BPP		(1 << 0)
#define 	AVIVO_D1GRPH_CONTROL_DEPTH_32BPP		(2 << 0)
#define 	AVIVO_D1GRPH_CONTROL_DEPTH_64BPP		(3 << 0)

#define 	AVIVO_D1GRPH_CONTROL_8BPP_INDEXED		(0 << 8)

#define 	AVIVO_D1GRPH_CONTROL_16BPP_ARGB1555		(0 << 8)
#define 	AVIVO_D1GRPH_CONTROL_16BPP_RGB565		(1 << 8)
#define 	AVIVO_D1GRPH_CONTROL_16BPP_ARGB4444		(2 << 8)
#define 	AVIVO_D1GRPH_CONTROL_16BPP_AI88			(3 << 8)
#define 	AVIVO_D1GRPH_CONTROL_16BPP_MONO16		(4 << 8)

#define 	AVIVO_D1GRPH_CONTROL_32BPP_ARGB8888		(0 << 8)
#define 	AVIVO_D1GRPH_CONTROL_32BPP_ARGB2101010	(1 << 8)
#define 	AVIVO_D1GRPH_CONTROL_32BPP_DIGITAL		(2 << 8)
#define 	AVIVO_D1GRPH_CONTROL_32BPP_8B_ARGB2101010 (3 << 8)


#define 	AVIVO_D1GRPH_CONTROL_64BPP_ARGB16161616	(0 << 8)

#define 	AVIVO_D1GRPH_SWAP_RB					(1 << 16)
#define 	AVIVO_D1GRPH_TILED						(1 << 20)
#define 	AVIVO_D1GRPH_MACRO_ADDRESS_MODE			(1 << 21)

#define 	R600_D1GRPH_ARRAY_MODE_LINEAR_GENERAL	(0 << 20)
#define 	R600_D1GRPH_ARRAY_MODE_LINEAR_ALIGNED	(1 << 20)
#define 	R600_D1GRPH_ARRAY_MODE_1D_TILED_THIN1	(2 << 20)
#define 	R600_D1GRPH_ARRAY_MODE_2D_TILED_THIN1	(4 << 20)

/* The R7xx *_HIGH surface regs are backwards; the D1 regs are in the D2
 * block and vice versa.  This applies to GRPH, CUR, etc.
 */
#define AVIVO_D1GRPH_LUT_SEL								0x6108
#define AVIVO_D1GRPH_PRIMARY_SURFACE_ADDRESS				0x6110
#define AVIVO_D1GRPH_SECONDARY_SURFACE_ADDRESS				0x6118
#define AVIVO_D1GRPH_PITCH									0x6120
#define AVIVO_D1GRPH_SURFACE_OFFSET_X						0x6124
#define AVIVO_D1GRPH_SURFACE_OFFSET_Y						0x6128
#define AVIVO_D1GRPH_X_START								0x612c
#define AVIVO_D1GRPH_Y_START								0x6130
#define AVIVO_D1GRPH_X_END									0x6134
#define AVIVO_D1GRPH_Y_END									0x6138
#define AVIVO_D1GRPH_UPDATE									0x6144
#define 	AVIVO_D1GRPH_SURFACE_UPDATE_PENDING				(1 << 2)
#define 	AVIVO_D1GRPH_UPDATE_LOCK						(1 << 16)
#define AVIVO_D1GRPH_FLIP_CONTROL							0x6148
#define 	AVIVO_D1GRPH_SURFACE_UPDATE_H_RETRACE_EN		(1 << 0)

#define AVIVO_D1CUR_CONTROL									0x6400
#define 	AVIVO_D1CURSOR_EN								(1 << 0)
#define 	AVIVO_D1CURSOR_MODE_SHIFT						8
#define 	AVIVO_D1CURSOR_MODE_MASK						(3 << 8)
#define 	AVIVO_D1CURSOR_MODE_24BPP						2
#define AVIVO_D1CUR_SURFACE_ADDRESS							0x6408
#define R700_D1CUR_SURFACE_ADDRESS_HIGH						0x6c0c
#define R700_D2CUR_SURFACE_ADDRESS_HIGH						0x640c
#define AVIVO_D1CUR_SIZE									0x6410
#define AVIVO_D1CUR_POSITION								0x6414
#define AVIVO_D1CUR_HOT_SPOT								0x6418
#define AVIVO_D1CUR_UPDATE									0x6424
#define 	AVIVO_D1CURSOR_UPDATE_LOCK						(1 << 16)

#define AVIVO_DC_LUT_RW_SELECT								0x6480
#define AVIVO_DC_LUT_RW_MODE								0x6484
#define AVIVO_DC_LUT_RW_INDEX								0x6488
#define AVIVO_DC_LUT_SEQ_COLOR								0x648c
#define AVIVO_DC_LUT_PWL_DATA								0x6490
#define AVIVO_DC_LUT_30_COLOR								0x6494
#define AVIVO_DC_LUT_READ_PIPE_SELECT						0x6498
#define AVIVO_DC_LUT_WRITE_EN_MASK							0x649c
#define AVIVO_DC_LUT_AUTOFILL								0x64a0

#define AVIVO_DC_LUTA_CONTROL								0x64c0
#define AVIVO_DC_LUTA_BLACK_OFFSET_BLUE						0x64c4
#define AVIVO_DC_LUTA_BLACK_OFFSET_GREEN					0x64c8
#define AVIVO_DC_LUTA_BLACK_OFFSET_RED						0x64cc
#define AVIVO_DC_LUTA_WHITE_OFFSET_BLUE						0x64d0
#define AVIVO_DC_LUTA_WHITE_OFFSET_GREEN					0x64d4
#define AVIVO_DC_LUTA_WHITE_OFFSET_RED						0x64d8

#define AVIVO_DC_LB_MEMORY_SPLIT							0x6520
#define 	AVIVO_DC_LB_MEMORY_SPLIT_MASK					0x3
#define 	AVIVO_DC_LB_MEMORY_SPLIT_SHIFT					0
#define 	AVIVO_DC_LB_MEMORY_SPLIT_D1HALF_D2HALF			0
#define 	AVIVO_DC_LB_MEMORY_SPLIT_D1_3Q_D2_1Q			1
#define 	AVIVO_DC_LB_MEMORY_SPLIT_D1_ONLY				2
#define 	AVIVO_DC_LB_MEMORY_SPLIT_D1_1Q_D2_3Q			3
#define 	AVIVO_DC_LB_MEMORY_SPLIT_SHIFT_MODE				(1 << 2)
#define 	AVIVO_DC_LB_DISP1_END_ADR_SHIFT					4
#define 	AVIVO_DC_LB_DISP1_END_ADR_MASK					0x7ff

#define AVIVO_D1MODE_DATA_FORMAT							0x6528
#define 	AVIVO_D1MODE_INTERLEAVE_EN						(1 << 0)
#define AVIVO_D1MODE_DESKTOP_HEIGHT							0x652C
#define AVIVO_D1MODE_VBLANK_STATUS							0x6534
#define 	AVIVO_VBLANK_ACK								(1 << 4)
#define AVIVO_D1MODE_VLINE_START_END						0x6538
#define AVIVO_D1MODE_VLINE_STATUS							0x653c
#define 	AVIVO_D1MODE_VLINE_STAT							(1 << 12)
#define AVIVO_DxMODE_INT_MASK								0x6540
#define 	AVIVO_D1MODE_INT_MASK							(1 << 0)
#define 	AVIVO_D2MODE_INT_MASK							(1 << 8)
#define AVIVO_D1MODE_VIEWPORT_START							0x6580
#define AVIVO_D1MODE_VIEWPORT_SIZE							0x6584
#define AVIVO_D1MODE_EXT_OVERSCAN_LEFT_RIGHT				0x6588
#define AVIVO_D1MODE_EXT_OVERSCAN_TOP_BOTTOM				0x658c

#define AVIVO_D1SCL_SCALER_ENABLE							0x6590
#define AVIVO_D1SCL_SCALER_TAP_CONTROL						0x6594
#define AVIVO_D1SCL_UPDATE									0x65cc
#define 	AVIVO_D1SCL_UPDATE_LOCK							(1 << 16)

/* second crtc */
#define AVIVO_D2CRTC_H_TOTAL								0x6800
#define AVIVO_D2CRTC_H_BLANK_START_END						0x6804
#define AVIVO_D2CRTC_H_SYNC_A								0x6808
#define AVIVO_D2CRTC_H_SYNC_A_CNTL							0x680c
#define AVIVO_D2CRTC_H_SYNC_B								0x6810
#define AVIVO_D2CRTC_H_SYNC_B_CNTL							0x6814

#define AVIVO_D2CRTC_V_TOTAL								0x6820
#define AVIVO_D2CRTC_V_BLANK_START_END						0x6824
#define AVIVO_D2CRTC_V_SYNC_A								0x6828
#define AVIVO_D2CRTC_V_SYNC_A_CNTL							0x682c
#define AVIVO_D2CRTC_V_SYNC_B								0x6830
#define AVIVO_D2CRTC_V_SYNC_B_CNTL							0x6834

#define AVIVO_D2CRTC_CONTROL								0x6880
#define AVIVO_D2CRTC_BLANK_CONTROL							0x6884
#define AVIVO_D2CRTC_INTERLACE_CONTROL						0x6888
#define AVIVO_D2CRTC_INTERLACE_STATUS						0x688C
#define AVIVO_D2CRTC_STATUS									0x689C
#define AVIVO_D2CRTC_STATUS_POSITION						0x68A0
#define AVIVO_D2CRTC_FRAME_COUNT							0x68A4
#define AVIVO_D2CRTC_STEREO_CONTROL							0x68C4
#define AVIVO_D2CRTC_UPDATE_LOCK							0x68E8

#define AVIVO_D2GRPH_ENABLE									0x6900
#define AVIVO_D2GRPH_CONTROL								0x6904
#define AVIVO_D2GRPH_LUT_SEL								0x6908
#define AVIVO_D2GRPH_SWAP_CNTL								0x690C
#define AVIVO_D2GRPH_PRIMARY_SURFACE_ADDRESS				0x6910
#define AVIVO_D2GRPH_SECONDARY_SURFACE_ADDRESS				0x6918
#define AVIVO_D2GRPH_PITCH									0x6920
#define AVIVO_D2GRPH_SURFACE_OFFSET_X						0x6924
#define AVIVO_D2GRPH_SURFACE_OFFSET_Y						0x6928
#define AVIVO_D2GRPH_X_START								0x692c
#define AVIVO_D2GRPH_Y_START								0x6930
#define AVIVO_D2GRPH_X_END									0x6934
#define AVIVO_D2GRPH_Y_END									0x6938
#define AVIVO_D2GRPH_UPDATE									0x6944
#define AVIVO_D2GRPH_FLIP_CONTROL							0x6948

#define AVIVO_D2CUR_CONTROL									0x6c00
#define AVIVO_D2CUR_SURFACE_ADDRESS							0x6c08
#define AVIVO_D2CUR_SIZE									0x6c10
#define AVIVO_D2CUR_POSITION								0x6c14

#define AVIVO_D2MODE_VBLANK_STATUS							0x6d34
#define AVIVO_D2MODE_VLINE_START_END						0x6d38
#define AVIVO_D2MODE_VLINE_STATUS							0x6d3c
#define AVIVO_D2MODE_VIEWPORT_START							0x6d80
#define AVIVO_D2MODE_VIEWPORT_SIZE							0x6d84
#define AVIVO_D2MODE_EXT_OVERSCAN_LEFT_RIGHT				0x6d88
#define AVIVO_D2MODE_EXT_OVERSCAN_TOP_BOTTOM				0x6d8c

#define AVIVO_D2SCL_SCALER_ENABLE							0x6d90
#define AVIVO_D2SCL_SCALER_TAP_CONTROL						0x6d94

#define AVIVO_DDIA_BIT_DEPTH_CONTROL						0x7214

#define AVIVO_DP_VID_STREAM_CNTL							0x770C

#define AVIVO_DACA_ENABLE									0x7800
#define 	AVIVO_DAC_ENABLE								(1 << 0)
#define AVIVO_DACA_SOURCE_SELECT							0x7804
#define 	AVIVO_DAC_SOURCE_CRTC1							(0 << 0)
#define 	AVIVO_DAC_SOURCE_CRTC2							(1 << 0)
#define 	AVIVO_DAC_SOURCE_TV								(2 << 0)

#define AVIVO_DACA_FORCE_OUTPUT_CNTL						0x783c
#define 	AVIVO_DACA_FORCE_OUTPUT_CNTL_FORCE_DATA_EN		(1 << 0)
#define 	AVIVO_DACA_FORCE_OUTPUT_CNTL_DATA_SEL_SHIFT		(8)
#define 	AVIVO_DACA_FORCE_OUTPUT_CNTL_DATA_SEL_BLUE		(1 << 0)
#define 	AVIVO_DACA_FORCE_OUTPUT_CNTL_DATA_SEL_GREEN		(1 << 1)
#define 	AVIVO_DACA_FORCE_OUTPUT_CNTL_DATA_SEL_RED		(1 << 2)
#define 	AVIVO_DACA_FORCE_OUTPUT_CNTL_DATA_ON_BLANKB_ONLY (1 << 24)
#define AVIVO_DACA_POWERDOWN								0x7850
#define 	AVIVO_DACA_POWERDOWN_POWERDOWN					(1 << 0)
#define 	AVIVO_DACA_POWERDOWN_BLUE						(1 << 8)
#define 	AVIVO_DACA_POWERDOWN_GREEN						(1 << 16)
#define 	AVIVO_DACA_POWERDOWN_RED						(1 << 24)

#define AVIVO_DACB_ENABLE									0x7a00
#define AVIVO_DACB_SOURCE_SELECT							0x7a04
#define 	AVIVO_DACB_FORCE_OUTPUT_CNTL					0x7a3c
#define 	AVIVO_DACB_FORCE_OUTPUT_CNTL_FORCE_DATA_EN		(1 << 0)
#define 	AVIVO_DACB_FORCE_OUTPUT_CNTL_DATA_SEL_SHIFT		(8)
#define 	AVIVO_DACB_FORCE_OUTPUT_CNTL_DATA_SEL_BLUE		(1 << 0)
#define 	AVIVO_DACB_FORCE_OUTPUT_CNTL_DATA_SEL_GREEN		(1 << 1)
#define 	AVIVO_DACB_FORCE_OUTPUT_CNTL_DATA_SEL_RED		(1 << 2)
#define 	AVIVO_DACB_FORCE_OUTPUT_CNTL_DATA_ON_BLANKB_ONLY (1 << 24)
#define AVIVO_DACB_POWERDOWN								0x7a50
#define 	AVIVO_DACB_POWERDOWN_POWERDOWN					(1 << 0)
#define 	AVIVO_DACB_POWERDOWN_BLUE						(1 << 8)
#define 	AVIVO_DACB_POWERDOWN_GREEN						(1 << 16)
#define 	AVIVO_DACB_POWERDOWN_RED						(1 << 24)

#define AVIVO_TMDSA_CNTL									0x7880
#define 	AVIVO_TMDSA_CNTL_ENABLE							(1 << 0)
#define 	AVIVO_TMDSA_CNTL_HPD_MASK						(1 << 4)
#define 	AVIVO_TMDSA_CNTL_HPD_SELECT						(1 << 8)
#define 	AVIVO_TMDSA_CNTL_SYNC_PHASE						(1 << 12)
#define 	AVIVO_TMDSA_CNTL_PIXEL_ENCODING					(1 << 16)
#define 	AVIVO_TMDSA_CNTL_DUAL_LINK_ENABLE				(1 << 24)
#define 	AVIVO_TMDSA_CNTL_SWAP							(1 << 28)
#define AVIVO_TMDSA_SOURCE_SELECT							0x7884
/* 78a8 appears to be some kind of (reasonably tolerant) clock?
 * 78d0 definitely hits the transmitter, definitely clock. */
/* MYSTERY1 This appears to control dithering? */
#define AVIVO_TMDSA_BIT_DEPTH_CONTROL							0x7894
#define 	AVIVO_TMDS_BIT_DEPTH_CONTROL_TRUNCATE_EN			(1 << 0)
#define 	AVIVO_TMDS_BIT_DEPTH_CONTROL_TRUNCATE_DEPTH			(1 << 4)
#define 	AVIVO_TMDS_BIT_DEPTH_CONTROL_SPATIAL_DITHER_EN		(1 << 8)
#define 	AVIVO_TMDS_BIT_DEPTH_CONTROL_SPATIAL_DITHER_DEPTH	(1 << 12)
#define 	AVIVO_TMDS_BIT_DEPTH_CONTROL_TEMPORAL_DITHER_EN		(1 << 16)
#define 	AVIVO_TMDS_BIT_DEPTH_CONTROL_TEMPORAL_DITHER_DEPTH	(1 << 20)
#define 	AVIVO_TMDS_BIT_DEPTH_CONTROL_TEMPORAL_LEVEL			(1 << 24)
#define 	AVIVO_TMDS_BIT_DEPTH_CONTROL_TEMPORAL_DITHER_RESET	(1 << 26)
#define AVIVO_TMDSA_DCBALANCER_CONTROL							0x78d0
#define 	AVIVO_TMDSA_DCBALANCER_CONTROL_EN					(1 << 0)
#define 	AVIVO_TMDSA_DCBALANCER_CONTROL_TEST_EN				(1 << 8)
#define 	AVIVO_TMDSA_DCBALANCER_CONTROL_TEST_IN_SHIFT		(16)
#define 	AVIVO_TMDSA_DCBALANCER_CONTROL_FORCE				(1 << 24)
#define AVIVO_TMDSA_DATA_SYNCHRONIZATION						0x78d8
#define 	AVIVO_TMDSA_DATA_SYNCHRONIZATION_DSYNSEL			(1 << 0)
#define 	AVIVO_TMDSA_DATA_SYNCHRONIZATION_PFREQCHG			(1 << 8)
#define AVIVO_TMDSA_CLOCK_ENABLE								0x7900
#define AVIVO_TMDSA_TRANSMITTER_ENABLE							0x7904
#define 	AVIVO_TMDSA_TRANSMITTER_ENABLE_TX0_ENABLE			(1 << 0)
#define 	AVIVO_TMDSA_TRANSMITTER_ENABLE_LNKC0EN				(1 << 1)
#define 	AVIVO_TMDSA_TRANSMITTER_ENABLE_LNKD00EN				(1 << 2)
#define 	AVIVO_TMDSA_TRANSMITTER_ENABLE_LNKD01EN				(1 << 3)
#define 	AVIVO_TMDSA_TRANSMITTER_ENABLE_LNKD02EN				(1 << 4)
#define 	AVIVO_TMDSA_TRANSMITTER_ENABLE_TX1_ENABLE			(1 << 8)
#define 	AVIVO_TMDSA_TRANSMITTER_ENABLE_LNKD10EN				(1 << 10)
#define 	AVIVO_TMDSA_TRANSMITTER_ENABLE_LNKD11EN				(1 << 11)
#define 	AVIVO_TMDSA_TRANSMITTER_ENABLE_LNKD12EN				(1 << 12)
#define 	AVIVO_TMDSA_TRANSMITTER_ENABLE_TX_ENABLE_HPD_MASK	(1 << 16)
#define 	AVIVO_TMDSA_TRANSMITTER_ENABLE_LNKCEN_HPD_MASK		(1 << 17)
#define 	AVIVO_TMDSA_TRANSMITTER_ENABLE_LNKDEN_HPD_MASK		(1 << 18)

#define AVIVO_TMDSA_TRANSMITTER_CONTROL							0x7910
#define 	AVIVO_TMDSA_TRANSMITTER_CONTROL_PLL_ENABLE			(1 << 0)
#define 	AVIVO_TMDSA_TRANSMITTER_CONTROL_PLL_RESET			(1 << 1)
#define 	AVIVO_TMDSA_TRANSMITTER_CONTROL_PLL_HPD_MASK_SHIFT	(2)
#define 	AVIVO_TMDSA_TRANSMITTER_CONTROL_IDSCKSEL			(1 << 4)
#define 	AVIVO_TMDSA_TRANSMITTER_CONTROL_BGSLEEP				(1 << 5)
#define 	AVIVO_TMDSA_TRANSMITTER_CONTROL_PLL_PWRUP_SEQ_EN	(1 << 6)
#define 	AVIVO_TMDSA_TRANSMITTER_CONTROL_TMCLK				(1 << 8)
#define 	AVIVO_TMDSA_TRANSMITTER_CONTROL_TMCLK_FROM_PADS		(1 << 13)
#define 	AVIVO_TMDSA_TRANSMITTER_CONTROL_TDCLK				(1 << 14)
#define 	AVIVO_TMDSA_TRANSMITTER_CONTROL_TDCLK_FROM_PADS		(1 << 15)
#define 	AVIVO_TMDSA_TRANSMITTER_CONTROL_CLK_PATTERN_SHIFT	(16)
#define 	AVIVO_TMDSA_TRANSMITTER_CONTROL_BYPASS_PLL			(1 << 28)
#define 	AVIVO_TMDSA_TRANSMITTER_CONTROL_USE_CLK_DATA		(1 << 29)
#define 	AVIVO_TMDSA_TRANSMITTER_CONTROL_INPUT_TEST_CLK_SEL	(1 << 31)

#define AVIVO_LVTMA_CNTL										0x7a80
#define 	AVIVO_LVTMA_CNTL_ENABLE								(1 << 0)
#define 	AVIVO_LVTMA_CNTL_HPD_MASK							(1 << 4)
#define 	AVIVO_LVTMA_CNTL_HPD_SELECT							(1 << 8)
#define 	AVIVO_LVTMA_CNTL_SYNC_PHASE							(1 << 12)
#define 	AVIVO_LVTMA_CNTL_PIXEL_ENCODING						(1 << 16)
#define 	AVIVO_LVTMA_CNTL_DUAL_LINK_ENABLE					(1 << 24)
#define 	AVIVO_LVTMA_CNTL_SWAP								(1 << 28)
#define AVIVO_LVTMA_SOURCE_SELECT								0x7a84
#define AVIVO_LVTMA_COLOR_FORMAT								0x7a88
#define AVIVO_LVTMA_BIT_DEPTH_CONTROL							0x7a94
#define 	AVIVO_LVTMA_BIT_DEPTH_CONTROL_TRUNCATE_EN			(1 << 0)
#define 	AVIVO_LVTMA_BIT_DEPTH_CONTROL_TRUNCATE_DEPTH		(1 << 4)
#define 	AVIVO_LVTMA_BIT_DEPTH_CONTROL_SPATIAL_DITHER_EN		(1 << 8)
#define 	AVIVO_LVTMA_BIT_DEPTH_CONTROL_SPATIAL_DITHER_DEPTH	(1 << 12)
#define 	AVIVO_LVTMA_BIT_DEPTH_CONTROL_TEMPORAL_DITHER_EN	(1 << 16)
#define 	AVIVO_LVTMA_BIT_DEPTH_CONTROL_TEMPORAL_DITHER_DEPTH	(1 << 20)
#define 	AVIVO_LVTMA_BIT_DEPTH_CONTROL_TEMPORAL_LEVEL		(1 << 24)
#define 	AVIVO_LVTMA_BIT_DEPTH_CONTROL_TEMPORAL_DITHER_RESET	(1 << 26)



#define AVIVO_LVTMA_DCBALANCER_CONTROL							0x7ad0
#define 	AVIVO_LVTMA_DCBALANCER_CONTROL_EN					(1 << 0)
#define 	AVIVO_LVTMA_DCBALANCER_CONTROL_TEST_EN				(1 << 8)
#define 	AVIVO_LVTMA_DCBALANCER_CONTROL_TEST_IN_SHIFT		(16)
#define 	AVIVO_LVTMA_DCBALANCER_CONTROL_FORCE				(1 << 24)

#define AVIVO_LVTMA_DATA_SYNCHRONIZATION						0x78d8
#define 	AVIVO_LVTMA_DATA_SYNCHRONIZATION_DSYNSEL			(1 << 0)
#define 	AVIVO_LVTMA_DATA_SYNCHRONIZATION_PFREQCHG			(1 << 8)
#define R500_LVTMA_CLOCK_ENABLE									0x7b00
#define R600_LVTMA_CLOCK_ENABLE									0x7b04

#define R500_LVTMA_TRANSMITTER_ENABLE							0x7b04
#define R600_LVTMA_TRANSMITTER_ENABLE							0x7b08
#define 	AVIVO_LVTMA_TRANSMITTER_ENABLE_LNKC0EN				(1 << 1)
#define 	AVIVO_LVTMA_TRANSMITTER_ENABLE_LNKD00EN				(1 << 2)
#define 	AVIVO_LVTMA_TRANSMITTER_ENABLE_LNKD01EN				(1 << 3)
#define 	AVIVO_LVTMA_TRANSMITTER_ENABLE_LNKD02EN				(1 << 4)
#define 	AVIVO_LVTMA_TRANSMITTER_ENABLE_LNKD03EN				(1 << 5)
#define 	AVIVO_LVTMA_TRANSMITTER_ENABLE_LNKC1EN				(1 << 9)
#define 	AVIVO_LVTMA_TRANSMITTER_ENABLE_LNKD10EN				(1 << 10)
#define 	AVIVO_LVTMA_TRANSMITTER_ENABLE_LNKD11EN				(1 << 11)
#define 	AVIVO_LVTMA_TRANSMITTER_ENABLE_LNKD12EN				(1 << 12)
#define 	AVIVO_LVTMA_TRANSMITTER_ENABLE_LNKCEN_HPD_MASK		(1 << 17)
#define 	AVIVO_LVTMA_TRANSMITTER_ENABLE_LNKDEN_HPD_MASK		(1 << 18)

#define R500_LVTMA_TRANSMITTER_CONTROL							0x7b10
#define R600_LVTMA_TRANSMITTER_CONTROL							0x7b14
#define 	AVIVO_LVTMA_TRANSMITTER_CONTROL_PLL_ENABLE			(1 << 0)
#define 	AVIVO_LVTMA_TRANSMITTER_CONTROL_PLL_RESET			(1 << 1)
#define 	AVIVO_LVTMA_TRANSMITTER_CONTROL_PLL_HPD_MASK_SHIFT	(2)
#define 	AVIVO_LVTMA_TRANSMITTER_CONTROL_IDSCKSEL			(1 << 4)
#define 	AVIVO_LVTMA_TRANSMITTER_CONTROL_BGSLEEP				(1 << 5)
#define 	AVIVO_LVTMA_TRANSMITTER_CONTROL_PLL_PWRUP_SEQ_EN	(1 << 6)
#define 	AVIVO_LVTMA_TRANSMITTER_CONTROL_TMCLK				(1 << 8)
#define 	AVIVO_LVTMA_TRANSMITTER_CONTROL_TMCLK_FROM_PADS		(1 << 13)
#define 	AVIVO_LVTMA_TRANSMITTER_CONTROL_TDCLK				(1 << 14)
#define 	AVIVO_LVTMA_TRANSMITTER_CONTROL_TDCLK_FROM_PADS		(1 << 15)
#define 	AVIVO_LVTMA_TRANSMITTER_CONTROL_CLK_PATTERN_SHIFT	(16)
#define 	AVIVO_LVTMA_TRANSMITTER_CONTROL_BYPASS_PLL			(1 << 28)
#define 	AVIVO_LVTMA_TRANSMITTER_CONTROL_USE_CLK_DATA		(1 << 29)
#define 	AVIVO_LVTMA_TRANSMITTER_CONTROL_INPUT_TEST_CLK_SEL	(1 << 31)

#define R500_LVTMA_PWRSEQ_CNTL								0x7af0
#define R600_LVTMA_PWRSEQ_CNTL								0x7af4
#define 	AVIVO_LVTMA_PWRSEQ_EN							(1 << 0)
#define 	AVIVO_LVTMA_PWRSEQ_PLL_ENABLE_MASK				(1 << 2)
#define 	AVIVO_LVTMA_PWRSEQ_PLL_RESET_MASK				(1 << 3)
#define 	AVIVO_LVTMA_PWRSEQ_TARGET_STATE					(1 << 4)
#define 	AVIVO_LVTMA_SYNCEN								(1 << 8)
#define 	AVIVO_LVTMA_SYNCEN_OVRD							(1 << 9)
#define 	AVIVO_LVTMA_SYNCEN_POL							(1 << 10)
#define 	AVIVO_LVTMA_DIGON								(1 << 16)
#define 	AVIVO_LVTMA_DIGON_OVRD							(1 << 17)
#define 	AVIVO_LVTMA_DIGON_POL							(1 << 18)
#define 	AVIVO_LVTMA_BLON								(1 << 24)
#define 	AVIVO_LVTMA_BLON_OVRD							(1 << 25)
#define 	AVIVO_LVTMA_BLON_POL							(1 << 26)

#define R500_LVTMA_PWRSEQ_STATE								0x7af4
#define R600_LVTMA_PWRSEQ_STATE								0x7af8
#define 	AVIVO_LVTMA_PWRSEQ_STATE_TARGET_STATE_R			(1 << 0)
#define 	AVIVO_LVTMA_PWRSEQ_STATE_DIGON					(1 << 1)
#define 	AVIVO_LVTMA_PWRSEQ_STATE_SYNCEN					(1 << 2)
#define 	AVIVO_LVTMA_PWRSEQ_STATE_BLON					(1 << 3)
#define 	AVIVO_LVTMA_PWRSEQ_STATE_DONE					(1 << 4)
#define 	AVIVO_LVTMA_PWRSEQ_STATE_STATUS_SHIFT			(8)

#define AVIVO_LVDS_BACKLIGHT_CNTL							0x7af8
#define 	AVIVO_LVDS_BACKLIGHT_CNTL_EN					(1 << 0)
#define 	AVIVO_LVDS_BACKLIGHT_LEVEL_MASK					0x0000ff00
#define 	AVIVO_LVDS_BACKLIGHT_LEVEL_SHIFT				8

#define AVIVO_DVOA_BIT_DEPTH_CONTROL						0x7988

#define AVIVO_DC_GPIO_HPD_A									0x7e94
#define AVIVO_DC_GPIO_HPD_Y									0x7e9c

#define AVIVO_DC_I2C_STATUS1								0x7d30
#define 	AVIVO_DC_I2C_DONE								(1 << 0)
#define 	AVIVO_DC_I2C_NACK								(1 << 1)
#define 	AVIVO_DC_I2C_HALT								(1 << 2)
#define 	AVIVO_DC_I2C_GO									(1 << 3)
#define AVIVO_DC_I2C_RESET									0x7d34
#define 	AVIVO_DC_I2C_SOFT_RESET							(1 << 0)
#define 	AVIVO_DC_I2C_ABORT								(1 << 8)
#define AVIVO_DC_I2C_CONTROL1								0x7d38
#define 	AVIVO_DC_I2C_START								(1 << 0)
#define 	AVIVO_DC_I2C_STOP								(1 << 1)
#define 	AVIVO_DC_I2C_RECEIVE							(1 << 2)
#define 	AVIVO_DC_I2C_EN									(1 << 8)
#define 	AVIVO_DC_I2C_PIN_SELECT(x)						((x) << 16)
#define 	AVIVO_SEL_DDC1									0
#define 	AVIVO_SEL_DDC2									1
#define 	AVIVO_SEL_DDC3									2
#define AVIVO_DC_I2C_CONTROL2								0x7d3c
#define 	AVIVO_DC_I2C_ADDR_COUNT(x)						((x) << 0)
#define 	AVIVO_DC_I2C_DATA_COUNT(x)						((x) << 8)
#define AVIVO_DC_I2C_CONTROL3								0x7d40
#define 	AVIVO_DC_I2C_DATA_DRIVE_EN						(1 << 0)
#define 	AVIVO_DC_I2C_DATA_DRIVE_SEL						(1 << 1)
#define 	AVIVO_DC_I2C_CLK_DRIVE_EN						(1 << 7)
#define 	AVIVO_DC_I2C_RD_INTRA_BYTE_DELAY(x)				((x) << 8)
#define 	AVIVO_DC_I2C_WR_INTRA_BYTE_DELAY(x)				((x) << 16)
#define 	AVIVO_DC_I2C_TIME_LIMIT(x)						((x) << 24)
#define AVIVO_DC_I2C_DATA									0x7d44
#define AVIVO_DC_I2C_INTERRUPT_CONTROL						0x7d48
#define 	AVIVO_DC_I2C_INTERRUPT_STATUS					(1 << 0)
#define 	AVIVO_DC_I2C_INTERRUPT_AK						(1 << 8)
#define 	AVIVO_DC_I2C_INTERRUPT_ENABLE					(1 << 16)
#define AVIVO_DC_I2C_ARBITRATION							0x7d50
#define 	AVIVO_DC_I2C_SW_WANTS_TO_USE_I2C					(1 << 0)
#define 	AVIVO_DC_I2C_SW_CAN_USE_I2C						(1 << 1)
#define 	AVIVO_DC_I2C_SW_DONE_USING_I2C					(1 << 8)
#define 	AVIVO_DC_I2C_HW_NEEDS_I2C						(1 << 9)
#define 	AVIVO_DC_I2C_ABORT_HDCP_I2C						(1 << 16)
#define 	AVIVO_DC_I2C_HW_USING_I2C						(1 << 17)

#define AVIVO_DC_GPIO_DDC1_MASK								0x7e40
#define AVIVO_DC_GPIO_DDC1_A								0x7e44
#define AVIVO_DC_GPIO_DDC1_EN								0x7e48
#define AVIVO_DC_GPIO_DDC1_Y								0x7e4c

#define AVIVO_DC_GPIO_DDC2_MASK								0x7e50
#define AVIVO_DC_GPIO_DDC2_A								0x7e54
#define AVIVO_DC_GPIO_DDC2_EN								0x7e58
#define AVIVO_DC_GPIO_DDC2_Y								0x7e5c

#define AVIVO_DC_GPIO_DDC3_MASK								0x7e60
#define AVIVO_DC_GPIO_DDC3_A								0x7e64
#define AVIVO_DC_GPIO_DDC3_EN								0x7e68
#define AVIVO_DC_GPIO_DDC3_Y								0x7e6c

#define AVIVO_DISP_INTERRUPT_STATUS							0x7edc
#define 	AVIVO_D1_VBLANK_INTERRUPT						(1 << 4)
#define 	AVIVO_D2_VBLANK_INTERRUPT						(1 << 5)


#endif /* __AVIVO_REG_H__ */
