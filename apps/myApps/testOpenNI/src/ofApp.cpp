#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    
    //ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetLogLevel(OF_LOG_ERROR);

    openNIDevice.setup();
    openNIDevice.addImageGenerator();
    openNIDevice.addDepthGenerator();
    openNIDevice.setRegister(true);
    openNIDevice.setMirror(true);
    openNIDevice.setResolution(ofGetWidth(), ofGetHeight(), 60);
    
    cout << "getwidth : " << openNIDevice.getWidth() << endl;
    cout << "getheight : " << openNIDevice.getHeight() << endl;
    
    // -----------
    // trial
    // -----------
    /*
    openNIDevice.addGestureGenerator();
    //openNIDevice.addAllGestures();
    openNIDevice.removeAllGestures();
    
    openNIDevice.addGesture("Click");
    //ofAddListener(openNIDevice.handEvent, this, &ofApp::handEvent);
    ofAddListener(openNIDevice.gestureEvent, this, &ofApp::gestureEvent);
    */
    // -----------
    
    // setup the hand generator
    openNIDevice.addHandsGenerator();
    
    // add all focus gestures (ie., wave, click, raise arm)
    openNIDevice.addAllHandFocusGestures();
    
    // or you can add them one at a time
    //vector<string> gestureNames = openNIDevice.getAvailableGestures(); // you can use this to get a list of gestures
                                                                         // prints to console and/or you can use the returned vector
    //openNIDevice.addHandFocusGesture("Wave");
    
    openNIDevice.setMaxNumHands(2);
    
    for(int i = 0; i < openNIDevice.getMaxNumHands(); i++){
        ofxOpenNIDepthThreshold depthThreshold = ofxOpenNIDepthThreshold(0, 0, false, true, true, true, true); 
        // ofxOpenNIDepthThreshold is overloaded, has defaults and can take a lot of different parameters, eg:
        // (ofxOpenNIROI OR) int _nearThreshold, int _farThreshold, bool _bUsePointCloud = false, bool _bUseMaskPixels = true, 
        // bool _bUseMaskTexture = true, bool _bUseDepthPixels = false, bool _bUseDepthTexture = false, 
        // int _pointCloudDrawSize = 2, int _pointCloudResolution = 2
        openNIDevice.addDepthThreshold(depthThreshold);
    }
    
    openNIDevice.start();
    
    //verdana.loadFont(ofToDataPath("verdana.ttf"), 24);
    
    shader1.load("test.vert", "flowerFountain.frag");
    
    setupSPK();
}


void ofApp::setupSPK() {
    ofDisableArbTex();
	sprite.loadImage("image.jpg");
	ofEnableArbTex();
	
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(0);
	
	sys.setup();
	
	group.setup(sys);
	group.setColor(ofxSPK::RangeC(ofColor(255, 255), ofColor(255, 255)),
				   ofxSPK::RangeC(ofColor(0, 0), ofColor(255, 0)));
	
	group.setLifeTime(0.5, 2);
	group.setFriction(0.1);
	group.setSize(0, ofxSPK::RangeF(30, 100));
	
	group.setGravity(ofVec3f(0, -10, 0));
	group.setMass(0.1, 1);
	
	rot.setup(SPK::Vortex::create(SPK::Vector3D(ofGetWidth()/2, ofGetHeight()/2),
								  SPK::Vector3D(0, 1, 0),
								  200,
								  10), group);
	
	group.reserve(10000);
}

//--------------------------------------------------------------
void ofApp::update(){
    openNIDevice.update();
    
    // reset all depthThresholds to 0,0,0
    /*
    for(int i = 0; i < openNIDevice.getMaxNumHands(); i++){
        ofxOpenNIDepthThreshold & depthThreshold = openNIDevice.getDepthThreshold(i);
        ofPoint leftBottomNearWorld = ofPoint(0,0,0);
        ofPoint rightTopFarWorld = ofPoint(0,0,0);
        ofxOpenNIROI roi = ofxOpenNIROI(leftBottomNearWorld, rightTopFarWorld);
        depthThreshold.setROI(roi);
    }
    */
    // iterate through users
    for (int i = 0; i < openNIDevice.getNumTrackedHands(); i++){
        
        // get a reference to this user
        ofxOpenNIHand & hand = openNIDevice.getTrackedHand(i);
        
        // get hand position
        ofPoint & handWorldPosition = hand.getWorldPosition(); // remember to use world position for setting ROIs!!!
        particlePos.push_back(hand.getPosition());
        
        /*
        // set depthThresholds based on handPosition
        ofxOpenNIDepthThreshold & depthThreshold = openNIDevice.getDepthThreshold(i); // we just use hand index for the depth threshold index
        ofPoint leftBottomNearWorld = handWorldPosition - 100; // ofPoint has operator overloading so it'll subtract/add 50 to x, y, z
        ofPoint rightTopFarWorld = handWorldPosition + 100;
        
        ofxOpenNIROI roi = ofxOpenNIROI(leftBottomNearWorld, rightTopFarWorld);
        depthThreshold.setROI(roi);
        */
    }
    updateSPK();
}

void ofApp::updateSPK() {
    //group.emitRandom(10, ofVec3f(ofGetMouseX(), ofGetMouseY()));
	
    for (int i = 0; i < particlePos.size(); i++) {
        group.emitRandom(2, ofVec3f(particlePos[i].x * ofGetWidth() / 640, particlePos[i].y * ofGetHeight() / 480));
        cout << "particlePos : " << particlePos[i] << endl;
    }
    
	sys.update();
    
	ofSetWindowTitle(ofToString(ofGetFrameRate()));
}
//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    
	//ofSetColor(0, 0, 0);
    
    ofPushMatrix();
    // draw debug (ie., image, depth, skeleton)
    //openNIDevice.drawDebug(0, 0, ofGetWidth()*2, ofGetHeight()); //when decide pos?
    ofPopMatrix();
    
    ofPushMatrix();
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    // iterate through users
    for (int i = 0; i < openNIDevice.getNumTrackedHands(); i++){
        
        // get a reference to this user
        ofxOpenNIHand & hand = openNIDevice.getTrackedHand(i);
        
        // get hand position
        ofPoint & handPosition = hand.getPosition();
        
        cout << "handPos : " << handPosition << endl;
        // draw a rect at the position
        ofSetColor(255,255,255, 190);
        ofCircle(handPosition.x * ofGetWidth() / 640, handPosition.y * ofGetHeight() / 480, 2);

        ofSetColor(255,255,255,127);
        ofCircle(handPosition.x * ofGetWidth() / 640, handPosition.y * ofGetHeight() / 480, 2*sin(PI * 2 + 10*ofGetElapsedTimef())+2);
        
        ofSetColor(255,255,255,63);
        ofCircle(handPosition.x * ofGetWidth() / 640, handPosition.y * ofGetHeight() / 480, 3*sin(PI * 2 + 10*ofGetElapsedTimef())+3);
        
        /*
        // set depthThresholds based on handPosition
        ofxOpenNIDepthThreshold & depthThreshold = openNIDevice.getDepthThreshold(i); // we just use hand index for the depth threshold index
        
        // draw ROI over the depth image
        ofSetColor(255,255,255);
        //depthThreshold.drawROI();
        
        // draw depth and mask textures below the depth image at 0.5 scale
        // you could instead just do pixel maths here for finger tracking etc
        // by using depthThreshold.getDepthPixels() and/or depthThreshold.getMaskPixels()
        // and turn off the textures in the initial setup/addDepthTexture calls
        
        ofPushMatrix();
        //ofTranslate(320 * i, 480);
        //ofScale(0.5, 0.5);
        //depthThreshold.drawDepth(); //add color?
        //depthThreshold.drawMask();
        ofPopMatrix();
        */
        // i think this is pretty good but might be a frame behind???
        
    }
    
    ofDisableBlendMode();
    ofPopMatrix();
    
    // draw some info regarding frame counts etc
    /*
	ofSetColor(0, 255, 0);
	string msg = " MILLIS: " + ofToString(ofGetElapsedTimeMillis()) + " FPS: " + ofToString(ofGetFrameRate()) + " Device FPS: " + ofToString(openNIDevice.getFrameRate());
    
	verdana.drawString(msg, 20, 480 - 20);
     */

    drawSPK();
        drawShader();
}

void ofApp::drawSPK() {
    ofEnableBlendMode(OF_BLENDMODE_ADD);
	
	// sys.debugDraw();
	
	// bind texture, enable point sprite while drawing particles
	sprite.bind();
	ofEnablePointSprites();
	sys.draw();
	ofDisablePointSprites();
	sprite.unbind();
    particlePos.clear();
}

void ofApp::drawShader() {
    float resolution[2];
    resolution[0] = ofGetWindowWidth();
    resolution[1] = ofGetWindowHeight();
    
    float time = ofGetElapsedTimef();
    //float mousePoint[2];
    // float handPoint[openNIDevice.getNumTrackedHands()][2];
    // hardcoding
    float handPoint1[2];
    float handPoint2[2];
    float handPoint3[2];
    float handPoint4[2];
    
    for (int i = 0; i < openNIDevice.getNumTrackedHands(); i++){
        
        // get a reference to this user
        ofxOpenNIHand & hand = openNIDevice.getTrackedHand(i);
        
        // get hand position
        ofPoint & handPosition = hand.getPosition();
        //handPoint[i][0] = handPosition.x;
        //handPoint[i][1] = handPosition.y;
        
        // ah...
        if (i == 1) handPoint1[0] = handPosition.x; handPoint1[1] = handPosition.y;
        if (i == 2) handPoint2[0] = handPosition.x; handPoint1[1] = handPosition.y;
        if (i == 3) handPoint3[0] = handPosition.x; handPoint1[1] = handPosition.y;
        if (i == 4) handPoint4[0] = handPosition.x; handPoint1[1] = handPosition.y;
        
    }
    //mousePoint[0] = mouseX;
    //mousePoint[1] = mouseY;
    
    shader1.begin();
    shader1.setUniform1f("time", time);
    shader1.setUniform2fv("resolution", resolution);
    //shader1.setUniform1f("vol", smoothedVol);
    
    shader1.setUniform2fv("hand1", handPoint1);
    shader1.setUniform2fv("hand2", handPoint2);
    shader1.setUniform2fv("hand3", handPoint3);
    shader1.setUniform2fv("hand4", handPoint4);
    
    //shader1.setUniform2fv("mouse", mousePoint);
    glEnable(GL_DEPTH_TEST);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(ofGetWidth(), 0);
    glVertex2f(ofGetWidth(), ofGetHeight());
    glVertex2f(0, ofGetHeight());
    glEnd();
    
    shader1.end();
}

//--------------------------------------------------------------
void ofApp::handEvent(ofxOpenNIHandEvent & event){
    // show hand event messages in the console
    ofLogNotice() << getHandStatusAsString(event.handStatus) << "for hand" << event.id << "from device" << event.deviceID;
}

void ofApp::gestureEvent(ofxOpenNIGestureEvent & event){
    // show hand event messages in the console
    if (event.gestureName == "Click") {
        cout << endl << endl << "clicked!!!" << endl;
    }
//    cout << endl << endl << "gesture name : " << event.gestureName << endl;
  //  cout << "gesture status : " << event.gestureStatus << endl << endl;
    
}

//--------------------------------------------------------------
void ofApp::exit(){
    openNIDevice.stop();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}
