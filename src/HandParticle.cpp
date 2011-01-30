/*
 *  HandParticle.cpp
 */

// From particle sample of http://yoppa.org
#include "HandParticle.h"

HandParticle::HandParticle(ofPoint _pos, float _radius, float _maxSpeed, float _phaseSpeed, float _lifeLength, bool _positionFluctuation) {
    pos = _pos;
    radius = _radius;
    phaseSpeed = _phaseSpeed;
    maxSpeed = _maxSpeed;
    lifeLength = _lifeLength;
	
	if (_positionFluctuation) {
		pos.x = ofRandom(pos.x-15.0f, pos.x+15.0f);
		pos.y = ofRandom(pos.y-15.0f, pos.y+15.0f);
	}
	
	
    speed.x = ofRandom(-maxSpeed, maxSpeed);
    speed.y = ofRandom(-maxSpeed, maxSpeed);
    phase = ofRandom(0, PI*2);
    dead = false;
}

HandParticle::~HandParticle(){}

void HandParticle::update() {
    phase += phaseSpeed;
    if (phase > lifeLength) {
        dead = true;
    }
    pos += speed;
}

void HandParticle::draw() {
    float r = sin(phase)*radius;
    ofSetColor(127, 255, 255, 11);
    ofCircle(pos.x, pos.y, radius*2.0);
    ofSetColor(31, 127, 255, 127);
    ofCircle(pos.x, pos.y, r);
    ofSetColor(255, 255, 255);
    ofCircle(pos.x, pos.y, r*0.25);
}

void HandParticle::setPos(ofPoint _pos) {
    pos = _pos;
}

void HandParticle::setRadius(float _radius) {
    radius = _radius;
}
