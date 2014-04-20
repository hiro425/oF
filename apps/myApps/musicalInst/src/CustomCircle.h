//
//  CustomCircle.h
//  musicalInst
//
//  Created by 川口 大志 on 14/04/16.
//
//

#ifndef __musicalInst__CustomCircle__
#define __musicalInst__CustomCircle__

#include "ofxBox2d.h"

class CustomCircle: public ofxBox2dCircle {
public:
    CustomCircle(int num);
    //void setup(b2World * b2dworld, float x, float y, float radius);
    void update();
    void draw();
    int num;
    ofSoundPlayer mySound;
    float radius;
    float soundSpeed;
};

#endif /* defined(__musicalInst__CustomCircle__) */
