/*
 *  DataStructure.h
 *  3d Ink
 *
 *  Created by David Roberts on 22/07/2009.
 *  Durham Univserity
 *  
 *  Used as an abstract header for basic data sturcure
 *  all data structures used must include this header 
 *	and implement the virtual methods
 */
#include <GLUT/glut.h>
#include <vector>
#include <cmath>
#include <queue>
#include <string>
#include "UtilityClasses.h"
#include "Settings.h"
#include "xmlParser.h"

using namespace std;

#if !defined(DataStructure_H)
#define DataStructure_H

class DataStructure {
public:
	
	/* Adds a point to the vector */
	virtual void addPoint(Point p)=0;
	
	/* Returns a pointer to the data */
	virtual void* getData()=0;
	
	/* Deletes the point at given x, y, z */
	virtual void deletePoint(Point p)=0;
	
	/* Clears the data structure */
	virtual void clearData()=0;
	
	/* Draws the data strcuture */
	virtual void drawScene()=0;
	
	/* Saves the data structure */
	virtual void saveData(FILE *file)=0;
	
	/* loads the data from a file */
	virtual void loadData(FILE *file)=0;
	
	virtual void saveXML(string fName)=0;
	
	virtual void loadXML(XMLNode sketchNode)=0;
};

/* List structure */
class DS_List: public DataStructure {
private:
	vector<Point> pointList;	
public:
	DS_List();
	
	~DS_List();
	
	/* Add a point to the data */
	void addPoint(Point p);
	
	/* Returns a pointer to the vector */
	void* getData();
	
	void deletePoint(Point p);
	
	void clearData();
	
	void drawScene();
	
	void saveData(FILE *file);
	
	void loadData(FILE *file);
	
	void loadXML(XMLNode sketchNode);
	
	void saveXML(string fName);
};

/* Point Region Quadtree */
class PR_Quadtree : public DataStructure {
private:
	GLfloat width, height;
	
	/* Recursive function for inserting */
	void insert(Area area, PRNode *parent, PRNode **n, Point *p);
	
	/* Recursive function for deleting */
	void deleteP(Area area, PRNode *parent, PRNode **n, Point *p);
	
	/* Follows and recurses back up the tree */
	void followParent(PRNode **n);
	
	/* Saves point */
	void savePoint(Point p, FILE *file);
	
	
public:
	PRNode *root;
	
	PR_Quadtree(GLfloat width, GLfloat height);
	
	~PR_Quadtree();
	
	void addPoint(Point p);
	
	void* getData();
	
	void deletePoint(Point p);
	
	void clearData();
	
	void drawScene();
	
	void saveData(FILE *file);
	
	void loadData(FILE *file);
	
	void loadXML(XMLNode sketchNode);
	
	void saveXML(string fName);
};

/* LastNode Quadtree */
class DS_Octree : public DataStructure {
private:
	GLfloat width, height, depth;
	
	/* Recursive fucntion or inserting */
	void insert(Area3D area, ONode *parent, ONode **n, Point *p);
	
public:
	ONode *root;
	
	DS_Octree(GLfloat width, GLfloat height, GLfloat depth);
	
	~DS_Octree();
	
	void addPoint(Point p);
	
	void* getData();
	
	void deletePoint(Point p);
	
	void clearData();
	
	void drawScene();
	
	void saveData(FILE *file);
	
	void loadData(FILE *file);
	
	void loadXML(XMLNode sketchNode);
	
	void saveXML(string fName);
};

/* Quad Tree */
class DS_QuadTree : public DataStructure {
private:
	/* Recursive method for adding a child node to quadtree */
	void processAddPoint(Node *parent, Node *child);
	
	/* Recursive method for deleting a node from a quadtree */
	void processDeletePoint(Node *parent, Node *toDelete);
	
	Node* findConjugateNode(Node *n);
	
	/* Finds which subtree should be expanded */
	int findConjugate(int n);
	
	/* Replaces the node to be deleted with the selected replacement */
	void replaceNode(Node *toDelete, Node *repl);
	
public:
	Node *root;
	
	DS_QuadTree();
	
	~DS_QuadTree();
	
	void addPoint(Point p);
	
	void* getData();
	
	void deletePoint(Point p);
	
	void clearData();
	
	void drawScene();
	
	void saveData(FILE *file);
	
	void loadData(FILE *file);
	
	void loadXML(XMLNode sketchNode);
	
	void saveXML(string fName);
};

#endif