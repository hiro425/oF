#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofBackground(0, 0, 0);
    
    box2d.init();
    box2d.setGravity(0, 5);
    box2d.createBounds(0, 0, ofGetWidth(), ofGetHeight());
    box2d.setFPS(30);
    
    box2d.registerGrabbing();
}

//--------------------------------------------------------------
void ofApp::update(){
    box2d.update();

}

//--------------------------------------------------------------
void ofApp::draw(){
    for (int i =0; i<circles.size(); i++) {
        ofFill();
        ofSetColor(0, 127, 255);
        circles[i].get()->draw();
    }
    
    box2d.drawGround();

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
    float r = ofRandom(10, 40);
    circles.push_back(ofPtr<ofxBox2dCircle>(new ofxBox2dCircle));
    circles.back().get()->setPhysics(1.0, 0.8, 0.5);
    circles.back().get()->setup(box2d.getWorld(), mouseX, mouseY, r);
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
