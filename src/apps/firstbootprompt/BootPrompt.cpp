/*
 * Copyright 2010, Stephan Aßmus <superstippi@gmx.de>
 * All rights reserved. Distributed under the terms of the MIT License.
 */


#include "BootPrompt.h"

#include <stdlib.h>

#include <Catalog.h>
#include <LaunchRoster.h>
#include <Locale.h>

#include "BootPromptWindow.h"


static int sExitValue;


int
main(int, char **)
{
	BootPromptApp app;
	app.Run();
	return sExitValue;
}


// #pragma mark -


const char* kAppSignature = "application/x-vnd.Haiku-FirstBootPrompt";


BootPromptApp::BootPromptApp()
	:
	BApplication(kAppSignature)
{
}


void
BootPromptApp::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case MSG_BOOT_DESKTOP:
			BLaunchRoster().Target("desktop");
			PostMessage(B_CLOSE_REQUESTED);
			sExitValue = 1;
			break;
		case MSG_RUN_INSTALLER:
			BLaunchRoster().Target("installer");
			PostMessage(B_CLOSE_REQUESTED);
			sExitValue = 0;
			break;

		default:
			BApplication::MessageReceived(message);
	}
}


void
BootPromptApp::ReadyToRun()
{
	// Prompt the user to select his preferred language.
	new BootPromptWindow();
}

