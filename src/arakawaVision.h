#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "BgEffect.h"
#include "SoundPlayer.h"

#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxBox2d.h"
#include "ofxSimpleGuiToo.h"

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

		void checkHuman();
		void checkDepthUpdated();
	
        void drawPointCloud();
        void drawHands();
        void drawBackground();
	
		void drawTitleScene();
		void drawPlayScene();

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

		// mode controll
		int					sceneNum; // 0:title, 1:play
        bool				showFullScreen;
        bool				debug;

        // for image
        int 				nearThreshold;
        int					farThreshold;
        int					nearDistance;
        int					farDistance;

        vector <ofxBox2dCircle *> contorParticles;


        // for background
        float				backgroundPosition;
        float				drawPointCloudSize;
        vector <BgEffect *> bgEffects;


        float				pointClowdRotate;

        ofxBox2d box2d;
        vector <ofxBox2dCircle *> box2dCircles;
        vector <ofxBox2dRect *> box2dRects;


        // fonts
        ofTrueTypeFont		msgFont;
	
		// sounds
		SoundPlayer *			soundPlayer;
	
        // for timer
		int					detectingTime;
		bool				hasHuman;
};

#endif
