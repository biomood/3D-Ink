/*
 *  ImageBrowser.cpp
 *  3d Ink
 *
 *  Created by David Roberts on 27/01/2010.
 *
 */
#include "FileGUI.h"
#define OK 1
#define CANCEL 0

ImageBrowser* ImageBrowser::ptr=NULL;

ImageBrowser::ImageBrowser(int mainWindow, string *fName, bool *done) {
	this->mainWindow = mainWindow;
	this->fName = fName;
	this->done = done;
	ptr = this;
}

ImageBrowser::~ImageBrowser() {
	
}

void ImageBrowser::buildInterface() {
	glui = GLUI_Master.create_glui("Image Browser");
	glui->set_main_gfx_window(mainWindow);
	fileText = glui->add_edittext("Image file", GLUI_EDITTEXT_TEXT, NULL, NULL);
	fileText->set_w(200);
	GLUI_Panel *panel = glui->add_panel("buttonPanel", GLUI_PANEL_NONE);
	glui->add_button_to_panel(panel, "OK", OK, &process);
	glui->add_column_to_panel(panel, false);
	glui->add_button_to_panel(panel, "Cancel", CANCEL, &process);
}

void ImageBrowser::closeWindow() {
	glui->close();
}

void ImageBrowser::getText() {
	*fName = fileText->get_text();
};

void ImageBrowser::process(int id) {
	switch (id) {
		case OK:
			ptr->getText();
			ptr->closeWindow();
			*ptr->done = true;
			break;
		case CANCEL:
			ptr->closeWindow();
			break;
		default:
			break;
	}
}