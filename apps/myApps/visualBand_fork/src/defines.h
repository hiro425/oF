//
//  defines.h
//  visualBand_fork
//
//  Created by 川口大志 on 2014/09/11.
//
//

#ifndef visualBand_fork_defines_h
#define visualBand_fork_defines_h

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
    CP_FLOW_X_SLIDE,
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
    CP_FOREST,
    CP_GRAY,
    CP_HIGHLIGHT
};

enum DrawType {
    CP_NO_DRAW,
    CP_POINT,
    CP_SPHERE,
    CP_LINE,
    CP_LINE_SIN,
    CP_LINE_IND,
    CP_CYLINDER,
    CP_RECT,
    CP_RECT_ROUNDED,
    CP_TRIANGLE,
    CP_TRIANGLE_IND,
    CP_CIRCLE,
    CP_ELLIPPSE,
    CP_CYLINDER_SPECTRUM,
    
};

struct property {
    vector<ofVec3f> points;
    vector<ofVec3f> distPoints;
    vector<ofFloatColor> colors;
    vector<ofIndexType> faces;
    vector<ofVec3f> mags;
    vector<bool> animated;
    vector<float> origMags;
    
    ofVec2f screen;
    ofVec2f div;
    DrawType dType;
    ActionType aType;
    ofVec3f minAddPos;
    ofVec3f maxAddPos;
    
    ofVec2f minMaxSize;
    
    float angle;
    
    ofVbo vbo;
    
    vector<float> drawBins;
    float vol;
};

#endif
