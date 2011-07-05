/*
 *  Controller.cpp
 *  3d Ink
 *
 *  Created by David Roberts on 26/07/2009.
 *
 */
#include "Controller.h"

#if !defined(GLUT_WHEEL_UP)
#  define GLUT_WHEEL_UP   3
#  define GLUT_WHEEL_DOWN 4
#endif

bool Controller::pressed = false;
bool Controller::scrollDown = false;
bool Controller::scrollUp = false;
GLfloat Controller::z = 700;
int Controller::xMouse = 0;
int Controller::yMouse = 0;
int Controller::xTablet = 0;
int Controller::yTablet = 0;
int Controller::noTabletButtons = 0;
bool newLine = false;
//if true, pen, else eraser
bool Controller::penType = true;
bool Controller::shape = false;
GLfloat Controller::pointSize = 3.0f;
Shape Controller::shapeType = Undefined;
Point Controller::startRect;
Point Controller::endRect;


Controller::Controller() {
}

Controller::~Controller() {
}

/* Handles mouse button events */
void Controller::processMouseButton(int button, int state, int x, int y) {
	switch (button) {
		case GLUT_LEFT_BUTTON:
			//printf("Mouse x=%d y=%d \n", x, y);
			
			if(state==GLUT_DOWN) {			
				pressed = true;
				newLine = true;
				
				// If user has selected shape, then need to get start and end points
				if (shape) {
					switch (shapeType) {
						case Rectangle:
							printf("Tried to make a rectangle \n");
							startRect = scaleCoords(x, y);
							break;
						default:
							break;
					}
				}
			}
			if(state==GLUT_UP) {
				pressed = false;
				// When mouse is lifted, put a blank space
				// we can then use GL_LINE when drawing
				if(newLine==true&&Settings::dataFlag==LIST&&!shape) {
					Point p;
					p.blankPoint = true;
					Model::getInstance()->addPoint(p);
					newLine = false;
				}
				else if (shape) {
					endRect = scaleCoords(x, y);
					printf("Got to the end of the rect \n");
					Model::getInstance()->addRectangle(startRect, endRect);
				}
				
			}
			break;
		case GLUT_WHEEL_UP:
			scrollUp = true;
			break;
		case GLUT_WHEEL_DOWN:
			scrollDown = true;
			break;
		default:
			break;
	}
}

/* Handles movement of the mouse when a button is clicked */
void Controller::processMouseActiveMotion(int x, int y) {
	xMouse=x;
	yMouse=y;
	
	// If drawing with pen and no shape
	if(pressed &!shape) {
		Point p;
		p = scaleCoords(x, y);
		
		if(penType) {
			//Add point to the model
			Model::getInstance()->addPoint(p);
		}
		else {
			Model::getInstance()->deletePoint(&p);
		}
	}
	// If scrolling up, go into screen else go out
	if(scrollUp) {
		z-=10;
		scrollUp = false;
	}
	if(scrollDown) {
		z+=10;
		scrollDown = false;
	}
}

/* Handles movement of a mouse when no button is clicked */
void Controller::processMousePassiveMotion(int x, int y) {
	xMouse = x;
	yMouse = y;
}

/* Handles standard keyboard events */
void Controller::handleKeyboard(unsigned char key, int xpos, int ypos) {
	// elegent eh?
	if(key==27)
		exit(0);
}

/* Handles special keyboard events (ALT, UP, etc...) */
void Controller::handleSpecialKeys(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_UP:
			if (z==(Settings::viewingDistance+Settings::gpdInFront-10)) {
				z = z;
			}
			else {
				z+=5;
			}
			//printf("Z value is %f \n", z);
			break;
		case GLUT_KEY_DOWN:
			if (z==(Settings::viewingDistance-Settings::gpdBehind)) {
				z = z;
			}
			else {
				z-=5;
			}
			//printf("Z value is %f \n", z);
			break;
		default:
			break;
	}
}

/* Handles tablet button events */
void Controller::processTabletButton(int button, int state, int x, int y) {
	//TODO: implement processTabletButton
	// GLUT has bad tablet handling!
}

/* Handles tablet movement */
void Controller::processTabletMotion(int x, int y) {
	xTablet=x;
	yTablet=y;
	
	Point p;
	p = scaleCoords(x, y);
	
	// Add the point to the Model
	Model::getInstance()->addPoint(p);
}

/* Handles scaling of coordinates */
Point Controller::scaleCoords(int x, int y) {
	Point p;
	
	GLdouble scale = (((z*tan(Model::getInstance()->vAngle/2))*2)/Settings::displayWidth);
	
	p.x = (((Settings::pixelWidth/2)-xMouse)/
		   (Settings::pixelWidth/Settings::displayWidth))*scale;
	p.y = (((Settings::pixelHeight/2)-yMouse)/
		   (Settings::pixelWidth/Settings::displayWidth))*scale;
	p.z = z;
	p.blankPoint = false;
	p.pointSize = Controller::pointSize;
	
	//printf("Point added is x=%f y=%f z=%f \n", p.x, p.y, p.z);
	
	p.pixelX = x;
	p.pixelY = y;
	
	return p;
}
