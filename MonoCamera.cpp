/*
 *  MonoCamera.cpp
 *  3d Ink
 *
 *	Code provided by Geng Sun, Durham University
 */
#include<stdio.h>
#include <cmath>
#include "Camera.h"

void MonoCamera::setupFrustum(GLdouble nearDisplayBoundary, GLdouble farDisplayBoundary, GLdouble nearSceneBoundary, GLdouble farSceneBoundary) {
	// only a single viewpoint
	cameraSeperation = 0.0;
	
	// calculate screen disparity ranges
	double nearDisparity = eyeSeperation*(zViewing - nearDisplayBoundary)/nearDisplayBoundary;
	double farDisparity = eyeSeperation*(farDisplayBoundary-zViewing)/farDisplayBoundary;
	double ratio = nearDisparity/farDisparity;
	
	// distance to the virtual display
	sceneZ = (farSceneBoundary*nearSceneBoundary+farSceneBoundary*nearSceneBoundary*ratio)/(farSceneBoundary+nearSceneBoundary*ratio);
	sceneWidth = sceneZ * tan(theta/2.0);
	sceneHeight = sceneWidth * displayRatio;
	
	monoTo = sceneZ;
	
	// frustum
	monoFrustum[0] = -nearSceneBoundary * sceneWidth / sceneZ;
	monoFrustum[1] = nearSceneBoundary * sceneWidth / sceneZ;
	monoFrustum[2] = -nearSceneBoundary * sceneHeight / sceneZ;
	monoFrustum[3] = nearSceneBoundary * sceneHeight / sceneZ;
	monoFrustum[4] = nearSceneBoundary;
	monoFrustum[5] = farSceneBoundary;
	
	Model::getInstance()->vAngle=theta;
}

double* MonoCamera::getMonoFrustum() {
	return monoFrustum;
}

double MonoCamera::getMonoTo() {
	return monoTo;
}