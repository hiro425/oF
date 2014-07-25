#pragma once

#include "ofMain.h"
#include "ofxSPK.h"
#include "ofxProcessFFT.h"

#define WIDTH 200
#define HEIGHT 200
#define NUM_PARTICLES WIDTH * HEIGHT

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
    
    //this holds all of our points
    int numWaves;
    vector<ofVec3f> points;
    vector<ofVec3f> points2;
    vector< vector<ofVec3f> > pointArray;
    
    // vbo wave
    ofCamera cam;
    int camAngle;
    ofVbo Vbo;
    ofVec3f Verts[NUM_PARTICLES];
    ofFloatColor Color[NUM_PARTICLES];
    
    // SPK
    vector <ofPoint> particlePos;
    
    ofxSPK::System sys;
    ofxSPK::Group group;
    ofImage sprite;
    ofxSPK::Modifier rot;
    
    void setupSPK();
    void updateSPK();
    void drawSPK();
    
    //FFT
    ProcessFFT fft;
    int objectNum = 100;
    struct bandwidthData {
        ofParameter<ofVec3f> pos;
        ofParameter<ofVec3f> size;
        ofParameter<ofVec3f> rotate;
        ofParameter<ofColor> color;
        bool                 bang;
        ofParameter<float>   thresh;
        float                vol;
        int                  moveDim;
    };
    bandwidthData lowD;
    bandwidthData midD;
    bandwidthData highD;
    bandwidthData commonD;
    void updateFFT();
    void checkBang(bandwidthData &d);
    bool fftDebugMode;
    
    //light
    ofLight pointLight;
    
    
    
    
    
};
