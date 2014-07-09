#pragma once


#include "ofMain.h"
#include "FlowLine.h"

#include "ofxOpenCv.h"
//#include "ofxKinect.h"
#include "ofxAssimpModelLoader.h"
#include "ofVboMesh.h"



class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    void updateFlowPoint(ofVec2f to, ofVec2f from);
    
    ofVideoGrabber 		    vidGrabber;
    ofxCvColorImage			colorImg;
    ofxCvGrayscaleImage 	grayImage;
    ofxCvGrayscaleImage 	prevGrayImage;
    bool                    bNewFrame;
    
    vector<FlowLine*> flow_lines;
    
    ofxAssimpModelLoader model;
    
    bool bAnimate;
    bool bAnimateMouse;
    float animationPosition;
    
    ofMesh mesh;
    ofLight	light;
    
    void updateModel();
    void drawModel();
    
    
    //void exit();
	void drawPointCloud();
    //ofxKinect kinect;
    
    
    
};