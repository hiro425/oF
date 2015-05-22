//
//  Primitive.h
//  VisualizerComponents
//
//  Created by Hiroshi Kawaguchi on 2015/05/21.
//
//

#ifndef VisualizerComponents_Primitive_h
#define VisualizerComponents_Primitive_h

#include <ofMain.h>
#include <SlidePoint.h>

class Primitive {
public:
    
    ofPoint pos;
    SlidePoint size;
    SlidePoint rotate;
    ofColor color;
    int drawMode;
    bool bFill;
    float audioVal;
    int rotAxisMode;
    
    Primitive():bFill(false),rotAxisMode(1){
        color.set(255,255,255,255);
        size.set(10,10);
        drawMode = 0;
        pos.set(ofGetWidth() * 0.5 ,ofGetHeight() * 0.5);
    }
    void update(){
        //size.set(audioVal*200.0,audioVal*200.0);
    }
    
    void draw(){
        ofPushMatrix();
        ofPushStyle();
        ofSetRectMode(OF_RECTMODE_CENTER);
        if(bFill)ofFill();
        else ofNoFill();
        
        ofSetColor(color.r,color.g,color.b,color.a);
        
        if(rotAxisMode == 1)ofTranslate(pos.x,pos.y,pos.z);
        else if(rotAxisMode == 0)ofTranslate(ofGetWidth()*0.5, ofGetHeight()*0.5);
        
        ofRotateX(rotate.x);
        ofRotateY(rotate.y);
        ofRotateZ(rotate.z);
        
        if(rotAxisMode == 0){
            ofTranslate(-ofGetWidth()*0.5, -ofGetHeight()*0.5);
            ofTranslate(pos.x,pos.y,pos.z);
        }
        if(drawMode == 0) ofDrawBox(0,0,0,size.x,size.y,size.z);
        else ofDrawSphere(0,0,0,size.x * 0.5);
        ofPopStyle();
        ofPopMatrix();
        
    }
};

#endif
