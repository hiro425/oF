#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofBackground(0,0,0);
    ofEnableSmoothing();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    myImage.loadImage("MonaLisa.jpg");
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(255,255,255);
    myImage.draw(20,20);
    unsigned char * pixels = myImage.getPixels();
    int w = myImage.width;
    int h = myImage.height;

    for (int i = 0; i < w; i+=4) {
        for (int j = 0; j < h; j+=4) {
            int valueR = pixels[(j*w + i)*3];
            int valueG = pixels[(j*w + i)*3 +1];
            int valueB = pixels[(j*w + i)*3 +2];
            
            ofSetColor(255,0,0,63);
            ofCircle(440+i, 20+j, 4*valueR/255.0);
            ofSetColor(0,255,0,63);
            ofCircle(440+i, 20+j, 4*valueG/255.0);
            ofSetColor(0,0,255,63);
            ofCircle(440+i, 20+j, 4*valueB/255.0);
            
        }
    }

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == 'x') {
        grabbedImage.grabScreen(430,10,420,642);
        grabbedImage.saveImage("grabbedImage.png");
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