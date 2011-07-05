/*
 *  SaveBrowser.cpp
 *  3D Editor
 *
 *  Created by David Roberts on 08/12/2009.
 */
#include "FileGUI.h"
#define OK 1
#define CANCEL 0

SaveBrowser *SaveBrowser::ptr=NULL;

SaveBrowser::SaveBrowser(int mainWindow, string *fName, bool *done) {
	this->fName = fName;
	this->mainWindow = mainWindow;
	this->done = done;
	ptr = this;
}

SaveBrowser::SaveBrowser(int mainWindow, string *fName) {
	this->fName = fName;
	this->mainWindow = mainWindow;
	ptr = this;
}

SaveBrowser::~SaveBrowser() {
	
}

void SaveBrowser::buildInterface() {
	glui = GLUI_Master.create_glui("Save Window");
	glui->set_main_gfx_window(mainWindow);
	glui->add_statictext("Enter a name for the file");
	editText = glui->add_edittext("File Name", GLUI_EDITTEXT_TEXT, NULL, NULL);
	editText->set_w(200);
	GLUI_Panel *panel = glui->add_panel("buttonPanel", GLUI_PANEL_NONE);
	glui->add_button_to_panel(panel, "OK", OK, &process);
	glui->add_column_to_panel(panel, false);
	glui->add_button_to_panel(panel, "Cancel", CANCEL, &process);
}

void SaveBrowser::process(int id) {
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

void SaveBrowser::getText() {
	// sets the name to name.3di (renamed text file)
	*fName = editText->get_text();
	*fName+=".3di";
}

void SaveBrowser::closeWindow() {
	glui->close();
}


