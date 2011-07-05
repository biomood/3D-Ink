/*
 *  View.cpp
 *  3d Ink
 *
 *  Created by David Roberts on 25/07/2009.
 *	Singleton
 */

#include "View.h"

View* View::viewInstance=NULL;

View::View() {
}

View::~View() {
}

void View::drawScene(DataStructure* p) {
	// Timing
	clock_t start,finish;
	double totalTime;
	if(Settings::dDrawingTime)
		start = clock();
	
	p->drawScene();
	
	// Timing
	if(Settings::dDrawingTime) {
		finish = clock();
		totalTime = (double(finish)-double(start))/CLOCKS_PER_SEC;
		printf("Time to display in seconds %f \n", totalTime);
	}	
}

View* View::getInstance() {
	if(viewInstance==NULL) {
		viewInstance = new View();
	}
	return viewInstance;
}

