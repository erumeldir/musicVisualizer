#include "ParticleSystem.h"
#include "SOIL.h"


ParticleSystem::ParticleSystem(Vector3 pos, int nParticles, double lifetime, char* spriteName, Shader* partShader)
{
  // create space for all for the particles
  particles = new Particle[nParticles];
  numParticles = nParticles;
  position = pos;
  globalLifetime = lifetime;
  particleSprite = SOIL_load_OGL_texture
    (
    spriteName,
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );

  if (0 == particleSprite)
  {
    printf("SOIL loading error: '%s'\n", SOIL_last_result());
  }

  particleShader = partShader;
}


ParticleSystem::~ParticleSystem()
{
  // deallocate particle array
  delete[] particles;
}


/*********************GETTERS AND SETTERSi*******************/
// gets position of system
Vector3 ParticleSystem::getPosition()
{
  return position;
}
// sets position
void ParticleSystem::setPosition(Vector3 pos)
{
  position = pos;
}
// gets particle in the particle array
Particle& ParticleSystem::getParticle(int particleNum)
{
  return particles[particleNum];
}
// sets particle in the array to the given particle
void ParticleSystem::setParticle(int particleNum, Particle newValue)
{
  particles[particleNum] = newValue;
}
// gets number of particles
int ParticleSystem::getNumParticles()
{
  return numParticles;
}
// gets the reference point for all the lifetimes of the particles
int ParticleSystem::getGlobalLifetime()
{
  return globalLifetime;
}
// sets the reference point for lifetimes of the particles
void ParticleSystem::setGlobalLifetime(int newLifetime)
{
  globalLifetime = newLifetime;
}

// TODO: optimize this!!
// finds next available spot in the particle array and returns its index
// returns -1 if no spot was found
int ParticleSystem::findNext()
{
  for (int i = 0; i < numParticles; i++)
  {
    if (particles[i].state == DEAD)
    {
      return i;
    }
  }

  // no available spots right now
  return -1;
}

// finds next avaialbe spot and makes it into new emitter
void ParticleSystem::triggerEmitter(Vector3 direction)
{
  // find next available spot
  int index = findNext();

  // if there WAS an available spot
  if (index != -1)
  {
    particles[index].state = ALIVE;
    particles[index].p0 = position;
    particles[index].lifetime = globalLifetime + rand() % 500;
    particles[index].time = 0;
    particles[index].v0 = direction;
  }
}



/*
 * draw
 *
 * overidden draw method from Node.
 * updates each particles lifetimes, 
 * draws all of the children
 * and passes them to the vertex shader
 */
void ParticleSystem::draw(Matrix4, Frustum, bool)
{
  // turning on flag in shader
  particleShader->uniform1i("particle", 1);
  glActiveTexture(GL_TEXTURE0 + 2);
  glBindTexture(GL_TEXTURE_2D, particleSprite);
  particleShader->uniform1i("tex", 2);

  glEnable(GL_POINT_SPRITE);

  // get the current time
  int currentTime = frameTimer.getElapsedTimeInMilliSec();
  // array of vertices to pass to shader
  float* vertices = new float[numParticles * 3];
  float* initialVel = new float[numParticles * 3];
  float* time = new float[numParticles];
  glPointSize(7);
  //glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);

  // update particles' existence time
  for (int i = 0; i < numParticles; i++)
  {
    // initial position of current point
    Vector3 currPoint = particles[i].p0;
    // initial vel of current point
    Vector3 currVel = particles[i].v0;

    particles[i].time += currentTime - prevFrameTime;

    // check if particle exceeds its lifetime
    if (particles[i].time > particles[i].lifetime)
      particles[i].reset();
    else
    {


      if (particles[i].state == ALIVE)
      {
      // draw particles' vertices
         glBegin(GL_POINTS);
         glColor4f(1, 1, 1, 1);
         Vector3 g(0, -9.8 * 30, 0);
         float t = particles[i].time / 1000.0;
         g.scale(0.5 * t * t);
         currVel.scale(t);
         currPoint += currVel + g;
         glVertex3f(currPoint[0], currPoint[1], currPoint[2]);        
         glEnd();

      }
      else{
        // draw particles' vertices
      /*  glBegin(GL_POINTS);
        glColor4f(0,0,0,0);
        glVertex3f(currPoint[0], currPoint[1], currPoint[2]);
        glEnd();*/
      }
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    // set initial position for vertex shader
    vertices[i * 3] = currPoint[0];
    vertices[i * 3 + 1] = currPoint[1];
    vertices[i * 3 + 2] = currPoint[2];
    // initial velocity for shader
    initialVel[i * 3] = currVel[0];
    initialVel[i * 3 + 1] = currVel[1];
    initialVel[i * 3 + 2] = currVel[2];
    // set time for shader
    time[i] = particles[i].time;
  }
  // update prevTime
  prevFrameTime = currentTime;


  //
  particleShader->uniform1i("particle", 0);
}

// overidden bounding sphere method from Node
void ParticleSystem::computeBoundingSphere(Matrix4)
{

}