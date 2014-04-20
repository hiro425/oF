//
//  CustomCircle.cpp
//  ofxOpenCvBox2d
//
//  Created by 川口 大志 on 14/04/16.
//
//

#include "CustomCircle.h"

void CustomCircle::draw() {
    float radius = getRadius();
    ofPushMatrix();
    ofTranslate(getPosition().x, getPosition().y);
    ofFill();
    ofSetColor(31, 127, 255, 100);
    ofCircle(0, 0, radius);
    ofSetColor(31, 127, 255, 200);
    ofCircle(0, 0, radius*0.7);
    ofPopMatrix();
}
