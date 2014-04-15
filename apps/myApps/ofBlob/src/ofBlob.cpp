//
//  ofBlob.cpp
//  ofBlob
//
//  Created by 川口 大志 on 14/04/15.
//
//

#include "ofBlob.h"

ofBlob::ofBlob() {
    pos   = ofPoint(ofGetWidth()/2, ofGetHeight()/2);
    dim   = 80.0;
    speed = ofPoint(0,0);
    phaseSpeed = ofRandom(0.1,0.5);
    phase = 0;
}

void ofBlob::draw() {
    ofSetColor(33, 63, 255, 100);
    ofCircle(pos.x, pos.y, moveDim);
    
    ofSetColor(255,0,0,200);
    ofCircle(pos.x, pos.y, dim/10);
    
    ofSetColor(33, 63, 255);
}

void ofBlob::update() {
    
    moveDim = dim + sin(phase) * dim/4;
    phase += phaseSpeed;
    if (phase > TWO_PI) {
        phase -= TWO_PI;
    }
    
    pos += speed;
    
    if (pos.x > ofGetWidth() - dim || pos.x < dim) {
        speed.x *= -1;
    }
    
    if (pos.y > ofGetHeight() - dim || pos.y < dim) {
        speed.y *= -1;
    }
    

}

void ofBlob::setPos(ofPoint _pos) {
    pos = _pos;
}

ofPoint ofBlob::getPos() {
    return pos;
}

void ofBlob::setDim(float _dim) {
    dim = _dim;
}

float ofBlob::getDim() {
    return dim;
}

void ofBlob::setSpeed(ofPoint _speed) {
    speed = _speed;
}

ofPoint ofBlob::getSpeed() {
    return speed;
}
