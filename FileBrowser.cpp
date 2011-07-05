/*
 *  FileBrowser.cpp
 *  3D Editor
 *
 *  Created by David Roberts on 15/11/2009.
 */
#include <exception>
#include "FileGUI.h"
#define OK 1
#define CANCEL 0

FileBrowser* FileBrowser::ptr=NULL;

FileBrowser::FileBrowser(int mainWindow, string *fName, bool* done) {
	this->mainWindow = mainWindow;
	this->fName = fName;
	this->done = done;
	ptr = this;
};

FileBrowser::FileBrowser(int mainWindow, string *fName) {
	this->mainWindow = mainWindow;
	this->fName = fName;
	ptr = this;
};

void FileBrowser::buildInterface() {
	glui = GLUI_Master.create_glui("Load Sketch File");
	glui->set_main_gfx_window(mainWindow);
	fileText = glui->add_edittext("Sketch file", GLUI_EDITTEXT_TEXT, NULL, NULL);
	fileText->set_w(200);
	GLUI_Panel *panel = glui->add_panel("buttonPanel", GLUI_PANEL_NONE);
	glui->add_button_to_panel(panel, "OK", OK, &process);
	glui->add_column_to_panel(panel, false);
	glui->add_button_to_panel(panel, "Cancel", CANCEL, &process);	
};

void FileBrowser::process(int id) {
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
	
};

void FileBrowser::closeWindow() {
	glui->close();
};

void FileBrowser::getText() {
	*fName = fileText->get_text();
};