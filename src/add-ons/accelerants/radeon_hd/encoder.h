/*
 * Copyright 2006-2011, Haiku, Inc. All Rights Reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *      Alexander von Gluck, kallisti5@unixzen.com
 */
#ifndef RADEON_HD_ENCODER_H
#define RADEON_HD_ENCODER_H


#include <SupportDefs.h>

void encoder_init();

void encoder_assign_crtc(uint8 crtcID);
uint32 encoder_pick_dig(uint32 connectorIndex);

void encoder_apply_quirks(uint8 crtcID);
void encoder_mode_set(uint8 crtcID);
status_t encoder_analog_setup(uint32 connectorIndex,
	uint32 pixelClock, int command);
status_t encoder_digital_setup(uint32 connectorIndex,
	uint32 pixelClock, int command);
status_t encoder_dig_setup(uint32 connectorIndex,
	uint32 pixelClock, int command);
status_t encoder_external_setup(uint32 connectorIndex, int command);
status_t encoder_tv_setup(uint32 connectorIndex,
	uint32 pixelClock, int command);

bool encoder_analog_load_detect(uint32 connectorIndex);
bool encoder_dac_load_detect(uint32 connectorIndex);
bool encoder_dig_load_detect(uint32 connectorIndex);
void encoder_output_lock(bool lock);
status_t transmitter_dig_setup(uint32 connectorIndex, uint32 pixelClock,
	uint8 laneNumber, uint8 laneSet, int command);
void encoder_crtc_scratch(uint8 crtcID);
void encoder_dpms_scratch(uint8 crtcID, bool power);
void encoder_dpms_set(uint8 crtcID, int mode);
void encoder_dpms_set_dig(uint8 crtcID, int mode);
void encoder_dpms_set_dvo(uint8 crtcID, int mode);

const char* encoder_name_lookup(uint32 encoderID);
uint32 encoder_object_lookup(uint32 connectorFlags, uint8 dacID);
uint32 encoder_type_lookup(uint32 encoderID, uint32 connectorFlags);
bool encoder_is_external(uint32 encoderID);
bool encoder_is_dp_bridge(uint32 encoderID);


#endif /* RADEON_HD_ENCODER_H */
