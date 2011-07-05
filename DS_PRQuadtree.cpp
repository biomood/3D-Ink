/*
 *  DS_PRQuadtree.cpp
 *  3d Ink
 *
 *  Created by David Roberts on 01/12/2009.
 */
#include "DataStructure.h"

PR_Quadtree::PR_Quadtree(GLfloat width, GLfloat height) {
	this->width = width;
	this->height = height;
	root = NULL;
};

PR_Quadtree::~PR_Quadtree() {
	
};

void PR_Quadtree::addPoint(Point p) {
	Area a;
	a.tL.x = 0.0;
	a.tL.y = 0.0;
	a.bR.x = width;
	a.bR.y = height;
	
	//printf("addpoint thinks x=%d y=%d \n", p.pixelX, p.pixelY);
	insert(a, NULL, &root, &p);
};

/* Recursive function for adding a node */
void PR_Quadtree::insert(Area area, PRNode *parent, PRNode **n, Point *p) {
	//printf("p has the value x=%d, y=%d \n", p->pixelX, p->pixelY);
	// N is NULL
	if ((*n)==NULL) {
		//printf("Node n is null \n");
		
		(*n)=new PRNode((*p));
		(*n)->isLeaf = true;
		(*n)->parent = parent;
		
		(*n)->NW = NULL;
		(*n)->NE = NULL;
		(*n)->SW = NULL;
		(*n)->SE = NULL;
		
		//printf("Made a leaf node at x=%d y=%d \n \n", p->pixelX, p->pixelY);
		return;
	}
	// N is a leaf
	else if ((*n)->isLeaf) {
		//printf("Node n is a leaf \n");
		
		// The node to insert is the same as the leaf found
		if ((p->pixelX==(*n)->p.pixelX)&&(p->pixelY==(*n)->p.pixelY)) {
			//printf("P z=%f and N z=%f \n", p->z, (*n)->p.z);
			
			if (p->z!=(*n)->p.z) {
				(*n)->zList.push_back(p);
			}
			else {
				//printf("Leaf and p are the same \n \n");
				(*n)->p = *p;
			}
			return;
		}
		
		(*n)->isLeaf = false;
		Point pCopy = (*n)->p;
		
		GLfloat mPX = (area.tL.x + area.bR.x)/2;
		GLfloat mPY = (area.tL.y + area.bR.y)/2;
		//printf("Middle point x=%f \n", mPX);
		//printf("Middle point y=%f \n", mPY);
		
		//printf("Made an internal node \n");
		// Find where the copy of p has to go
		if ((pCopy.pixelX <= mPX)&&(pCopy.pixelY <= mPY)) { //NW
			(*n)->NW = new PRNode(pCopy);
			(*n)->NW->parent = (*n);
			(*n)->NW->isLeaf = true;
			
			(*n)->NW->NW=NULL;
			(*n)->NW->NE=NULL;
			(*n)->NW->SW=NULL;
			(*n)->NW->SE=NULL;
			
			//printf("Put copy of p at NW as leaf \n \n");
		}
		else if ((pCopy.pixelX > mPX)&&(pCopy.pixelY <= mPY)) { //NE
			(*n)->NE = new PRNode(pCopy);
			(*n)->NE->parent = (*n);
			(*n)->NE->isLeaf = true;
			
			(*n)->NE->NW=NULL;
			(*n)->NE->NE=NULL;
			(*n)->NE->SW=NULL;
			(*n)->NE->SE=NULL;
			
			//printf("Put copy of p at NE as leaf \n \n");
		}
		else if ((pCopy.pixelX <= mPX)&&(pCopy.pixelY > mPY)) { //SW
			(*n)->SW = new PRNode(pCopy);
			(*n)->SW->parent = (*n);
			(*n)->SW->isLeaf = true;
			
			(*n)->SW->NW=NULL;
			(*n)->SW->NE=NULL;
			(*n)->SW->SW=NULL;
			(*n)->SW->SE=NULL;
			
			//printf("Put copy of p at SW as leaf \n \n");
		}
		else if ((pCopy.pixelX > mPX)&&(pCopy.pixelY > mPY)) { //SE
			(*n)->SE = new PRNode(pCopy);
			(*n)->SE->parent = (*n);
			(*n)->SE->isLeaf = true;
			
			(*n)->SE->NW=NULL;
			(*n)->SE->NE=NULL;
			(*n)->SE->SW=NULL;
			(*n)->SE->SE=NULL;
			
			//printf("Put copy of p at SE as leaf \n \n");
		}
		
		
		insert(area, (*n)->parent, n, p);
		
	}
	// N is an internal node
	else if (!(*n)->isLeaf) {
		//printf("Node n is an internal node \n");
		
		// Find middle point
		GLfloat mPX = (area.tL.x + area.bR.x)/2;
		GLfloat mPY = (area.tL.y + area.bR.y)/2;
		//printf("Middle point x=%f \n", mPX);
		//printf("Middle point y=%f \n", mPY);
		
		if ((p->pixelX <= mPX)&&(p->pixelY <= mPY)) { //NW
			
			Area aa;
			aa.tL = area.tL;
			aa.bR.x = (area.tL.x + area.bR.x)/2;
			aa.bR.y = (area.tL.y + area.bR.y)/2;
			
			//printf("Followed NW path \n \n");
			insert(aa, (*n), &(*n)->NW, p);
		}
		else if ((p->pixelX > mPX)&&(p->pixelY <= mPY)) { //NE
			Area aa;
			aa.tL.x = (area.tL.x + area.bR.x)/2;
			aa.tL.y = area.tL.y;
			aa.bR.x = area.bR.x;
			aa.bR.y = (area.tL.y + area.bR.y)/2;
			
			//printf("Followed NE path \n \n");
			insert(aa, (*n), &(*n)->NE, p);
		}
		else if ((p->pixelX <= mPX)&&(p->pixelY > mPY)) { //SW
			Area aa;
			aa.tL.x = area.tL.x;
			aa.tL.y = (area.tL.y + area.bR.y)/2;
			aa.bR.x = (area.tL.x + area.bR.x)/2;
			aa.bR.y = area.bR.y;
			
			//printf("Followed SW path \n \n");
			insert(aa, (*n), &(*n)->SW, p);
		}
		else if ((p->pixelX > mPX)&&(p->pixelY > mPY)) { //SE
			Area aa;
			aa.tL.x = (area.tL.x + area.bR.x)/2;
			aa.tL.y = (area.tL.y + area.bR.y)/2;
			aa.bR   =  area.bR;
			
			//printf("Followed SE path \n \n");
			insert(aa, (*n), &(*n)->SE, p);
		}
		
	}
	
 	
};

void* PR_Quadtree::getData() {
	return this;
};

void PR_Quadtree::deletePoint(Point p) {
	//printf("Delete is being called \n\n");
	
	Area a;
	a.tL.x = 0.0;
	a.tL.y = 0.0;
	a.bR.x = width;
	a.bR.y = height;
	
	int s;
	for (s=6; s>0; --s) {
		Point p1 = p;
		p1.pixelX+=s;
		p1.pixelY+=s;
		deleteP(a, NULL, &root, &p1);
		
		Point p2 =p;
		p2.pixelX-=s;
		p2.pixelY-=s;
		deleteP(a, NULL, &root, &p2);
	}
	
	deleteP(a, NULL, &root, &p);
};

void PR_Quadtree::deleteP(Area area, PRNode *parent, PRNode **n, Point *p) {
	//printf("p has the value x=%d, y=%d \n", p->pixelX, p->pixelY);
	// N is NULL
	if ((*n)==NULL) {
		//printf("Node n is null \n");
		
		//printf("No Node found \n\n");
		return;
	}
	// N is a leaf
	else if ((*n)->isLeaf) {
		if ((p->pixelX==(*n)->p.pixelX)&&(p->pixelY==(*n)->p.pixelY)) {
			//printf("Found the correct point to delete \n");
			
			// If p is in the zList
			if (p->z!=(*n)->p.z) {
				for (int i=0; i<(*n)->zList.size(); i++) {
					if ((*n)->zList[i]->z==p->z) {
						(*n)->zList.erase((*n)->zList.begin()+i);
						
						//printf("Deleted the node \n");
						return;
					}
				}
			}
			else if (p->z==(*n)->p.z) {
				if ((*n)->zList.empty()) {
					//PRNode *parent = (*n)->parent;
					(*n)=NULL;
					//printf("Deleted the node \n");
					//followParent(&parent);
					return;
				}
				else {
					(*n)->p = *(*n)->zList[0];
					(*n)->zList.erase((*n)->zList.begin());
					
					//printf("Deleted the node \n");
					return;
				}
			}

		}
	}
	// N is an internal node
	else if (!(*n)->isLeaf) {
		//printf("Node n is an internal node \n");
		
		// Find middle point
		GLfloat mPX = (area.tL.x + area.bR.x)/2;
		GLfloat mPY = (area.tL.y + area.bR.y)/2;
		//printf("Middle point x=%f \n", mPX);
		//printf("Middle point y=%f \n", mPY);
		
		if ((p->pixelX <= mPX)&&(p->pixelY <= mPY)) { //NW
			
			Area aa;
			aa.tL = area.tL;
			aa.bR.x = (area.tL.x + area.bR.x)/2;
			aa.bR.y = (area.tL.y + area.bR.y)/2;
			
			//printf("Followed NW path \n \n");
			deleteP(aa, (*n), &(*n)->NW, p);
		}
		else if ((p->pixelX > mPX)&&(p->pixelY <= mPY)) { //NE
			Area aa;
			aa.tL.x = (area.tL.x + area.bR.x)/2;
			aa.tL.y = area.tL.y;
			aa.bR.x = area.bR.x;
			aa.bR.y = (area.tL.y + area.bR.y)/2;
			
			//printf("Followed NE path \n \n");
			deleteP(aa, (*n), &(*n)->NE, p);
		}
		else if ((p->pixelX <= mPX)&&(p->pixelY > mPY)) { //SW
			Area aa;
			aa.tL.x = area.tL.x;
			aa.tL.y = (area.tL.y + area.bR.y)/2;
			aa.bR.x = (area.tL.x + area.bR.x)/2;
			aa.bR.y = area.bR.y;
			
			//printf("Followed SW path \n \n");
			deleteP(aa, (*n), &(*n)->SW, p);
		}
		else if ((p->pixelX > mPX)&&(p->pixelY > mPY)) { //SE
			Area aa;
			aa.tL.x = (area.tL.x + area.bR.x)/2;
			aa.tL.y = (area.tL.y + area.bR.y)/2;
			aa.bR   =  area.bR;
			
			//printf("Followed SE path \n \n");
			deleteP(aa, (*n), &(*n)->SE, p);
		}
		
	}
	
};

/* Follow the parent node back up the tree, removes parent, keeps quadtree consistent */
void PR_Quadtree::followParent(PRNode **n) {
	//printf("called this \n");
	
	// 3 nodes have data, just return, don't need to recurse
	if (((*n)->NW==NULL && (*n)->NE!=NULL && (*n)->SW!=NULL && (*n)->SE!=NULL) ||
		((*n)->NW!=NULL && (*n)->NE==NULL && (*n)->SW!=NULL && (*n)->SE!=NULL) ||
		((*n)->NW!=NULL && (*n)->NE!=NULL && (*n)->SW==NULL && (*n)->SE!=NULL) ||
		((*n)->NW!=NULL && (*n)->NE!=NULL && (*n)->SW!=NULL && (*n)->SE==NULL)) {
		return;
	}
	// 2 nodes have data, just return, don't need to recurse
	else if (((*n)->NW==NULL && (*n)->NE==NULL && (*n)->SW!=NULL && (*n)->SE!=NULL) ||
		((*n)->NW==NULL && (*n)->NE!=NULL && (*n)->SW==NULL && (*n)->SE!=NULL) ||
		((*n)->NW==NULL && (*n)->NE!=NULL && (*n)->SW!=NULL && (*n)->SE==NULL) ||
		((*n)->NW!=NULL && (*n)->NE==NULL && (*n)->SW==NULL && (*n)->SE!=NULL) ||
		((*n)->NW!=NULL && (*n)->NE==NULL && (*n)->SW!=NULL && (*n)->SE==NULL) ||
		((*n)->NW!=NULL && (*n)->NE!=NULL && (*n)->SW==NULL && (*n)->SE==NULL)) {
		return;
	}
	// 1 node has data, fun times happen, cant be 0 or I've royally f****d this up
	else {
		// Find the node that has data
		PRNode *node;
		if ((*n)->NW!=NULL) {
			(*n)->NW->parent = (*n)->parent;
			node = (*n)->NW;
		}
		else if ((*n)->NE!=NULL) {
			(*n)->NE->parent = (*n)->parent;
			node = (*n)->NE;
		}
		else if ((*n)->SW!=NULL) {
			(*n)->SW->parent = (*n)->parent;
			node = (*n)->SW;
		}
		else if ((*n)->SE!=NULL) {
			(*n)->SE->parent = (*n)->parent;
			node = (*n)->SE;
		}
		
		if ((*n)->parent==NULL) {
			return;
		}
		else if ((*n)->parent!=NULL) {
			// Now link up child of n properly
			if ((*n)->parent->NW==(*n)) {
				(*n)->parent->NW = node;
			}
			else if ((*n)->parent->NE==(*n)) {
				(*n)->parent->NE = node;
			}
			else if ((*n)->parent->SW==(*n)) {
				(*n)->parent->SW = node;
			}
			else if ((*n)->parent->SE==(*n)) {
				(*n)->parent->SE = node;
			}
		}
		
		followParent(&(*n)->parent);
		
	}

	
	
}

/* Sets the root node to NULL, effectively emptying the tree */
void PR_Quadtree::clearData() {
	queue<PRNode*> q;
	q.push(root);
	
	while (!q.empty()) {
		PRNode *n = q.front();
		q.pop();
		
		if (n!=NULL) {
			if (!n->isLeaf) {
				q.push(n->NW);
				q.push(n->NE);
				q.push(n->SW);
				q.push(n->SE);
				
				delete n;
			}
			else if (n->isLeaf) {
				delete n;
			}
		}
	}
	
	root = NULL;
};

/* Draws the quadtree, performs a BFS on the tree */
void PR_Quadtree::drawScene() {
	queue<PRNode*> q;
	q.push(root);
	
	glPushMatrix();

	while (!q.empty()) {
		PRNode *n = q.front();
		q.pop();
		
		if (n!=NULL) {
			if (n->isLeaf) {
				glPointSize(n->p.pointSize);
				
				glBegin(GL_POINTS);
				glColor3f(n->p.colour.red, n->p.colour.green, n->p.colour.green);
				glVertex3f(n->p.x, n->p.y, n->p.z);
				glEnd();
				
				if (!n->zList.empty()) {
					for (int i=0; i<n->zList.size(); i++) {
						glColor3f(n->zList[i]->colour.red, n->zList[i]->colour.red, n->zList[i]->colour.red);
						glVertex3f(n->p.x, n->p.y, n->p.z);
					}
				}
			}
			else if(!n->isLeaf) {
				q.push(n->NW);
				q.push(n->NE);
				q.push(n->SW);
				q.push(n->SE);
			}
		}
	}

	glPopMatrix();
};

void PR_Quadtree::saveData(FILE *file) {
	fprintf(file, "PRQUADTREE \n");
	
	queue<PRNode*> q;
	q.push(root);
	
	while (!q.empty()) {
		PRNode *n = q.front();
		q.pop();
		
		if (n!=NULL) {
			if (n->isLeaf) {
				savePoint(n->p, file);
				if (!n->zList.empty()) {
					for (int i=0; i<n->zList.size(); i++) {
						savePoint(n->p, file);
					}
				}
			}
			else if(!n->isLeaf) {
				q.push(n->NW);
				q.push(n->NE);
				q.push(n->SW);
				q.push(n->SE);
			}
		}
	}
}

void PR_Quadtree::savePoint(Point p, FILE *file) {
	fprintf(file, "POINT \n");
	fprintf(file, "X %f \n", p.x);
	fprintf(file, "Y %f \n", p.y);
	fprintf(file, "Z %f \n", p.z);
	fprintf(file, "PIXELX %d \n", p.pixelX);
	fprintf(file, "PIXELY %d \n", p.pixelY);
	fprintf(file, "Red %f \n", p.colour.red);
	fprintf(file, "Green %f \n", p.colour.green);
	fprintf(file, "Blue %f \n", p.colour.blue);
	fprintf(file, "PointSize %f \n", p.pointSize);
	fprintf(file, "BLANK %d \n", p.blankPoint);
	fprintf(file, "\n");
}

void PR_Quadtree::loadData(FILE *file) {
	printf("Opening the file: QUADTREE \n");
	int pi=0;
	
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
			
			// Don't want to add a blankpoint, will crash PRQuadtree, badly
			if (!p.blankPoint) {
				this->addPoint(p);
				printf("\n");
				printf("Added a point %d \n \n", pi);
				pi++;
			}
		}
		else if (strcmp(sh, "END")==0) {
			fclose(file);
			break;
		}
	}
}

void PR_Quadtree::saveXML(string fName) {
	
}

void PR_Quadtree::loadXML(XMLNode sketchNode) {
	
}