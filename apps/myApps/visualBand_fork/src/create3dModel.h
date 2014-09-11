//
//  create3dModel.h
//  visualBand
//
//  Created by 川口大志 on 2014/08/07.
//
//

#ifndef __visualBand__create3dModel__
#define __visualBand__create3dModel__

#include "ofMain.h"
#include "createObject.h"
#include "ofxAssimpModelLoader.h"


class create3dModel: public createObject {
public:

    void init(int w, int h, const ofVec2f &sc, const ofVec3f &v, ActionType a = CP_NO_ACTION);
    //void _add(const ofVec3f &v, const ofFloatColor &c, const ofIndexType &f, const ofVec3f &mag);
    void draw();
    
    vector<ofxAssimpModelLoader*> models;
    
    void drawWithMesh();
    
    ofTexture texture;
    ofShader shader;
};

#endif /* defined(__visualBand__create3dModel__) */
