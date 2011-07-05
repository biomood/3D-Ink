/*  Created at the University of Durham
 *  by David Roberts, 2010
 *
 *  3D Ink
 */

#include <iostream>
#include <stdlib.h>
#include <GLUT/glut.h>

#include "FreeImage.h"
#include "Model.h"
#include "View.h"
#include "Controller.h"
#include "Camera.h"
#include "Settings.h"
#include "Analysis.h"
#include "FileGUI.h"

#define WINTITLE "3DInk"
#define BLACK 1
#define RED 2
#define GREEN 3
#define BLUE 4
#define YELLOW 18
#define OPEN 5
#define SAVE 6
#define PEN 10
#define ERASER 11
#define EXIT 12
#define NEWIMAGE 19
#define POINTLARGE 13
#define POINTMEDIUM 14
#define POINTSMALL 15
#define RECTANGLE 151
#define CIRCLE 152
#define STEREO 16
#define MONO 17
#define EXPORT 20
#define BACKGROUNDIMAGE 21
#define ANAGLYPH 22
#define STEREOBACKGROUNDIMAGE 23
#define OPENXML 24
#define SAVEXML 25

int mainWindow;

// Stereo and Mono camera setups
GLdouble displayBoundaries[] = {(Settings::viewingDistance)-(Settings::gpdInFront), 
	(Settings::viewingDistance)+(Settings::gpdBehind)};

GLdouble sceneBoundaries[] = {(Settings::viewingDistance)-((Settings::sceneDepth)/2), 
	(Settings::viewingDistance)+((Settings::sceneDepth)/2)};

GLdouble focalLength = (sceneBoundaries[0]+sceneBoundaries[2])/2;

// Standard display
MonoCamera *monoCamera;
// Stereoscopic display
StereoCamera *stereoCamera;    

// FPS
int frame=0,fpsTime,timebase=0;
char fpsBuffer[50];

// Saving/Loading
string fName;
bool save = false;
bool fileOpen = false;
bool saveXML = false;

// Stops the mouse from being shown when exporting image
bool toExport = false;

// Background
bool background = false;
bool setBackground = false;
string backgroundName;
GLuint bgTextureID = 1;

// Stereo background
string leftBackground;
string rightBackground;
bool stereoBackground = false;
bool setStereoBackground = false;
GLuint bgLeftID = 2;
GLuint bgRightID = 3;

// Anaglyph
bool toAnaglyph = false;

bool initGLWindow(void);
void initLighting(void);
void display(void);
void displayStereo(void);
void displayNonStereo(void);
void displayAnaglyph(void);
void drawMouse(int x, int y, GLfloat z);
void reshape(int width, int height);
void motionUpdate(void);
void createMenu(void);
void processMenu(int value);
void saveFile(void);
void loadFile(void);
void exportImage(void);
void loadBackground(void);
void drawBackground(void);
void loadStereoBackground(void);
void drawLeftBackground(void);
void drawRightBackground(void);

int main (int argc, char **argv) {
	srand(time(NULL));
	
	glutInit(&argc, argv);
	// Autodetect the screen resolution
	Settings::pixelWidth = glutGet(GLUT_SCREEN_WIDTH);
	Settings::pixelHeight = glutGet(GLUT_SCREEN_HEIGHT);
	
	// If window initialisation fails, exit and return error;
	if(initGLWindow() != GL_TRUE)
		exit(1);
	initLighting();
	
	
	monoCamera = new MonoCamera((Settings::viewingDistance), (Settings::displayWidth), 
								Settings::pixelHeight, Settings::pixelWidth);
	stereoCamera = new StereoCamera((Settings::viewingDistance), (Settings::displayWidth), 
									Settings::pixelHeight, Settings::pixelWidth);
	
	// Sets the initial pen colour to black
	Model::getInstance()->setColour(0.0f, 0.0f, 0.0f);
	
	if (Settings::stereoMode==ANAGLYPHMODE) {
		//printf("Change colour /n");
		Model::getInstance()->setColour(0.0f, 0.0f, 0.0f);
	}
	
	// Analysis code
	Point aPoint;
	if (Settings::dPoints)
		aPoint = Analysis::addPoints(Settings::dNoPoints);
	if (Settings::dPoints && Settings::dErasingTime)
		Analysis::deletePoint(aPoint);
	if (Settings::dTree && Settings::dataFlag==PRQUADTREE)
		Analysis::addPRPoint();
	if (Settings::dPRSize && Settings::dataFlag==PRQUADTREE) {
		Analysis::addPoints(10000000);
		Analysis::getPRSize();
	}
	
	glutMainLoop();
	return(0);
}

/* Initialises and creates the opengl window */
bool initGLWindow(void) {
	if(Settings::stereoMode==STEREOMODE) {
		glutInitDisplayMode(GLUT_DOUBLE|GLUT_ACCUM|GLUT_RGB|GLUT_DEPTH|GLUT_STEREO);
	}
	else {
		glutInitDisplayMode(GLUT_DOUBLE|GLUT_ACCUM|GLUT_RGB|GLUT_DEPTH);
	}
	
	glutInitWindowSize(Settings::pixelWidth, Settings::pixelHeight);
	mainWindow = glutCreateWindow(WINTITLE);
	
	glutDisplayFunc(&display);
	glutReshapeFunc(&reshape);
	glutIdleFunc(&motionUpdate);
	// Keyboard controlls
	glutKeyboardFunc(&Controller::handleKeyboard);
	glutSpecialFunc(&Controller::handleSpecialKeys);
	// Mouse controlls
    glutMouseFunc(&Controller::processMouseButton);
    glutMotionFunc(&Controller::processMouseActiveMotion);
	glutPassiveMotionFunc(&Controller::processMousePassiveMotion);
    // Tablet controls
	glutTabletMotionFunc(&Controller::processTabletMotion);
	glutTabletButtonFunc(&Controller::processTabletButton);
	Controller::noTabletButtons = glutDeviceGet(GLUT_NUM_TABLET_BUTTONS);
	// Pop-up menu
	createMenu();
	// Set the cursor
	glutSetCursor(GLUT_CURSOR_NONE);
    // Enable anti-aliasing
    glEnable(GL_LINE_SMOOTH);
    
	if(Settings::fullScreen) 
		glutFullScreen();
	
	return true;
}

/* Initialises the lighting for the opengl scene */
void initLighting(void) {
	GLfloat light_position0[] = {80.0, 80.0, 30.0, 0.0};
	GLfloat light_position1[] = {-80.0, -80.0, 30.0, 0.0};
	GLfloat light1_diffuse[] = {1.0, 1.0, 1.0, 1.0};
	
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClearDepth(1.0);
	glShadeModel(GL_SMOOTH);
	
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
}

/* Refreshes the display */
void motionUpdate(void) {
	// Counts the frames per sec and displays
	if(Settings::dFPS) {
		frame++;
		fpsTime=glutGet(GLUT_ELAPSED_TIME);	
		if (fpsTime - timebase > 1000) {
			printf("FPS is %f \n", frame*1000.0/(fpsTime-timebase));
			timebase = fpsTime;		
			frame = 0;
		}
	}
	
	glutPostRedisplay();
}

/* Resizes the size and dimensions of the window */
void reshape(int width, int height) {
	glViewport(0, 0, width, height);
}

/* Displays the opengl scene */
void display(void) {
	if (save) {
		saveFile();
		save = false;
	}
	if (fileOpen) {
		loadFile();
		fileOpen = false;
	}
	if (saveXML) {
		Model::getInstance()->saveXML(fName);
		saveXML = false;
	}
	if (setBackground) {
		loadBackground();
		setBackground = false;
	}
	if (setStereoBackground) {
		printf("Trying to load stereo");
		loadStereoBackground();
		setStereoBackground = false;
	}
	
	if(Settings::stereoMode==STEREOMODE) {
		displayStereo();
	}
	else if (Settings::stereoMode==MONOMODE) {
		displayNonStereo();
		View::getInstance()->drawScene(Model::getInstance()->getData());
		drawMouse(Controller::xMouse, Controller::yMouse, Controller::z);
		
		if (background) {
			drawBackground();
		}
	}
	else if (Settings::stereoMode==ANAGLYPHMODE) {
		displayAnaglyph();
	}
	
	glutSwapBuffers();
}

/* Displays the scene in stereo */
void displayStereo(void) {
	stereoCamera->setupFrustum(displayBoundaries[0], displayBoundaries[1], sceneBoundaries[0], sceneBoundaries[1]);
	// Left view
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(stereoCamera->getLeftFrustum()[0],stereoCamera->getLeftFrustum()[1],stereoCamera->getLeftFrustum()[2],
			  stereoCamera->getLeftFrustum()[3],stereoCamera->getLeftFrustum()[4],stereoCamera->getLeftFrustum()[5]); 
	
	glMatrixMode(GL_MODELVIEW);
	
	glDrawBuffer(GL_BACK_LEFT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();             
	gluLookAt(stereoCamera->getLeftFrom(), 0.0, 0.0, 
			  stereoCamera->getLeftTo()[0], 0.0, stereoCamera->getLeftTo()[2],
			  0.0, 1.0, 0.0);
	// Draw the model for the left eye
	View::getInstance()->drawScene(Model::getInstance()->getData());
	// Draw the mouse for the left eye
	drawMouse(Controller::xMouse, Controller::yMouse, Controller::z); 
	
	if (background) {
		drawBackground();
	}
	if (stereoBackground) {
		drawLeftBackground();
	}
	
	// Right view
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(stereoCamera->getRightFrustum()[0],stereoCamera->getRightFrustum()[1],stereoCamera->getRightFrustum()[2],
			  stereoCamera->getRightFrustum()[3],stereoCamera->getRightFrustum()[4],stereoCamera->getRightFrustum()[5]); 
	
	glMatrixMode(GL_MODELVIEW);
	
	glDrawBuffer(GL_BACK_RIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();             
	gluLookAt(stereoCamera->getRightFrom(), 0.0, 0.0, 
			  stereoCamera->getRightTo()[0], 0.0, stereoCamera->getRightTo()[2],
			  0.0, 1.0, 0.0);
	// Draw the model for the right eye
	View::getInstance()->drawScene(Model::getInstance()->getData());
	// Draw the mouse for the right eye
	drawMouse(Controller::xMouse, Controller::yMouse, Controller::z);
	
	if (background) {
		drawBackground();
	}
	if (stereoBackground) {
		drawRightBackground();
	}
}

 /* Displays the scene in standard (non-stereo) */
void displayNonStereo(void) {
	monoCamera->setupFrustum(displayBoundaries[0], displayBoundaries[1], sceneBoundaries[0], sceneBoundaries[1]);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(monoCamera->getMonoFrustum()[0], monoCamera->getMonoFrustum()[1], monoCamera->getMonoFrustum()[2], 
			  monoCamera->getMonoFrustum()[3], monoCamera->getMonoFrustum()[4], monoCamera->getMonoFrustum()[5]);
	
	glMatrixMode(GL_MODELVIEW);
	glDrawBuffer(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 0.0, 
			 0.0, 0.0, monoCamera->getMonoTo(),
			  0.0, 1.0, 0.0);
}

/* Displays the scene in anaglyph mode */
void displayAnaglyph(void) {
	stereoCamera->setupFrustum(displayBoundaries[0], displayBoundaries[1], sceneBoundaries[0], sceneBoundaries[1]);
	// Left view
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(stereoCamera->getLeftFrustum()[0],stereoCamera->getLeftFrustum()[1],stereoCamera->getLeftFrustum()[2],
			  stereoCamera->getLeftFrustum()[3],stereoCamera->getLeftFrustum()[4],stereoCamera->getLeftFrustum()[5]); 
	
	
	glMatrixMode(GL_MODELVIEW);
	
	
	// Anaglyph bit
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Blue
	glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE);
	glLoadIdentity();             
	gluLookAt(stereoCamera->getLeftFrom(), 0.0, 0.0, 
			  stereoCamera->getLeftTo()[0], 0.0, stereoCamera->getLeftTo()[2],
			  0.0, 1.0, 0.0);
	
	// Draw the model for the left eye
	View::getInstance()->drawScene(Model::getInstance()->getData());
	// Draw the mouse for the left eye
	drawMouse(Controller::xMouse, Controller::yMouse, Controller::z); 
	if (background) {
		drawBackground();
	}
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	
	
	// Right view
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(stereoCamera->getRightFrustum()[0],stereoCamera->getRightFrustum()[1],stereoCamera->getRightFrustum()[2],
			  stereoCamera->getRightFrustum()[3],stereoCamera->getRightFrustum()[4],stereoCamera->getRightFrustum()[5]); 
	
	glMatrixMode(GL_MODELVIEW);
	
	// Anaglyph bit
	glClear(GL_DEPTH_BUFFER_BIT);
	// Red
	glColorMask(GL_FALSE, GL_TRUE, GL_TRUE, GL_TRUE);
	glLoadIdentity();             
	gluLookAt(stereoCamera->getRightFrom(), 0.0, 0.0, 
			  stereoCamera->getRightTo()[0], 0.0, stereoCamera->getRightTo()[2],
			  0.0, 1.0, 0.0);
	
	// Draw the model for the right eye
	View::getInstance()->drawScene(Model::getInstance()->getData());
	// Draw the mouse for the right eye
	drawMouse(Controller::xMouse, Controller::yMouse, Controller::z);
	
	if (background) {
		drawBackground();
	}
	
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

/* Draws the mouse cursor */
void drawMouse(int x, int y, GLfloat z) {
	if (!toExport) {
		GLdouble scale = (((z*tan(Model::getInstance()->vAngle/2))*2)/Settings::displayWidth);
		glPushMatrix();
		glPointSize(10.0f);
		glColor3f(Model::getInstance()->getColour().red,
				  Model::getInstance()->getColour().green,
				  Model::getInstance()->getColour().blue);
		glBegin(GL_POINTS);
		glVertex3f((((Settings::pixelWidth/2)-x)/(Settings::pixelWidth/Settings::displayWidth))*scale, 
				   (((Settings::pixelHeight/2)-y)/(Settings::pixelWidth/Settings::displayWidth))*scale, z);
		glEnd();
		glPopMatrix();
	}
}

/* Creates and sets the pop-up menu */
void createMenu(void) {
	int colourSubMenu = glutCreateMenu(processMenu);
	glutAddMenuEntry("Black", BLACK);
	glutAddMenuEntry("Red", RED);
	glutAddMenuEntry("Green", GREEN);
	glutAddMenuEntry("Blue", BLUE);
	glutAddMenuEntry("Yellow", YELLOW);
	
	int fileSubMenu = glutCreateMenu(processMenu);
	glutAddMenuEntry("New Image", NEWIMAGE);
	glutAddMenuEntry("Export Image", EXPORT);
	glutAddMenuEntry("Import Single Image", BACKGROUNDIMAGE);
	glutAddMenuEntry("Import Stereo Image", STEREOBACKGROUNDIMAGE);
	glutAddMenuEntry("Open", OPEN);
	glutAddMenuEntry("Save", SAVE);
	//glutAddMenuEntry("Open XML", OPENXML);
	//glutAddMenuEntry("Save XML", SAVEXML);
	glutAddMenuEntry("", -1);
	glutAddMenuEntry("Exit", EXIT);
	
	
	
	int penTypeSubMenu = glutCreateMenu(processMenu);
	glutAddMenuEntry("Pen", PEN);
	glutAddMenuEntry("Eraser", ERASER);
	
	int pointSizeSubMenu = glutCreateMenu(processMenu);
	glutAddMenuEntry("Large", POINTLARGE);
	glutAddMenuEntry("Medium", POINTMEDIUM);
	glutAddMenuEntry("Small", POINTSMALL);
	
	int shapeSubMenu = glutCreateMenu(processMenu);
	glutAddMenuEntry("Rectangle", RECTANGLE);
	glutAddMenuEntry("Circle", CIRCLE);
	
	int generalPenMenu = glutCreateMenu(processMenu);
	glutAddSubMenu("Pen Type", penTypeSubMenu);
	
	
	// As the colours don't work with the anaglyph I'm using
	// no need to display the colour menu
	if (Settings::stereoMode!=ANAGLYPHMODE) {
		glutAddSubMenu("Colour", colourSubMenu);
	}
	
	glutAddSubMenu("Line Width", pointSizeSubMenu);
	glutAddSubMenu("Shapes", shapeSubMenu);
	
	int displaySubMenu = glutCreateMenu(processMenu);
	glutAddMenuEntry("Non-Stereo", MONO);
	glutAddMenuEntry("Stereo", STEREO);
	glutAddMenuEntry("Anaglyph", ANAGLYPH);
	
	int mainMenu;
	mainMenu = glutCreateMenu(processMenu);
	glutAddSubMenu("File", fileSubMenu);
	glutAddSubMenu("Display", displaySubMenu);
	glutAddSubMenu("Ink", generalPenMenu);
	
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

/* Processes the events from the menu */
void processMenu(int value) {
	switch (value) {
		// Set colour	
		case BLACK:
			Model::getInstance()->setColour(0.0f, 0.0f, 0.0f);
			Controller::penType = true;
			break;
		case RED:
			Model::getInstance()->setColour(2.5f, 0.0f, 0.0f);
			Controller::penType = true;
			break;
		case GREEN:
			Model::getInstance()->setColour(0.0f, 2.5f, 0.0f);
			Controller::penType = true;
			break;
		case BLUE:
			Model::getInstance()->setColour(0.0f, 0.0f, 2.5f);
			Controller::penType = true;
			break;
		case YELLOW:
			Model::getInstance()->setColour(4.5f, 4.5f, 0.5f);
			Controller::penType = true;
			break;
		
		// Set pen type
		case PEN:
			Controller::penType = true;
			Controller::shape = false;
			break;
		case ERASER:
			Controller::penType = false;
			break;
		case EXIT:
			// Probably should shut stuff down properly
			exit(0);
		
		// Set the width of the line 
		case POINTLARGE:
			Controller::pointSize = 12.0f;
			Controller::penType = true;
			break;
		case POINTMEDIUM:
			Controller::pointSize = 6.0f;
			Controller::penType = true;
			break;
		case POINTSMALL:
			Controller::pointSize = 3.0f;
			Controller::penType = true;
			break;
		
		// Set shape options 	
		case RECTANGLE:
			Controller::shape = !Controller::shape;
			Controller::shapeType = Rectangle;
			break;
		case CIRCLE:
			
			break;
			
		// Set the display type 
		case STEREO:
			Settings::stereoMode = STEREOMODE;
			break;
		case MONO:
			Settings::stereoMode = MONOMODE;
			break;
		case ANAGLYPH:
			Settings::stereoMode = ANAGLYPHMODE;
			break;
		
		// File options
		case NEWIMAGE:
			Model::getInstance()->clearData();
		case EXPORT:
			toExport = true;
			exportImage();
			toExport = false;
			break;
		case SAVE: {
			if (!fileOpen) {
				SaveBrowser *sB = new SaveBrowser(mainWindow, &fName, &save);
				sB->buildInterface();
			}
			else {
				save = true;
			}
		}
		break;
		case OPEN: {
			FileBrowser *fb = new FileBrowser(mainWindow, &fName, &fileOpen);
			fb->buildInterface();
			break;
		}
		case OPENXML: {
			
			break;	
		}
		case SAVEXML: {
			Model::getInstance()->saveXML("test3d.xml");
			break;
		}
		case BACKGROUNDIMAGE: {
			ImageBrowser *imBrowser = new ImageBrowser(mainWindow, &backgroundName, &setBackground);
			imBrowser->buildInterface();
			break;
		}
		case STEREOBACKGROUNDIMAGE: {
			StereoBrowser *stereoBrowser = new StereoBrowser(mainWindow, &leftBackground, &rightBackground, &setStereoBackground);
			stereoBrowser->buildInterface();
			break;
		}
		default:
			break;
	}
}

/* Calls the save method in Model */
void saveFile() {
	Model::getInstance()->saveXML(fName);
}

/* Calls the load method in Model */
void loadFile() {
	//printf("File opened is %s \n", fName.c_str());
	/**if(!Model::getInstance()->loadFile(fName)) {
		ErrorMessage *eMessage = 
		new ErrorMessage(mainWindow, "File is corrupt, please use another file");
		eMessage->buildInterface();
	}**/
	
	if (!Model::getInstance()->loadXML(fName)) {
		ErrorMessage *eMessage = 
		new ErrorMessage(mainWindow, "File may be corrupt, please use another file");
		eMessage->buildInterface();
	}
}

/* Exports the left and right eye view as images */
void exportImage(void) {
	unsigned char *image;
	image = (unsigned char*)malloc((3*Settings::pixelWidth*Settings::pixelHeight)*sizeof(char));
	
	
	glPixelStorei(GL_PACK_ALIGNMENT,1);
	
	// Left Image
	glReadBuffer(GL_BACK_LEFT);
	glReadPixels(0, 0, (int)Settings::pixelWidth, (int)Settings::pixelHeight, 
				 GL_BGR,GL_UNSIGNED_BYTE, image);
	
	FIBITMAP* leftImage = FreeImage_ConvertFromRawBits(image, (int)Settings::pixelWidth, 
													   (int)Settings::pixelHeight, 
													   3*Settings::pixelWidth, 24, 0xFF0000, 
													   0x00FF00, 0x0000FF, false); 
	FreeImage_Save(FIF_BMP, leftImage, "left.bmp", 0);
	
	// Right image
	glReadBuffer(GL_BACK_RIGHT);
	glReadPixels(0, 0, (int)Settings::pixelWidth, (int)Settings::pixelHeight,
				 GL_BGR, GL_UNSIGNED_BYTE, image);
	
	FIBITMAP* rightImage = FreeImage_ConvertFromRawBits(image, (int)Settings::pixelWidth, 
														(int)Settings::pixelHeight, 
														3*Settings::pixelWidth, 24, 0xFF0000, 
														0x00FF00, 0x0000FF, false); 
	FreeImage_Save(FIF_BMP, rightImage, "right.bmp", 0);
	
	free(image);
}

/* Loads the background image */
void loadBackground(void) {
	//printf("Background image loaded %s", backgroundName.c_str());
	
	FREE_IMAGE_FORMAT fImageFormat = FreeImage_GetFileType(backgroundName.c_str(), 0);
	FIBITMAP *fBitMap = FreeImage_Load(fImageFormat, backgroundName.c_str(), 0);
	fBitMap = FreeImage_ConvertTo24Bits(fBitMap);
	
	glGenTextures(1, &bgTextureID);
	glBindTexture(GL_TEXTURE_2D, bgTextureID);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	BYTE *bits = new BYTE[FreeImage_GetWidth(fBitMap) * FreeImage_GetHeight(fBitMap) * 3];
	BYTE *pixels = (BYTE*)FreeImage_GetBits(fBitMap);
	
    // Switch in RGB format
	for(int pix=0; pix<FreeImage_GetWidth(fBitMap) * FreeImage_GetHeight(fBitMap); pix++) {
		bits[pix*3+0]=pixels[pix*3+2];
		bits[pix*3+1]=pixels[pix*3+1];
		bits[pix*3+2]=pixels[pix*3+0];
	}
	
	glTexImage2D(GL_TEXTURE_2D, 0, 3, FreeImage_GetWidth(fBitMap), FreeImage_GetHeight(fBitMap), 
				 0, GL_RGB, GL_UNSIGNED_BYTE, bits);
	FreeImage_Unload(fBitMap);
	delete bits;
	
	background = true;
}

/* Draws the background image 
 just puts the image as texture to a quad the size of the screen */
void drawBackground(void) {
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, bgTextureID);
	
	GLdouble scale = (((740*tan(Model::getInstance()->vAngle/2))*2)/Settings::displayWidth);
	glPushMatrix();
	GLfloat x0 = (((Settings::pixelWidth/2)-0)/(Settings::pixelWidth/Settings::displayWidth))*scale;
	GLfloat y0 = (((Settings::pixelHeight/2)-0)/(Settings::pixelWidth/Settings::displayWidth))*scale;
	GLfloat x1 = (((Settings::pixelWidth/2)-Settings::pixelWidth)/(Settings::pixelWidth/Settings::displayWidth))*scale;
	GLfloat y1 = (((Settings::pixelHeight/2)-Settings::pixelHeight)/(Settings::pixelWidth/Settings::displayWidth))*scale;
	
	glColor3f(5.0f, 5.0f, 5.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); //Top right
	glVertex3f(x1, y0, 740);
	
	glTexCoord2f(0.0f, 1.0f); //Top Left
	glVertex3f(x0, y0, 740);
	
	glTexCoord2f(0.0f, 0.0f); //Bottom left
	glVertex3f(x0, y1, 740);
	
	glTexCoord2f(1.0f, 0.0f); //Bottom right
	glVertex3f(x1, y1, 740);
	
	glEnd();
	glPopMatrix();
	
	glDisable(GL_TEXTURE_2D);
}

/* Loads the stereo background */
void loadStereoBackground(void) {
	printf("Left Background loaded = %s", leftBackground.c_str());
	
	// Import left image
	FREE_IMAGE_FORMAT fImageFormatLeft = FreeImage_GetFileType(leftBackground.c_str(), 0);
	FIBITMAP *fBitMapLeft = FreeImage_Load(fImageFormatLeft, leftBackground.c_str(), 0);
	fBitMapLeft = FreeImage_ConvertTo24Bits(fBitMapLeft);
	
	glGenTextures(1, &bgLeftID);
	glBindTexture(GL_TEXTURE_2D, bgLeftID);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	BYTE *bitsLeft = new BYTE[FreeImage_GetWidth(fBitMapLeft) * FreeImage_GetHeight(fBitMapLeft) * 3];
	BYTE *pixelsLeft = (BYTE*)FreeImage_GetBits(fBitMapLeft);
	// Switch in RGB format
	for(int pix=0; pix<FreeImage_GetWidth(fBitMapLeft) * FreeImage_GetHeight(fBitMapLeft); pix++) {
		bitsLeft[pix*3+0]=pixelsLeft[pix*3+2];
		bitsLeft[pix*3+1]=pixelsLeft[pix*3+1];
		bitsLeft[pix*3+2]=pixelsLeft[pix*3+0];
	}
	
	glTexImage2D(GL_TEXTURE_2D, 0, 3, FreeImage_GetWidth(fBitMapLeft), FreeImage_GetHeight(fBitMapLeft), 
				 0, GL_RGB, GL_UNSIGNED_BYTE, bitsLeft);
	FreeImage_Unload(fBitMapLeft);
	delete bitsLeft;
	
	printf("Right Background loaded = %s", rightBackground.c_str());
	// Import right image
	FREE_IMAGE_FORMAT fImageFormatRight = FreeImage_GetFileType(rightBackground.c_str(), 0);
	FIBITMAP *fBitMapRight = FreeImage_Load(fImageFormatRight, rightBackground.c_str(), 0);
	fBitMapRight = FreeImage_ConvertTo24Bits(fBitMapRight);
	
	glGenTextures(1, &bgRightID);
	glBindTexture(GL_TEXTURE_2D, bgRightID);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	BYTE *bitsRight = new BYTE[FreeImage_GetWidth(fBitMapRight) * FreeImage_GetHeight(fBitMapRight) * 3];
	BYTE *pixelsRight = (BYTE*)FreeImage_GetBits(fBitMapRight);
	// Switch in RGB format
	for(int pix=0; pix<FreeImage_GetWidth(fBitMapRight) * FreeImage_GetHeight(fBitMapRight); pix++) {
		bitsRight[pix*3+0]=pixelsRight[pix*3+2];
		bitsRight[pix*3+1]=pixelsRight[pix*3+1];
		bitsRight[pix*3+2]=pixelsRight[pix*3+0];
	}
	
	glTexImage2D(GL_TEXTURE_2D, 0, 3, FreeImage_GetWidth(fBitMapRight), FreeImage_GetHeight(fBitMapRight), 
				 0, GL_RGB, GL_UNSIGNED_BYTE, bitsRight);
	FreeImage_Unload(fBitMapRight);
	delete bitsRight;
	
	stereoBackground = true;
}

/* Draws the left stereo background */
void drawLeftBackground(void) {
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, bgLeftID);
	
	GLdouble scale = (((740*tan(Model::getInstance()->vAngle/2))*2)/Settings::displayWidth);
	glPushMatrix();
	GLfloat x0 = (((Settings::pixelWidth/2)-0)/(Settings::pixelWidth/Settings::displayWidth))*scale;
	GLfloat y0 = (((Settings::pixelHeight/2)-0)/(Settings::pixelWidth/Settings::displayWidth))*scale;
	GLfloat x1 = (((Settings::pixelWidth/2)-Settings::pixelWidth)/(Settings::pixelWidth/Settings::displayWidth))*scale;
	GLfloat y1 = (((Settings::pixelHeight/2)-Settings::pixelHeight)/(Settings::pixelWidth/Settings::displayWidth))*scale;
	
	glColor3f(5.0f, 5.0f, 5.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); //Top right
	glVertex3f(x1, y0, 740);
	
	glTexCoord2f(0.0f, 1.0f); //Top Left
	glVertex3f(x0, y0, 740);
	
	glTexCoord2f(0.0f, 0.0f); //Bottom left
	glVertex3f(x0, y1, 740);
	
	glTexCoord2f(1.0f, 0.0f); //Bottom right
	glVertex3f(x1, y1, 740);
	
	glEnd();
	glPopMatrix();
	
	glDisable(GL_TEXTURE_2D);
}

/* Draws the right stereo background */
void drawRightBackground(void) {
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, bgRightID);
	
	GLdouble scale = (((740*tan(Model::getInstance()->vAngle/2))*2)/Settings::displayWidth);
	glPushMatrix();
	GLfloat x0 = (((Settings::pixelWidth/2)-0)/(Settings::pixelWidth/Settings::displayWidth))*scale;
	GLfloat y0 = (((Settings::pixelHeight/2)-0)/(Settings::pixelWidth/Settings::displayWidth))*scale;
	GLfloat x1 = (((Settings::pixelWidth/2)-Settings::pixelWidth)/(Settings::pixelWidth/Settings::displayWidth))*scale;
	GLfloat y1 = (((Settings::pixelHeight/2)-Settings::pixelHeight)/(Settings::pixelWidth/Settings::displayWidth))*scale;
	
	glColor3f(5.0f, 5.0f, 5.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); //Top right
	glVertex3f(x1, y0, 740);
	
	glTexCoord2f(0.0f, 1.0f); //Top Left
	glVertex3f(x0, y0, 740);
	
	glTexCoord2f(0.0f, 0.0f); //Bottom left
	glVertex3f(x0, y1, 740);
	
	glTexCoord2f(1.0f, 0.0f); //Bottom right
	glVertex3f(x1, y1, 740);
	
    
	glEnd();
	glPopMatrix();
	
	glDisable(GL_TEXTURE_2D);
}