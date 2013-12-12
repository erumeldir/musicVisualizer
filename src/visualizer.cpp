#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <iostream>
#include "visualizer.h"
#include <math.h>
#include <algorithm>

#define ENABLE_INIT_TESTING false

//initizlize visualizer instance
bool Visualizer::instanceFlag = false;
Visualizer* Visualizer::instance = NULL;
Shader* testShader;
Shader* testShader2;
Shader* testShader3;

map<char*, Shader*> shader_map;

map<char*, FBO*> fbo_map;

Vector3* controlPoints;
BezierPatch4 * patch1;
BezierSurface * surface;

float blurSize = 0.0;
bool blurDirUp = true;



/*******************************SCENE UPDATE**********************************/
void Visualizer::updateScene()
{
  audioManager->update();

	//do update stuff here
	blurSize = 0.0;

	bool fftSucceeded = false;
	fftSucceeded = audioManager->getLogFFT(fftBuf,FFT_SIZE,fftBands,FFT_NUM_BANDS);
	if(fftSucceeded)
	{
		/*if (fftBuf[7] > .17)
			blurSize = fftBuf[7]/1000.0 + fftBuf[7]/1300.0;
		else
			blurSize = fftBuf[7]/1000.0;
		float barLoc;

		//DRAW STUFF WITH NEW FFT DATA
		for(int i=0;i<FFT_SIZE-1;i++)
		{
			barLoc = log(i*(22050.0/FFT_SIZE));
			glLineWidth(1);
			glColor3f(1, 1, 1);
			glBegin(GL_LINES);
				glVertex3f((barLoc-5)*2,-5.0+(fftBuf[i]*40.0),0.0);
				glVertex3f((barLoc-5)*2, -5.0, 0.0);
			glEnd();
		}*/
		//test logfft stuff
		bool clampSucceeded = AudioManager::clampBands(fftBands, FFT_NUM_BANDS, patchBands, BANDS_IN_USE, START_BAND);
		//for(int i=START_BAND;i<BANDS_IN_USE+START_BAND;i++)
		for(int i=0;i<BANDS_IN_USE;i++)
		{
			glLineWidth(4.5);
			Vector4 color = colorMap.getColor(fftBands[i]);
			glColor4f(color[0],color[1],color[2],color[3]);
			glBegin(GL_LINES);
				glVertex3f((i-FFT_NUM_BANDS/2)*.25+5,60.0 + patchBands[i]*20,0.0);
				glVertex3f((i-FFT_NUM_BANDS/2)*.25+5, 60.0, 0.0);
			glEnd();
		}
    memcpy(&patchBands[BANDS_IN_USE],&patchBands[0],sizeof(float) * 8);

    for (int i = 0; i < 4; i++)
    {
      float temp = patchBands[BANDS_IN_USE + i];
      patchBands[BANDS_IN_USE + i] = patchBands[BANDS_IN_USE + 7 - i];
      patchBands[BANDS_IN_USE + 8 - i] = temp;
    }
  //  bool clampSucceeded = AudioManager::clampBands(fftBands, FFT_NUM_BANDS, patchBands, BANDS_IN_USE, START_BAND);
		surface->addBand(patchBands);
	}
	else
	{
		cout << "FFT ERROR!" << endl;
	}	
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

	// Create main framebuffer
	fbo_map["main"] = new FBO(width, height);
	fbo_map["main"]->generate();

	// Create Framebuffers for performing gaussian blur
	fbo_map["blur1"] = new FBO(width, height);
	fbo_map["blur1"]->generate();
	fbo_map["blur2"] = new FBO(width, height);
	fbo_map["blur2"]->generate();

	// Create main geometry and material shader
	shader_map["mainShader"] = new Shader("shaders/mainShader.vs", "shaders/mainShader.fs", true);
	shader_map["passthrough"] = new Shader("shaders/passthrough.vs", "shaders/passthrough.fs", true);

	// Set up resources for post processing
	shader_map["verticalGaussian"] = new Shader("shaders/gaussianBlur.vs", "shaders/gaussianBlurVertical.fs", true);
	shader_map["horizontalGaussian"] = new Shader("shaders/gaussianBlur.vs", "shaders/gaussianBlurHorizontal.fs", true);

	// Set up fx shader
	shader_map["coolShader"] = new Shader("shaders/coolShader.vs", "shaders/coolShader.fs", true);

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
 // testShader3 = new Shader("shaders/mainShader.vs", "shaders/mainShader.fs", true);
 // ShaderGroup* testShad3 = new ShaderGroup(testShader3);

  // Highest layer: Transform up right and apply blue shader
  MatrixTransform* right = new MatrixTransform();
  right->localTranslate(0, -8, -8);
  
  // test bezier surface
  surface = new BezierSurface(BANDS_IN_USE+8,15, 1,17,150,25, &colorMap);

  // Test glowing
  GlowGroup* testGlow = new GlowGroup(shader_map["mainShader"],true);
  testGlow->addChild(new Sphere(10.0, 20, 10));
  //testGlow->addChild(surface);
  right->addChild(testGlow);


  //testShad3->addChild(surface);
  //right->addChild(testShad3);
 // right->addChild(surface);
  //testGlow->addChild(surface);
 // testShad3->addChild(surface);
  right->addChild(surface);
  world->addChild(right);


	//testing code
	if(ENABLE_INIT_TESTING)
	{
		//TESTING, replace with scene stuff
		testSphere = new Sphere(3,20,20);
		world->addChild(testSphere);

		// TESTING: ShaderGroup object turns things blue to test shadergroup
		testShader = new Shader("shaders/simpleBlue.vert", "shaders/simpleBlue.frag", true);
		testShader2 = new Shader("shaders/simpleRed.vert", "shaders/simpleRed.frag", true);
		testShader3 = new Shader("shaders/simpleGreen.vert", "shaders/simpleGreen.frag", true);
		ShaderGroup* testShad = new ShaderGroup(testShader);
		ShaderGroup* testShad2 = new ShaderGroup(testShader2);
		ShaderGroup* testShad3 = new ShaderGroup(testShader);

		// Highest layer: Transform up right and apply blue shader
		MatrixTransform* right = new MatrixTransform();
		right->localTranslate(10, 10, 0);
		Sphere* testSphere2 = new Sphere(3, 10, 10);

		// Second layer: Transform below the highest layer and apply red shader
		MatrixTransform* down = new MatrixTransform();
		down->localTranslate(0, -9, 0);
		Sphere* testSphere3 = new Sphere(10, 10, 10);

		// Third layer: Transform below second layer and apply green shader
		MatrixTransform* down2 = new MatrixTransform();
		down2->localTranslate(0, -9, 0);
		// testing the bezier Patches
		controlPoints = new Vector3[16];
		controlPoints[0] = Vector3(-1.5, -1.5, 4.0); controlPoints[1] = Vector3(-0.5, -1.5, 2.0);
		controlPoints[2] = Vector3(0.5, -1.5, -1.0);  controlPoints[3] = Vector3(1.5, -1.5, 2.0);
		controlPoints[4] = Vector3(-1.5, -0.5, 1.0); controlPoints[5] = Vector3(-0.5, -0.5, 3.0);
		controlPoints[6] = Vector3(0.5, -0.5, 0.0);  controlPoints[7] = Vector3(1.5, -0.5, -1.0);
		controlPoints[8] = Vector3(-1.5, 0.5, 4.0); controlPoints[9] = Vector3(-0.5, 0.5, 0.0);
		controlPoints[10] = Vector3(0.5, 0.5, 3.0);  controlPoints[11] = Vector3(1.5, 0.5, 4.0);
		controlPoints[12] = Vector3(-1.5, 1.5, -2.0); controlPoints[13] = Vector3(-0.5, 1.5, -2.0);
		controlPoints[14] = Vector3(0.5, 1.5, 0.0);  controlPoints[15] = Vector3(1.5, 1.5, -1.0);

		patch1 = new BezierPatch4(controlPoints);


		// Connect scene graph according to described layers
		// highest layer
		world->addChild(testShad);
		testShad->addChild(right);
		right->addChild(testSphere2);
		right->addChild(down);
		// second layer
		down->addChild(testShad2);
		down->addChild(down2);
		testShad2->addChild(testSphere3);
		// third layer
		down2->addChild(testShad3);
		testShad3->addChild(patch1);
	}

  //light the scene
	lights[0] = new DirectionalLight(GL_LIGHT0);
	lights[0]->setPosition(20,50,30);
	lights[0]->update();
	lights[0]->enable();
  
	//Init audio
	audioManager = new AudioManager();
	//fftBuf = new float[FFT_SIZE];

	audioManager->loadSound("timescar.mp3");
	audioManager->play();

  // Set world to look at the surface
  world->localRotateY(M_PI);
  world->localTranslate(Vector3(0,-71,208));

  //test color gradients
  //colorMap.addColor(Vector3(   0.0,   0.0,     0.0));
  colorMap.addColor(Vector3(0.1992,   0.0,  0.2578));
  colorMap.addColor(Vector3(0.5039, 0.0036, 0.2031));
  colorMap.addColor(Vector3(0.9726, 0.2773, 0.0351));
  colorMap.addColor(Vector3(0.9922, 0.6719, 0.1367));
  colorMap.addColor(Vector3(0.9648, 0.8984,    0.0));

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

	// Set render target to framebuffer
	fbo_map["main"]->activate();
	
	shader_map["mainShader"]->bind();
	shader_map["mainShader"]->uniform1f("glow", 0.0);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear color and depth buffers
	glMatrixMode(GL_MODELVIEW);

	//draw stuff here
	Matrix4 IDENTITY;
	IDENTITY.identity();
	scene->draw(cam.getViewMatrix(),frustum,culling);

	Visualizer::getInstance()->updateScene();

	// Allow OpenGL time to finish what it is doing
	glFlush();

	shader_map["mainShader"]->unbind();

	// Switch projection matrix to identity
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);

	// Switch FBO
	fbo_map["main"]->deactivate();
	fbo_map["blur1"]->activate();
	
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// POST-PROCESSING: Horizontal Gaussian Blur
	shader_map["horizontalGaussian"]->bind();

	// Use result from first pass as input texture to shader
	glActiveTexture(GL_TEXTURE0 + 0);
	fbo_map["main"]->activateTexture();
	shader_map["horizontalGaussian"]->uniform1i("tex", 0);

	glActiveTexture(GL_TEXTURE0 + 1);
	fbo_map["main"]->activateMask();
	shader_map["horizontalGaussian"]->uniform1i("mask", 1);
	blurSize = .0051;
	// Set how blurred the result should be
	shader_map["horizontalGaussian"]->uniform1f("blurSize", 1.0/1024.0);
	shader_map["horizontalGaussian"]->uniform1f("sigma", 10.0);

	// Draw result on a quad
	glLoadIdentity();
	glBegin(GL_QUADS);
		glTexCoord2f(0,0);
		glVertex2f(-1,-1);
		glTexCoord2f(0,1);
		glVertex2f(-1,1);
		glTexCoord2f(1,1);
		glVertex2f(1,1);
		glTexCoord2f(1,0);
		glVertex2f(1,-1);
	glEnd();

	shader_map["horizontalGaussian"]->unbind();

	// Switch FBO to second pass Gaussian blur FBO
	fbo_map["blur1"]->deactivate();
	fbo_map["blur2"]->activate();

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// POST-PROCESSING: Vertical Gaussian Blur
	shader_map["verticalGaussian"]->bind();

	// Set the result from first-pass gaussian blur as the texture for the second pass
	glActiveTexture(GL_TEXTURE0 + 0);
	fbo_map["blur1"]->activateTexture();
	shader_map["verticalGaussian"]->uniform1i("tex", 0);

	// Set how blurred the result should be
	shader_map["verticalGaussian"]->uniform1f("blurSize", 1.0/1024.0);
	shader_map["verticalGaussian"]->uniform1f("sigma", 3.0);

	// Draw result on a quad
	glLoadIdentity();
	glBegin(GL_QUADS);
		glTexCoord2f(0,0);
		glVertex2f(-1,-1);
		glTexCoord2f(0,1);
		glVertex2f(-1,1);
		glTexCoord2f(1,1);
		glVertex2f(1,1);
		glTexCoord2f(1,0);
		glVertex2f(1,-1);
	glEnd();

	shader_map["verticalGaussian"]->unbind();

	// Switch back to physical screen
	fbo_map["blur2"]->deactivate();

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// FINAL RENDERING PASSES
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	shader_map["coolShader"]->bind();
	shader_map["coolShader"]->uniform1f("time", GetTickCount()/1000.0);
	shader_map["coolShader"]->uniform2f("resolution",Visualizer::getInstance()->height,Visualizer::getInstance()->width);
	// Draw result on a quad
	glLoadIdentity();
	glBegin(GL_QUADS);
		glTexCoord2f(0,0);
		glVertex2f(-1,-1);
		glTexCoord2f(0,1);
		glVertex2f(-1,1);
		glTexCoord2f(1,1);
		glVertex2f(1,1);
		glTexCoord2f(1,0);
		glVertex2f(1,-1);
	glEnd();
	shader_map["coolShader"]->unbind();

	shader_map["passthrough"]->bind();

	// Set the result from first-pass gaussian blur as the texture for the second pass
	glActiveTexture(GL_TEXTURE0 + 0);
	fbo_map["main"]->activateTexture();
	shader_map["passthrough"]->uniform1i("tex", 0);

	// Draw result on a quad
	glLoadIdentity();
	glBegin(GL_QUADS);
		glTexCoord2f(0,0);
		glVertex2f(-1,-1);
		glTexCoord2f(0,1);
		glVertex2f(-1,1);
		glTexCoord2f(1,1);
		glVertex2f(1,1);
		glTexCoord2f(1,0);
		glVertex2f(1,-1);
	glEnd();
	
	glBlendFunc(GL_ONE,GL_ONE);

		// Set the result from first-pass gaussian blur as the texture for the second pass
	glActiveTexture(GL_TEXTURE0 + 0);
	fbo_map["blur2"]->activateTexture();
	shader_map["passthrough"]->uniform1i("tex", 0);

	// Draw result on a quad
	glLoadIdentity();
	glBegin(GL_QUADS);
		glTexCoord2f(0,0);
		glVertex2f(-1,-1);
		glTexCoord2f(0,1);
		glVertex2f(-1,1);
		glTexCoord2f(1,1);
		glVertex2f(1,1);
		glTexCoord2f(1,0);
		glVertex2f(1,-1);
	glEnd();

	shader_map["passthrough"]->unbind();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	// Restore original projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	// Show result on screen
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

	// Resize the FBOs
	fbo_map["main"]->updateSize(w, h);
	fbo_map["blur1"]->updateSize(w, h);
	fbo_map["blur2"]->updateSize(w, h);
}

/*
 * Called on window idle
 */
void Visualizer::idleCallback()
{
	displayCallback();
}

/*
 * Handle standard keypresses
 */
void Visualizer::onKeyboard(unsigned char key, int x, int y)
{
  float randBand[FFT_NUM_BANDS] = { 0};
	switch(key)
	{
  case 'a':
    for (int i = 0; i < FFT_NUM_BANDS; i++)
    {
      randBand[i] = rand() % 5;
    }
    surface->addBand(randBand);
    break;
  case 'r':
    Visualizer::getInstance()->world->localRotateY(.05);
    break;
  case 'R':
    Visualizer::getInstance()->world->localRotateY(-.05);
    break;
  case 'x':
    Visualizer::getInstance()->world->globalTranslate(1, 0, 0);
    break;
  case 'X':
    Visualizer::getInstance()->world->globalTranslate(-1, 0, 0);
    break;
  case 'Y':
    Visualizer::getInstance()->world->globalTranslate(0, -1, 0);
    break;
  case 'y':
    Visualizer::getInstance()->world->globalTranslate(0, 1, 0);
    break;
  case 'Z':
    Visualizer::getInstance()->world->globalTranslate(0, 0, -1);
    break;
  case 'z':
    Visualizer::getInstance()->world->globalTranslate(0, 0, 1);
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

  /*
  glDeleteRenderbuffers(1, &rbo_depth);
  glDeleteTextures(1, &fbo_texture);
  glDeleteFramebuffers(1, &fbo);
  glDeleteBuffers(1, &vbo_fbo_vertices);
  */
}