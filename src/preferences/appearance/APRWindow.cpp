/*
 * Copyright 2002-2011, Haiku. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		DarkWyrm (darkwyrm@earthlink.net)
 *		Alexander von Gluck, kallisti5@unixzen.com
 *		Stephan Aßmus <superstippi@gmx.de>
 */


#include "APRWindow.h"

#include <Button.h>
#include <Catalog.h>
#include <LayoutBuilder.h>
#include <Locale.h>
#include <Messenger.h>
#include <SeparatorView.h>
#include <TabView.h>
#include "AntialiasingSettingsView.h"
#include "APRView.h"
#include "defs.h"
#include "FontView.h"
#include "LookAndFeelSettingsView.h"
#include "boolx.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "APRWindow"


static const uint32 kMsgSetDefaults = 'dflt';
static const uint32 kMsgRevert = 'rvrt';
static const uint32 kdarkmode =  'dark';

APRWindow::APRWindow(BRect frame)
	:
	BWindow(frame, B_TRANSLATE_SYSTEM_NAME("Appearance                                                                                                                                                                                                                                                                       "), B_TITLED_WINDOW,
		B_NOT_RESIZABLE | B_NOT_ZOOMABLE | B_AUTO_UPDATE_SIZE_LIMITS
			| B_QUIT_ON_WINDOW_CLOSE, B_ALL_WORKSPACES)
{
	fDefaultsButton = new BButton("defaults", B_TRANSLATE("Defaults"),
		new BMessage(kMsgSetDefaults), B_WILL_DRAW);


	fDarkModeButton = new BButton("darkmode", B_TRANSLATE("Dark Mode"),
		new BMessage(kdarkmode), B_WILL_DRAW);

	fRevertButton = new BButton("revert", B_TRANSLATE("Revert"),
		new BMessage(kMsgRevert), B_WILL_DRAW);

	BTabView* tabView = new BTabView("tabview", B_WIDTH_FROM_LABEL);

	fFontSettings = new FontView(B_TRANSLATE("Fonts"));

	fColorsView = new APRView(B_TRANSLATE("Colors"));

	fLookAndFeelSettings = new LookAndFeelSettingsView(
		B_TRANSLATE("Look and feel"));

	fAntialiasingSettings = new AntialiasingSettingsView(
		B_TRANSLATE("Antialiasing"));

	tabView->AddTab(fFontSettings);
	tabView->AddTab(fColorsView);
	tabView->AddTab(fLookAndFeelSettings);
	tabView->AddTab(fAntialiasingSettings);
	tabView->SetBorder(B_NO_BORDER);

	_UpdateButtons();

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.SetInsets(0, B_USE_DEFAULT_SPACING, 0, B_USE_DEFAULT_SPACING)
		.Add(tabView)
		.Add(new BSeparatorView(B_HORIZONTAL))
		.AddGroup(B_HORIZONTAL)
			.Add(fDefaultsButton)
			.Add(fRevertButton)
			.Add(fDarkModeButton)
			.SetInsets(B_USE_WINDOW_SPACING, B_USE_DEFAULT_SPACING,
				B_USE_DEFAULT_SPACING, 0)
			.AddGlue();
}


void
APRWindow::MessageReceived(BMessage *message)
{
	switch (message->what) {
		case kMsgUpdate:
			_UpdateButtons();
			break;

		case kMsgSetDefaults:
			fFontSettings->SetDefaults();
			fColorsView->SetDefaults();
			fLookAndFeelSettings->SetDefaults();
			fAntialiasingSettings->SetDefaults();
			_UpdateButtons();
			break;

		case kMsgRevert:
			fFontSettings->Revert();
			fColorsView->Revert();
			fLookAndFeelSettings->Revert();
			fAntialiasingSettings->Revert();
			_UpdateButtons();
			break;

		default:
			BWindow::MessageReceived(message);
			break;
	}
}


void
APRWindow::_UpdateButtons()
{
	fDefaultsButton->SetEnabled(_IsDefaultable());
	fRevertButton->SetEnabled(_IsRevertable());
	fDarkModeButton->SetEnabled(_IsDarkable());
}


bool
APRWindow::_IsDefaultable() const
{
	is_dark = false;
	return fFontSettings->IsDefaultable()
		|| fColorsView->IsDefaultable()
		|| fLookAndFeelSettings->IsDefaultable()
		|| fAntialiasingSettings->IsDefaultable();
}


bool
APRWindow::_IsRevertable() const
{
	is_dark = false;
	return fFontSettings->IsRevertable()
		|| fColorsView->IsRevertable()
		|| fLookAndFeelSettings->IsRevertable()
		|| fAntialiasingSettings->IsRevertable();
}

bool
APRWindow::_IsDarkable() const 
{
	is_dark = true;
	return fFontSettings->IsDefaultable()
		|| fColorsView->IsDarkable()
		|| fLookAndFeelSettings->IsDefaultable()
		|| fAntialiasingSettings->IsDefaultable();


}