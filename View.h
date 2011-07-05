/*
 *  View.h
 *  3d Ink
 *
 *  Created by David Roberts on 25/07/2009.
 *  Durham University
 *
 */
#include <ctime>
#include "UtilityClasses.h"
#include "DataStructure.h"

#if !defined (View_H)
#define View_H

class View {
private:
	static View *viewInstance;
	
	View();
	
	~View();
public:
	
	void drawScene(DataStructure* p);
	
	static View *getInstance();
	
};

#endif

