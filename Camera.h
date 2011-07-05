/*
 *  Camera.h
 *  3d Ink
 *	
 *	Based on code provided by Geng Sun, Durham University
 *
 */
#include <GLUT/GLUT.h>
#include <cmath>
#include "Settings.h"
#include "Model.h"
#include "Controller.h"

#if !defined(CAMERA_H)
#define CAMERA_H

class Camera {
protected:
	GLdouble zViewing;  // viewing distance from viewer to the display
	GLdouble eyeSeperation;
	GLdouble distanceToVirtualDisplay;
	GLdouble halfWidth;
	GLdouble height;
	GLdouble cameraSeperation;
	GLdouble displayRatio;
	GLdouble sceneZ;    
	GLdouble sceneWidth;
	GLdouble sceneHeight;   
	GLdouble viewingAngle;
	GLdouble theta;
public:
	Camera(GLdouble zViewing, GLdouble displayWidth, GLdouble pHeight, GLdouble pWidth) {
		this->zViewing = zViewing;
		this->eyeSeperation = 65.0/2.0;
		this->halfWidth=displayWidth/2.0;
		this->displayRatio= (pHeight/pWidth);
		this->height = halfWidth * displayRatio;
		this->viewingAngle = atan(halfWidth/zViewing) * 180.0 / M_PI * 2;
		this->theta = viewingAngle * (M_PI/180.0);
		
		Settings::vAngle = theta;
	}
	
	~Camera() {
	}
	
	virtual void setupFrustum(GLdouble nearDisplayBoundary, GLdouble farDisplayBoundary, GLdouble nearSceneBoundary, GLdouble farSceneBoundary)=0;
};

/*
 * Stereo Camera
 */
class StereoCamera: public Camera {
private:
	GLdouble leftFrustum[6];
	GLdouble rightFrustum[6];
	GLdouble leftFrom;
	GLdouble leftTo[3];
	GLdouble rightFrom;
	GLdouble rightTo[3];
public:
	StereoCamera(GLdouble zViewing, GLdouble displayWidth, GLdouble pixelHeight, GLdouble pixelWidth): Camera(zViewing, displayWidth, pixelHeight, pixelWidth) {
		
	}
	
	void setupFrustum(GLdouble nearDisplayBoundary, GLdouble farDisplayBoundary, GLdouble nearSceneBoundary, GLdouble farSceneBoundary);
	
	double* getLeftFrustum();
	
	double* getRightFrustum();
	
	double getLeftFrom();
	
	double* getLeftTo();
	
	double getRightFrom();
	
	double* getRightTo();
};

/*
 * Mono Camera
 */
class MonoCamera: public Camera {
private:
	double monoFrustum[6];
	double monoTo;
public:
	MonoCamera(GLdouble zViewing, GLdouble displayWidth, GLdouble pixelHeight, GLdouble pixelWidth): Camera(zViewing, displayWidth, pixelHeight, pixelWidth) {
	}
	
	void setupFrustum(GLdouble nearDisplayBoundary, GLdouble farDisplayBoundary, GLdouble nearSceneBoundary, GLdouble farSceneBoundary);
	
	double* getMonoFrustum();
	
	double getMonoTo();
};

#endif
