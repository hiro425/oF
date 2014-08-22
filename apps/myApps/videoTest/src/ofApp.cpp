#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(255, 255, 255);
    ofEnableDepthTest();
    
    frameByframe = true;
    
    fingerMovie.setPixelFormat(OF_PIXELS_RGBA);
    ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_PIXELS_AND_TEXTURE;
	fingerMovie.loadMovie("movies/fingers.mov", decodeMode);
	fingerMovie.play();
    
    diffuseColor = ofColor(255.f, 255.f, 255.f);
    specularColor = ofColor(255.f, 255.f, 255.f);
    
    pointLight.setDiffuseColor( diffuseColor );
	pointLight.setSpecularColor( specularColor );
	pointLight.setPointLight();
    pointLight.setPosition(ofGetWidth(), ofGetHeight(), 1080);

    int bufferSize = 256;
    soundStream.setup(this, 0, 1, 44100, bufferSize, 4);
    left.assign(bufferSize, 0.0);
    
    thresh = 0.2;
    bang = false;
    
}

//--------------------------------------------------------------
void ofApp::update(){
    fingerMovie.update();
    //fingerMovie.setPaused(frameByframe);
    if (bang == true) {
        fingerMovie.setSpeed(2);
    }
    else {
        fingerMovie.setSpeed(0);
    }

    if (fingerMovie.getCurrentFrame() > 170 ) {
        fingerMovie.setFrame(0);
    }
    
    checkBang();
}

void ofApp::checkBang() {
    if (bang == false && smoothedVol > thresh) {
        bang = true;
    }
    if (smoothedVol < thresh) {
        bang = false;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetHexColor(0xFFFFFF);
    
    ofEnableLighting();
    ofEnableAlphaBlending();
    //ofEnableBlendMode(OF_BLENDMODE_ADD);
    pointLight.enable();
    
    if (fingerMovie.isLoaded()) {
        fingerMovie.draw(0, 0, ofGetWidth(), ofGetHeight());
    }
    
    ofSetColor(255, 255, 255, 127);
    ofDrawSphere(ofGetWidth()/5*4 + 500 * smoothedVol, ofGetHeight()/2, 10 + 100*smoothedVol);

    pointLight.disable();
    ofDisableLighting();
}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
	
	float curVol = 0.0;
	int   numCounted = 0;
    
	for (int i = 0; i < bufferSize; i++){
            left[i]   = input[i] * 5;
            curVol += left[i] * left[i];
            numCounted++;
 	}
	
	curVol /= (float)numCounted;
	curVol  = sqrt( curVol );
    
    smoothedVol *= 0.93;
    smoothedVol += 0.07 * curVol;
    
    cout << smoothedVol << endl;
    
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