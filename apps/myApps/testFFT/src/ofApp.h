#pragma once

#include "ofMain.h"
#include "ofxProcessFFT.h"
#include "ofxFft.h"
#include "ofxGui.h"
#include "Primitive.h"

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
		
    
    ProcessFFT fft;
    
    void setupPrims();
    void drawPrims();
    
    vector<Primitive> lowPrims;
    vector<Primitive> midPrims;
    vector<Primitive> highPrims;
    int objectNum = 100;
    void makePrims();
    void randomiseAll();
    
    ofParameter<ofVec3f> lowPos;
    ofParameter<ofVec3f> lowSize;
    ofParameter<ofVec3f> lowRotate;
    
    ofParameter<ofVec3f> midPos;
    ofParameter<ofVec3f> midSize;
    ofParameter<ofVec3f> midRotate;
    
    ofParameter<ofVec3f> highPos;
    ofParameter<ofVec3f> highSize;
    ofParameter<ofVec3f> highRotate;
    
    struct primsData {
        ofParameter<ofVec3f> pos;
        ofParameter<ofVec3f> size;
        ofParameter<ofVec3f> rotate;
        ofParameter<ofColor> color;

    };
    primsData lowData;
    primsData midData;
    primsData highData;
    
    void insertPrimsData(vector<Primitive> &p, primsData &d, float vol, float orientRatio);
    void insertRandomiseData(primsData &d);

    struct bandwidthData {
        bool bang;
        ofParameter<float> thresh;
        float vol;
        int moveDim;
    };
    bandwidthData lowD;
    bandwidthData midD;
    bandwidthData highD;
    
    void checkBang(bandwidthData &d);
    
    void drawTwoRings();

    
    ofParameter<ofColor> color;
    slide globalRotate;
    ofColor currentColor;
    int rotAxisMode;
    ofxToggle bGlobalRotate;
    
    //light
    ofLight pointLight;
    
    //shader
    ofShader shader1;
    void drawShader();
    
};
