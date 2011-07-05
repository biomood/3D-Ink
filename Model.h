/*
 *  Model.h
 *  3d Ink
 *
 *  Created by David Roberts on 22/07/2009.
 *  Durham University
 *	Singleton
 *
 */
#include <string>
#include <cmath>
#include "Settings.h"
#include "UtilityClasses.h"
#include "DataStructure.h"
#include "FileGUI.h"
#include "xmlParser.h"

using namespace std;

#if !defined(Model_H)
#define Model_H

class Model {
private:
	static Model *modelInstance; 
	DataStructure *data;
	Colour colour;
	GLfloat sceneWidth;
	GLfloat sceneHeight;
	FILE *file;
	
	Model();
	
	~Model();
	
public:
	GLdouble vAngle;
	
	static Model *getInstance();
	
	void addPoint(Point p);
	
	void addRectangle(Point startRect, Point endRect);
	
	DataStructure* getData();
	
	void deletePoint(Point *p);
	
	void setColour(float red, float green, float blue);
	
	Colour getColour();
	
	void clearData();
	
	void saveFile(string fName);
	
	bool loadFile(string fName);
	
	void saveXML(string fName);
	
	bool loadXML(string fName);
};

#endif
