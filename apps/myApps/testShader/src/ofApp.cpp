#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    preFragName = "";
    fragIndex = 1;
    postFragName = ".frag";
    loadShader();

}

void ofApp::loadShader() {
    sprintf(charIndex, "%d", fragIndex);
    fragFile = preFragName + charIndex + postFragName;
    
    ofFile file(fragFile);
    if(!file.exists()){
        ofLogError("The file " + fragFile + " is missing");
        fragIndex = 1;
        sprintf(charIndex, "%d", fragIndex);
        fragFile = preFragName + charIndex + postFragName;
    }
    
    shader1.load("test.vert", fragFile);
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
    float resolution[2];
    resolution[0] = ofGetWindowWidth();
    resolution[1] = ofGetWindowHeight();
    
    float time = ofGetElapsedTimef();
    float mousePoint[2];
    mousePoint[0] = mouseX;
    mousePoint[1] = mouseY;
    
    shader1.begin();
    shader1.setUniform1f("time", time);
    shader1.setUniform2fv("resolution", resolution);
    
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
void ofApp::keyPressed(int key){
    if( key == ' ' ){
		//ofSetWindowPosition(0, 1280);
        fragIndex++;
        loadShader();
        
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
