#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0, 0, 0);
    ofEnableAlphaBlending();
    ofSetFrameRate(30);
    
    vidGrabber.initGrabber(320, 240);
    colorImg.allocate(320, 240);
    grayImage.allocate(320, 240);
    grayBg.allocate(320, 240);
    grayDiff.allocate(320, 240);
    
    bLearnBackground= true;
    threshold = 20;
    simpleAmount = 1.0;
    bReversePoints = false;
    
    box2d.init();
    box2d.setGravity(0,20);
    box2d.createBounds();
    box2d.setFPS(30);

}

//--------------------------------------------------------------
void ofApp::update(){
    box2d.update();
    
    bool bNewFrame = false;
    
    vidGrabber.update();
    bNewFrame = vidGrabber.isFrameNew();
    
    if (bNewFrame) {
        colorImg.setFromPixels(vidGrabber.getPixels(), 320, 240);
        grayImage = colorImg;
        
        if (bLearnBackground) {
            grayBg = grayImage;
            bLearnBackground = false;
        }
        grayDiff.absDiff(grayBg, grayImage);
        grayDiff.threshold(threshold);
        contourFinder.findContours(grayDiff, 40, (340*240), 40, true);
        
        if (contourFinder.nBlobs > 0) {
            ofPoint p;
            lineStrip.setWorld(box2d.getWorld());
            lineStrip.clear();
            if(bReversePoints) {
                for (int i = simpleContour.size()-1; i>0; i--) {
                    p.x = simpleContour[i].x*ofGetWidth()/320;
                    p.y = simpleContour[i].y*ofGetHeight()/240;
                    lineStrip.addPoint(p.x, p.y);
                }
            }
            else {
                for (int i = 0; i<simpleContour.size(); i++) {
                    p.x = simpleContour[i].x*ofGetWidth()/320;
                    p.y = simpleContour[i].y*ofGetHeight()/240;
                    lineStrip.addPoint(p.x, p.y);
                }
            }
            lineStrip.createShape();
        }
    }

    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
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
