//
//  createObject.h
//  visualBand
//
//  Created by 川口大志 on 2014/08/07.
//
//

#ifndef __visualBand__createObject__
#define __visualBand__createObject__

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "defines.h"
#include "createAction.h"
#include "createPrimitive.h"

class createObject {
public:

    
    DrawType   dType;
    ActionType aType;
    ColorType  cType;
    bool addFlg;
    bool animationFlg;
    bool updateAddFlg;
    bool noiseFlg;
    bool bitNoiseFlg;
    
    ofFloatColor minColor;
    ofFloatColor maxColor;

    property prop;
    float angle;
    
    createAction cAction;
    createPrimitive cPrimitive;
    
    vector<ofxAssimpModelLoader*> models;
    
    // ------------------------------- function
    createObject();
    
    void setup(ofVec2f d, ofVec2f s, ofVec3f minp, ofVec3f maxp, ofVec2f size, DrawType t, ActionType a, ColorType c);
    void update();
    void callback();
    void draw();
    void _add();
    void _add(const ofVec3f &mag);
    
    
    void clear();
    void eraseOld();
    
    void setDrawType(DrawType d);
    void setActionType(ActionType a);
    void setColorType(ColorType c);
    
    ofColor getColor();
    int     getIndex();
    
    void updatePos(int index, const ofVec3f &v);
    void updateMags(float mag);
    void updateDrawBins(vector <float> &d);
};


#endif /* defined(__visualBand__createObject__) */
