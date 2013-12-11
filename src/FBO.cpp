#include "FBO.h"

using namespace std;

FBO::FBO(unsigned int w, unsigned int h) : width(w), height(h)
{
	isValidFBO = false;
	isActivated = false;
	depthStencilBuf = false;
	maskBuf = false;
}


void FBO::generateColorOnly()
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

	// Link texture and depth buffer together to create framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTex, 0);

	// Set draw buffers
	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers);

	// Check if framebuffer creation succeeded
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		fprintf(stderr, "Framebuffer creation failed!\n");
		exit(-1);
	}

	isValidFBO = true;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::generateColorAndMask()
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


	// Create mask texture
	maskBuf = true;
	glGenTextures(1, &fboMask);
	glBindTexture(GL_TEXTURE_2D, fboMask);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);

	// Link texture and depth buffer together to create framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, fboMask, 0);

	// Set draw buffers
	GLenum DrawBuffers[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
	glDrawBuffers(2, DrawBuffers);

	// Check if framebuffer creation succeeded
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		fprintf(stderr, "Framebuffer creation failed!\n");
		exit(-1);
	}

	isValidFBO = true;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::generateColorAndDepth()
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

	// Create combined depth/stencil buffer
	depthStencilBuf = true;
	glGenRenderbuffers(1, &rbo_depth_stencil);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth_stencil);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

	// Link texture and depth buffer together to create framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTex, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_depth_stencil);

	// Set draw buffers
	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers);

	// Check if framebuffer creation succeeded
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		fprintf(stderr, "Framebuffer creation failed!\n");
		exit(-1);
	}

	isValidFBO = true;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

	// Create mask texture
	maskBuf = true;
	glGenTextures(1, &fboMask);
	glBindTexture(GL_TEXTURE_2D, fboMask);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);

	// Create combined depth/stencil buffer
	depthStencilBuf = true;
	glGenRenderbuffers(1, &rbo_depth_stencil);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth_stencil);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

	// Link texture and depth buffer together to create framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, fboMask, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_depth_stencil);

	// Set draw buffers
	GLenum DrawBuffers[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
	glDrawBuffers(2, DrawBuffers);

	// Check if framebuffer creation succeeded
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		fprintf(stderr, "Framebuffer creation failed!\n");
		exit(-1);
	}

	isValidFBO = true;

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

void FBO::activateMask()
{
	glBindTexture(GL_TEXTURE_2D,fboMask);
}

void FBO::deactivateMask()
{
	deactivateTexture();
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

	if (maskBuf)
	{
		glBindTexture(GL_TEXTURE_2D, fboMask);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
	if (depthStencilBuf)
	{
		glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth_stencil);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
}