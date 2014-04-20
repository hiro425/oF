//
//  CustomRect.h
//  musicalInst
//
//  Created by 川口大志 on 2014/04/19.
//
//

#ifndef __musicalInst__CustomRect__
#define __musicalInst__CustomRect__

#pragma once
#include "ofxBox2d.h"

class CustomRect: public ofxBox2dRect {
public:
    CustomRect(int _num);
    void update();
    
    int num;
    ofSoundPlayer mySound;
    float soundSpeed;
    float amp;
    float lfo;
    float radius;
};

#endif /* defined(__musicalInst__CustomRect__) */
