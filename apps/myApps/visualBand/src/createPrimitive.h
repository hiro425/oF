//
//  createPrimitive.h
//  visualBand
//
//  Created by 川口大志 on 2014/08/03.
//
//

#ifndef __visualBand__createPrimitive__
#define __visualBand__createPrimitive__

#include "ofMain.h"
#include "createObject.h"


class createPrimitive: public createObject {
public:
    createPrimitive() {}
    void init(int w, int h, const ofVec2f &sc, const ofVec3f &v, DrawType t = CP_POINT, ActionType a = CP_SLIDE);
    void draw();
    
    float noiseSeed;
    float noiseMulti;
    bool particleFlg;
    
    ofTexture texture;
    ofShader shader;
};

#endif /* defined(__visualBand__createPrimitive__) */
