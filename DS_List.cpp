/*
 *  DS_List.cpp
 *  3d Ink
 *
 *  Created by David Roberts on 22/07/2009.
 */

#include "DataStructure.h"
using namespace std;

DS_List::DS_List() {
	
}

DS_List::~DS_List() {
	
}

/* Adds a point to the vector */
void DS_List::addPoint(Point p) {
	// Add to back of vector (resizes vector if needed)
	pointList.push_back(p);
}

/* Returns a pointer to the vector */
void* DS_List::getData() {
	return this;
}

/* Deletes the point at given x, y, z */
void DS_List::deletePoint(Point p) {
	// Actual Erasing here
	for(int i=0; i<pointList.size(); ++i) {
		Point* pp = &pointList[i];
		if(((*pp).x>=p.x-1 && (*pp).x<=p.x+1) && ((*pp).y>=p.y-1 && (*pp).y<=p.y+1) && ((*pp).z=p.z)) {
			//pointList.erase(pointList.begin()+i);
			(*pp).blankPoint = true;
			//return;
		}
	}
}

/* Clears the list */
void DS_List::clearData() {
	pointList.clear();
}

/* Draws the list */
void DS_List::drawScene() {
	if(!pointList.empty())
		glLineWidth(pointList[0].pointSize);
	
	glPushMatrix();
	glBegin(GL_LINE_STRIP);
	for(int i=0; i<pointList.size(); ++i) {
		// If not blankPoint then draw line
		if(!pointList[i].blankPoint) {
			glColor3f(pointList[i].colour.red, pointList[i].colour.green , 
					  pointList[i].colour.blue);
			glVertex3f(pointList[i].x, pointList[i].y, pointList[i].z);
		}
		else {
			// If blankPoint, break the line
			glEnd();
			// Gets the next line width
			int j=i;
			j++;
			if(pointList.size() > j)
				glLineWidth(pointList[j].pointSize);
			glBegin(GL_LINE_STRIP);
		}
	}
	glEnd();
	glPopMatrix();
}

void DS_List::saveData(FILE *file) {
	fprintf(file, "LIST \n");
	
	for (int i=0; i<pointList.size(); i++) {
		fprintf(file, "POINT \n");
		fprintf(file, "X %f \n", pointList[i].x);
		fprintf(file, "Y %f \n", pointList[i].y);
		fprintf(file, "Z %f \n", pointList[i].z);
		fprintf(file, "PIXELX %d \n", pointList[i].pixelX);
		fprintf(file, "PIXELY %d \n", pointList[i].pixelY);
		fprintf(file, "Red %f \n", pointList[i].colour.red);
		fprintf(file, "Green %f \n", pointList[i].colour.green);
		fprintf(file, "Blue %f \n", pointList[i].colour.blue);
		fprintf(file, "PointSize %f \n", pointList[i].pointSize);
		fprintf(file, "BLANK %d \n", pointList[i].blankPoint);
		fprintf(file, "\n");
	}
}

void DS_List::loadData(FILE *file) {
	printf("Opening the file: LIST \n");
	
	while (!feof(file)) {
		char sh[12];
		
		fscanf(file, "%s", sh);
		
		if (strcmp(sh, "POINT")==0) {
			char blank[12];
			Point p;
			
			fscanf(file, "%s %f", blank, &p.x);
			fscanf(file, "%s %f", blank, &p.y);
			fscanf(file, "%s %f", blank, &p.z);
			fscanf(file, "%s %d", blank, &p.pixelX);
			fscanf(file, "%s %d", blank, &p.pixelY);
			fscanf(file, "%s %f", blank, &p.colour.red);
			fscanf(file, "%s %f", blank, &p.colour.green);
			fscanf(file, "%s %f", blank, &p.colour.blue);
			fscanf(file, "%s %f", blank, &p.pointSize);
			
			int i;
			fscanf(file, "%s %d", blank, &i);
			if (i==1) {
				p.blankPoint = true;
			}
			else if (i==0) {
				p.blankPoint = false;
			}
			
			this->addPoint(p);
		}
		else if (strcmp(sh, "END")==0) {
			fclose(file);
			break;
		}
	}
}

void DS_List::saveXML(string fName) {
	XMLNode xMainNode = XMLNode::createXMLTopNode("xml", TRUE);
	xMainNode.addAttribute("version", "1.0");
	XMLNode sketchNode = xMainNode.addChild("sketch");
	sketchNode.addAttribute("type", "list");
	
	for (int i=0; i<pointList.size(); i++) {
		char char_val[100];
		XMLNode pointNode = sketchNode.addChild("point");
		
		XMLNode coord = pointNode.addChild("coord");
		sprintf(char_val, "%f", pointList[i].x);
		coord.addAttribute("x", char_val);
		sprintf(char_val, "%f", pointList[i].y);
		coord.addAttribute("y", char_val);
		sprintf(char_val, "%f", pointList[i].z);
		coord.addAttribute("z", char_val);
		
		XMLNode pixelNode = pointNode.addChild("pixel");
		sprintf(char_val, "%d", pointList[i].pixelX);
		pixelNode.addAttribute("x", char_val);
		sprintf(char_val, "%d", pointList[i].pixelY);
		pixelNode.addAttribute("y", char_val);
		
		XMLNode colourNode = pointNode.addChild("colour");
		sprintf(char_val, "%f", pointList[i].colour.red);
		colourNode.addAttribute("red", char_val);
		sprintf(char_val, "%f", pointList[i].colour.green);
		colourNode.addAttribute("green", char_val);
		sprintf(char_val, "%f", pointList[i].colour.blue);
		colourNode.addAttribute("blue", char_val);
		
		XMLNode infoNode = pointNode.addChild("info");
		sprintf(char_val, "%f", pointList[i].pointSize);
		infoNode.addAttribute("size", char_val);
		sprintf(char_val, "%d", pointList[i].blankPoint);
		infoNode.addAttribute("blank", char_val);
		
	}
	
	xMainNode.writeToFile(fName.c_str(), "ISO-8859-1");
}

void DS_List::loadXML(XMLNode sketchNode) {
	//XMLNode xMainNode = XMLNode::openFileHelper(fName.c_str(), "PMML");
	//XMLNode sketchNode = xMainNode.getChildNode("sketch");
	printf("Got to the xml load method \n");
	
	int n = sketchNode.nChildNode("point");
	printf("Number of point nodes %d \n", n);
	
	
	for (int i=0; i<sketchNode.nChildNode(); i++) {
		printf("Tried to load a point \n");
		Point p;
		XMLNode pointNode  = sketchNode.getChildNode(i);
		
		XMLNode coord = pointNode.getChildNode("coord");
		p.x = atof(coord.getAttribute("x"));
		p.y = atof(coord.getAttribute("y"));
		p.z = atof(coord.getAttribute("z"));
		
		XMLNode pixelNode = pointNode.getChildNode("pixel");
		p.pixelX = atoi(pixelNode.getAttribute("x"));
		p.pixelY = atoi(pixelNode.getAttribute("y"));
		
		XMLNode colourNode = pointNode.getChildNode("colour");
		p.colour.red = atof(colourNode.getAttribute("red"));
		p.colour.green = atof(colourNode.getAttribute("green"));
		p.colour.blue = atof(colourNode.getAttribute("blue"));
		
		XMLNode infoNode = pointNode.getChildNode("info");
		p.pointSize = atof(infoNode.getAttribute("size"));
		p.blankPoint = atoi(infoNode.getAttribute("blank"));
		
		this->addPoint(p);
	}
	
}