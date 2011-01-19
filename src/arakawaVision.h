#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "HandParticle.h"
#include "BgEffect.h"

#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxBox2d.h"

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>


class arakawaVision : public ofBaseApp
{

	public:

		void setup();
		void update();
		void draw();
		void exit();
	

		void keyPressed  (int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
	
		void checkDepthUpdated(ofxCvGrayscaleImage newGrayImage);
		void drawPointCloud();
		void drawHands();
		void drawHandCircle();
		void drawBackground();
	
		void createBox2dObjects(float x, float y);
		void drawBox2dObjects();
	
		// Kinect
		ofxKinect kinect;
		int angle;

		ofxCvColorImage		colorImage;
		ofxCvGrayscaleImage		checkGrayImage;

		ofxCvGrayscaleImage 	grayImage;
		ofxCvGrayscaleImage 	grayThresh;
		ofxCvGrayscaleImage 	grayThreshFar;

		ofxCvContourFinder 	contourFinder;
		
		bool				showFullScreen;
		bool				debug;

		// for image
		int 				nearThreshold;
		int					farThreshold;
		int					nearDistance;
		int					farDistance;

		vector <HandParticle *> circles;
		vector <ofxBox2dCircle *> contorParticles;


		// for background
		float				backgroundPosition;
		float				drawPointCloudSize;
		vector <BgEffect *> bgEffects;
	
	
		float				pointClowdRotate;
	
		ofxBox2d box2d;
		vector <ofxBox2dCircle *> box2dCircles;
		vector <ofxBox2dRect *> box2dRects;
		bool drawing;
		ofPoint drawStart;
		int shape; // 0:circle, 1:rect
	
	
		// fonts
		ofTrueTypeFont		fontOratorStd;
		// for timer
		int					frameCount;
};

#endif
