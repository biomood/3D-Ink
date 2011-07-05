/*
 *  Analysis.h
 *  3d Ink
 *
 *  Created by David Roberts on 05/11/2009.
 *  Durham University
 *
 */
#include <cmath>
#include <GLUT/GLUT.h>
#include "Settings.h"
#include "Model.h"
#include "UtilityClasses.h"

#if!defined(ANALYIS_H)
#define ANALYIS_H

class Analysis {
public:
	/* Adds the given number of points to the model */
	static Point addPoints(int noPoints) {
		clock_t start,finish;
		double totalTime;
		start = clock();
		
		int width = 1250;
		int height= 800;
		
		GLfloat x=0; 
		GLfloat y=500;
		GLfloat z=700; 
		
		int index=0;
		Point p;
		while(index!=noPoints) {
			GLdouble scale = (((z*tan(0.449422f/2))*2)/320);
			
			p.x = (((Settings::pixelWidth/2)-x)/
				   (Settings::pixelWidth/Settings::displayWidth))*scale;
			p.y = (((Settings::pixelHeight/2)-y)/
				   (Settings::pixelWidth/Settings::displayWidth))*scale;
			p.z = z;
			
			p.pixelX = x;
			p.pixelY = y;
			p.z = z;
			
			p.blankPoint = false;
			
			p.pointSize = 3.0f;
			
			Model::getInstance()->addPoint(p);
			if (x==width) {
				x=0;
				y+=1;
			}
			if (y==height) {
				x=0;
				y=0;
				z+=10;
			}
			x+=1;
			index++;
			
			// Gets last x,y,z for deleting
		}
		
		finish = clock();
		totalTime = (double(finish)-double(start))/CLOCKS_PER_SEC;
		printf("Time to add %d Points %f \n", noPoints, totalTime);
		
		return p;
	};
	
	/* Deletes the last point in the list */
	static void deletePoint(Point p) {
		Model::getInstance()->deletePoint(&p);
	};
	
	/* Adds two points to the PRQuadtree */
	static void addPRPoint() {
		GLdouble scale = (((700*tan(0.449422f/2))*2)/320);
		
		Point p;
		p.pixelX = 200;
		p.pixelY = 195;
		p.x = (((Settings::pixelWidth/2)-p.pixelX)/
			   (Settings::pixelWidth/Settings::displayWidth))*scale;
		p.y = (((Settings::pixelHeight/2)-p.pixelY)/
			   (Settings::pixelWidth/Settings::displayWidth))*scale;
		p.z = 700;
		Model::getInstance()->addPoint(p);
		
		Point pp;
		pp.pixelX = 700;
		pp.pixelY = 700;
		pp.x = (((Settings::pixelWidth/2)-pp.pixelX)/
				(Settings::pixelWidth/Settings::displayWidth))*scale;
		pp.y = (((Settings::pixelHeight/2)-pp.pixelY)/
				(Settings::pixelWidth/Settings::displayWidth))*scale;
		pp.z = 700;
		Model::getInstance()->addPoint(pp);
		
		Point ppp;
		ppp.pixelX = 340;
		ppp.pixelY = 250;
		ppp.x = (((Settings::pixelWidth/2)-ppp.pixelX)/
				(Settings::pixelWidth/Settings::displayWidth))*scale;
		ppp.y = (((Settings::pixelHeight/2)-ppp.pixelY)/
				(Settings::pixelWidth/Settings::displayWidth))*scale;
		ppp.z = 700;
		Model::getInstance()->addPoint(ppp);
		
		Point pppp;
		pppp.pixelX = 650;
		pppp.pixelY = 410;
		pppp.x = (((Settings::pixelWidth/2)-pppp.pixelX)/
				(Settings::pixelWidth/Settings::displayWidth))*scale;
		pppp.y = (((Settings::pixelHeight/2)-pppp.pixelY)/
				(Settings::pixelWidth/Settings::displayWidth))*scale;
		pppp.z = 700;
		Model::getInstance()->addPoint(pppp);
		
		Point p2;
		p2.pixelX = 650;
		p2.pixelY = 410;
		p2.x = (((Settings::pixelWidth/2)-p2.pixelX)/
				(Settings::pixelWidth/Settings::displayWidth))*scale;
		p2.y = (((Settings::pixelHeight/2)-p2.pixelY)/
				(Settings::pixelWidth/Settings::displayWidth))*scale;
		p2.z = 700;
		Model::getInstance()->addPoint(p2);
		
		Point p3;
		p3.pixelX = 1280;
		p3.pixelY = 777;
		p3.x = (((Settings::pixelWidth/2)-p3.pixelX)/
				(Settings::pixelWidth/Settings::displayWidth))*scale;
		p3.y = (((Settings::pixelWidth/2)-p3.pixelX)/
				(Settings::pixelWidth/Settings::displayWidth))*scale;
		p3.z = 700;
		Model::getInstance()->addPoint(p3);
		
		Point p4;
		p4.pixelX = 1500;
		p4.pixelY = 776;
		p4.x = (((Settings::pixelWidth/2)-p4.pixelX)/
				(Settings::pixelWidth/Settings::displayWidth))*scale;
		p4.y = (((Settings::pixelWidth/2)-p4.pixelX)/
				(Settings::pixelWidth/Settings::displayWidth))*scale;
		p4.z = 700;
		Model::getInstance()->addPoint(p4);
		
		Point p5;
		p5.pixelX = 1500;
		p5.pixelY = 777;
		p5.x = (((Settings::pixelWidth/2)-p4.pixelX)/
				(Settings::pixelWidth/Settings::displayWidth))*scale;
		p5.y = (((Settings::pixelWidth/2)-p4.pixelX)/
				(Settings::pixelWidth/Settings::displayWidth))*scale;
		p5.z = 710;
		Model::getInstance()->addPoint(p5);
		
	}
	
	/* Finds out how large the PR quadtree is */
	static void getPRSize() {
		int i=1;
		
		PR_Quadtree *PR = static_cast<PR_Quadtree*>(Model::getInstance()->getData());
		PRNode *root = PR->root;
		
		queue<PRNode*> q;
		q.push(root);
		
		while (!q.empty()) {
			PRNode *n = q.front();
			q.pop();
			i++;
		
			if (n!=NULL) {
				if (n->isLeaf) {
					i+=n->zList.size();
				}
				else if (!n->isLeaf) {
					q.push(n->NW);
					q.push(n->NE);
					q.push(n->SW);
					q.push(n->SE);
				}
			}
		}
		printf("The no. of nodes in the PR tree is %d \n", i);
		printf("Size of PRNode is %lu total size is %lu \n", sizeof(PRNode), sizeof(PRNode)*i);
		exit(0);
	}
};

#endif
