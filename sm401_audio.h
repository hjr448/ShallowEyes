#ifndef sm401_audio_h
#define sm401_audio_h

void sm_init_audio(unsigned short onoff);
void sm_exit_audio();

void sm_sounds_init(unsigned short n, unsigned char* buffer);
void sm_sounds_play(unsigned short n);



#endif
