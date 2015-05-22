//
//  VisualizerObject.cpp
//  VisualizerComponents
//
//  Created by Hiroshi Kawaguchi on 2015/05/21.
//
//

#include "VisualizerObject.h"

void VisualizerObject::setup() {
    for(int i = 0; i < objectNum; i++){
        Primitive drawObject;
        drawObject.drawMode = i % 2;
        prims.push_back(drawObject);
    }
    ofSetSphereResolution(2);
    
    pos.set("pos",ofVec3f(0,0,0),ofVec3f(0,0,0),ofVec3f(300,300,300));
    size.set("scale",ofVec3f(50,60,80),ofVec3f(0,0,0),ofVec3f(1000,1000,1000));
    rotate.set("rotate",ofVec3f(0,0,100),ofVec3f(0,0,0),ofVec3f(360.0,360.0,360.0));
}

void VisualizerObject::update(float smoothedVol) {
    float cVol = smoothedVol * 0.06;
    
    cout << "size: " << size << endl;
    for(int i = 0; i <prims.size(); i++){
        
        prims[i].audioVal = smoothedVol;
        
        prims[i].pos.x = (i - prims.size() * 0.5) * cVol * pos->x + ofGetWidth() * 0.5;
        prims[i].pos.y = (i - prims.size() * 0.5) * cVol * pos->y + ofGetHeight() * 0.5;
        prims[i].pos.z = (i - prims.size() * 0.5) * cVol * pos->z;
        
        
        prims[i].rotate.x = i * cVol * rotate->x;
        prims[i].rotate.y = i * cVol * rotate->y;
        prims[i].rotate.z = i * cVol * rotate->z;
        
        prims[i].size.x = i * cVol * size->x + 40;
        prims[i].size.y = i * cVol * size->y + 40;
        prims[i].size.z = i * cVol * size->z + 40;
        
//        prims[i].color = currentColor;
        
        prims[i].bFill = bFill.y;
//        prims[i].rotAxisMode = rotAxisMode;
        prims[i].update();
    }
}

void VisualizerObject::draw() {
    ofEnableAlphaBlending();
    ofBackground(0,0,0,0);
    ofPushMatrix();
    for(int i = 50; i <prims.size(); i++){
        prims[i].draw();
    }
    ofPopMatrix();
}


void VisualizerObject::randomiseAll() {
    
    int rand = (int)ofRandom(0,5);
    if(rand == 0)pos.set(ofVec3f(100,0,0));
    else if(rand == 1)pos.set(ofVec3f(0,100,0));
    else if(rand == 2)pos.set(ofVec3f(0,0,100));
    else if(rand == 3)pos.set(ofVec3f(ofRandom(50,200),ofRandom(50,200),ofRandom(50,200)));
    else if(rand == 4)pos.set(ofVec3f(0,0,0));
    
    rand = (int)ofRandom(0,5);
    if(rand == 0)rotate.set(ofVec3f(100,0,0));
    else if(rand == 1)rotate.set(ofVec3f(0,100,0));
    else if(rand == 2)rotate.set(ofVec3f(0,0,100));
    else if(rand == 3)rotate.set(ofVec3f(ofRandom(0,100),ofRandom(0,100),ofRandom(0,100)));
    else if(rand == 4)rotate.set(ofVec3f(0,0,0));
    
    rand = (int)ofRandom(0,4);
    if(rand == 0)size.set(ofVec3f(ofRandom(100,300),0,0));
    else if(rand == 1)size.set(ofVec3f(0,ofRandom(100,300),0));
    else if(rand == 2)size.set(ofVec3f(0,0,ofRandom(100,300)));
    else if(rand == 3)size.set(ofVec3f(ofRandom(0,300),ofRandom(0,300),ofRandom(0,300)));
    
}
