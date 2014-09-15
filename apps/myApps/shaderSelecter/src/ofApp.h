#pragma once

#include "ofMain.h"
#include "ofxFXObject.h"

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
    
    void audioIn(float * input, int bufferSize, int nChannels);
    ofSoundStream soundStream;
    vector <float> left;
    int 	bufferCounter;
    float smoothedVol;
    float smoothedVol2;
    float smoothedVol3;
    
    // shader
    ofShader shader1;
    
    string fragFile;
    void setupShader();
    void loadShader();
    void drawShader();
    bool bShaderTog;
    bool bShader;
    void shaderContents(string s);
    
    vector<string> fragContentsList;
    string presetDir;
    string datadir;
    string shaderMode;
    string prevMode;
    int listIndex;
    bool bTransitionStop;
    bool bForceUpdate;
};
