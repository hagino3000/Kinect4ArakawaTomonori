/*
 *  BgEffect.cpp
 *  arakawa1
 *
 *  Created by Takashi on 11/01/13.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "BgEffect.h"


//コンストラクタ
BgEffect::BgEffect(int _direction, float _position) {
	
	direction = _direction;
	speed = ofRandom(50, 200);
	dead = false;
	
	if (direction == 0) {
		length = ofRandom(ofGetWidth()/4, ofGetWidth()/2);
		startPos = ofPoint(-length, _position);
		endPos = ofPoint(0, _position);
	} else {
		length = ofRandom(ofGetHeight()/4, ofGetHeight()/2);
		startPos = ofPoint(_position, -length);
		endPos = ofPoint(_position, 0);
	}
}

BgEffect::~BgEffect(){}

void BgEffect::update() {
	if (direction == 0) {
		startPos.x += speed;
		endPos.x += speed;
		
		if (startPos.x > ofGetWidth()) {
			dead = true;
		}
	} else {
		startPos.y += speed;
		endPos.y += speed;
		
		if (startPos.y > ofGetHeight()) {
			dead = true;
		}
	}
}

void BgEffect::draw() {
	ofSetColor(255, 255, 255, 175);
	if (direction == 0) {
		ofLine(endPos.x, endPos.y, endPos.x-10, endPos.y);
	} else {
		ofLine(endPos.x, endPos.y, endPos.x, endPos.y-10);		
	}
	ofSetColor(255, 255, 255, 135);
	if (direction == 0) {
		ofLine(startPos.x+100, startPos.y, endPos.x, endPos.y);
	} else {
		ofLine(startPos.x, startPos.y+100, endPos.x, endPos.y);
	}
	ofSetColor(255, 255, 255, 85);
	if (direction == 0) {
		ofLine(startPos.x, startPos.y, startPos.x+100, startPos.y);
	} else {
		ofLine(startPos.x, startPos.y, startPos.x, startPos.y+100);
	}
	
}

