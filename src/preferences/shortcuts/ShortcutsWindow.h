/*
 * Copyright 1999-2009 Jeremy Friesner
 * Copyright 2009-2010 Haiku, Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Jeremy Friesner
 */
#ifndef SHORTCUTS_WINDOW_H
#define SHORTCUTS_WINDOW_H


#include <ColumnListView.h>
#include <Entry.h>
#include <Window.h>


class BButton;
class BColumnListView;
class BFilePanel;
class BMessage;
class ShortcutsSpec;

// This class defines our preferences/configuration window.
class ShortcutsWindow : public BWindow {
public:
							ShortcutsWindow();
							~ShortcutsWindow();

	virtual	void 			DispatchMessage(BMessage* message,
								BHandler* handler);
	virtual	void 			Quit();
	virtual	void 			MessageReceived(BMessage* message);
	virtual	bool 			QuitRequested();

	// BMessage 'what' codes, representing commands understood by this Window.
	enum {
		ADD_HOTKEY_ITEM = 'SpKy',	// Add a new hotkey entry to the GUI list.
		REMOVE_HOTKEY_ITEM,			// Remove a hotkey entry from the GUI list.
		HOTKEY_ITEM_SELECTED,		// Give the "focus bar" to the specified
									// entry.
		HOTKEY_ITEM_MODIFIED,		// Update the state of an entry to reflect
									// user's changes.
		OPEN_KEYSET,				// Bring up a File requester to load new
									// settings.
		APPEND_KEYSET,				// Bring up a File requester to append
									// settings.
		REVERT_KEYSET,				// Dump the current state and re-read
									// settings from disk.
		SAVE_KEYSET,				// Save the current settings to disk
		SAVE_KEYSET_AS,				// Bring up a File requester to save
									// current settings.
		SELECT_APPLICATION,			// Set the current entry to point to the
									// given file.
	};

private:
			BMenuItem* 			_CreateActuatorPresetMenuItem(const char* label)
									const;
			void 				_AddNewSpec(const char* defaultCommand);
			void 				_MarkKeySetModified();
			bool 				_LoadKeySet(const BMessage& loadMessage);
			bool 				_SaveKeySet(BEntry& saveEntry);
			bool				_GetSettingsFile(entry_ref* ref);
			void 				_LoadWindowSettings(
									const BMessage& loadMessage);
			void 				_SaveWindowSettings(BEntry& saveEntry);
			bool				_GetWindowSettingsFile(entry_ref* ref);

			BButton*			fAddButton;
			BButton*			fRemoveButton;
			BButton*			fSaveButton;
			BColumnListView*	fColumnListView;
			BFilePanel*			fSavePanel;
				// for saving settings
			BFilePanel*			fOpenPanel;
				// for loading settings
			BFilePanel*			fSelectPanel;
				// for selecting apps to launch

			// Points to the settings file to save to
			BEntry				fLastSaved;

			// true iff changes were made since last load or save
			bool				fKeySetModified;

			// true iff the file-requester's ref should be appended to current
			bool				fLastOpenWasAppend;

			BRow*				fSelectedRow;
};


#endif	// SHORTCUTS_WINDOW_H
