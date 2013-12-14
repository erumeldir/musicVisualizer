#ifndef _PARTICLE_SYSTEM_H_
#define _PARTICLE_SYSTEM_H_

#include "Timer.h"
#include "Geometry.h"
#include "Shader.h"
#include <GL/glut.h>

enum ParticleState { DEAD, ALIVE };

using namespace std;

struct Particle {
  Vector3 p0;             // Initial position of the particle
  Vector3 v0;             // Initial velocity of the particle
  int time;               // how long the particle has been alive (in milliseconds)
  int lifetime;           // max lifetime of this particular particle (in milliseconds)
  ParticleState state;    // current state of the particle (dead or alive)
  
  // reset the particle back to it's initial state
  void reset()
  {
    time = 0;
    state = DEAD;
  }
};

class ParticleSystem : public Geode
{
protected:
  Vector3 position;       // position of the particle system
  Particle* particles;    // array of particles
  int numParticles;       // number of current active particles
  int globalLifetime;  // global lifetime reference point of all the particles in the system
  int prevFrameTime;   // time at which the previous frame was rendered
  GLuint particleSprite;  //the texture for the particle
  Shader* particleShader; // pointer to the particle shader

  Timer frameTimer;       // object used to calculate milliseconds

public:
  ParticleSystem(Vector3 pos, int nParticles, double lifetime, char* spriteName, Shader* particleShader);
  ~ParticleSystem();

  void draw(Matrix4, Frustum, bool);    // overidden draw method from Node
  void computeBoundingSphere(Matrix4);  // overidden bounding sphere method from Node

  int findNext();                   // finds next available spot in the particle array
  void triggerEmitter(Vector3);     // makes new emitter with given position

  // getters and setters
  Vector3 getPosition();            // gets position of system
  void setPosition(Vector3);        // sets position
  Particle& getParticle(int);       // gets particle in the particle array
  void setParticle(int, Particle);  // sets particle in the array to the given particle
  int getNumParticles();            // gets number of particles
  int getGlobalLifetime();       // gets the reference point for all the lifetimes of the particles
  void setGlobalLifetime(int);   // sets the reference point for lifetimes of the particles
  Particle& operator [](int i) { return getParticle(i); } // operator overload for brackets
};

#endif