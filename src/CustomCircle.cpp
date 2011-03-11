/*
 *  CustomCircle.cpp
 *  arakawaVision
 *
 *  Created by Takashi on 11/02/21.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "CustomCircle.h"

void CustomCircle::setRandomColor() {
	fillColor = ofColor();
	fillColor.r = ofRandom(0, 200);
	fillColor.g = ofRandom(180, 255);
	fillColor.b = ofRandom(150, 255);
}

void CustomCircle::draw() {
	if (dead) return;
	
	float radius = getRadius();
	glPushMatrix();
	glTranslatef(getPosition().x, getPosition().y, 0);
	ofFill();
	ofSetColor(fillColor.r, fillColor.g, fillColor.b, 150);
	ofCircle(0, 0, radius);
	ofNoFill();
	ofSetColor(fillColor.r, fillColor.g, fillColor.b, 255);
	ofCircle(0, 0, radius);
	glPopMatrix();
}


void CustomRect::setRandomColor() {
	fillColor = ofColor();
	fillColor.r = ofRandom(150, 255);
	fillColor.g = ofRandom(0, 200);
	fillColor.b = ofRandom(200, 255);	
}

void CustomRect::draw() {
	if(dead) return;
	
	//wow this is a pain
	b2Shape* s = body->GetShapeList();
	const b2XForm& xf = body->GetXForm();
	b2PolygonShape* poly = (b2PolygonShape*)s;
	int count = poly->GetVertexCount();
	const b2Vec2* localVertices = poly->GetVertices();
	b2Assert(count <= b2_maxPolygonVertices);
	b2Vec2 verts[b2_maxPolygonVertices];
	for(int32 i=0; i<count; ++i) {
		verts[i] = b2Mul(xf, localVertices[i]);
	}
	
	
	ofEnableAlphaBlending();
	ofSetColor(fillColor.r, fillColor.g, fillColor.b, 180);
	ofFill();
	ofBeginShape();
	for (int32 i = 0; i <count; i++) {
		ofVertex(verts[i].x*OFX_BOX2D_SCALE, verts[i].y*OFX_BOX2D_SCALE);
	}
	ofEndShape();
	
	ofSetColor(fillColor.r, fillColor.g, fillColor.b);
	ofNoFill();
	ofBeginShape();
	for (int32 i = 0; i <count; i++) {
		ofVertex(verts[i].x*OFX_BOX2D_SCALE, verts[i].y*OFX_BOX2D_SCALE);
	}
	ofEndShape(true);	
	ofDisableAlphaBlending();
	
}