//
//  shapeObject.h
//  testOpenNI
//
//  Created by 川口大志 on 2014/07/09.
//
//

#ifndef __testOpenNI__shapeObject__
#define __testOpenNI__shapeObject__

#include "ofMain.h"
#include "ofVbo.h"

class shapeObject {
    
private:
    ofPoint pos;
    float dim;
    ofPoint speed;
    float phase;
    float phaseSpeed;
    float moveDim;
    
    vector<ofVec3f> poses;
    vector<ofVec3f> dims;
    vector<ofPoint> speeds;
    vector<float> phases;
    vector<float> phaseSpeeds;
    vector<float> moveDims;
    vector<bool> decayFlag;
    vector<ofFloatColor> colors;
    
    vector<ofVec3f> lampPoses;
    vector<ofVec3f> lampDims;
    vector<ofVec3f> lampDimsOrig;
    vector<ofVec3f> lampSpeeds;
    vector<bool> createFlag;
    vector<ofFloatColor> lampColors;
public:
    shapeObject();
    shapeObject(string MODE);
    void    init();
    void    draw();
    void    draw(float vol);
    void    update();
    void    update(string MODE);
    void    setPos(ofPoint pos);
    ofPoint getPos();
    void    setDim(float dim);
    float   getDim();
    ofPoint getSpeed();
    void    setSpeed(ofPoint speed);
    bool    canSeparate;
    float   createTime;
    float   gravity;
    float   friction;
    
    void setup(string filename, int num);
    void _draw(float vol);
    void _init();
    void _update(float vol);
    bool _touchEvent(ofPoint handPos);
    void _setupLamp();
    void _initLamp(ofVec3f p);
    int lampNum;
    ofTexture lampTexture;
    
    void addPoint(float x, float y, float z);
    
    // vector to store all values
    vector <ofVec3f> points;
    vector <ofVec3f> sizes;
    
    ofVbo vbo;
    ofShader shader;
    ofEasyCam camera;
    
    float camDist;
    
    ofTexture texture;
    
    ofVbo lamps;
    ofShader lampShader;
    void _addLamp(ofVec3f p, ofVec3f d);
    
};

#endif /* defined(__testOpenNI__shapeObject__) */
