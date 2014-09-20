//
//  fern.h
//  drawingPlants
//
//  Created by 川口大志 on 2014/09/19.
//
//

#ifndef __drawingPlants__fern__
#define __drawingPlants__fern__

#include "ofMain.h"

class fern {
public:
    fern();
    float xm;
    float ym;
    float h;
    
    void draw(int k, float x, float y);
};

#endif /* defined(__drawingPlants__fern__) */
