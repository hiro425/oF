//
//  drawHelper.h
//  drawingPlants
//
//  Created by 川口大志 on 2014/09/21.
//
//

#ifndef __drawingPlants__drawHelper__
#define __drawingPlants__drawHelper__

#include "ofMain.h"

class drawHelper {
public:
    static drawHelper &getInstance();
    vector <ofVec3f> flowers;
    vector <ofVec3f> scales;
    vector <ofFloatColor> colors;
    int check;
    ofVbo vbo;
    
private:
    drawHelper(){};
    drawHelper(const drawHelper &other){}
    drawHelper &operator=(const drawHelper &other){}
};

#endif /* defined(__drawingPlants__drawHelper__) */
