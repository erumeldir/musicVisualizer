#ifndef _VISUALIZER_H_
#define _VISUALIZER_H_

#define FFT_SIZE      4096	//the fft window resolution (fine tune for speed/quality)
#define FFT_NUM_BANDS 64	//number of bands to divide log spectrum into (higher = more clarity & more spread out spectrum)
#define NUM_PATCHES	  39	//number of patches in the horizontal direction
#define BANDS_IN_USE  NUM_PATCHES+1
#define START_BAND    10	//the band number to start at (first band # = 0)

#include "Geometry.h"
#include "Light.h"
#include "Camera.h"
#include "Matrix4.h"
#include "ShaderGroup.h"
#include "BezierSurface.h"
#include "AudioManager.h"
#include "FBO.h"


/*
 * Main class to contain all methods for the visualizer
 * Follows the singleton pattern
 */
class Visualizer
{
private:
	const static int NUM_LIGHTS = 1;
	const static int DEFAULT_CULLING = false;

	static bool instanceFlag;
	static Visualizer* instance;
	Visualizer(int*,char**);

	void init(int*,char**);	//window, gl and application init

	void updateScene();		//update the scene

	int width;
	int height;

	//state vars
	bool cullingEnabled;

	//camera and frustm for culling logic
	Frustum frustum;
	Camera camera;

	//lights
	Light* lights[NUM_LIGHTS];

	//geometry
    Sphere* testSphere;

	//scene graph groups
	MatrixTransform* world;

	//Audio
	AudioManager* audioManager;
	//float*        fftBuf;

	float fftBuf[FFT_SIZE];
	float fftBands[FFT_NUM_BANDS];
	float patchBands[BANDS_IN_USE];

public:
	static Visualizer* getInstance(int*,char**);
	static Visualizer* getInstance();
	~Visualizer();

	//callback methods for glut
	static void idleCallback(void);					//handle idle events
	static void displayCallback(void);				//handle draw events
	static void onReshape(int,int);					//handle reshape events
	static void onKeyboard(unsigned char,int,int);	//handle keyboard events

	int getWidth() { return width; }	//get the window width
	int getHeight() { return height; }	//get the window height	

	void run();		//runs the app
};

#endif