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
        float volume = (1.0 - dist/300.0) * 0.2;
        
        if (volume < 0) {
            volume = 0;
        }
        
        mySound.setVolume(volume);
        mySound.play();
    }
}

void CustomCircle::update() {
    if (num == 0) {
        float * vol = ofSoundGetSpectrum(1);
        radius = vol[0] * 2000;
    }
    else {
        radius = sin(mySound.getPosition() * TWO_PI * 20) * 20 + 5;
        float dist = ofDist(getPosition().x, getPosition().y, ofGetWidth()/2, ofGetHeight()/2);
        float volume = (1.0 - dist/300.0)*0.2;
        float pan = getPosition().x / ofGetWidth();
        
        if (volume < 0) {
            volume = 0;
        }
        mySound.setVolume(volume);
        mySound.setPan(pan);
    }
}

void CustomCircle::draw() {
    ofFill();
    if (num == 0) {
        ofSetColor(255,0,0,100);
    }
    else {
        ofSetColor(0,63,255,100);
    }
    
    ofCircle(getPosition().x, getPosition().y, radius);
    
    if (num == 0) {
        ofSetColor(255,0,0);
    }
    else {
        ofSetColor(0,63,255);
    }
    
    ofCircle(getPosition().x, getPosition().y, 3);
}