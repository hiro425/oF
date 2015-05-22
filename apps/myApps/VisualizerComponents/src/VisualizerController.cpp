//
//  VisualizerController.cpp
//  VisualizerComponents
//
//  Created by Hiroshi Kawaguchi on 2015/05/21.
//
//

#include <VisualizerController.h>

void VisualizerController::setup(ofBaseApp *app) {
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    inputSound.setup(app);
    vObj.setup();
}

void VisualizerController::update() {
    if (inputSound.isBang()) {
        vObj.randomiseAll();
    }
    
    vObj.update(inputSound.getVol());
    
}

void VisualizerController::draw() {
    vObj.draw();
    
}

void VisualizerController::audioIn(float * input, int bufferSize, int nChannels) {
    inputSound.audioIn(input, bufferSize, nChannels);
}

