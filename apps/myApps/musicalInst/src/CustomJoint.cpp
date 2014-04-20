//
//  CustomJoint.cpp
//  musicalInst
//
//  Created by 川口大志 on 2014/04/19.
//
//

#include "CustomJoint.h"

void CustomJoint::draw() {
    if (!alive) return;
    
    b2Vec2 p1 = joint->GetAnchorA();
    b2Vec2 p2 = joint->GetAnchorB();
    p1 *= OFX_BOX2D_SCALE;
    p2 *= OFX_BOX2D_SCALE;
    
    float dist = ofDist(p1.x, p1.y, p2.x, p2.y);
    ofEnableSmoothing();
    ofNoFill();
    ofSetLineWidth(2);
    ofSetColor(255, 255, 255, 100-(dist/3.0));
    ofLine(p1.x, p1.y, p2.x, p2.y);
    ofDisableSmoothing();
    
}
