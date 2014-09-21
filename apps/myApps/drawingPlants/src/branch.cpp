//
//  branch.cpp
//  drawingPlants
//
//  Created by 川口大志 on 2014/09/19.
//
//  TODO: ofTriangle

#include "branch.h"

branch::branch(float lev, float ind, float ex, float why, float zi) {
    numChildren = 2;
    maxLevels   = 3;
    
    level = lev;
    index = ind;
    
    strokeW = (1/level) * 10;
    alph = 255/level;
    len = 1;//(1/level) * ofRandom(100, 300);
    rot = ofRandom(30, 150);
    rot2 = ofRandom(0, 360);
    lenChange = ofRandom(0.5,1);
    rotChange = ofRandom(20) -10;
    lenMax = (1/index) * ofGetWidth();
    witherFlg = false;
    flowerNum = 0;
    
    if (level == 1) {
        lenChange = 1;
        rot = 90;
        rot2 = 90;
    }

    updateMe(ex, why, zi);

    ofDisableArbTex();
	ofLoadImage(texture, "dot.png");
#ifdef TARGET_OPENGLES
    shader.load("shaders_gles/shader");
#else
    shader.load("shaders/shader");
#endif
    
    drawHelper &dH = drawHelper::getInstance();
    dH.check = level;
}

void branch::updateMe(float ex, float why, float zi) {
    x = ex;
    y = why;
    z = zi;

    len += lenChange;
    if      (len < 0)      len = 0;
    else if (len > lenMax) len = lenMax;
 
    
    float radian = ofDegToRad(rot);
    endx = x + (len * cos(radian));
    endy = y + (len * sin(radian));
    endz = z + (len * cos(ofDegToRad(rot2)));

    drawHelper &dH = drawHelper::getInstance();
    
    if (witherFlg) {
        if (dH.flowers.size()>0) {
            dH.flowers.pop_back();
            dH.colors.pop_back();
            dH.scales.pop_back();
            if (level == 1 && len < 100) {
                dH.flowers.clear();
                dH.colors.clear();
                dH.scales.clear();
            }
        }
        if (dH.flowers.size()>0) {
            dH.flowers.pop_back();
            dH.colors.pop_back();
            dH.scales.pop_back();
        }
        

        for (int i = 0; i < children.size(); i++) {
            if (children[i].len == 0) {
                children.erase(children.begin()+i);
            }
        }
        if (lenChange > 0)    lenChange =  0;
        if (!children.size()) lenChange = -3;
    }
    else {
        if (lenChange<0) lenChange = 1;
        

        //if (level == maxLevels && flowers.size() < 200) {
        if (level == maxLevels && flowerNum < 100) {
            dH.flowers.push_back(ofVec3f(ofRandom(endx-50, endx+50), ofRandom(endy-50, endy+50), ofRandom(endz-50, endz+50)));
            
            int rand = ofRandom(3);
            float r,g,b;
            
            if      (rand == 0) r = 251., g = 154., b = 133.;
            else if (rand == 1) r = 246., g = 191., b = 188.;
            else if (rand == 2) r = 253., g = 217., b = 217.;
            else                r = 250., g = 141., b = 148.;

            ofFloatColor color = ofFloatColor(r/255., g/255., b/255.);
            dH.colors.push_back(color);
            
            dH.scales.push_back(ofVec3f(20.));
            
            flowerNum++;
        }
        
        if (int(len) % 80 == 0) {
            if (level < maxLevels) {
                for (int i = 0; i < numChildren; i++) {
                    branch b = branch(level+1, level + int(len/80), endx,endy, endz);
                    children.push_back(b);
                }
            }
            
            else {
                lenChange = 0.01;
            }
            
        }
    }

    for (int i = 0; i < children.size(); i++) {
        children[i].updateMe(children[i].x, children[i].y,children[i].z);
    }

    dH.vbo.setVertexData(&dH.flowers[0], dH.flowers.size(), GL_DYNAMIC_DRAW);
    dH.vbo.setColorData(&dH.colors[0], dH.colors.size(), GL_DYNAMIC_DRAW);
    dH.vbo.setNormalData(&dH.scales[0], dH.scales.size(), GL_DYNAMIC_DRAW);
}

void branch::drawMe() {
    ofSetLineWidth(strokeW);
    ofFill();
    ofSetColor(89, 69, 57);
    ofLine(x, y, z, endx, endy, endz);
    for (int i = 0; i < children.size(); i++) {
        children[i].drawMe();
    }
    
    if (level == 1) {
        drawHelper &dH = drawHelper::getInstance();
        
        //glDepthMask(GL_FALSE);
        //ofEnableAlphaBlending();
        //ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        ofEnablePointSprites();
        
        shader.begin();
        texture.bind();
        dH.vbo.draw(GL_POINTS, 0, dH.flowers.size());
        texture.unbind();
        shader.end();
        ofDisablePointSprites();
        //glDepthMask(GL_TRUE);
    }

}

void branch::witherMe() {
    witherFlg = !witherFlg;
    for (int i = 0; i < children.size(); i++) {
        children[i].witherMe();
    }
    
}