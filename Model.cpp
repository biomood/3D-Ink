/*
 *  Model.cpp
 *  3d Ink
 *
 *  Created by David Roberts on 25/07/2009.
 *	Singleton
 */
#include "Controller.h"
#include "Model.h"

Model* Model::modelInstance=NULL;

Model::Model() {
	if (Settings::dataFlag==LIST)
		data = new DS_List();
	else if (Settings::dataFlag==QUADTREE)
		data = new DS_QuadTree();
	else if (Settings::dataFlag==PRQUADTREE) {
		data = new PR_Quadtree(Settings::pixelWidth, Settings::pixelHeight);
	}
}

Model::~Model() {
}

/* Adds given point to chosen data structre */
void Model::addPoint(Point p) {
	p.colour = colour;
	data->addPoint(p);
}

/* Adds a rectange to the scene 
	Use line drawing algorithm instead */
void Model::addRectangle(Point startRect, Point endRect) {	
	int x0, y0, x1, y1;
	x0 = startRect.pixelX;
	y0 = startRect.pixelY;
	x1 = endRect.pixelX;
	y1 = endRect.pixelY;
	
	printf("x0=%d y0=%d x1=%d y1=%d \n", x0, y0, x1, y1);
	// Bresenham's line algorithm
	bool steep = (abs(y1-y0) > abs(x1-x0));
	if (steep) {
		//swap
		int temp;
		temp = x0;
		x0 = y0;
		y0 = temp;
		
		temp = x1;
		x1 = y1;
		y1 = temp;
	}
	if (x0 > x1) {
		int temp;
		temp = x0;
		x0 = x1;
		x1 = temp;
		
		temp = y0;
		y0 = y1;
		y1 = temp;
	}
	int deltaX = x1 - x0;
	int deltaY = abs(y1 - y0);
	GLfloat error = 0;
	GLfloat deltaError = deltaY / deltaX;
	
	int yStep;
	int y = y0;
	
	if (y0 < y1) {
		yStep = 1;
	}
	else {
		yStep = -1;
	}
	
	for (int x=x0; x<=x1; x++) {
		printf("Iteration of the loop \n");
		
		if (steep) {
			printf("added steep point x=%d y=%d \n", y, x);
			addPoint(Controller::scaleCoords(y, x));
		}
		else {
			printf("added unsteep point x=%d y=%d \n", x, y);
			addPoint(Controller::scaleCoords(x, y));
		}
		error = error + deltaError;
		if (error >= 0.5) {
			y = y + yStep;
			error = error -1.0;
		}
	}
	Point p;
	p.blankPoint = true;
	addPoint(p);
}

/* Returns a pointer to the data */
DataStructure* Model::getData() {
	return data;
}

/* Deletes the point at given x, y, z */
void Model::deletePoint(Point *p) {
	// Timing
	clock_t start,finish;
	double totalTime;
	if(Settings::dErasingTime)
		start = clock();
	
	data->deletePoint((*p));
	
	// Timing
	if(Settings::dErasingTime) {
		finish = clock();
		totalTime = (double(finish)-double(start))/CLOCKS_PER_SEC;
		printf("Time to erase in seconds %f \n", totalTime);
	}
}

void Model::setColour(float red, float green, float blue) {
	Colour newColour;
	newColour.red = red;
	newColour.green = green;
	newColour.blue = blue;
	colour = newColour;
}

/* Returns the current selected colour */
Colour Model::getColour() {
	return colour;
}

void Model::clearData() {
	data->clearData();
}

/* Saves the file */
void Model::saveFile(string fName) {
	file = fopen(fName.c_str(), "w");
	data->saveData(file);
	fprintf(file, "END");
}

/* Loads the file 
 modify this bit here so that this opens xml */
bool Model::loadFile(string fName) {
	if((file = fopen(fName.c_str(), "r"))!=NULL) {
		char type[12];
		fscanf(file, "%s", type);
		
		if (strcmp(type, "LIST")==0) {
			printf("Found a list \n");
			data = new DS_List();
			Settings::dataFlag = LIST;
		}
		else if (strcmp(type, "PRQUADTREE")==0) {
			printf("Found a quadtree \n");
			data = new PR_Quadtree(Settings::pixelWidth, Settings::pixelHeight);
			Settings::dataFlag = PRQUADTREE;
		}
		
		data->loadData(file);
		return true;
	}
	else {
		return false;
	}

}

void Model::saveXML(string fName) {
	data->saveXML(fName);
}

bool Model::loadXML(string fName) {
	if (fopen(fName.c_str(), "r")!=NULL) {
		printf("Tried opening the file \n");
		XMLNode sketchNode=XMLNode::openFileHelper(fName.c_str(),"sketch");
		printf("Opening the file \n");
		
		
		//XMLNode sketchNode = xMainNode.getChildNode("sketch");
		string type = sketchNode.getAttribute("type");
		printf("Opened sketch node \n");
		
		printf("Type of data is %s \n", type.c_str());
		
		bool success;
		if (type=="list") {
			printf("Got to the list if statement \n");
			data = new DS_List();
			Settings::dataFlag = LIST;
		}
		else if (type=="sketch") {
			data = new PR_Quadtree(Settings::pixelWidth, Settings::pixelHeight);
			Settings::dataFlag = PRQUADTREE;
		}
		else {
			printf("ERROR: Unknown sketch type");
			success = false;
			return success;
		}
	
		data->loadXML(sketchNode);
		return success;
	}
	else {
		return false;
	}

}

Model* Model::getInstance() {
	if(modelInstance==NULL) {
		modelInstance = new Model();
	}
	return modelInstance;
}