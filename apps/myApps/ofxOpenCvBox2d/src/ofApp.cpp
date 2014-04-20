#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0, 0, 0);
    //ofEnableAlphaBlending();
    ofSetFrameRate(60);
    
    vidGrabber.initGrabber(320, 240);
    colorImg.allocate(320, 240);
    grayImage.allocate(320, 240);
    grayBg.allocate(320, 240);
    grayDiff.allocate(320, 240);
    
    bLearnBackground= true;
    threshold = 100;
    
    box2d.init();
    box2d.setGravity(0,1);
    box2d.createBounds(0,0,colorImg.width, colorImg.height);
    box2d.setFPS(30);
    box2d.checkBounds(true);

    static const int NUM = 1000;
    for (int i = 0; i < NUM; i++) {
        /*
        CustomCircle circle;
        circle.setPhysics(1.0, 0.8, 0.0);
        circle.setup(box2d.getWorld(), ofRandom(colorImg.width), ofRandom(colorImg.height), 3);
        circles.push_back(circle);
        */
        circles.push_back(ofPtr<CustomCircle>(new CustomCircle));
        circles.back().get()->setPhysics(1.0, 0.8, 0.5);
        circles.back().get()->setup(box2d.getWorld(), ofRandom(colorImg.width), ofRandom(colorImg.height), 3);
    }
    
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
        contourFinder.findContours(grayDiff, 20, (340*240), 10, false);
        
        for (int i = 0; i < contourCircles.size() ; i++) {
            //contourCircles[i]->destroy();
            box2d.getWorld()->DestroyBody(contourCircles[i]->body);
        }
        contourCircles.clear();
        
        for (int i = 0; i < contourFinder.nBlobs; i++) {
            for (int j = 0; j < contourFinder.blobs[i].pts.size(); j+=4) {
                ofPoint pos = contourFinder.blobs[i].pts[j];
                /*
                ofxBox2dCircle circle;
                circle.setup(box2d.getWorld(), pos.x, pos.y, 4);
                contourCircles.push_back(circle);
                */
                ofPtr<ofxBox2dCircle> circle = ofPtr<ofxBox2dCircle>(new ofxBox2dCircle);
                contourCircles.push_back(circle);
                //contourCircles.back().get()->setPhysics(1.0, 0.8, 0.5);
                contourCircles.back().get()->setup(box2d.getWorld(), pos.x, pos.y, 4);
            }
        }
    }

    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    float ratioX = ofGetWidth()/320;
    float ratioY = ofGetHeight()/240;
    
    ofPushMatrix();
    ofScale((float)ofGetWidth() / (float)grayDiff.width, (float)ofGetHeight() / (float)grayDiff.height);
    ofSetColor(255, 255, 255);
    colorImg.draw(0, 0);
    contourFinder.draw();
    ofNoFill();
    ofSetColor(255, 0, 0);
    for (int i = 0; i < contourCircles.size(); i++) {
        contourCircles[i]->draw();
    }
    for (int i = 0; i < circles.size(); i++) {
        circles[i]->draw();
    }
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case ' ':
            bLearnBackground = true;
            break;
        case '+':
            threshold++;
            if (threshold > 255) threshold = 255;
            break;
        case '-':
            threshold--;
            if (threshold < 0) threshold = 0;
            break;
            
        default:
            break;
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
