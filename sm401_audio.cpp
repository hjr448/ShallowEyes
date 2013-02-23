#include <al.h>
#include <alc.h>
#include <stdio.h>

#include "sm401_voc.h"

#include "sm401_audio.h"

static CVoc *g_pVocLoader = NULL;
ALCcontext *pContext = NULL;
ALCdevice *pDevice = NULL;
ALuint      uiBuffer[18];
ALuint      uiSource[18];  

bool sound_on;

void sm_init_audio(unsigned short onoff){
	sound_on = (onoff==1);
	if (sound_on){
		pDevice = alcOpenDevice(NULL);
		if (pDevice){
			pContext = alcCreateContext(pDevice, NULL);
			if (pContext){
				printf("\nOpened %s Device\n", alcGetString(pDevice, ALC_DEVICE_SPECIFIER));
				alcMakeContextCurrent(pContext);
			}else{
				alcCloseDevice(pDevice);
				printf("Failed to initialize OpenAL\n");
				sound_on=false;
				return;
			}
		}else{
			printf("Failed to initialize OpenAL\n");
			sound_on=false;
			return;
		}
		g_pVocLoader = new CVoc();
	}
};
void sm_exit_audio(){
	if (sound_on){
		int i;
		for (i=0;i<18;i++){
			if (uiSource[i]){
				alSourceStop(uiSource[i]);
				alDeleteSources(1, &uiSource[i]);
				alDeleteBuffers(1, &uiBuffer[i]);
			}
		}
		alcMakeContextCurrent(NULL);
		alcDestroyContext(pContext);
		alcCloseDevice(pDevice);
		if (g_pVocLoader)
		{
			delete g_pVocLoader;
			g_pVocLoader = NULL;
		}
	}
};

void sm_sounds_init(unsigned short n, unsigned char* buffer){
	if (sound_on){
	VOCID			VocID;
	ALint			iDataSize, iFrequency;
	ALenum			eBufferFormat;
	ALchar			*pData;

	alGenBuffers( 1, &uiBuffer[n] );
	
	g_pVocLoader->LoadVocBuffer(buffer, &VocID);
	g_pVocLoader->GetVocSize(VocID, (unsigned long*)&iDataSize);
	g_pVocLoader->GetVocData(VocID, (void**)&pData);
	g_pVocLoader->GetVocFrequency(VocID, (unsigned long*)&iFrequency);
	g_pVocLoader->GetVocALBufferFormat(VocID, &alGetEnumValue, (unsigned long*)&eBufferFormat);
	
	alGetError();
	alBufferData(uiBuffer[n], eBufferFormat, pData, iDataSize, iFrequency);
	if (alGetError() != AL_NO_ERROR) printf("Failed to load FX%d.VOC\n", n+1);

	// Generate a Source to playback the Buffer
    alGenSources( 1, &uiSource[n] );
	// Attach Source to Buffer
	alSourcei( uiSource[n], AL_BUFFER, uiBuffer[n] );
	//alSourcePlay( uiSource[n] );
	}
}

void sm_sounds_play(unsigned short n){
	if (sound_on){
	    alSourcePlay( uiSource[n] );
	}
}

int test() {
	int i;
	ALuint      uiBuffer;
	ALuint      uiSource;  
	ALint       iState;

	// Load Voc file into OpenAL Buffer
	VOCID			VocID;
	ALint			iDataSize, iFrequency;
	ALenum			eBufferFormat;
	ALchar			*pData;
	
		// Play Source
    alSourcePlay( uiSource );
	printf("Playing Source ");
		
	do
	{
		//Sleep(100);
		printf(".");
		// Get Source State
		alGetSourcei( uiSource, AL_SOURCE_STATE, &iState);
	} while (iState == AL_PLAYING);

	printf("\n");

	return 0;
}


