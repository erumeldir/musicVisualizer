#include <stdlib.h>
#include <iostream>
#include "visualizer.h"

#define ENABLE_INIT_TESTING true

//initizlize visualizer instance
bool Visualizer::instanceFlag = false;
Visualizer* Visualizer::instance = NULL;
Shader* testShader;
Shader* testShader2;
Shader* testShader3;

Shader* verticalGaussian;
Shader* horizontalGaussian;

GLuint fbo, fbo_texture, rbo_depth;
GLuint fbo_blur, fbo_texture_blur, rbo_depth_blur;
GLuint vbo_fbo_vertices;
GLuint attrib_v_coord_verticalGaussian, attrib_v_coord_horizontalGaussian;
GLuint uni_fbo_texture_verticalGaussian, uni_fbo_texture_horizontalGaussian;

Vector3* controlPoints;
BezierPatch4 * patch1;

float blurSize = 0.000;
bool blurDirUp = true;



/*******************************SCENE UPDATE**********************************/
void Visualizer::updateScene()
{
  audioManager->update();

	//do update stuff here
	blurSize = 0.0;
	bool fftSucceeded = false;
	fftSucceeded = audioManager->getFFT(fftBuf,FFT_SIZE);
	if(fftSucceeded)
	{
		if (fftBuf[7] > .17)
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
				glVertex3f((barLoc-5)*2,-5.0+(fftBuf[i]*20.0),0.0);
        glVertex3f((barLoc-5)*2, -5.0, 0.0);
			glEnd();
		}
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

	// Set up framebuffer
	// Create texture
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &fbo_texture);
	glBindTexture(GL_TEXTURE_2D, fbo_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	// Create depth buffer
	glGenRenderbuffers(1, &rbo_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
	glBindRenderbuffer(GL_TEXTURE_2D, 0);
	// Link texture and depth buffer together to create framebuffer
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_texture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_depth);
	// Check if framebuffer creation succeeded
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		fprintf(stderr, "Framebuffer creation failed!\n");
		exit(-1);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Set up framebuffer
	// Create texture
	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, &fbo_texture_blur);
	glBindTexture(GL_TEXTURE_2D, fbo_texture_blur);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	// Create depth buffer
	glGenRenderbuffers(1, &rbo_depth_blur);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth_blur);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
	glBindRenderbuffer(GL_TEXTURE_2D, 0);
	// Link texture and depth buffer together to create framebuffer
	glGenFramebuffers(1, &fbo_blur);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_blur);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_texture_blur, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_depth_blur);
	// Check if framebuffer creation succeeded
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		fprintf(stderr, "Framebuffer creation failed!\n");
		exit(-1);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Create quad for drawing
	GLfloat fbo_vertices[] = {
		-1, -1,
		1, -1,
		-1, 1,
		1, 1,
	};
	glGenBuffers(1, &vbo_fbo_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_fbo_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fbo_vertices), fbo_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Set up resources for post processing
	verticalGaussian = new Shader("shaders/gaussianBlur.vs", "shaders/gaussianBlurVertical.fs", true);
	horizontalGaussian = new Shader("shaders/gaussianBlur.vs", "shaders/gaussianBlurHorizontal.fs", true);
	attrib_v_coord_verticalGaussian = glGetAttribLocation(verticalGaussian->getPid(), "v_coord");
	uni_fbo_texture_verticalGaussian = glGetUniformLocation(verticalGaussian->getPid(), "tex");
	attrib_v_coord_horizontalGaussian = glGetAttribLocation(horizontalGaussian->getPid(), "v_coord");
	uni_fbo_texture_horizontalGaussian = glGetUniformLocation(horizontalGaussian->getPid(), "tex");

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

	//testing code
	if(ENABLE_INIT_TESTING)
	{
		//TESTING, replace with scene stuff
		testSphere = new Sphere(6,20,20);
		world->addChild(testSphere);

		// TESTING: ShaderGroup object turns things blue to test shadergroup
		testShader = new Shader("shaders/simpleBlue.vert", "shaders/simpleBlue.frag", true);
		testShader2 = new Shader("shaders/simpleRed.vert", "shaders/simpleRed.frag", true);
		testShader3 = new Shader("shaders/simpleGreen.vert", "shaders/simpleGreen.frag", true);
		ShaderGroup* testShad = new ShaderGroup(testShader);
		ShaderGroup* testShad2 = new ShaderGroup(testShader2);
		ShaderGroup* testShad3 = new ShaderGroup(testShader3);

		// Highest layer: Transform up right and apply blue shader
		MatrixTransform* right = new MatrixTransform();
		right->localTranslate(10, 10, 0);
		Sphere* testSphere2 = new Sphere(3, 10, 10);

		// Second layer: Transform below the highest layer and apply red shader
		MatrixTransform* down = new MatrixTransform();
		down->localTranslate(0, -9, 0);
		Sphere* testSphere3 = new Sphere(3, 10, 10);

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
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear color and depth buffers
	glMatrixMode(GL_MODELVIEW);

	//draw stuff here
	Matrix4 IDENTITY;
	IDENTITY.identity();
  Vector3 randomVec(rand() % 5, rand() % 5, rand() % 5);
  patch1->setControlPoint(0, 0, randomVec);
	//scene->draw(cam.getViewMatrix(),frustum,culling);

  Visualizer::getInstance()->updateScene();



	glFlush();

	glActiveTexture(GL_TEXTURE0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo_blur);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Horizontal blur
	horizontalGaussian->bind();
	glBindTexture(GL_TEXTURE_2D, fbo_texture);
	glUniform1i(uni_fbo_texture_horizontalGaussian, 0);
	GLuint test = glGetUniformLocation(horizontalGaussian->getPid(), "blurSize");
	glUniform1f(test, blurSize);
	glEnableVertexAttribArray(attrib_v_coord_horizontalGaussian);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo_fbo_vertices);
	glVertexAttribPointer(
		attrib_v_coord_horizontalGaussian,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		0
	);

	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glLoadIdentity();
	glBegin(GL_QUADS);
		glVertex2f(-1,-1);
		glVertex2f(-1,1);
		glVertex2f(1,1);
		glVertex2f(1,-1);
	glEnd();

	glDisableVertexAttribArray(attrib_v_coord_horizontalGaussian);
	horizontalGaussian->unbind();

	// Switch back to physical screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glActiveTexture(GL_TEXTURE1);

	// Draw texture to screen
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	verticalGaussian->bind();
	glBindTexture(GL_TEXTURE_2D, fbo_texture_blur);
	glUniform1i(uni_fbo_texture_verticalGaussian, 1);
	test = glGetUniformLocation(verticalGaussian->getPid(), "blurSize");
	glUniform1f(test, blurSize);
	glEnableVertexAttribArray(attrib_v_coord_verticalGaussian);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo_fbo_vertices);
	glVertexAttribPointer(
		attrib_v_coord_verticalGaussian,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		0
	);

	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glLoadIdentity();
	glBegin(GL_QUADS);
		glVertex2f(-1,-1);
		glVertex2f(-1,1);
		glVertex2f(1,1);
		glVertex2f(1,-1);
	glEnd();

	glDisableVertexAttribArray(attrib_v_coord_verticalGaussian);
	verticalGaussian->unbind();

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

	// Resize the framebuffer and depth buffer
	glBindTexture(GL_TEXTURE_2D, fbo_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, w, h);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glBindTexture(GL_TEXTURE_2D, fbo_texture_blur);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth_blur);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, w, h);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
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
	switch(key)
	{
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

  glDeleteRenderbuffers(1, &rbo_depth);
  glDeleteTextures(1, &fbo_texture);
  glDeleteFramebuffers(1, &fbo);
  glDeleteBuffers(1, &vbo_fbo_vertices);
}