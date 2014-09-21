#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(255);
    ofSetFrameRate(60);
    
    ofNoFill();
    b = new branch(1,1,ofGetWidth()/2, 0, 0);
    
    camAngle   = 90.0;
    rotateMode = true;
    camRadius = ofGetWidth()/4*3;
    cam.resetTransform();
	cam.setFov(60);
	cam.clearParent();
    camPos = ofVec3f(ofGetWidth()/2, ofGetHeight()/2, camRadius);
    camDistPos = camPos;
    cam.setPosition(camPos);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    b->updateMe(ofGetWidth()/2, 0, 0);
    
    if (rotateMode) {
        float posx, posy, posz;
        camAngle += 0.5;
        posx = ofGetWidth()/2 + camRadius*cos(camAngle * PI /180);
        posy = ofGetHeight()/2;
        posz = camRadius*sin(camAngle * PI /180);
        camPos = ofVec3f(posx, posy, posz);
        cam.setPosition(camPos);
    }
    cam.setPosition(camPos);
    cam.lookAt(ofVec3f(ofGetWidth()/2, ofGetHeight()/2, 0));

    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){
    cam.begin();
    
    b->drawMe();
    
    cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case 'w':
            b->witherMe();
            break;
        case 'z':
            rotateMode = !rotateMode;
            break;
        default:
            break;
    }
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

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
