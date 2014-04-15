#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofBackground(0,0,0);
    ofEnableSmoothing();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    camWidth = 480;
    camHeight = 320;
    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(camWidth, camHeight);
    
}

//--------------------------------------------------------------
void testApp::update(){
    vidGrabber.grabFrame();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(255,255,255);
    vidGrabber.draw(20,20);
    
    unsigned char * pixels = vidGrabber.getPixels();
    for (int i = 0; i < camWidth; i +=8) {
        for (int j = 0; j < camHeight; j+=8) {
            unsigned char r = pixels[(j*camWidth+i) * 3];
            unsigned char g = pixels[(j*camWidth+i) * 3+1];
            unsigned char b = pixels[(j*camWidth+i) * 3+2];
            
            ofSetColor(255,0,0,100);
            ofCircle(camWidth+ 40 + i, 20+j, 10*(float)r/255.0);
            ofSetColor(0,255,0,100);
            ofCircle(camWidth+ 40 + i, 20+j, 10*(float)g/255.0);
            ofSetColor(0,0,255,100);
            ofCircle(camWidth+ 40 + i, 20+j, 10*(float)b/255.0);

        }
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == 's' || key == 'S') {
        vidGrabber.videoSettings();
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