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
    debugFlg = -1;
    
    //verdana.loadFont(ofToDataPath("verdana.ttf"), 24);
    
    //shader1.load("test.vert", "flowerFountain.frag");
    
    //setupSPK();
    //setupDrops();
    sprites.setup("snow/decoration_ca_038.png", 80);
    
    int bufferSize = 256;
    soundStream.setup(this, 0, 1, 44100, bufferSize, 4);
    left.assign(bufferSize, 0.0);
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
	group.setColor(ofxSPK::RangeC(ofColor(196, 150, 3), ofColor(196, 150, 3)),
				   ofxSPK::RangeC(ofColor(255, 255, 200), ofColor(255, 255, 200)));
	
	group.setLifeTime(0.5, 2);
	group.setFriction(0.1);
	group.setSize(0, ofxSPK::RangeF(30, 100));
	
	group.setGravity(ofVec3f(0, -10, 0));
	group.setMass(0.1, 1);
	
	rot.setup(SPK::Vortex::create(SPK::Vector3D(ofGetWidth()/2, ofGetHeight()),
								  SPK::Vector3D(1, 0, 0),
								  200,
								  10), group);
	
	group.reserve(10000);
    
    
}

void ofApp::setupDrops() {
    for (int i = 0; i < 20; i++) {
        shapeObject s("Drops");
        shapes.push_back(s);
    }
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
        ofPoint & handPosition = hand.getPosition();
        sprites._touchEvent(ofPoint(handPosition.x * ofGetWidth() / 640, handPosition.y * ofGetHeight() / 480));
    }
    //updateSPK();
    sprites._update(smoothedVol);
    
    //updateShapes();
    //updateDrops();
}

void ofApp::updateSPK() {
    //group.emitRandom(10, ofVec3f(ofGetMouseX(), ofGetMouseY()));
	
    for (int i = 0; i < particlePos.size(); i++) {
        group.emitRandom(2, ofVec3f(particlePos[i].x * ofGetWidth() / 640, particlePos[i].y * ofGetHeight() / 480));
        //cout << "particlePos : " << particlePos[i] << endl;
    }
    
	sys.update();
    
	ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

void ofApp::updateShapes() {
    
    int sSize = shapes.size();
    
    if (sSize < 1) {
        shapeObject s;
        s.setPos(ofPoint(ofGetWidth()/2, ofGetHeight()/2));
        s.setDim(ofGetHeight()/3);
        s.setSpeed(ofPoint(0,0));
        shapes.push_back(s);
    }
    
    //for (int i = 0; i < openNIDevice.getNumTrackedHands(); i++){
    if (openNIDevice.getNumTrackedHands()) {
        // get a reference to this user
        ofxOpenNIHand & hand = openNIDevice.getTrackedHand(0);
        
        // get hand position
        ofPoint & handPosition = hand.getPosition();
    
    

    for (int i = 0; i<sSize; i++) {
        ofPoint pos      = shapes[i].getPos();
        ofPoint handPos = ofPoint(handPosition.x * ofGetWidth() / 640, handPosition.y * ofGetHeight() / 480);
        float dim        = shapes[i].getDim();
        float distDim    = ofDist(pos.x, pos.y, handPos.x, handPos.y);
        
        if (shapes[i].canSeparate && distDim - dim/3 < 0) {
            shapes[i].canSeparate = false;
            shapes[i].createTime = ofGetElapsedTimef();
            shapes[i].setPos(ofPoint(pos.x - dim/10, pos.y));
            shapes[i].setDim(dim/2);
            shapes[i].setSpeed(ofPoint(ofRandom(-3, -1), ofRandom(-1, 1)));
            
            shapeObject s1;
            s1.setPos(ofPoint(pos.x, pos.y - dim/10));
            s1.setDim(dim/2);
            s1.setSpeed(ofPoint(ofRandom(-1, 1), ofRandom(-3, -1)));
            shapes.push_back(s1);
            
            shapeObject s2;
            s2.setPos(ofPoint(pos.x + dim/10, pos.y));
            s2.setDim(dim/2);
            s2.setSpeed(ofPoint(ofRandom(1, 3), ofRandom(-1, 1)));
            shapes.push_back(s2);
            
            shapeObject s3;
            s3.setPos(ofPoint(pos.x, pos.y + dim/10));
            s3.setDim(dim/2);
            s3.setSpeed(ofPoint(ofRandom(-1, 1), ofRandom(1, 3)));
            shapes.push_back(s3);
        }

        
    }
    }
    for (int i = 0; i < shapes.size(); i++) {
        shapes[i].update();
    }
}

void ofApp::updateDrops() {
    int sSize = shapes.size();
    
    //for (int i = 0; i < openNIDevice.getNumTrackedHands(); i++){
    if (openNIDevice.getNumTrackedHands()) {
        // get a reference to this user
        ofxOpenNIHand & hand = openNIDevice.getTrackedHand(0);
        
        // get hand position
        ofPoint & handPosition = hand.getPosition();
        
    for (int i = 0; i<sSize; i++) {
        ofPoint pos      = shapes[i].getPos();
        ofPoint handPos = ofPoint(handPosition.x * ofGetWidth() / 640, handPosition.y * ofGetHeight() / 480);
        float dim        = shapes[i].getDim()/2;
        float distDim    = ofDist(pos.x, pos.y, handPos.x, handPos.y);
        
        if (distDim - dim < 0) {
            shapes[i].createTime = ofGetElapsedTimef();
            shapes[i].setSpeed(ofPoint(0,0));
            
        }
        
    }
    }
    for (int i = 0; i < shapes.size(); i++) {
        shapes[i].update("Drops");
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    
	//ofSetColor(0, 0, 0);
    

    
    if (debugFlg > 0) {
        ofPushMatrix();
        // draw debug (ie., image, depth, skeleton)
        openNIDevice.drawDebug(0, 0, ofGetWidth()*2, ofGetHeight()); //when decide pos?
        ofPopMatrix();
    }
    
    
    ofPushMatrix();
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    // iterate through users
    for (int i = 0; i < openNIDevice.getNumTrackedHands(); i++){
        
        // get a reference to this user
        ofxOpenNIHand & hand = openNIDevice.getTrackedHand(i);
        
        // get hand position
        ofPoint & handPosition = hand.getPosition();
        
        //cout << "handPos : " << handPosition << endl;
        // draw a rect at the position
        ofSetColor(255, 150, 3, 190);
        ofCircle(handPosition.x * ofGetWidth() / 640, handPosition.y * ofGetHeight() / 480, 3);

        ofSetColor(255, 150, 3,127);
        ofCircle(handPosition.x * ofGetWidth() / 640, handPosition.y * ofGetHeight() / 480, 4*sin(PI * 2 + 5*ofGetElapsedTimef())+2);
        
        ofSetColor(255, 150, 3,63);
        ofCircle(handPosition.x * ofGetWidth() / 640, handPosition.y * ofGetHeight() / 480, 5*sin(PI * 2 + 5*ofGetElapsedTimef())+3);
        
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

    //drawSPK();
    //drawShapes();
    sprites._draw(smoothedVol);
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

void ofApp::drawShapes() {
    for (int i = 0; i < shapes.size(); i++) {
        shapes[i].draw(smoothedVol);
    }
}

void ofApp::drawShader() {
    float resolution[2];
    resolution[0] = ofGetWindowWidth();
    resolution[1] = ofGetWindowHeight();
    
    float time = ofGetElapsedTimef();
    //float mousePoint[2];

    //mousePoint[0] = mouseX;
    //mousePoint[1] = mouseY;
    
    shader1.begin();
    shader1.setUniform1f("time", time);
    shader1.setUniform2fv("resolution", resolution);
    //shader1.setUniform1f("vol", smoothedVol);
    
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
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
	
	float curVol = 0.0;
	
	// samples are "interleaved"
	int numCounted = 0;
    
	//lets go through each sample and calculate the root mean square which is a rough way to calculate volume
	for (int i = 0; i < bufferSize; i++){
		left[i]		= input[i] * 5;//*0.5;
        
		curVol += left[i] * left[i];
		numCounted++;
	}
	
	//this is how we get the mean of rms :)
	curVol /= (float)numCounted;
	
	// this is how we get the root of rms :)
	curVol = sqrt( curVol );
	
	smoothedVol *= 0.93;
	smoothedVol += 0.07 * curVol;
	
	bufferCounter++;
	
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if (key == ' ') {
        debugFlg *= -1;
    }
    cout << "debug : " << debugFlg  << endl;
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
