/*
 *  UtilityClasses.h
 *  3d Ink
 *
 *  Created by David Roberts on 23/07/2009.
 *  Durham University
 */
#include <vector>
#include <GLUT/glut.h>
using namespace std;

#if !defined(UtilityClasses_H)
#define UtilityClasses_H

/* Struct for storing colour information */
typedef struct Colour {
	float red;
	float green;
	float blue;
};

/* A struct just to store point data, used in DataStructures */
typedef struct Point {
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat pointSize;
	// if this point marks the start of a new line
	bool blankPoint;
	Colour colour;
	
	int pixelX;
	int pixelY;
};


/* For use with the PR Quadtree */
class PRNode {
public:
	Point p;	
	PRNode *NW, *NE, *SW, *SE;
	// Stores a refernce to the parent of the node
	PRNode *parent;
	vector<Point*> zList;
	bool isLeaf;
	bool isValid;
	
	PRNode() {
		isValid = false;
	}
	
	PRNode(bool valid) {
		isValid = valid;
	}
	
	PRNode(Point p) {
		this->p = p;
	}
	
	~PRNode() {
		
	}
};

class ONode {
public:
	Point p;
	// Near
	ONode *N_NW, *N_NE, *N_SW, *N_SE;
	// Far
	ONode *F_NW, *F_NE, *F_SW, *F_SE;
	// Stores a refernce to the parent of the node
	ONode *parent;
	bool isLeaf;
	bool isValid;
	
	ONode() {
		isValid = false;
	}
	
	ONode(bool valid) {
		isValid = valid;
	}
	
	ONode(Point p) {
		this->p = p;
	}
	
	~ONode() {
		
	}
};

/* Node struct for Quadtree */
typedef struct Node {
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat pointSize;
	Colour colour;
	
	int type;
	
	Node *NW;
	Node *NE;
	Node *SW;
	Node *SE;
};

typedef struct Co {
	GLfloat x;
	GLfloat y;
};

typedef struct Co3D {
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

typedef struct Area {
	Co tL;
	Co bR;
};

typedef struct Area3D {
	Co3D topLeftForward;
	Co3D bottomRightBackward;
};

/* Uses when making shapes */
enum Shape {
	Rectangle,
	Circle,
	Undefined
};

#endif