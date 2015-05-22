//
//  VizualizerController.h
//  VisualizerComponents
//
//  Created by Hiroshi Kawaguchi on 2015/05/21.
//
//

#ifndef VisualizerComponents_VizualizerController_h
#define VisualizerComponents_VizualizerController_h

#include "ofMain.h"
#include "VisualizerObject.h"
#include "InputSound.h"

class VisualizerController {
private:
    InputSound inputSound;
    VisualizerObject vObj;
    
public:
    void setup(ofBaseApp *app);
    
    void update();
    void draw();
    
    void audioIn(float * input, int bufferSize, int nChannels);
};

#endif
