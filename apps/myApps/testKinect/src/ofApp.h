#pragma once

#include "ofMain.h"

#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxAssimpModelLoader.h"
#include "ofxSPK.h"

#include "ofxOpenNI.h"


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
		
    
    void drawPointCloud();
    
    ofxKinect kinect;
    
    bool bThreshWithOpenCV;
	bool bDrawPointCloud;
	
	int nearThreshold;
	int farThreshold;
	
	int angle;
    
    ofxCvColorImage colorImg;
    ofxCvGrayscaleImage grayImage; // grayscale depth image
	ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
	ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
    
    ofxCvContourFinder contourFinder;
    
    ofEasyCam easyCam;
    
    bool bInRange;
    bool bInProgress;

    //model
    ofxAssimpModelLoader model;
    ofxAssimpModelLoader model2;
    vector <ofxAssimpModelLoader*> models;
    
    bool bAnimate;
    bool bAnimateMouse;
    float animationPosition;
    
    void setupModel(ofxAssimpModelLoader &mod);
    void setupModel(ofxAssimpModelLoader *mod);
    void addModel();
    void updateModels();
    void drawModels();
    
    ofMesh mesh;
    ofLight	light;
    
    // SPK
    vector <ofPoint> particlePos;
    
    ofxSPK::System sys;
    ofxSPK::Group group;
    ofImage sprite;
    ofxSPK::Modifier rot;

    void setupSPK();
    void updateSPK();
    void drawSPK();
    
    //openNI
    ofxOpenNI openNIDevice;
    void exit();
    
    
    
};
