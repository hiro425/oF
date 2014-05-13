#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    bufSize = 256;
    
    ofBackground(0,0,0);
    ofSetColor(0,0,255);
    
    ofSoundStreamSetup(0, 2, this, 44100, bufSize, 4);
    
    left = new float[bufSize];
    right = new float[bufSize];

}

//--------------------------------------------------------------
void ofApp::update(){

}


//--------------------------------------------------------------
void ofApp::audioReceived(float * input, int bufferSize, int nChannles) {
    for (int i = 0; i < bufferSize; i++) {
        left[i]  = input[i*2];
        right[i] = input[i*2+1];
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    float audioHeight = ofGetHeight()/2;
    float phaseDiff   = ofGetWidth()/float(bufSize);
    
    for (int i = 0; i < bufSize; i++) {
        ofLine(i*phaseDiff, audioHeight/2, i*phaseDiff, audioHeight/2 + left[i]*audioHeight);
        ofLine(i*phaseDiff, audioHeight/2*3, i*phaseDiff, audioHeight/2*3 + right[i]*audioHeight);
        
        if (abs(left[i]) > 0.5) cout << "left[i]" << left[i] << "\n";
        if (abs(right[i]) > 0.5)cout << "right[i]" << right[i] << "\n";
    }

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

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
