#pragma once

#include "ofMain.h"

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
		
    
    ofEasyCam cam;
    ofVbo Vbo;
    ofVec3f Verts[NUM_PARTICLES];
    ofFloatColor Color[NUM_PARTICLES];
    
};
