#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofBackground(0,0,0);
    ofEnableSmoothing();
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    fingersMovie.loadMovie("fingers.mov");
    fingersMovie.play();

}

//--------------------------------------------------------------
void testApp::update(){
    fingersMovie.idleMovie();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(0xFFFFF);
    fingersMovie.draw(20,20);
    unsigned char * pixels = fingersMovie.getPixels();
    
    for (int i = 0; i < fingersMovie.width; i+=8) {
        for (int j = 0; j < fingersMovie.height; j +=8) {
            unsigned char r = pixels[(j*320 + i) * 3];
            unsigned char g = pixels[(j*320 + i) * 3+1];
            unsigned char b = pixels[(j*320 + i) * 3+2];
            
            ofSetColor(255,0,0,100);
            ofCircle(360+i, 20+j, 10.0*(float)r/255.0);
            ofSetColor(0,255,0,100);
            ofCircle(360+i, 20+j, 10.0*(float)g/255.0);
            ofSetColor(0,0,255,100);
            ofCircle(360+i, 20+j, 10.0*(float)b/255.0);

        }
        
    }

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if ( key == '0') {
        fingersMovie.firstFrame();
    }

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    fingersMovie.setPosition((float)x/(float)ofGetWidth());
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    fingersMovie.setPaused(true);

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    fingersMovie.setPaused(false);
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