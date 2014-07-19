#pragma once

#include "ofMain.h"
#include "ofxOculusRift.h"
#include "ofxSPK.h"
#include "ofxGui.h"

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
		
    
    		void drawSceneGeometry();
    ofxOculusRift		oculusRift;
	
    float				lastUpdateTime;
	
    ofLight				light;
    ofEasyCam			cam;
	
    ofTrueTypeFont		fontWorld;
    
    // visualizer
            void audioIn(float * input, int bufferSize, int nChannels);
            ofSoundStream soundStream;
    vector <float> left;
    
    int 	bufferCounter;
    
    float smoothedVol;
    float scaledVol;
    
    ofLight pointLight;
    float radius;
	ofVec3f center;
    
    
    // patterns
    void sphereDiffuse();
    ofxToggle dif;
    void sphereGrow();
    ofxToggle grow;
    void sphereProceed();
    ofxToggle proc;
    void sphereSpiral();
    ofxToggle spi;
    void sphereScatter();
    ofxToggle scat;
    
};
