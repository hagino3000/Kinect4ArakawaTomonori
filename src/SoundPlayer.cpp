/*
 *  SoundPlayer.cpp
 *  arakawaVision
 *
 *  Created by Takashi on 11/01/21.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "SoundPlayer.h"

//コンストラクタ
SoundPlayer::SoundPlayer() {
	
	isPlaying = false;
	isDetecting = false;
	
	airSoundsNum = 5;
	
	// TODO
}

SoundPlayer::~SoundPlayer(){}

void setVol(ofSoundPlayer *p, float vol, float minimum=0.0f, float maximun=1.0f) {
	//*p->setVolume(vol);
}

void SoundPlayer::update() {
	/*
	ofSoundUpdate();
	
	ofSoundPlayer * curSound = airSounds[currentPlayPosition];
	ofSoundPlayer * nextSound = airSounds[(currentPlayPosition+1)%airSoundsNum];
	if (curSound->getPosition() > 0.85f) {
		curSound->setVolume(max(curSound->volume-0.005, 0.1));
		
		if (nextSound->getIsPlaying()) {
			if (!isNotifying && !isDetecting) {
				nextSound->setVolume(min(nextSound->volume+0.005, 1.0));
			}
		} else {
			nextSound->play();
		}		
	} else {
		if (isNotifying || isDetecting) {
			curSound->setVolume(max(curSound->volume-0.01, 0.3));
		} else {
			curSound->setVolume(min(curSound->volume+0.01, 1.0));
		}
	}
	
	float notifyV = isNotifying ? 0.01 : -0.01;
	soundNotify1.setVolume(max(min(soundNotify1.volume+notifyV, 0.2f), 0.0f));
	soundNotify2.setVolume(max(min(soundNotify2.volume+notifyV, 1.0f), 0.0f));
	
	float detectV = isDetecting ? 0.005 : -0.005;
	soundArakawaWorld.setVolume(max(min(soundArakawaWorld.volume+detectV, 1.0f), 0.0f));
	
	if (!curSound->getIsPlaying()) {
		currentPlayPosition = (currentPlayPosition + 1) % airSoundsNum;
		std::cout << "Sound Change!! " + ofToString(currentPlayPosition) << endl;
	}
	*/
}

void SoundPlayer::play() {
	//airSounds[currentPlayPosition]->play();
}

void SoundPlayer::playNotify() {
	/*
	isNotifying = true;
	if (soundNotify1.getIsPlaying() == false) {
		soundNotify1.play();
	}
	if (soundNotify2.getIsPlaying() == false) {
		soundNotify2.play();
	}
	*/
}

void SoundPlayer::stopNotify() {
	/*
	isNotifying = false;
	if (soundNotify1.volume < 0.05) {
		soundNotify1.stop();
	}
	if (soundNotify2.volume < 0.05) {
		soundNotify2.stop();
	}
	*/
}

void SoundPlayer::playDetecting() {
	/*
	isDetecting = true;
	isNotifying = false;
	if (soundArakawaWorld.getIsPlaying() == false) {
		soundArakawaWorld.play();
	}
	*/
}

void SoundPlayer::stopDetecting() {
	/*
	isDetecting = false;
	if (soundArakawaWorld.volume < 0.05) {
		soundArakawaWorld.stop();
	}
	*/
}