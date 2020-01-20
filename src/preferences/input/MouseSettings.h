/*
 * Copyright 2019, Haiku, Inc.
 * Distributed under the terms of the MIT License.
 *
 * Author:
 *		Preetpal Kaur <preetpalok123@gmail.com>
 */


#ifndef MOUSE_SETTINGS_H
#define MOUSE_SETTINGS_H


#include <Input.h>
#include <InterfaceDefs.h>
#include <Point.h>
#include <SupportDefs.h>

#include "kb_mouse_settings.h"


class BPath;

class MouseSettings {
public:
		MouseSettings();
		~MouseSettings();

		void Revert();
		bool IsRevertable();
		void Defaults();
		bool IsDefaultable();
		void Dump();

		BPoint WindowPosition() const { return fWindowPosition; }
		void SetWindowPosition(BPoint corner);

		int32 MouseType() const { return fSettings.type; }
		void SetMouseType(int32 type);

		bigtime_t ClickSpeed() const;
		void SetClickSpeed(bigtime_t click_speed);

		int32 MouseSpeed() const { return fSettings.accel.speed; }
		void SetMouseSpeed(int32 speed);

		int32 AccelerationFactor() const { return fSettings.accel.accel_factor; }
		void SetAccelerationFactor(int32 factor);

		uint32 Mapping(int32 index) const;
		void Mapping(mouse_map &map) const;
		void SetMapping(int32 index, uint32 button);
		void SetMapping(mouse_map &map);

		mode_mouse MouseMode() const { return fMode; }
		void SetMouseMode(mode_mouse mode);

		mode_focus_follows_mouse FocusFollowsMouseMode() const {
			return fFocusFollowsMouseMode;
		}
		void SetFocusFollowsMouseMode(mode_focus_follows_mouse mode);

		bool AcceptFirstClick() const { return fAcceptFirstClick; }
		void SetAcceptFirstClick(bool accept_first_click);

private:
		static status_t _GetSettingsPath(BPath &path);
		void _RetrieveSettings();
		status_t _SaveSettings();

		mouse_settings	fSettings, fOriginalSettings;
		mode_mouse		fMode, fOriginalMode;
		mode_focus_follows_mouse	fFocusFollowsMouseMode;
		mode_focus_follows_mouse	fOriginalFocusFollowsMouseMode;
		bool			fAcceptFirstClick, fOriginalAcceptFirstClick;
		BPoint			fWindowPosition;
};

#endif	// MOUSE_SETTINGS_H
