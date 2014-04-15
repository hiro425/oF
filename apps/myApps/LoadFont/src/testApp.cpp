#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofBackground(255,255,255);
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    
    georgia.loadFont("Georgia.ttf", 60);
    fontX = ofGetWidth();

}

//--------------------------------------------------------------
void testApp::update(){
    fontX -= 1;
    ofRectangle rect= georgia.getStringBoundingBox("Hello! I am openFrameworks", 0, 0);
    
    if (fontX < -rect.width) {
        fontX = ofGetWidth();
    }

    
}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(0,127,255);
    georgia.drawString("Hello! I am openframeworks", fontX, ofGetHeight()/2);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}