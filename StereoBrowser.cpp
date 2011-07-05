/*
 *  StereoBrowser.cpp
 *  3d Ink
 *
 *  Created by David Roberts on 03/02/2010.
 */
#include "FileGUI.h"
#define OK 1
#define CANCEL 0

StereoBrowser *StereoBrowser::ptr=NULL;

StereoBrowser::StereoBrowser(int mainWindow, string *lName, string *rName, bool *done) {
	this->mainWindow = mainWindow;
	this->lName = lName;
	this->rName = rName;
	this->done = done;
	ptr = this;
}

StereoBrowser::~StereoBrowser() {
	
}

void StereoBrowser::buildInterface() {
	glui = GLUI_Master.create_glui("Import Stereo Images");
	glui->set_main_gfx_window(mainWindow);
	leftText = glui->add_edittext("Left Image", GLUI_EDITTEXT_TEXT, NULL, NULL);
	leftText->set_w(200);
	rightText = glui->add_edittext("Right Image", GLUI_EDITTEXT_TEXT, NULL, NULL);
	rightText->set_w(200);
	GLUI_Panel *panel = glui->add_panel("buttonPanel", GLUI_PANEL_NONE);
	glui->add_button_to_panel(panel, "OK", OK, &process);
	glui->add_column_to_panel(panel, false);
	glui->add_button_to_panel(panel, "Cancel", CANCEL, &process);	
}

void StereoBrowser::process(int id) {
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

void StereoBrowser::closeWindow() {
	glui->close();
}

void StereoBrowser::getText() {
	*lName = leftText->get_text();
	*rName = rightText->get_text();
}










































