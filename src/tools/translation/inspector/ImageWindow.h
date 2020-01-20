/*****************************************************************************/
// ImageWindow
// Written by Michael Wilber, OBOS Translation Kit Team
//
// ImageWindow.h
//
// BWindow class for displaying an image.  Uses ImageView class for its
// view.
//
//
// Copyright (c) 2003 OpenBeOS Project
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
/*****************************************************************************/

#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H

#include <Window.h>
#include <FilePanel.h>
#include "ImageView.h"

class ImageWindow : public BWindow {
public:
	ImageWindow(BRect rect, const char *name);
	~ImageWindow();
	void MessageReceived(BMessage *pmsg);
	bool QuitRequested();

private:
	BFilePanel *fpopenPanel;
	ImageView *fpimageView;
};

#endif // #ifndef IMAGEWINDOW_H
