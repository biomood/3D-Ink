/*
 *  FileBrowser.h
 *  3D Editor
 *
 *  Created by David Roberts on 15/11/2009.
 *
 */
#include <string>
#include <GLUT/GLUT.h>
#include <GLUI/GLUI.h>
using namespace std;

#define OPENFILE 1
#define CANCELFILE 2
#define UPDATELIST 3

#if!defined(GUI_H)
#define GUI_H

class FileBrowser {
private:
	GLUI *glui;
	GLUI_EditText *fileText;
	bool selected;
	bool *done;
	int mainWindow;
	string *fName;
	static void process(int id);
	static FileBrowser *ptr;
	
public:
	FileBrowser(int mainWindow, string *fName, bool* done);
	
	FileBrowser(int mainWindow, string *fName);
	
	~FileBrowser();
	
	void buildInterface();
	
	void closeWindow();
	
	void getText();
};


class SaveBrowser  {
private:
	GLUI *glui;
	GLUI_EditText *editText;
	int mainWindow;
	string *fName;
	bool *done;
	static SaveBrowser *ptr;
	static void process(int id);
	
public:
	SaveBrowser(int mainWindow, string *fName, bool* done);
	
	SaveBrowser(int mainWindow, string *fName);
	
	~SaveBrowser();
	
	void buildInterface();
	
	void closeWindow();
	
	void getText();
};

class ErrorMessage {
private:
	GLUI *glui;
	GLUI_StaticText *statText;
	int mainWindow;
	string error;
	static ErrorMessage *ptr;
	static void process(int id);
	
public:
	ErrorMessage(int mainWindow, string error);
	
	~ErrorMessage();
	
	void buildInterface();
	
	void closeWindow();
};

class ImageBrowser {
	GLUI *glui;
	GLUI_EditText *fileText;
	bool selected;
	bool *done;
	int mainWindow;
	string *fName;
	static ImageBrowser *ptr;
	static void process(int id);
	
public:
	ImageBrowser(int mainWindow, string *fName, bool* done);
	
	~ImageBrowser();
	
	void buildInterface();
	
	void closeWindow();
	
	void getText();
};

class StereoBrowser {
	GLUI *glui;
	int mainWindow;
	static StereoBrowser *ptr;
	static void process(int id);
public:
	GLUI_EditText *leftText;
	GLUI_EditText *rightText;
	string *lName;
	string *rName;
	bool *done;
	
	StereoBrowser(int mainWindow, string *lName, string *rName, bool *done);
	
	~StereoBrowser();
	
	void buildInterface();
	
	void closeWindow();
	
	void getText();
};
#endif