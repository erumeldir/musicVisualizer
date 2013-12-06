#include "AudioManager.h"
#include <iostream>
#include <math.h>

using namespace std;

float AudioManager::MAX_BAND = 14.427473;
float AudioManager::MIN_SPECTRUM_FREQ = 20;
float AudioManager::BOTTOM_SPECTRUM_FREQ = 60;

/*
 * Takes in a size for the fft window. Should be a power of 2
 */
AudioManager::AudioManager()
{
	initFMOD();
	currentStream = NULL;
}

/*
 *
 */
AudioManager::~AudioManager()
{

}

/*
 * Start FMOD with necessary options
 */
int AudioManager::initFMOD()
{
	fmodErrThrown = false;
	bool computerHasAudio = true;

	cout << "starting FMOD" << endl;
    //create our fmod system
    result = FMOD::System_Create(&system);
    FMOD_ERRCHECK(result);
    if(fmodErrThrown)
        return -1;

    result = system->getVersion(&version);
    FMOD_ERRCHECK(result);
    if(fmodErrThrown)
        return -1;

	cout << "Using FMOD Version " << version << endl;

	//ensure the computer's fmod version is correct
    if(version < FMOD_VERSION)
    {
		printf("ERROR! You are using an old version of FMOD %d. This program requires %d.\n",version,FMOD_VERSION);
        return -1;
    }

	//check to make sure the computer has audio drivers/hardware
    result = system->getNumDrivers(&numDrivers);
    FMOD_ERRCHECK(result);

    if(numDrivers == 0)
    {
        result = system->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		printf("WARNING: No driver found. Sound not enabled\n.");
        computerHasAudio = false;
        FMOD_ERRCHECK(result);
        if(fmodErrThrown)
            return -1;
    }
    else
    {
        result = system->getDriverCaps(0, &caps, 0, &speakerMode);
        FMOD_ERRCHECK(result);
        if(fmodErrThrown)
            return -1;
    }

	//set the user selected speaker mode
    result = system->setSpeakerMode(speakerMode);
    FMOD_ERRCHECK(result);
    if(fmodErrThrown)
        return 0;

    printf("Checking for audio hardware emulation...\n");

    if(caps & FMOD_CAPS_HARDWARE_EMULATED)
    {
        //the user has the hardware acceleration set to off! this is really bad for latency
        result = system->setDSPBufferSize(1024, 10);
        printf("WARNING: Hardware acceleration off!\n");
        FMOD_ERRCHECK(result);
        if(fmodErrThrown)
            return 0;
    }

	//get the driver info
    result = system->getDriverInfo(0, driverName, 256, 0);
    FMOD_ERRCHECK(result);
    if(fmodErrThrown)
        return 0;

    printf("Driver: %s\n\n", driverName);

    if(strstr(driverName, "SigmaTel"))
    {
        //SigmaTel devices crackle if format is PCM 16bit
        //set to PCM floating point to fix
        result = system->setSoftwareFormat(48000,FMOD_SOUND_FORMAT_PCMFLOAT, 0, 0, FMOD_DSP_RESAMPLER_LINEAR);
        FMOD_ERRCHECK(result)
        if(fmodErrThrown)
            return 0;
    }

	//System::init(Max channels, init flags, extra driver data) only change max channels/flags
    result = system->init(100, FMOD_INIT_NORMAL | FMOD_INIT_VOL0_BECOMES_VIRTUAL, 0);
    if(result == FMOD_ERR_OUTPUT_CREATEBUFFER)
    {
        //selected speaker mode not supported by the soundcard, switch back to stereo and retry init
        result = system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
        fmodErrThrown = false;
        result = system->init(100, FMOD_INIT_NORMAL, 0);
    }
    FMOD_ERRCHECK(result);
    if(fmodErrThrown)
        return 0;

    if(computerHasAudio)
        return 1;
    else
        return 0;
}

/*
 * Readies a sound for playback. Unloads the current sound if exists
 */
bool AudioManager::loadSound(char* filename)
{
	//TODO add sound unloading

	result = system->createStream(filename, FMOD_DEFAULT, 0, &currentStream);
	FMOD_ERRCHECK(result);
	if(fmodErrThrown)
		return false;
	else
		return true;
}

/*
 * Plays the currently loaded sound
 */
bool AudioManager::play()
{
	fmodErrThrown = false;

	bool isPlaying = false;
	result = mainChannel->isPlaying(&isPlaying);
	FMOD_ERRCHECK(result);
	if(isPlaying)
	{
		result = mainChannel->stop();
		FMOD_ERRCHECK(result);
	}

	//nothing on channel or channel uninitialized (free)
	if(result == 36)
	{
		fmodErrThrown = false;
	}

	if(!fmodErrThrown && currentStream != NULL)
	{
		result = currentStream->setMode(FMOD_LOOP_OFF);
		FMOD_ERRCHECK(result);
		result = system->playSound(FMOD_CHANNEL_FREE, currentStream, false, &mainChannel);
		FMOD_ERRCHECK(result);

		if(fmodErrThrown)
			return false;
		else
			return true;
	}

	return false;
}

/*
 * Stops the currently loaded sound
 */
bool AudioManager::stop()
{
	fmodErrThrown = false;

	bool isPlaying = false;
	result = mainChannel->isPlaying(&isPlaying);
	FMOD_ERRCHECK(result);
	if(isPlaying)
	{
		result = mainChannel->stop();
		FMOD_ERRCHECK(result);

		if(fmodErrThrown)
			return false;
		else
			return true;
	}
	return false;
}

/*
 * Get the fft at the current time in the channel
 */
bool AudioManager::getFFT(float* fftArray, int size)
{
	fmodErrThrown = false;

	bool isPlaying = false;
	result = mainChannel->isPlaying(&isPlaying);
	FMOD_ERRCHECK(result);

	if(isPlaying)
	{
		result = mainChannel->getSpectrum(fftArray,size,0,FMOD_DSP_FFT_WINDOW_HANNING);
		FMOD_ERRCHECK(result);

		//TODO set spectrum to 0s
		if(fmodErrThrown)
			return false;
		else
			return true;
	}
	return false;
}

/*
 * Get a equal division band log fft at the current time in the channel
 * The current method used is fairly expensive because it averages all frequencies that
 * fall in a current log band. (Higher frequency ranges might not need as many averages 
 * since everything is very compressed visually). TODO: Optimize & Detect missing bands
 */
bool AudioManager::getLogFFT(float* fftArray, int fftSize, float* bandArray, int bands)
{
	//reset the bands
	for(int i=0; i<bands; i++)
	{
		bandArray[i] = 0.0;
	}

	//compute fft
	if(getFFT(fftArray,fftSize))
	{
		float logFreq;
		float freqSum = 0;
		int numFreqSummed = 0;
		int currentBand = 0;
		float bandwidth = MAX_BAND / ((float)bands - 1.0);
		float halfBandwidth = bandwidth / 2.0;

		//for(int i=ceil((MIN_SPECTRUM_FREQ*fftSize)/22050.0); i<fftSize; i++)
		for(int i=0;i<fftSize;i++)
		{
			//logFreq = log( (i*(22050.0/fftSize)-MIN_SPECTRUM_FREQ) + 1.0) / log(2.0);
			logFreq = log(i+1.0) / log(2.0);
			//moved beyond the current band
			while(logFreq > (currentBand * bandwidth + halfBandwidth))
			{
				//calculate band average
				if(numFreqSummed == 0)
					bandArray[currentBand] = 0;
				else
					bandArray[currentBand] = freqSum/numFreqSummed;

				//inc band and reset sums
				currentBand++;
				freqSum = numFreqSummed = 0.0;
			}

			freqSum += fftArray[i];
			numFreqSummed++;
		}
	}
	return false;
}

/*
 * Update FMOD
 */
void AudioManager::update()
{
  system->update();
}