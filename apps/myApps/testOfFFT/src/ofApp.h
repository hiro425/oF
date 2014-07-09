#pragma once

#include "ofMain.h"
#include "ofxFft.h"

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
    
    void audioReceived(float* input, int bufferSize, int nChannels);
    void plot(vector<float>& buffer, float scale, float offset);
	
	int plotHeight, bufferSize;
    
	ofxFft* fft;
	
	ofMutex soundMutex;
    vector < vector <float> > drawBins;
	vector<float> middleBins, audioBins;
    ofVboMesh	mesh;
};
