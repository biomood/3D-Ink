/*
 *  Controller.h
 *  3d Ink
 *
 *  Created by David Roberts on 26/07/2009.
 *  Durham University
 *
 */
#include <GLUT/glut.h>
#include <GL/glfw.h>
#include <cmath>
#include "Model.h"
#include "Settings.h"
#include "UtilityClasses.h"
using namespace std;

#if !defined(CONTROLLER_H)
#define CONTROLLER_H

class Controller {
private:
	static bool pressed, scrollDown, scrollUp;
	static int xTablet, yTablet;
	static Model* modelPointer;
public:
	static int noTabletButtons;
	static int xMouse, yMouse;
	static GLfloat z;
	//if true, pen, else eraser
	static bool penType;
	static GLfloat pointSize;
	static bool shape;
	static Shape shapeType;
	static Point startRect;
	static Point endRect;
	
	Controller();
	
	~Controller();
	
	/* Handles mouse button events */
	static void processMouseButton(int button, int state, int x, int y);
	
	/* Handles movement of a mouse when a button is clicked */
	static void processMouseActiveMotion(int x, int y);
	
	/* Handles movement of a mouse when no button is clicked */
	static void processMousePassiveMotion(int x, int y);
	
	/* Handles standard keyboard events */
	static void handleKeyboard(unsigned char key, int xpos, int ypos);
	
	/* Handles special keyboard events */
	static void handleSpecialKeys(int key, int x, int y);
	
	/* Handles tablet button events */
	static void processTabletButton(int button, int state, int x, int y);
	
	/* Handles tablet movement */
	static void processTabletMotion(int x, int y);
	
	/* Handles scaling of coordinates */
	static Point scaleCoords(int x, int y);
};

#endif
