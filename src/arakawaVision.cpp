#include "arakawaVision.h"

float getNearDistance(int nearDepth) {
	return 25 + nearDepth * 1.7;
}
int box2;

//--------------------------------------------------------------
void arakawaVision::setup() {
    std::cout << "Start setup()"<<endl;

	sceneNum = 0;
    debug = false;

    // Setup Kinect
    angle = 5;
    //kinect.init(true);  //shows infrared image
    kinect.init();
    //kinect.setVerbose(true);
    kinect.open();
    kinect.setCameraTiltAngle(angle);
	
	std::cout << "Kinect width" + ofToString(kinect.width) <<endl;
	std::cout << "Kinect height" + ofToString(kinect.height) <<endl;


    // Setup ofScreen
    //ofSetFullscreen(true);
    ofSetFrameRate(30);
    ofSetVerticalSync(true);
    ofSetCircleResolution(64);	
    ofBackground(0, 0, 0);
    ofEnableSmoothing();

    // For images
    grayImage.allocate(kinect.width, kinect.height);
    checkGrayImage.allocate(kinect.width, kinect.height);
    grayThresh.allocate(kinect.width, kinect.height);
    grayThreshFar.allocate(kinect.width, kinect.height);

    // For hand detection (0 to 255) 255 is far, 0 is close
	nearThreshold = 25;
	farThreshold = 85;
    nearDistance = getNearDistance(nearThreshold);

    // For box2d
    box2d.init();
    box2d.setGravity(0, 5);
    box2d.createBounds(0, 0, kinect.width, kinect.height);
    box2d.setFPS(15);

    // Fonts
    msgFont.loadFont("Courier New.ttf",14, true, true);
    msgFont.setLineHeight(20.0f);
	
	// soudns
	soundPlayer = new SoundPlayer();
	soundPlayer->play();


    drawPointCloudSize = 0.0f;
    backgroundPosition = 0;

	detectingTime = 0;
	hasHuman = false;

    ofxBox2dCircle *c = new ofxBox2dCircle();
    c->setPhysics(1, 1, 1);
    c->setup(box2d.getWorld(), 400, 400, 1);
	
	/// setup config gui
	gui.setup();
	gui.config->gridSize.x = 300;
	gui.addTitle("KINECT SETTINGS");
	gui.addSlider("Tilt Angle", angle, -30, 30);
	gui.addTitle("DEPTH SETTINGS");
	gui.addSlider("Near Distance", nearThreshold, 20, 120);
	gui.addSlider("Far Distance", farThreshold, 50, 150);
	string titleArray[] = {"Lame", "Alright", "Better", "Best"};
	gui.addComboBox("box2", box2, 3,  titleArray);
	
	//gui.loadFromXML();
	gui.setDefaultKeys(true);
	gui.show();
}

//--------------------------------------------------------------
void arakawaVision::update() {

    kinect.update();
    box2d.update();
	soundPlayer->update();

    grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
	checkDepthUpdated();

	// 二値化
    unsigned char * pix = grayImage.getPixels();
	int width = grayImage.getWidth();
	int height = grayImage.getHeight();
    int numPixels = width * height;
	int maxThreshold = 255 - nearThreshold;
	int minThreshold = 255 - farThreshold;

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			int i = y * width + x;
			int adjust = 0;
			// kusigahamaの腹を検出するのを避ける
			if (240 < x && x < 400 && y > 250) {
				adjust = (80 - abs(320 - x))/6;
			}
			if( pix[i] > minThreshold + adjust && pix[i] < maxThreshold ){
				pix[i] = 255; // white
			}else{
				pix[i] = 0; // black
			}			
		}
	}

    //update the cv image
    grayImage.flagImageChanged();
    grayImage.mirror(false, true);

    // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
    // also, find holes is set to true so we will get interior contours as well....
    contourFinder.findContours(grayImage, 250, (kinect.width*kinect.height)/3, 3, false);


    for (int i=0; i<contorParticles.size(); i++) {
        contorParticles[i]->destroyShape();
    }
    contorParticles.clear();

    for (int j = 0; j < contourFinder.nBlobs; j++){
        float centerX = contourFinder.blobs[j].centroid.x;
        float centerY = contourFinder.blobs[j].centroid.y;
        createBox2dObjects(ofRandom(centerX-30, centerX+30), 20);

        for (int i = 0; i < contourFinder.blobs[j].nPts; i+=5){
            ofxBox2dCircle *p = new ofxBox2dCircle();
            p->setup(box2d.getWorld(), contourFinder.blobs[j].pts[i].x, contourFinder.blobs[j].pts[i].y, 5, true);
            p->setPhysics(1.0, 0.9, 0.7);
            contorParticles.push_back(p);
        }

    }


    // 背景更新
    backgroundPosition = backgroundPosition+0.2;

    for(vector <BgEffect *>::iterator it = bgEffects.begin(); it != bgEffects.end();){
        (*it)->update();
        if ((*it)->dead) {
            delete (*it);
            it = bgEffects.erase(it);
        } else {
            ++it;
        }
    }

    if (contourFinder.nBlobs > 0) {
        drawPointCloudSize = min(drawPointCloudSize+0.3f, 7.0f);
		detectingTime = min(++detectingTime, 60);
    } else {
        drawPointCloudSize = max(drawPointCloudSize-0.2f, 0.0f);
		detectingTime = max(--detectingTime, 0);
		checkHuman();
    }
	
	if (detectingTime == 60) {
		soundPlayer->playDetecting();
	}
	if (detectingTime == 0) {
		soundPlayer->stopDetecting();
	}

}
void arakawaVision::checkHuman() {
	if (detectingTime > 0 && ofGetFrameNum()%2 == 2) {
		return;
	}
	
	int detSize = 0;
	
	int w = kinect.width;
    int h = kinect.height;
    int distanceRange = 100;
    int step = 5;
		
    for (int i = 0; i < w; i+=step) {
        for (int j = 0; j < h; j+=step) {
            int distance = kinect.getDistanceAt(i, j);
            if (nearDistance < distance && distance < nearDistance+distanceRange) {
				float size = distanceRange - (distance - nearDistance);
                if(size < 15) {
					detSize++;
                }
            }
        }
    }
	
	if (detSize > 150) {
		soundPlayer->playNotify();
		hasHuman = true;
	}
	if (hasHuman && detSize < 20) {
		hasHuman = false;
		soundPlayer->stopNotify();
	}
			
}

void arakawaVision::createBox2dObjects(float x, float y) {
    if (ofGetFrameNum() % 4 == 0) {
        ofxBox2dCircle *c = new ofxBox2dCircle();
        c->setPhysics(2, 0.8, 2);
        c->setup(box2d.getWorld(), x, y, ofRandom(3, 14));
        box2dCircles.push_back(c);
        if (box2dCircles.size() > 60) {
            box2dCircles.at(0)->destroyShape();
            box2dCircles.erase(box2dCircles.begin());
        }
    } else if (ofGetFrameNum() % 5 == 0) {
        int size = ofRandom(5, 20);
        ofxBox2dRect *c = new ofxBox2dRect();
        c->setPhysics(2, 0.8, 10);
        c->setup(box2d.getWorld(), x, y, size, size);
        box2dRects.push_back(c);
        if (box2dRects.size() > 60) {
            box2dRects.at(0)->destroyShape();
            box2dRects.erase(box2dRects.begin());
        }
    }

}

//--------------------------------------------------------------
void arakawaVision::draw() {

    drawBackground();

    ofSetColor(255, 255, 255);
	
	if (sceneNum == 0) {
		drawTitleScene();
	} else {
		drawPlayScene();
	}
}

void arakawaVision::drawPlayScene() {
    if (debug) {
        grayImage.draw(10, 10, 360, 270);
        kinect.draw(380, 10, 360, 270);
        ofSetColor(255, 255, 255);
        ofNoFill();
        ofRect(10, 10, 360, 270);
		
        char reportStr[1024];
        sprintf(reportStr, "set near threshold %i, set far threshold %i, num blobs found %i",nearThreshold, farThreshold, contourFinder.nBlobs);
        ofDrawBitmapString(reportStr, 20, ofGetHeight()-30);
        ofDrawBitmapString("tilt angle: " + ofToString(angle),20,ofGetHeight()-15);
    }
	
    msgFont.drawString("accel is: " + ofToString(kinect.getMksAccel().x, 2) + " / " 
					   + ofToString(kinect.getMksAccel().y, 2) + " / "
					   + ofToString(kinect.getMksAccel().z, 2), 20, ofGetHeight()-60);
    msgFont.drawString("fps: "+ ofToString(ofGetFrameRate()), 20, ofGetHeight()-40);
    //msgFont.drawString("All your base is belong to Arakawa Tomonori....", 20, ofGetHeight()-80);
	
	
	
    glScalef((float)ofGetWidth() / (float)grayImage.width, (float)ofGetHeight() / (float)grayImage.height, 1.0f);
	
	
    if (debug) {
        for (int i = 0; i < contourFinder.nBlobs; i++){
            ofPushMatrix();
            contourFinder.blobs[i].draw(0,0);
            ofSetColor(255, 0, 0);
            ofFill();
            ofEllipse(contourFinder.blobs[i].centroid.x, contourFinder.blobs[i].centroid.y, 4, 4);
			
            ofPopMatrix();
        }
		
    }
    ofNoFill();
	
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	
	drawHands();
	drawBox2dObjects();
    drawPointCloud();
}

void arakawaVision::drawTitleScene() {
	gui.draw();
	
	char reportStr[1024];
	sprintf(reportStr, "set near threshold %i, set far threshold %i, num blobs found %i",nearThreshold, farThreshold, contourFinder.nBlobs);
	ofDrawBitmapString(reportStr, 20, ofGetHeight()-30);
	ofDrawBitmapString("tilt angle: " + ofToString(angle),20,ofGetHeight()-15);
	
}

void arakawaVision::drawBox2dObjects(){
    ofPushMatrix();
    for(int i = 0; i < box2dCircles.size(); i++) {
        box2dCircles[i]->draw();
    }
    for(int i = 0; i < box2dRects.size(); i++) {
        box2dRects[i]->draw();
    }	
    if (debug) {
        for(int i = 0; i < contorParticles.size(); i++) {
            contorParticles[i]->draw();
        }		
    }
    ofPopMatrix();
}

void arakawaVision::drawBackground(){

    ofFill();
    int gridRowNum = 20;
    int gridColNum = 30;
    int winWidth = ofGetWidth();
    int winHeight = ofGetHeight();
    float gridWidth = winWidth/gridColNum+1;
    float gridHeight = winHeight/gridRowNum+1;



    ofPushMatrix();
    if (backgroundPosition > gridHeight) {
        backgroundPosition-=gridHeight;
    }
    ofTranslate(0, -backgroundPosition, 0);
    ofSetColor(255, 255, 255, 60);

    if (ofGetFrameNum()%7 == 0) {
        int dist = ofRandom(1, gridColNum);
        bgEffects.push_back(new BgEffect(1, winWidth/gridColNum*dist));
    } else if (ofGetFrameNum()%11 == 0) {
        int dist = ofRandom(1, gridRowNum);
        bgEffects.push_back(new BgEffect(0, winHeight/gridRowNum*dist));
    }

    for (int i = 0; i < bgEffects.size(); i++) {
        bgEffects[i]->draw();
    }


    for (int j=0; j<gridColNum+1; j++) {
        for (int i=0; i<gridRowNum+2; i++) {
            ofSetColor(165/gridColNum*j, 165/gridRowNum*i, 250, 100);
            ofRect(winWidth/gridColNum*j, winHeight/gridRowNum*i, gridWidth, gridHeight);
        }
    }
    ofPopMatrix();
    ofNoFill();
}

void arakawaVision::drawHands() {
	
	if (detectingTime == 0) {
		return;
	}
    ofPushMatrix();	

    ofFill();
	nearDistance = getNearDistance(nearThreshold);
    int w = kinect.width;
    int h = kinect.height;
    int distanceRange = 100;
    int step = 4;
    for (int i = 0; i < w; i+=step) {
        for (int j = 0; j < h; j+=step) {
            int distance = kinect.getDistanceAt(i, j);
            if (nearDistance < distance && distance < nearDistance+distanceRange) {
                float size = distanceRange - (distance - nearDistance);
                if (size > 0){
                    ofSetColor(120+size, 120+size, 255, 70+size);
                    ofCircle(w-i, j, min(0.9+(size/100), 1.5));
                }
            }			
        }
    }	
    ofPopMatrix();


    ofNoFill();
    ofPushMatrix();

    ofPopMatrix();
}

void arakawaVision::drawPointCloud() {
    if (drawPointCloudSize >= 3.0f) {
        return;
    }
    ofSetColor(255, 255, 255);
    ofScale(400, 400, 400);
    int w = kinect.width;
    int h = kinect.height;
    ofTranslate(1.2, 0.5, drawPointCloudSize);
    ofRotateY(190);

    glBegin(GL_POINTS);
    int step = 2;
    for(int y = 0; y < h; y += step) {
        for(int x = 0; x < w; x += step) {
            ofPoint cur = kinect.getWorldCoordinateFor(x, y);
            glColor3f(max(0.0f, 1.8f-kinect.getDistanceAt(x, y)/250), min(1.0f, kinect.getDistanceAt(x, y)/250-0.8f), 0.0);
            glVertex3f(cur.x, cur.y, cur.z);
        }
    }
    glEnd();
}

/**
 * Check depth data is updated.
 *
 * If not updated, close and reopen the Kinect.
 */
void arakawaVision::checkDepthUpdated(){
    if (ofGetFrameNum() % 300 == 0) {
        std::cout << "checkDepthUpdated" << endl;
        unsigned char * nextDepth = kinect.getDepthPixels();

        if (ofGetFrameNum() != 300) {
            std::cout << "Start compare depth" << endl;
			unsigned char * currentDepthPixels = checkGrayImage.getPixels();
			
		    int pixNum = kinect.width * kinect.height;
            for (int i=0; i<pixNum; i++) {
                if (nextDepth[i] != currentDepthPixels[i]) {
                    break;
				}
				if (i > pixNum / 2) {
					std::cout << "Depth pixels did not updated!!" << endl;
					kinect.close();
					kinect.open();
					kinect.setCameraTiltAngle(angle);
					break;
				}
			}                  
		}
		checkGrayImage.setFromPixels(nextDepth, kinect.width, kinect.height);
	}
}

//--------------------------------------------------------------
void arakawaVision::exit(){
    kinect.close();
    std::cout << "Close Kinect and exit"<<endl;
}


//--------------------------------------------------------------
void arakawaVision::keyPressed (int key)
{
    switch (key)
    {
        case '>':
        case '.':
            farThreshold ++;
            if (farThreshold > 255) farThreshold = 255;
            break;
        case '<':		
        case ',':		
            farThreshold --;
            if (farThreshold < 0) farThreshold = 0;
            break;

        case '+':
        case '=':
            nearThreshold ++;
            if (nearThreshold > 255) nearThreshold = 255;
            break;
        case '-':		
            nearThreshold --;
            if (nearThreshold < 0) nearThreshold = 0;
            break;
        case 'd':
            //解析結果の表示の on / off
            debug ? debug=false : debug=true;
            break;            
        case 'f':
            //フルスクリーンに
            ofToggleFullscreen();
            break;
			
		case ' ':
			sceneNum = ++sceneNum%2;
			kinect.setCameraTiltAngle(angle);
			break;

        case OF_KEY_UP:
            angle++;
            if(angle>30) angle=30;
            kinect.setCameraTiltAngle(angle);
            break;

        case OF_KEY_DOWN:
            angle--;
            if(angle<-30) angle=-30;
            kinect.setCameraTiltAngle(angle);
            break;
    }
}

//--------------------------------------------------------------
void arakawaVision::mouseMoved(int x, int y)
{}

//--------------------------------------------------------------
void arakawaVision::mouseDragged(int x, int y, int button)
{}

//--------------------------------------------------------------
void arakawaVision::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void arakawaVision::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void arakawaVision::windowResized(int w, int h)
{}

