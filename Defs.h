#ifndef DEFINITIONS_H
#define DEFINITIONS_H

//#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstdlib>
#include <string>
#include <sdl.h>
#include <sdl_mixer.h>


using namespace std;

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

Mix_Music* music = nullptr;
Mix_Chunk* placeBetsSnd = nullptr;
Mix_Chunk* cardShuffleSnd = nullptr;	// Channel -1
Mix_Chunk* playerWinSnd = nullptr;		// Channel 6
Mix_Chunk* playerLoseSnd = nullptr;		// Channel 3
Mix_Chunk* blackjackSnd = nullptr;		// Channel 0
Mix_Chunk* gameFinishSnd = nullptr;		// Channel 4
Mix_Chunk* dealerBustSnd = nullptr;
Mix_Chunk* playerBustSnd = nullptr;
Mix_Chunk* tieSnd = nullptr;			// Channel 1
Mix_Chunk* noBlackjackSnd = nullptr;	// Channel 2
Mix_Chunk* tieRegSnd = nullptr;			// Channel 5
Mix_Chunk* gotBlackjackSnd = nullptr;	// Channel 7

const int AUDIO_RATE = 44100;
const Uint16 AUDIO_FORMAT = AUDIO_S16;
const int AUDIO_CHANNELS = 2; //stereo
const int AUDIO_BUFFERS = 2048;


#endif