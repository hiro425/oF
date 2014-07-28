#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "particle.h"
#include "ofxControlPanel.h"
#include "ofxProcessFFT.h"


class testApp : public ofSimpleApp{
	
	public:
		
		void setup();
		void update();
		void draw();
		
		void keyPressed  (int key);
		void keyReleased (int key);
		
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased();
		
		// let's make a vector of them
		vector <particle> particles;
    vector <particle> particles2;
	
		ofxControlPanel panel;
	
	ofCamera cam;
    ofLight pointLight;
    bool rotateMode;
    float camAngle;
    
    
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
    
    // ---------------------------------
    vector <ofVec3f> points;
    vector <ofVec3f> sizes;
    vector <ofFloatColor> colors;
    vector <float> origSizes;
    vector < vector<ofVec3f> > pastPoints;
    
    ofVbo vbo;
    ofShader shader;
    ofTexture texture;
    
	
};

#endif
	
