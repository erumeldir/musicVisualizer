#include <iostream>
#include <stdlib.h>
#include "GLee.h"
#include <GL/glut.h>
#include "FBO.h"

using namespace std;

FBO::FBO(unsigned int w, unsigned int h) : width(w), height(h)
{
	isActivated = false;
	depthBuf = false;
}


void FBO::generate()
{
	// Create framebuffer
	glGenFramebuffers(1, &fboId);
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);

	// Create color buffer
	glGenTextures(1, &fboTex);
	glBindTexture(GL_TEXTURE_2D, fboTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);


	// Create depth buffer
	depthBuf = true;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

	// Link texture and depth buffer together to create framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTex, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	// Set draw buffers
	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers);

	// Check if framebuffer creation succeeded
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		fprintf(stderr, "Framebuffer creation failed!\n");
		exit(-1);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void FBO::activate()
{
	isActivated = true;
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);
}

void FBO::deactivate()
{
	isActivated = false;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::activateTexture()
{
	glBindTexture(GL_TEXTURE_2D,fboTex);
}

void FBO::deactivateTexture()
{
	glBindTexture(GL_TEXTURE_2D,0);
}

void FBO::destroy()
{
	glDeleteFramebuffers(1, &fboId);
	
	isActivated = false;
}

void FBO::updateSize(unsigned int w, unsigned int h)
{
	width = w;
	height = h;

	glBindTexture(GL_TEXTURE_2D, fboTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	if (depthBuf)
	{
		glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
}