/*
 *  Settings.h
 *  3d Ink
 *
 *  Created by David Roberts on 27/07/2009.
 *  Durham University
 *	Static
 */
#include <GL/glfw.h>
#include <GLUT/GLUT.h>

#define LIST 1
#define QUADTREE 2
#define PRQUADTREE 3
#define OCTREE 4

#define STEREOMODE 111
#define MONOMODE 112
#define ANAGLYPHMODE 113

#if !defined(SETTINGS_H)
#define SETTINGS_H

class Settings {
public:
	static const GLdouble viewingDistance = 700;
	static const GLdouble displayWidth = 320;
	
	// Display settings
	static GLdouble pixelWidth;
	static GLdouble pixelHeight;
	
	// Scene data
	static GLfloat sceneWidth;
	static GLfloat sceneHeight;
	static GLfloat vAngle;
	
	// Control levels of depth
	//static const GLdouble gpdInFront = 50;
	//static const GLdouble gpdBehind = 50;
	//static const GLdouble sceneDepth = 100;
	
	// More depth, but more strain
	static const GLdouble gpdInFront = 50;
	static const GLdouble gpdBehind = 50;
	static const GLdouble sceneDepth = 100;
	
	static int stereoMode;
	static const bool fullScreen = true;
	static int dataFlag;
	
	// Debug
	static const bool dFPS = false;
	static const bool dPoints = false;
	static const bool dDrawingTime = false;
	static const bool dErasingTime = false;
	static const int dNoPoints = 100000;
	static const bool dTree = false;
	static const bool dPRSize = false;
};

#endif