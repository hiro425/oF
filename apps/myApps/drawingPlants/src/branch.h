//
//  branch.h
//  drawingPlants
//
//  Created by 川口大志 on 2014/09/19.
//
//

#ifndef __drawingPlants__branch__
#define __drawingPlants__branch__

#include "ofMain.h"
#include "drawHelper.h"

class branch {
    float level;
    float index;
    float x;
    float y;
    float z;
    float endx;
    float endy;
    float endz;
    int   numChildren;
    int   maxLevels;
    vector <branch> children;
    vector <ofVec3f> flowers;
    vector <ofVec3f> scales;
    vector <ofFloatColor> colors;
    
    float strokeW;
    float alph;
    float len, lenChange, lenMax;
    float rot, rotChange, rot2;
    float flowerNum;
    
    bool witherFlg;
    
    ofVbo vbo;
    
    ofTexture texture;
    ofShader shader;
    
public:
    branch(float lev, float ind, float ex, float why, float zi);
    void updateMe(float ex, float why, float zi);
    void drawMe();
    void witherMe();
    
};

#endif /* defined(__drawingPlants__branch__) */
