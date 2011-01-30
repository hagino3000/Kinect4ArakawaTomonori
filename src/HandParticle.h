/*
 *  HandParticle.h
 */

// From particle sample of http://yoppa.org
#ifndef _MY_CIRCLE
#define _MY_CIRCLE

#include "ofMain.h"

class HandParticle {
	
public:
    HandParticle(ofPoint pos, float radius, float maxSpeed = 0.4, float phaseSpeed = 0.06, float lifeLength = 20.0, bool positionFluctuation = false);
    ~HandParticle();
    void update();
    void draw();
    void setPos(ofPoint pos);
    void setRadius(float radius);
    ofPoint speed;
    float maxSpeed;
    float phase;
    float phaseSpeed;
    bool dead;
    float lifeLength;
	
private:
    ofPoint pos;
    float radius;
};

#endif