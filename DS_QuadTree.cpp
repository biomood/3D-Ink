/*
 *  DS_QuadTree.cpp
 *  3d Ink
 *
 *  Created by David Roberts on 14/11/2009.
 *  This is here only for interest and research, deleteing does not work!
 *	very complex for deleting a node, possible extension?
 */
#include "DataStructure.h"

DS_QuadTree::DS_QuadTree() {
	root = NULL;
};

DS_QuadTree::~DS_QuadTree() {
	
};

//TODO: implement for z 
void DS_QuadTree::addPoint(Point p) {
	//TODO: have to allow adding just one point!!!!!! 
	if(p.blankPoint)
		return;
	
	//printf("Point passed has x=%f y=%f z=%f \n", p.x, p.y, p.z);
	
	Node *n = new Node();
	n->x = p.x;
	n->y = p.y;
	n->z = p.z;
	n->colour = p.colour;
	
	n->NW = NULL;
	n->NE = NULL;
	n->SW = NULL;
	n->SE = NULL;
	
	processAddPoint(root, n);
};

/* Recursive method for adding a child node to quadtree */
void DS_QuadTree::processAddPoint(Node *parent, Node *child) {
	if(root==NULL) {  // Tree is empty
		root = new Node();
		root = child;
		//printf("Made root with x=%f y=%f z=%f \n", root->x, root->y, root->z);
	}
	else {
		//NW
		if(((*child).x<(*parent).x)&&((*child).y>(*parent).y)) {
			if(parent->NW==NULL) {
				parent->NW = new Node();
				child->type=1;
				parent->NW = child;
				//printf("Made a point at NW with x=%f y=%f z=%f \n", parent->NW->x,
					//parent->NW->y, parent->NW->z);
				return;
			}
			// If node already exists, replace
			else if((parent->NW->x == child->x)&&(parent->NW->y == child->y)) {
				parent->NW->colour = child->colour;
				return;
			}
			else {
				processAddPoint(parent->NW, child);
				return;
			}
		}
		//NE
		else if(((*child).x>(*parent).x)&&((*child).y>(*parent).y)) {
			if(parent->NE==NULL) {
				parent->NE = new Node();
				child->type=2;
				parent->NE = child;
				//printf("Made a point at NE with x=%f y=%f z=%f \n", parent->NE->x,
				//parent->NE->y, parent->NE->z);
				return;
			}
			// If node already exists, replace
			else if((parent->NE->x == child->x)&&(parent->NE->y == child->y)) {
				parent->NE->colour = child->colour;
				return;
			}
			else {
				processAddPoint(parent->NE, child);
				return;
			}
		}
		//SW
		else if(((*child).x<(*parent).x)&&((*child).y<(*parent).y)) {
			if(parent->SW==NULL) {
				parent->SW = new Node();
				child->type=3;
				parent->SW = child;
				//printf("Made a point at SW with x=%f y=%f z=%f \n", parent->SW->x,
				//parent->SW->y, parent->SW->z);
				return;
			}
			// If node already exists, replace
			else if((parent->SW->x == child->x)&&(parent->SW->y == child->y)) {
				parent->SW->colour = child->colour;
				return;
			}
			else {
				processAddPoint(parent->SW, child);
				return;
			}
		}
		//SE
		else if(((*child).x>(*parent).x)&&((*child).y<(*parent).y)) {
			if(parent->SE==NULL) {
				parent->SE = new Node();
				child->type=4;
				parent->SE = child;
				//printf("Made a point at SE with x=%f y=%f z=%f \n", parent->SE->x,
				//parent->SE->y, parent->SE->z);
				return;
			}
			// If node already exists, replace
			else if((parent->SE->x == child->x)&&(parent->SE->y == child->y)) {
				parent->SE->colour = child->colour;
				return;
			}
			else {
				processAddPoint(parent->SE, child);
				return;
			}
		}
	}
};

void* DS_QuadTree::getData() {
	return this;
};

void DS_QuadTree::deletePoint(Point p) {
	Node *child;
	child->x = p.x;
	child->y = p.y;
	child->z = p.z;
	processDeletePoint(root, child);
};

/* Find the point to be deleted, child */
void DS_QuadTree::processDeletePoint(Node *parent, Node *toDelete) {
	// child = parent
	if((parent->x==toDelete->x)&&(parent->y==toDelete->y)&&(parent->z==toDelete->z)) {
		// Parent has no children, can just delete then
		if((parent->NW==NULL)&&(parent->NE==NULL)&&(parent->SW==NULL)&&(parent->SE==NULL)) {
			parent = NULL;
			return;
		}
		// Found node but has children
		else {
			Node *cand1 = findConjugateNode(parent->NW);
			Node *cand2 = findConjugateNode(parent->NE);
			Node *cand3 = findConjugateNode(parent->SW);
			Node *cand4 = findConjugateNode(parent->SE);
			Node *repl;
			int r = rand()%4+1;
			bool found = false;
			
			while (!found) {
				switch (r) {
					case 1: 
						if (cand1!=NULL) {
							repl=cand1;
							found = true;
						}
						break;
					case 2:
						if (cand2!=NULL) {
							repl=cand2;
							found = true;
						}
						break;
					case 3:
						if (cand3!=NULL) {
							repl=cand3;
							found = true;
						}
						break;
					case 4:
						if (cand4!=NULL) {
							repl==cand4;
							found = true;
						}
						break;
					default:
						break;
				}
				r = rand()%4+1;
			}
			// Now we've got the replacement node, the complicated bit
			replaceNode(toDelete, repl);
		}
	}
	else {
		//NW
		if(((*toDelete).x<(*parent).x)&&((*toDelete).y>(*parent).y)) {
			if(parent->NW!=NULL) {
				processDeletePoint(parent->NW, toDelete);
			}
		}
		//NE
		else if(((*toDelete).x>(*parent).x)&&((*toDelete).y>(*parent).y)) {
			if(parent->NE!=NULL) {
				processDeletePoint(parent->NE, toDelete);
			}
		}
		//SW
		else if(((*toDelete).x<(*parent).x)&&((*toDelete).y<(*parent).y)) {
			if(parent->SW!=NULL) {
				processDeletePoint(parent->SW, toDelete);
			}
		}
		//SE
		else if(((*toDelete).x>(*parent).x)&&((*toDelete).y<(*parent).y)) {
			if(parent->SE!=NULL) {
				processDeletePoint(parent->SE, toDelete);
			}
		}
	
	}
	
};

/* Returns candidate replacement node */
Node* DS_QuadTree::findConjugateNode(Node *n) {
	if(n==NULL) {
		printf("NULL found at cojugate, should never get this! \n");
		return NULL;
	}
	// Is terminal node, return
	else if((n->NW==NULL)&&(n->NE==NULL)&&(n->SW==NULL)&&(n->SE==NULL)) {
		return n;
	}
	else {
		int chosen = findConjugate(n->type);
		switch (chosen) {
			case 1:
				if (n->NW==NULL) {
					return n;
				}
				return findConjugateNode(n->NW);
				break;
			case 2:
				if (n->NE==NULL) {
					return n;		
				}
				return findConjugateNode(n->NE);
				break;
			case 3:
				if (n->SW==NULL) {
					return n;
				}
				return findConjugateNode(n->SW);
				break;
			case 4:
				if (n->SE==NULL) {
					return n;
				}
				return findConjugateNode(n->SE);
				break;
			default:
				break;
		}
	}
	return NULL;
};

void DS_QuadTree::replaceNode(Node *toDelete, Node *repl) {
	// List of subtrees to be reinserted
	vector<Node*> reinsertList;
	// Get the nodes in the cross-hatched section
	// these have to be re-inserted
	
};

int DS_QuadTree::findConjugate(int n) {
	return ((n+1)%4)+1;
};

void DS_QuadTree::clearData() {
	root = NULL;
};

void DS_QuadTree::drawScene() {
	
};

void DS_QuadTree::saveData(FILE *file) {
	
};		
			
void DS_QuadTree::loadData(FILE *file) {
	
};

void DS_QuadTree::saveXML(string fName) {
	
}

void DS_QuadTree::loadXML(XMLNode sketchNode) {
	
}

