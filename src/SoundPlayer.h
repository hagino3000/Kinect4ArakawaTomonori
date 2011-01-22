/*
 *  SoundPlayer.h
 *  arakawaVision
 *
 *  Created by Takashi on 11/01/21.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _SOUND_PLAYER
#define _SOUND_PLAYER

#include "ofMain.h"

class SoundPlayer {
	
public:
    SoundPlayer();
    ~SoundPlayer();
    void update();
    void play();
	void stop();
	void playNotify();
	void stopNotify();
	void playDetecting();
	void stopDetecting();
	
private:
	// sounds
	ofSoundPlayer		soundAir1;
	ofSoundPlayer		soundArakawaWorld;
	ofSoundPlayer		soundEffect1;
	ofSoundPlayer		soundNotify1;
	ofSoundPlayer		soundNotify2;
	
	
	vector <ofSoundPlayer *> airSounds;
	int airSoundsNum;
	int currentPlayPosition;
	
	
	ofSoundPlayer		curSound;
	
	bool				isPlaying;
	bool				isNotifying;
	bool				isDetecting;
};

#endif