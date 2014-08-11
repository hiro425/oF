#pragma once

#include "ofMain.h"
#include "ofxProcessFFT.h"
#include "createPrimitive.h"
#include "ofxPostProcessing.h"
#include "create3dModel.h"

enum DrawMode {
    VB_PRIMITIVE,
    VB_MODEL
};

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

    ofCamera cam;
    float camAngle;
    ofVec3f camPos;
    ofVec3f camDistPos;
    float camRadius;
    bool rotateMode;
    float decay;
    
    vector<createPrimitive> primitives;
    
    bool warpFlg;
    bool flashFlg;
    bool forceBang;
    DrawMode dMode;
    
    // audio
    ofSoundStream  soundStream;
    vector <float> left;
    vector <float> ch1;
    vector <float> ch2;
    float          smoothedVol;
    
    
    struct bandwidthData {
        ofParameter<ofVec3f> pos;
        ofParameter<ofVec3f> size;
        ofParameter<ofVec3f> rotate;
        ofParameter<ofColor> color;
        bool                 bang;
        ofParameter<float>   thresh;
        float                vol;
        int                  moveDim;
        createObject      *c;
    };
    bandwidthData sphereD;
    bandwidthData triangleD;
    vector<bandwidthData> primsD;
    
    void audioIn(float * input, int bufferSize, int nChannels);
    void checkBang(bandwidthData &d);

    
    //light
    ofLight pointLight;
    ofColor diffuseColor;
    ofColor specularColor;
    bool darkFlg;
    
    ofxPostProcessing post;
    ofImage backImage;

    //model
    create3dModel dog;
    bandwidthData dogD;
    
    vector<create3dModel> models;
    vector<bandwidthData> modelsD;
};


