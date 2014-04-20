//
//  CustomJoint.h
//  musicalInst
//
//  Created by 川口大志 on 2014/04/19.
//
//

#ifndef __musicalInst__CustomJoint__
#define __musicalInst__CustomJoint__

#include "ofxBox2d.h"

class CustomJoint: public ofxBox2dJoint {
public:
    void draw();
    //void setup(b2World* b2world, b2Body* body1, b2Body* body2, float frequencyHz=4.f, float damping=.5f, bool bCollideConnected=true);

};


#endif /* defined(__musicalInst__CustomJoint__) */
