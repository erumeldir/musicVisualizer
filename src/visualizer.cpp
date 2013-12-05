#include <stdlib.h>
#include <iostream>
#include "visualizer.h"

#define NUM_BANDS 16    // number of bands

//initizlize visualizer instance
bool Visualizer::instanceFlag = false;
Visualizer* Visualizer::instance = NULL;
Shader* testShader;
Shader* testShader2;
Shader* testShader3;

Vector3* controlPoints;
BezierPatch4 * patch1;
BezierSurface * surface;



/*******************************SCENE UPDATE**********************************/
void Visualizer::updateScene()
{
	//do update stuff here

}


/******************************VISUALIZER INIT********************************/

/*
 * Constructor which creates the application, and runs all the necessary init
 */
Visualizer::Visualizer(int* argcp, char** argv)
{
	init(argcp,argv);
}

/*
 * Initializes the window, sets up gl and initializes the app instances/vars
 */
void Visualizer::init(int* argcp, char** argv)
{
	//TODO make fullscreen

	//set up window
	width = 512;
	height = 512;
	glutInit(argcp,argv);										// init glut
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);   // open an OpenGL context with double buffering, RGB colors, and depth buffering
    glutInitWindowSize(width, height);							// set initial window size
    glutCreateWindow("Music Visualizer");    		            // open window and set window title

	//set up OpenGL
	glEnable(GL_DEPTH_TEST);            	    // enable depth buffering
    glClear(GL_DEPTH_BUFFER_BIT);       	    // clear depth buffer
    glClearColor(0.0, 0.0, 0.0, 0.0);   	    // set clear color to black
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // set polygon drawing mode to fill front and back of each polygon
    glDisable(GL_CULL_FACE);					// disable backface culling to render both sides of polygons
    glShadeModel(GL_SMOOTH);             	    // set shading to smooth
    glMatrixMode(GL_PROJECTION); 
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);

	//install callbacks
	glutDisplayFunc(displayCallback);
	glutReshapeFunc(onReshape);
	glutIdleFunc(idleCallback);
	glutKeyboardFunc(onKeyboard);

	//initialize vars
	cullingEnabled = DEFAULT_CULLING;

	//create the scene
	world = new MatrixTransform();
	world->reset();	//reset to identity

  // TESTING: ShaderGroup object turns things blue to test shadergroup
  testShader3 = new Shader("shaders/simpleGreen.vert", "shaders/simpleGreen.frag", true);
  ShaderGroup* testShad3 = new ShaderGroup(testShader3);

  // Highest layer: Transform up right and apply blue shader
  MatrixTransform* right = new MatrixTransform();
  right->localTranslate(0, -8, 0);
  
  // test bezier surface
  surface = new BezierSurface(NUM_BANDS, 3);

  testShad3->addChild(surface);
  right->addChild(testShad3);
  world->addChild(right);

	//light the scene
	lights[0] = new DirectionalLight(GL_LIGHT0);
	lights[0]->setPosition(20,50,30);
	lights[0]->update();
	lights[0]->enable();
}

/*
 * Creates a new instance of the visualizer, initializes it and returns it
 */
Visualizer* Visualizer::getInstance(int *argcp, char **argv)
{
	if(!instanceFlag)
	{
		instance = new Visualizer(argcp,argv);
		instanceFlag = true;
		return instance;
	}
	else
	{
		return instance;
	}
}

/*
 * returns the current visualizer or NULL if it doesnt exist (THIS SHOULD NEVER HAPPEN! *crosses fingers*)
 */
Visualizer* Visualizer::getInstance()
{
	if(!instanceFlag)
	{
		return NULL;
	}
	else
	{
		return instance;
	}
}

/*
 * runs the application
 */
void Visualizer::run()
{
	glutMainLoop();
}

/**************************GLUT CALLBACK FUNCTIONS****************************/

/*
 * Draw frame
 */
void Visualizer::displayCallback()
{
	Node* scene = Visualizer::getInstance()->world;
	Camera cam = Visualizer::getInstance()->camera;
	Frustum frustum = Visualizer::getInstance()->frustum;
	bool culling = Visualizer::getInstance()->cullingEnabled;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear color and depth buffers
	glMatrixMode(GL_MODELVIEW);

	//draw stuff here
	Matrix4 IDENTITY;
	IDENTITY.identity();
 // Vector3 randomVec(rand() % 5, rand() % 5, rand() % 5);
 // patch1->setControlPoint(0, 0, randomVec);
	scene->draw(cam.getViewMatrix(),frustum,culling);
	glFlush();
	glutSwapBuffers();
}

/*
 * Handle a window reshape event
 */
void Visualizer::onReshape(int w, int h)
{
	//update window size
	Visualizer::getInstance()->width = w;
	Visualizer::getInstance()->height = h;

	glViewport(0, 0, w, h);  // set new viewport size
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-10.0, 10.0, -10.0, 10.0, 10, 1000.0); // set perspective projection viewing frustum
    glTranslatef(0, 0, -20);		//may need to adjust this
    glMatrixMode(GL_MODELVIEW);
}

/*
 * Called on window idle
 */
void Visualizer::idleCallback()
{
	Visualizer::getInstance()->updateScene();
	displayCallback();
}

/*
 * Handle standard keypresses
 */
void Visualizer::onKeyboard(unsigned char key, int x, int y)
{
  double amps[NUM_BANDS];
	switch(key)
	{
  case 'a':
    for (int i = 0; i < NUM_BANDS; i++)
    {
      amps[i] = rand() % 5;
    }
    surface->addBand(amps);
    break;
  case 'r':
    Visualizer::getInstance()->world->localRotateY(.05);
    break;
	default:
		break;
	}
}

/********************************* MAIN **************************************/
int main(int argc, char* argv[])
{
  Visualizer* musicVis;
  musicVis = Visualizer::getInstance(&argc,argv);
  musicVis->run();
}