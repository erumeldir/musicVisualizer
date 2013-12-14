#ifndef _AUDIO_MANAGER_H_
#define _AUDIO_MANAGER_H_

#include <stdio.h>
#include "fmod.hpp"
#include "fmod_errors.h"
#include "Timer.h"

//error check macro for FMOD init
#define FMOD_ERRCHECK(val_check) \
        if(val_check != FMOD_OK) { \
				printf("FMOD ERROR! (%d) %s\n", result, FMOD_ErrorString(val_check)); \
                fmodErrThrown = true; \
        }

class AudioManager
{
private:
	static float MAX_BAND;
	static float MIN_SPECTRUM_FREQ;
	static float BOTTOM_SPECTRUM_FREQ;
  float powerArray[22];
  Timer beatTimer;
  int timeBetweenBeats;
  int timeElapsed;

	FMOD::System*    system;
	FMOD_RESULT		 result;
	unsigned int     version;
	int			     numDrivers;
	FMOD_SPEAKERMODE speakerMode; //number of channels
    FMOD_CAPS        caps;
    char             driverName[256];
    bool             fmodErrThrown; //used to check success without fatal exits/crashes
    float            distFactor;

	//sounds
	FMOD::Sound* currentStream;
	FMOD::Channel* mainChannel;

	int initFMOD();	//call the fmod init routine

public:
	AudioManager();
	~AudioManager();

	bool loadSound(char*);
	bool play();
	bool stop();

	bool getFFT(float*,int);					//get a linear fft
	bool getLogFFT(float*,int,float*,int);		//get a logarithmic fft
	static bool clampBands(float*,int,float*,int,int);	//clamp the bands to a smaller subset range
  bool detectBeat(float* currentBands);

  void update();
};

#endif