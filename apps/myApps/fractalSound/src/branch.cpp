//
//  branch.cpp
//  fractalSound
//
//  Created by 川口大志 on 2014/04/19.
//
//

#include "branch.h"

branch::branch(float lev, float ind, float ex, float why) {
    level = lev;
    index = ind;
    update(ex, why);
}

void branch::init(int numChildren, int maxLevel) {
    
}
/*
void branch::newTree() {
    branch _trunk = new branch(1, 0, ofGetWidth()/2, 50);
    _trunk.draw();
}
*/
void branch::update(float ex, float why) {
    x = ex;
    y = why;
    endx = x + 150;
    endy = y + 15;
}

void branch::draw() {
    ofLine(x, y, endx, endy);
    ofCircle(x, y, 5);
}
