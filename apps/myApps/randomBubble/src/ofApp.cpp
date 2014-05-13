#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(255, 255, 255);
    ofSetCircleResolution(64);
    ofSetFrameRate(60);
    
    bufSize = 256;
    ofSoundStreamSetup(0, 2, this, 44100, bufSize, 4);
    left = new float[bufSize];
    right = new float[bufSize];
    
    draw_by_voice = false;

}

//--------------------------------------------------------------
void ofApp::update(){
    crcl.updatePos();
    
    bool over_threshold = false;

    for (int i = 0; i < bufSize; i++) {
        if (abs(left[i]) > 1 || abs(right[i]) > 1) {
            //crcl.addCircles(1);
            over_threshold = true;
            //cout << "true\n";
        }
        //if (abs(left[i]) > 800) cout << "left[i]" << left[i] << "\n";
        //if (abs(right[i]) >800)cout << "right[i]" << right[i] << "\n";
    }
    
    if (over_threshold && draw_by_voice) {
        crcl.addCircles(10);
        cout << "here\n";
    }
    //cout << "bool" << draw_by_voice <<"\n";
    //cout << "end\n";
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
    crcl.drawCircles();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == 'v') {
        draw_by_voice = true;
        cout << "on\n";
    }
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
    crcl.addCircles(5);
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
