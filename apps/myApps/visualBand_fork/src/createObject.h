//
//  createObject.h
//  visualBand
//
//  Created by 川口大志 on 2014/08/07.
//
//

#ifndef __visualBand__createObject__
#define __visualBand__createObject__

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

enum ActionType {
    CP_NO_ACTION,
    CP_SLIDE,
    CP_ROTATE,
    CP_ROTATE_HORIZONTAL,
    CP_WAVE,
    CP_GATHER,
    CP_COME,
    CP_COME_CONSTANT,
    CP_CHANGE_COLOR,
    CP_SPREAD,
    CP_SPREAD_BY_ONE,
    CP_BIT_NOISE,
    CP_FLOW_X,
    CP_FLOW_X_SIN,
    CP_FLOW_X_ROTATE,
};

enum TransformType {
    CP_NO_TRANS,
    CP_DIFFUSE
};

enum ColorType {
    CP_MONO,
    CP_RANDOM,
    CP_FIRE,
    CP_SEA,
    CP_FOREST
};

enum DrawType {
    CP_NO_DRAW,
    CP_POINT,
    CP_SPHERE,
    CP_LINE,
    CP_LINE_SIN,
    CP_LINE_IND,
    CP_RECT,
    CP_RECT_ROUNDED,
    CP_TRIANGLE,
    CP_TRIANGLE_IND,
    CP_CIRCLE,
    CP_ELLIPPSE,
    
};


class createObject {
public:
    int width;
    int height;
    ofVec2f screen;
    ofVec3f initPos;
    ofVec3f mainPos;
    
    DrawType   dType;
    ActionType aType;
    ColorType  cType;
    bool addFlg;
    bool animationFlg;
    bool updateAddFlg;
    bool noiseFlg;
    bool bitNoiseFlg;
    
    ofFloatColor minColor;
    ofFloatColor maxColor;
    
    ofVbo vbo;
    vector<ofVec3f> points;
    vector<ofVec3f> distPoints;
    vector<ofFloatColor> colors;
    vector<ofIndexType> faces;
    vector<ofVec3f> mags;
    vector<bool> animated;
    vector<float> origMags;
    
    float angle;
    
    vector<ofxAssimpModelLoader*> models;
    
    // ------------------------------- function
    createObject();
    
    virtual void init(){}
    virtual void draw(){}
    void update();
    void callback();
    void add();
    void addRandom();
    void _add(const ofVec3f &v, const ofFloatColor &c, const ofIndexType &f, const ofVec3f &mag);
    
    
    void clear();
    void eraseOld();
    
    void setDrawType(DrawType d);
    void setActionType(ActionType a);
    void setColorType(ColorType c);
    
    ofColor getColor();
    int     getIndex();
    
    void updatePos(int index, const ofVec3f &v);
    void updateMags(float mag);
};


#endif /* defined(__visualBand__createObject__) */
