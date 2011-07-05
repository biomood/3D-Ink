/*
 *  DS_LastNode_Quadtree.cpp
 *  3d Ink
 *
 *  Created by David Roberts on 06/07/2010.
 *
 */
#include "DataStructure.h"

DS_Octree::DS_Octree(GLfloat width, GLfloat height, GLfloat depth) {
	this->width = width;
	this->height = height;
	this->depth = depth;
	root = NULL;
};

DS_Octree::~DS_Octree() {
	
};

void DS_Octree::addPoint(Point p) {
	Area3D a;
	a.topLeftForward.x = 0.0;
	a.topLeftForward.y = 0.0;
	a.topLeftForward.z = Settings::viewingDistance-Settings::gpdBehind;
	
	a.bottomRightBackward.x = width;
	a.bottomRightBackward.y = height;
	a.bottomRightBackward.z = Settings::viewingDistance+Settings::gpdInFront;
	
	insert(a, NULL, &root, &p);
};

void DS_Octree::insert(Area3D area, ONode *parent, ONode **n, Point *p) {
	if ((*n)==NULL) {
		(*n)=new ONode((*p));
		(*n)->isLeaf = true;
		(*n)->parent = parent;
		
		(*n)->N_NW = NULL;
		(*n)->N_NE = NULL;
		(*n)->N_SW = NULL;
		(*n)->N_SE = NULL;
		
		(*n)->F_NW = NULL;
		(*n)->F_NE = NULL;
		(*n)->F_SW = NULL;
		(*n)->F_SE = NULL;
		
		//printf("Made a leaf node at x=%d y=%d \n \n", p->pixelX, p->pixelY);
		return;
	}
};

void* DS_Octree::getData() {
	return this;
};

void DS_Octree::deletePoint(Point p) {
	
};

void DS_Octree::clearData() {
	
};

void DS_Octree::drawScene() {
	
};

void DS_Octree::saveData(FILE *file) {
	
};

void DS_Octree::loadData(FILE *file) {
	
};

void DS_Octree::saveXML(string fName) {
	
}

void DS_Octree::loadXML(XMLNode sketchNode) {
	
}
