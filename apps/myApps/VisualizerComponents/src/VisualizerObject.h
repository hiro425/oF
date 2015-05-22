//
//  VisualizerObject.h
//  VisualizerComponents
//
//  Created by Hiroshi Kawaguchi on 2015/05/21.
//
//

#ifndef __VisualizerComponents__VisualizerObject__
#define __VisualizerComponents__VisualizerObject__

#include <ofMain.h>
#include <SlidePoint.h>
#include <Primitive.h>

class VisualizerObject {
private:
    static const int objectNum = 100;
    ofParameter<ofVec3f> pos;
    ofParameter<ofVec3f> size;
    ofParameter<ofVec3f> rotate;
    ofParameter<int> repeat;
    ofParameter<ofColor> color;
    SlidePoint globalRotate;
    
    ofPoint bFill;
    bool bReverse;
    
    vector<Primitive>prims;
    
public:
    void setup();
    void update(float smoothedVol);
    void draw();
    
    void randomiseAll();
    
};

#endif /* defined(__VisualizerComponents__VisualizerObject__) */
