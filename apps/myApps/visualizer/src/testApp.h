#pragma once

#include "ofMain.h"
#include "Primitive.h"
#include "ofxGui.h"
#include "time.h"

#define kNumCameras 2
using namespace std;

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
    void setupPanel();
        void randomiseAll();  //モード別の初期値？を設定
    
        ofSoundStream soundStream;
        vector <float> left; // buffersize分の音量？を格納する配列
    
        int 	bufferCounter;
    
        float smoothedVol;
        float scaledVol;
    
        vector<Primitive>prims;
    
        static const int objectNum = 300; //何個描画するか。
        bool bang;
        bool bHide;
        ofxPanel panel;
        ofParameter<ofVec3f> pos;
        ofParameter<ofVec3f> size;
        ofParameter<ofVec3f> rotate;
        ofParameter<float> thresh;
        ofParameter<int> repeat;
        ofParameter<ofColor> color;
        ofxToggle bGlobalRotate;
        ofxToggle bFillTog;
        ofxToggle bBackgroundChange;
        slide globalRotate;
        ofColor currentColor;
    
        ofPoint bFill;
        bool bReverse;
        ofxToggle bReverseTog;
        int rotAxisMode;
    ofxToggle bPrims;
    ofxToggle bPrimsTog;
    
    
    //hikwgc: cameras
    void setupCam();
    ofCamera cam[kNumCameras];
    ofParameter<ofVec3f> camInitPos;
    ofParameter<ofVec3f> camMovedPos;
    float camAngle;
    float camSpeed;
    float orientPosRatio;
    int cameraMode;
    
    //update method
    void updateWhenOverThreshold();
    void updateValue();
    void updateCamera();
    void camRotationXZ();
    void camRotationYZ();
    void camRotationXYZ();
    
    // hikwgc: patterns
    int makeMode;
    void makePrims();
    void makePrimsDynamic();
    void makeOnePrim();
    void makePrimsSimply();
    ofxToggle bOrbits;
    
    // shader
    ofShader shader1;
    string preFragName;
    string postFragName;
    int fragIndex;
    char charIndex[20];
    string fragFile;
    void setupShader();
    void loadShader();
    void drawShader();
    ofxToggle bShaderTog;
    ofxToggle bShader;
    void shaderContents(char *s);
    
    vector<int> fragContentList;
    char *shaderMode;
    string prevMode;
    int listIndex;
    ofxToggle bRandomStop;
    
};
