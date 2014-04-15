//
//  ofBlob.h
//  ofBlob
//
//  Created by 川口 大志 on 14/04/15.
//
//

#ifndef __ofBlob__ofBlob__
#define __ofBlob__ofBlob__

#include "ofMain.h"

class ofBlob {
    
private:
    ofPoint pos;
    float dim;
    ofPoint speed;
    float phase;
    float phaseSpeed;
    float moveDim;
public:
    ofBlob();
    void    draw();
    void    update();
    void    setPos(ofPoint pos);
    ofPoint getPos();
    void    setDim(float dim);
    float   getDim();
    ofPoint getSpeed();
    void    setSpeed(ofPoint speed);
};

#endif /* defined(__ofBlob__ofBlob__) */
