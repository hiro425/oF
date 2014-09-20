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
    vector <ofColor> colors;
    
    float strokeW;
    float alph;
    float len, lenChange;
    float rot, rotChange;
    
    bool witherFlg;
    
public:
    branch(float lev, float ind, float ex, float why);
    void updateMe(float ex, float why);
    void drawMe();
    void witherMe();
    
};

#endif /* defined(__drawingPlants__branch__) */
