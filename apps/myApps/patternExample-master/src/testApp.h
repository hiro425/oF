#pragma once

#include "ofMain.h"
#include "Primitive.h"
#include "ofxGui.h"

#define kNumTestNodes 4
#define kNumCameras 2
#define kNumLights 3

class testApp : public ofBaseApp{

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
        void audioIn(float * input, int bufferSize, int nChannels);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    //------------ここまでテンプレ
        void randomiseAll();  //モード別の初期値？を設定
    
        ofSoundStream soundStream;
        vector <float> left; // buffersize分の音量？を格納する配列
    
        int 	bufferCounter;
    
        float smoothedVol;
        float scaledVol;
    
        vector<Primitive>prims;
    
        static const int objectNum = 200; //何個描画するか。
    
    //hikwgc: UI???? -> いや違う
        bool bang;
        bool bHide;
        ofxPanel panel; //hikwgc: これはUIパーツ
        ofParameter<ofVec3f> pos; //hikwgc:この３つで、描画してるっぽ
        ofParameter<ofVec3f> size; //hikwgc:
        ofParameter<ofVec3f> rotate; //hikwgc:
        ofParameter<float> thresh;
        ofParameter<int> repeat;
        ofParameter<ofColor> color;
        ofxToggle bGlobalRotate;
        ofxToggle bFillTog;
        ofxToggle bBackgroundChange;
        slide globalRotate;
        dash  cameraRotate;
    
        ofPoint bFill;
        bool bReverse;
        ofxToggle bReverseTog;
        int rotAxisMode;
    
    
    //hikwgc: cameras
    void reset();
    
    ofNode testNodes[kNumTestNodes];
    ofLight light[kNumLights];
    ofCamera cam[kNumCameras];
    int lookatIndex[kNumCameras]; // which test node to lookat (one for each camera)
    int parentIndex[kNumCameras]; // which test node to link (parent) to (one for each camera)
    bool doMouseOrbit[kNumCameras]; // whether camera is in mouseOrbit mode or not
    float orbitRadius;
    
    ofParameter<ofVec3f> camInitPos;
    ofParameter<ofVec3f> camMovedPos;
    float camAngle;
    float camSpeed;
    
    int camToView; // which camera index are we looking through
    int camToConfigure;
    
    float orientPosRatio;
    int cameraMode;
    
    void updateCamera();
    void camRotationXZ();
    void camRotationYZ();
    void camRotationXYZ();
    void drawCamera();
    
    
    // hikwgc: patterns
    int makeMode;
    void makePrims();
    void makePrimsDynamic();
    void makeOnePrim();
    void makePrimsSimply();
    
};
