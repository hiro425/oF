//
//  branch.h
//  fractalSound
//
//  Created by 川口大志 on 2014/04/19.
//
//

#ifndef __fractalSound__branch__
#define __fractalSound__branch__

#include "ofMain.h"

class branch {
public:
    branch(float lev, float ind, float ex, float why);
    void setup();
    void newTree();
    void update(float ex, float why);
    void draw();
    void init(int numChildren, int maxLevel);
    
    float level;
    float index;
    float x;
    float y;
    float endx;
    float endy;
    int numChildren;
    int maxLevel;
};

#endif /* defined(__fractalSound__branch__) */
