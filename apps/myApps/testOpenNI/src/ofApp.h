#ifndef _TEST_APP
#define _TEST_APP

#include "ofxOpenNI.h"
#include "ofMain.h"
#include "ofxSPK.h"

#define MAX_DEVICES 2

class ofApp : public ofBaseApp{

public:
    
	void setup();
	void update();
	void draw();
    void exit();
    
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

private:
    
    void handEvent(ofxOpenNIHandEvent & event);
    void gestureEvent(ofxOpenNIGestureEvent & event);
    
	ofxOpenNI openNIDevice;
    //ofTrueTypeFont verdana;
    
    // SPK
    vector <ofPoint> particlePos;
    
    ofxSPK::System sys;
    ofxSPK::Group group;
    ofImage sprite;
    ofxSPK::Modifier rot;
    
    void setupSPK();
    void updateSPK();
    void drawSPK();
    
    // Shader
    ofShader shader1;
    void drawShader();
    
};

#endif
