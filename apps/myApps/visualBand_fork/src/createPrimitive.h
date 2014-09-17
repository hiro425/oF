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
#include "defines.h"


class createPrimitive {
public:
    createPrimitive() {}
    void setup(property &p);
    void draw(property &p);
    
    property prop;
    
    float noiseSeed;
    float noiseMulti;
    bool particleFlg;
    
    
    ofTexture texture;
    ofShader shader;
};

#endif /* defined(__visualBand__createPrimitive__) */
