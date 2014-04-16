//
//  CustomCircle.cpp
//  musicalInst
//
//  Created by 川口 大志 on 14/04/16.
//
//

#include "CustomCircle.h"
CustomCircle::CustomCircle(int _num) {
    num = _num;
    radius = 0;
    
    if (num < 0) {
        float notes[] = {0.5, 0.75, 1, 1.5, 20.0, 3.0};
        soundSpeed = notes[((int)(ofRandom(0, 5)))];
        mySound.loadSound("ambientSound.wav");
        mySound.setLoop(true);
        mySound.setSpeed(soundSpeed);
        
        float dist = ofDist(getPosition().x, getPosition().y, ofGetWidth()/2, ofGetHeight()/2);
    }
}