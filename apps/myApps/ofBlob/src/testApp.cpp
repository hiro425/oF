#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofBackground(0,0,0);
    ofSetCircleResolution(32);
    ofEnableAlphaBlending();
    ofSetFrameRate(30);
}

//--------------------------------------------------------------
void testApp::update(){
    for (int i = 0; i < blobs.size(); i++) {
        blobs[i].update();
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    for (int i = 0; i < blobs.size(); i++) {
        blobs[i].draw();
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key) {
        case 'f':
            ofToggleFullscreen();
            break;
        case 'r':
            blobs.clear();
        default:
            break;
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
    
    int bSize = blobs.size();
    
    if (bSize < 1) {
        ofBlob b;
        b.setPos(ofPoint(ofGetWidth()/2, ofGetHeight()/2));
        b.setDim(ofGetHeight()/3);
        b.setSpeed(ofPoint(0,0));
        blobs.push_back(b);
    }
    
    for (int i = 0; i<bSize; i++) {
        ofPoint pos      = blobs[i].getPos();
        ofPoint mousePos = ofPoint(mouseX, mouseY);
        float dim        = blobs[i].getDim();
        float distDim    = ofDist(pos.x, pos.y, mousePos.x, mousePos.y);
        
        if (distDim - dim < 0) {
            blobs[i].setPos(ofPoint(pos.x - dim/10, pos.y));
            blobs[i].setDim(dim/2);
            blobs[i].setSpeed(ofPoint(ofRandom(-3, -1), ofRandom(-1, 1)));

            ofBlob b1;
            b1.setPos(ofPoint(pos.x, pos.y - dim/10));
            b1.setDim(dim/2);
            b1.setSpeed(ofPoint(ofRandom(-1, 1), ofRandom(-3, -1)));
            blobs.push_back(b1);

            ofBlob b2;
            b2.setPos(ofPoint(pos.x + dim/10, pos.y));
            b2.setDim(dim/2);
            b2.setSpeed(ofPoint(ofRandom(1, 3), ofRandom(-1, 1)));
            blobs.push_back(b2);

            ofBlob b3;
            b3.setPos(ofPoint(pos.x, pos.y + dim/10));
            b3.setDim(dim/2);
            b3.setSpeed(ofPoint(ofRandom(-1, 1), ofRandom(1, 3)));
            blobs.push_back(b3);
        }
        
    }
                 
                 
    
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