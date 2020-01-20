/*
 * Copyright 2006-2012 Haiku, Inc. All Rights Reserved.
 * Copyright 1997, 1998 R3 Software Ltd. All Rights Reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Stephan Aßmus, superstippi@gmx.de
 *		John Scipione, jscipione@gmail.com
 *		Timothy Wayper, timmy@wunderbear.com
 */
#ifndef CALC_OPTIONS_H
#define CALC_OPTIONS_H


#include <SupportDefs.h>


enum {
	KEYPAD_MODE_COMPACT,
	KEYPAD_MODE_BASIC,
	KEYPAD_MODE_SCIENTIFIC
};

class BMessage;

struct CalcOptions {
	bool auto_num_lock;		// automatically activate numlock
	bool audio_feedback;	// provide audio feedback
	bool degree_mode;		// radian or degree mode
	uint8 keypad_mode;		// keypad mode options

				CalcOptions();

	void		LoadSettings(const BMessage* archive);
	status_t	SaveSettings(BMessage* archive) const;
};

#endif // CALC_OPTIONS_H
