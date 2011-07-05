/*
 *  ErrorMessage.cpp
 *  3D Ink
 *
 *  Created by David Roberts on 28/12/2009.
 */
#include "FileGUI.h"
#define OK 1

ErrorMessage *ErrorMessage::ptr=NULL;

ErrorMessage::ErrorMessage(int mainWindow, string error) {
	this->mainWindow = mainWindow;
	this->error = error;
	ptr = this;
}

ErrorMessage::~ErrorMessage() {
	
}

void ErrorMessage::buildInterface() {
	glui = GLUI_Master.create_glui("Error Message");
	glui->set_main_gfx_window(mainWindow);
	glui->add_statictext(error.c_str());
	glui->add_button("OK", OK, &process);
}

void ErrorMessage::process(int id) {
	switch (id) {
		case OK:
			ptr->closeWindow();
			break;
		default:
			break;
	}
}

void ErrorMessage::closeWindow() {
	glui->close();
}