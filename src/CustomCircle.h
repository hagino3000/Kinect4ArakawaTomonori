/*
 *  CustomCircle.h
 *  arakawaVision
 *
 *  Created by Takashi on 11/02/21.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxVectorMath.h"
#include "ofxBox2d.h"


class CustomCircle : public ofxBox2dCircle {
public: 
	void draw();
	void setRandomColor();

private:
	ofColor fillColor;
};


class CustomRect : public ofxBox2dRect {
public: 
	void draw();
	void setRandomColor();
	
private:
	ofColor fillColor;	
};