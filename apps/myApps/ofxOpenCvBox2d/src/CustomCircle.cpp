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
    glPushMatrix();
    glTranslatef(getPosition().x, getPosition().y, 0);
    ofFill();
    ofSetColor(31, 127, 255, 100);
    ofCircle(0, 0, radius);
    ofCircle(0, 0, radius*0.7);
    glPopMatrix();
}
