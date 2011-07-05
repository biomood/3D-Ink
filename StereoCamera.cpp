/*
 *  StereoCamera.cpp
 *  3d Ink
 *
 *  Code provided by Geng Sun, Durham University
 *
 */
#include <cmath>
#include "Camera.h"

void StereoCamera::setupFrustum(GLdouble nearDisplayBoundary, GLdouble farDisplayBoundary, GLdouble nearSceneBoundary, GLdouble farSceneBoundary) {
	// calculate screen disparity ranges
	GLdouble nearDisparity = eyeSeperation*(zViewing - nearDisplayBoundary)/nearDisplayBoundary;
	GLdouble farDisparity = eyeSeperation*(farDisplayBoundary-zViewing)/farDisplayBoundary;
	
	GLdouble ratio = nearDisparity/farDisparity;
	// distance to the vitrual display
	sceneZ = (farSceneBoundary*nearSceneBoundary+farSceneBoundary*nearSceneBoundary*ratio)/(farSceneBoundary+nearSceneBoundary*ratio);
	sceneWidth = sceneZ * tan(theta/2.0);
	sceneHeight = sceneWidth * displayRatio;
	
	GLdouble distanceToScene = sceneWidth / halfWidth;
	
	// calculate camera position
	// check that this means angle of seperation
	GLdouble angleSeperation = nearSceneBoundary*distanceToScene*nearDisparity/(sceneZ-nearSceneBoundary);
	cameraSeperation = angleSeperation*2;
	
	// Left camera position
	leftFrom =  angleSeperation;
	leftTo[0] = angleSeperation;
	leftTo[2] = sceneZ;
	
	// Left camera frustum
	leftFrustum[0] = -(nearSceneBoundary * ((sceneWidth - angleSeperation) / sceneZ));  // Left
	leftFrustum[1] =  (nearSceneBoundary * ((sceneWidth + angleSeperation) / sceneZ));  // Right
	leftFrustum[2] = -(nearSceneBoundary * sceneHeight / sceneZ);  // Bottom
	leftFrustum[3] =  (nearSceneBoundary * sceneHeight / sceneZ);  // Top
	leftFrustum[4] =   nearSceneBoundary;  // Near
	leftFrustum[5] =   farSceneBoundary;  // Far
	
	// Right camera position
	rightFrom =  -angleSeperation;
	rightTo[0] = -angleSeperation;
	rightTo[2] = sceneZ;
	
	// Right camera frustum
	rightFrustum[0] = -(nearSceneBoundary * ((sceneWidth + angleSeperation) / sceneZ));  // Left
	rightFrustum[1] =  (nearSceneBoundary * ((sceneWidth - angleSeperation) / sceneZ));  // Right
	rightFrustum[2] = -(nearSceneBoundary * sceneHeight / sceneZ);  // Bottom
	rightFrustum[3] =  (nearSceneBoundary * sceneHeight / sceneZ);  // Top
	rightFrustum[4] =  nearSceneBoundary;  // Near
	rightFrustum[5] =  farSceneBoundary;  // Far
	
	Model::getInstance()->vAngle=theta;
}

double* StereoCamera::getLeftFrustum() {
	return leftFrustum;
}

double* StereoCamera::getRightFrustum() {
	return rightFrustum;
}

double StereoCamera::getLeftFrom() {
	return leftFrom;
}

double* StereoCamera::getLeftTo() {
	return leftTo;
}

double StereoCamera::getRightFrom() {
	return rightFrom;
}

double* StereoCamera::getRightTo() {
	return rightTo;
}