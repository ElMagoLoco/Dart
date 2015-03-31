#include "Sound.h"


SoundSystem* gSound = 0;

SoundSystem::SoundSystem()
{
	init();
}

SoundSystem::~SoundSystem()
{
	FR(mSystem->release());
}
//init fmod
void SoundSystem::init()
{
	
	//check version of file matches dll
	UINT version;
	int numDrivers;
	FMOD_CAPS caps;
	FMOD_SPEAKERMODE speakerMode;
	char name[256];
	FMOD_RESULT result;

	//create system
	FR(System_Create(&mSystem));
	//check version
	FR(mSystem->getVersion(&version));

	if (version < FMOD_VERSION)
	{
		OutputDebugString(L"ERROR: Version of FMOD is older than dll version.\n");
	}
	//get number of sound cards 
	FR(mSystem->getNumDrivers(&numDrivers));
	if (numDrivers == 0)
	{
		FR(mSystem->setOutput(FMOD_OUTPUTTYPE_NOSOUND));
	}
	//if at least one sound card, set speaker output to match
	else
	{
		//caps of default sound card
		FR(mSystem->getDriverCaps(0, &caps, 0, &speakerMode));
		//set speaker mode to match
		FR(mSystem->setSpeakerMode(speakerMode));
	}
	//if hardware acceleration disabled, must make software buffer larger to prevent stuttering
	if (caps & FMOD_CAPS_HARDWARE_EMULATED)
		FR(mSystem->setDSPBufferSize(1024, 10));
	//kludge for SigmaTel sound drivers
	//get name of driver
	FR(mSystem->getDriverInfo(0, name, 256, 0));
	//sigmatel sound devices crackle for some reason if format is PCM 16-bit
	//PCM floating point output solves this
	if (strstr(name, "SigmaTel"))
	{
		//won't use the normal wrapper here because we take special actions below
		result = mSystem->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0, 0, FMOD_DSP_RESAMPLER_LINEAR);
	}
	//now we can init system
	FR(mSystem->init(100, FMOD_INIT_NORMAL, 0));
	//if selected speaker mode isn't supported, switch back to safe fall back
	if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)
	{
		FR(mSystem->setSpeakerMode(FMOD_SPEAKERMODE_STEREO));
		FR(mSystem->init(100, FMOD_INIT_NORMAL, 0));
	}
}
