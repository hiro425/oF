#pragma once

#include "ofMain.h"
#include "ofxMPMFluid.h"
#include "ofxAutoControlPanel.h"

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
	
	
    ofxMPMFluid fluid;
	
	bool  bFullscreen;
	bool  bShowControlPanel;
	float scaleFactor;
	
    ofxAutoControlPanel gui;
	void setupControlPanel();
    
	//-----------------------------------
	// For PostScript output, if you like that sorta thing
    //	ofxVectorGraphics epsOutput;
	void outputPostscript();
	bool bEpsCapture;
};
