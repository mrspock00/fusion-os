/*
 * MainApp.h - Media Player for the Haiku Operating System
 *
 * Copyright (C) 2006 Marcus Overhagen <marcus@overhagen.de>
 * Copyright (C) 2008 Stephan Aßmus <superstippi@gmx.de>
 *
 * Released under the terms of the MIT license.
 */
#ifndef __MAIN_APP_H
#define __MAIN_APP_H


#include <Application.h>
#include <Entry.h>

#include "MainWin.h"


enum  {
	M_NEW_PLAYER				= 'nwpl',
	M_PLAYER_QUIT				= 'plqt',
	M_SETTINGS					= 'stng',

	M_SHOW_OPEN_PANEL			= 'swop',
	M_SHOW_SAVE_PANEL			= 'swsp',
		// "target"		- This BMessenger will be sent the result message.
		// "message"	- This message will be sent back to the target, with
		//				the additional fields that a BFilePanel provides.
		//				If no result message is specified, the constants below
		//				will be used.
		// "title"		- String that will be used to name the panel window.
		// "label"		- String that will be used to name the Default button.

	M_OPEN_PANEL_RESULT			= 'oprs',
	M_SAVE_PANEL_RESULT			= 'sprs',

	M_OPEN_PREVIOUS_PLAYLIST	= 'oppp',

	M_URL_RECEIVED				= 'urrc'
};


#define NAME "MediaPlayer"


class BFilePanel;
class SettingsWindow;


class MainApp : public BApplication {
public:
								MainApp();
	virtual						~MainApp();

			MainWin*			NewWindow(BMessage* message = NULL);
			int32				PlayerCount() const;

private:
	virtual	bool				QuitRequested();
	virtual	void				ReadyToRun();
	virtual	void				RefsReceived(BMessage* message);
	virtual	void				ArgvReceived(int32 argc, char** argv);
	virtual	void				MessageReceived(BMessage* message);

private:
			void				_ShowSettingsWindow();
			void				_BroadcastMessage(const BMessage& message);

			void				_ShowOpenFilePanel(const BMessage* message);
			void				_ShowSaveFilePanel(const BMessage* message);
			void				_ShowFilePanel(BFilePanel* panel,
									uint32 command, const BMessage* message,
									const char* defaultTitle,
									const char* defaultLabel);

			void				_HandleOpenPanelResult(
									const BMessage* message);
			void				_HandleSavePanelResult(
									const BMessage* message);
			void				_HandleFilePanelResult(BFilePanel* panel,
									const BMessage* message);

			void				_StoreCurrentPlaylist(
									const BMessage* message) const;
			status_t			_RestoreCurrentPlaylist(
									BMessage* message) const;

			void				_InstallPlaylistMimeType();

private:
			int32				fPlayerCount;
			SettingsWindow*		fSettingsWindow;

			BFilePanel*			fOpenFilePanel;
			BFilePanel*			fSaveFilePanel;
			entry_ref			fLastFilePanelFolder;

			bool				fAudioWindowFrameSaved;
			bigtime_t			fLastSavedAudioWindowCreationTime;
};

extern MainApp* gMainApp;
extern const char* kAppSig;

#endif	// __MAIN_APP_H
