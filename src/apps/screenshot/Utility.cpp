/*
 * Copyright 2010-2014, Haiku Inc. All rights reserved.
 * Copyright 2010 Wim van der Meer <WPJvanderMeer@gmail.com>
 * Copyright Karsten Heimrich, host.haiku@gmx.de. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Axel Dörfler
 *		Karsten Heimrich
 *		Fredrik Modéen
 *		Christophe Huriaux
 *		Wim van der Meer
 */


#include "Utility.h"

#include <Bitmap.h>
#include <BitmapStream.h>
#include <Catalog.h>
#include <Clipboard.h>
#include <Entry.h>
#include <File.h>
#include <FindDirectory.h>
#include <Locale.h>
#include <Looper.h>
#include <MimeType.h>
#include <NodeInfo.h>
#include <Path.h>
#include <Region.h>
#include <Screen.h>
#include <String.h>
#include <Translator.h>
#include <View.h>

#include <AutoDeleter.h>


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Screenshot"


const char* Utility::sDefaultFileNameBase = B_TRANSLATE_MARK_COMMENT(
	"screenshot", "Base filename of screenshot files");


Utility::Utility()
	:
	wholeScreen(NULL),
	cursorBitmap(NULL),
	cursorAreaBitmap(NULL)
{
}


Utility::~Utility()
{
	delete wholeScreen;
	delete cursorBitmap;
	delete cursorAreaBitmap;
}


void
Utility::CopyToClipboard(const BBitmap& screenshot) const
{
	if (be_clipboard->Lock()) {
		be_clipboard->Clear();
		BMessage* clipboard = be_clipboard->Data();
		if (clipboard != NULL) {
			BMessage* bitmap = new BMessage();
			screenshot.Archive(bitmap);
			clipboard->AddMessage("image/bitmap", bitmap);
			be_clipboard->Commit();
		}
		be_clipboard->Unlock();
	}
}


/*!	Save the screenshot to the file with the specified filename and type.
	Note that any existing file with the same filename will be overwritten
	without warning.
*/
status_t
Utility::Save(BBitmap* screenshot, const char* fileName, uint32 imageType)
	const
{
	BString fileNameString(fileName);

	// Generate a default filename when none is given
	if (fileNameString.Compare("") == 0) {
		BPath homePath;
		if (find_directory(B_USER_DIRECTORY, &homePath) != B_OK)
			return B_ERROR;

		BEntry entry;
		int32 index = 1;
		BString extension = FileNameExtension(imageType);
		do {
			fileNameString.SetTo(homePath.Path());
			fileNameString << "/" << B_TRANSLATE_NOCOLLECT(sDefaultFileNameBase)
				<< index++ << extension;
			entry.SetTo(fileNameString);
		} while (entry.Exists());
	}

	// Create the file
	BFile file(fileNameString, B_CREATE_FILE | B_ERASE_FILE | B_WRITE_ONLY);
	if (file.InitCheck() != B_OK)
		return B_ERROR;

	// Write the screenshot bitmap to the file
	BBitmapStream stream(screenshot);
	BTranslatorRoster* roster = BTranslatorRoster::Default();
	status_t status = roster->Translate(&stream, NULL, NULL, &file, imageType,
		B_TRANSLATOR_BITMAP);

	BBitmap* bitmap;
	stream.DetachBitmap(&bitmap);
		// The stream takes over ownership of the bitmap

	if (status != B_OK)
		return status;

	// Set the file MIME attribute (don't mind too much if this fails)
	BNodeInfo nodeInfo(&file);
	if (nodeInfo.InitCheck() == B_OK)
		nodeInfo.SetType(_MimeType(imageType));

	return B_OK;
}


BBitmap*
Utility::MakeScreenshot(bool includeMouse, bool activeWindow,
	bool includeBorder) const
{
	if (wholeScreen == NULL)
		return NULL;

	int cursorWidth = 0;
	int cursorHeight = 0;

	if (cursorBitmap != NULL) {
		BRect bounds = cursorBitmap->Bounds();
		cursorWidth = bounds.IntegerWidth() + 1;
		cursorHeight = bounds.IntegerHeight() + 1;
	}

	if (includeMouse && cursorBitmap != NULL) {
		// Import the cursor bitmap into wholeScreen
		wholeScreen->ImportBits(cursorBitmap->Bits(),
			cursorBitmap->BitsLength(), cursorBitmap->BytesPerRow(),
			cursorBitmap->ColorSpace(), BPoint(0, 0), cursorPosition,
			cursorWidth, cursorHeight);

	} else if (cursorAreaBitmap != NULL) {
		// Import the cursor area bitmap into wholeScreen
		wholeScreen->ImportBits(cursorAreaBitmap->Bits(),
			cursorAreaBitmap->BitsLength(), cursorAreaBitmap->BytesPerRow(),
			cursorAreaBitmap->ColorSpace(), BPoint(0, 0), cursorPosition,
			cursorWidth, cursorHeight);
	}

	BBitmap* screenshot = NULL;

	if (activeWindow && activeWindowFrame.IsValid()) {
		BRect frame(activeWindowFrame);
		if (includeBorder) {
			frame.InsetBy(-borderSize, -borderSize);
			frame.top -= tabFrame.bottom - tabFrame.top;
		}

		screenshot = new BBitmap(frame.OffsetToCopy(B_ORIGIN),
			includeBorder ? B_RGBA32 : B_RGB32, true);

		if (screenshot->ImportBits(wholeScreen->Bits(),
				wholeScreen->BitsLength(), wholeScreen->BytesPerRow(),
				wholeScreen->ColorSpace(), frame.LeftTop(),
				BPoint(0, 0), frame.IntegerWidth() + 1,
				frame.IntegerHeight() + 1) != B_OK) {
			delete screenshot;
			return NULL;
		}

		if (includeBorder)
			_MakeTabSpaceTransparent(screenshot, frame);
	} else
		screenshot = new BBitmap(wholeScreen);

	return screenshot;
}


BString
Utility::FileNameExtension(uint32 imageType) const
{
	BMimeType mimeType(_MimeType(imageType));

	BMessage message;
	if (mimeType.GetFileExtensions(&message) == B_OK) {
		BString extension;
		if (message.FindString("extensions", 0, &extension) == B_OK) {
			extension.Prepend(".");
			return extension;
		}
	}

	return "";
}


status_t
Utility::FindTranslator(uint32 imageType, translator_id& id,
	BString* _mimeType) const
{
	translator_id* translators = NULL;
	int32 numTranslators = 0;

	BTranslatorRoster* roster = BTranslatorRoster::Default();
	status_t status = roster->GetAllTranslators(&translators, &numTranslators);
	if (status != B_OK)
		return status;

	ArrayDeleter<translator_id> deleter(translators);

	for (int32 x = 0; x < numTranslators; x++) {
		const translation_format* formats = NULL;
		int32 numFormats;

		if (roster->GetOutputFormats(translators[x], &formats, &numFormats)
				== B_OK) {
			for (int32 i = 0; i < numFormats; ++i) {
				if (formats[i].type == imageType) {
					id = translators[x];
					if (_mimeType != NULL)
						*_mimeType = formats[i].MIME;
					return B_OK;
				}
			}
		}
	}

	return B_ERROR;
}


BString
Utility::_MimeType(uint32 imageType) const
{
	translator_id id;
	BString type;
	FindTranslator(imageType, id, &type);
	return type;
}


/*!	Makes the space around the tab transparent, and also makes sure that the
	contents of the window aren't, as the screen does not have an alpha channel.
*/
void
Utility::_MakeTabSpaceTransparent(BBitmap* screenshot, BRect frame) const
{
	if (!frame.IsValid() || screenshot->ColorSpace() != B_RGBA32)
		return;

	// Set the transparency to opaque on the complete bitmap
	uint8* pixel = (uint8*)screenshot->Bits();
	uint32 count = screenshot->BitsLength();
	for (uint32 i = 0; i < count; i += 4) {
		pixel[i + 3] = 255;
	}

	// Then make the space around the tab transparent
	if (!frame.Contains(tabFrame))
		return;

	float tabHeight = tabFrame.bottom - tabFrame.top;

	BRegion tabSpace(frame);
	frame.OffsetBy(0, tabHeight);
	tabSpace.Exclude(frame);
	tabSpace.Exclude(tabFrame);
	frame.OffsetBy(0, -tabHeight);
	tabSpace.OffsetBy(-frame.left, -frame.top);
	BScreen screen;
	BRect screenFrame = screen.Frame();
	tabSpace.OffsetBy(-screenFrame.left, -screenFrame.top);

	BView view(screenshot->Bounds(), "bitmap", B_FOLLOW_ALL_SIDES, 0);
	screenshot->AddChild(&view);
	if (view.Looper() && view.Looper()->Lock()) {
		view.SetDrawingMode(B_OP_COPY);
		view.SetHighColor(B_TRANSPARENT_32_BIT);

		for (int i = 0; i < tabSpace.CountRects(); i++)
			view.FillRect(tabSpace.RectAt(i));

		view.Sync();
		view.Looper()->Unlock();
	}
	screenshot->RemoveChild(&view);
}
