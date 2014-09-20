//
//  fern.cpp
//  drawingPlants
//
//  Created by 川口大志 on 2014/09/19.
//
//

#include "fern.h"

float W1x(float x, float y) {
    return 0.836*x+0.044*y;
}
float W1y(float x, float y) {
    return -0.044*x+0.836*y+0.169;
}
float W2x(float x, float y) {
    return -0.141*x+0.302*y;
}
float W2y(float x, float y) {
    return 0.302*x+0.141*y+0.127;
}
float W3x(float x, float y) {
    return 0.141*x-0.302*y;
}
float W3y(float x, float y) {
    return 0.302*x+0.141*y+0.169;
}
float W4x(float x, float y) {
    return 0;
}
float W4y(float x, float y) {
    return 0.175337*y;
}

fern::fern() {
    float xm=0;
    float ym=0.5;
    float h=0.6;
}


void fern::draw(int k, float x, float y) {
    if (0<k) {
        draw(k-1, W1x(x,y), W1y(x,y));
        if (ofRandom(1.0) < 0.3) draw(k-1, W2x(x,y), W2y(x,y));
        if (ofRandom(1.0) < 0.3) draw(k-1, W3x(x,y), W3y(x,y));
        if (ofRandom(1.0) < 0.3) draw(k-1, W4x(x,y), W4y(x,y));
    } else {
        float s = 490;
        ofSetColor(0, 128, 0);
        ofSetLineWidth(1);
        ofLine(x*s+ofGetWidth()*.5, ofGetHeight()-y*s, x*s+ofGetWidth()*.5+1, ofGetHeight()-y*s+1);
    }
}