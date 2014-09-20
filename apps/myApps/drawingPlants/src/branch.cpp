//
//  branch.cpp
//  drawingPlants
//
//  Created by 川口大志 on 2014/09/19.
//
//

#include "branch.h"

branch::branch(float lev, float ind, float ex, float why) {
    numChildren = 1;
    maxLevels   = 3;
    
    level = lev;
    index = ind;
    
    strokeW = (1/level) * 20;
    alph = 255/level;
    len = 1;//(1/level) * ofRandom(100, 300);
    rot = ofRandom(0, 180);
    lenChange = ofRandom(0.1,1);
    rotChange = ofRandom(20) -10;
    
    witherFlg = false;
    
    updateMe(ex, why);
    /*
    if (level < maxLevels) {
        for (int i = 0; i < numChildren; i++) {
            branch b = branch(level+1, x, endx,endy);
            children.push_back(b);
        }
    }
     */
}

void branch::updateMe(float ex, float why) {
    x = ex;
    y = why;

    /*
    rot += rotChange;
    if (rot > 180) rot = 0;
    else if (rot < 0) rot = 180;
    */
     
    /*
    len -= lenChange;
    if (len < 0) lenChange *= -1;
    else if (len > 200) lenChange *= -1;
    */

    if (level == 1) {
        rot = 90;
        lenChange = 1;
    }

    len += lenChange;
    if (len < 0) {
        len = 0;
    }
 
    float radian = ofDegToRad(rot);
    endx = x + (len * cos(radian));
    endy = y + (len * sin(radian));
    
    if (witherFlg) {
        if (flowers.size()>0) {
            flowers.pop_back();
            colors.pop_back();
        }

        for (int i = 0; i < children.size(); i++) {
            if (children[i].len == 0) {
                children.erase(children.begin()+i);
            }
        }
    }
    else {
        if (level == maxLevels && flowers.size() < 200) {
            flowers.push_back(ofVec3f(ofRandom(endx-50, endx+50), ofRandom(endy-50, endy+50), 0));
            
            int rand = ofRandom(3);
            ofColor color;
            if (rand == 0) color = ofColor(251,154,133);
            else if (rand == 1) color = ofColor(246,191,188);
            else if (rand == 2) color = ofColor(253,217,217);
            else color = ofColor(250, 141, 148);
            colors.push_back(color);
            
        }
        
        if (int(len) % 100 == 0) {
            if (level < maxLevels) {
                for (int i = 0; i < numChildren; i++) {
                    branch b = branch(level+1, x, endx,endy);
                    children.push_back(b);
                }
            }
            else {
                lenChange = 0.05;
            }
        }
    }

    for (int i = 0; i < children.size(); i++) {
        //children[i].updateMe(endx, endy);
        children[i].updateMe(children[i].x, children[i].y);
    }
    
}

void branch::drawMe() {
    ofSetLineWidth(strokeW);
    ofSetColor(89, 69, 57);
    ofLine(x, y, endx, endy);
    for (int i = 0; i < children.size(); i++) {
        children[i].drawMe();
    }
    
    if (level == maxLevels) {
        for (int i = 0; i < flowers.size(); i++) {
            ofSetColor(colors[i]);
            ofEllipse(flowers[i].x, flowers[i].y, 5, 5);
        }
    }
    
}

void branch::witherMe() {
    if (abs(lenChange)< 1) {
        lenChange = 1;
    }
    lenChange *= -1;
    witherFlg = !witherFlg;
    
    for (int i = 0; i < children.size(); i++) {
        children[i].witherMe();
    }
    
}