//
//  InputSound.h
//  VisualizerComponents
//
//  Created by Hiroshi Kawaguchi on 2015/05/22.
//
//

#ifndef VisualizerComponents_InputSound_h
#define VisualizerComponents_InputSound_h

class InputSound {
private:
    bool bang;
    bool isOverThreshold;
    bool prevStateOverThreshold;
    
    int bufferCounter;
    
    ofSoundStream soundStream;
    vector <float> left;
    
    float smoothedVol;
    float scaledVol;
    ofParameter<float> thresh;
    void updateState() {
        bang = false;
        isOverThreshold = smoothedVol > thresh;
        if (!prevStateOverThreshold && isOverThreshold) {
            bang == true;
        }
        prevStateOverThreshold = isOverThreshold;
    }

public:
    bool isBang() { return bang; }
    float getVol() { return smoothedVol; }

    void setup(ofBaseApp *app) {
        int bufferSize = 256;
        soundStream.setup(app, 0, 1, 44100, bufferSize, 4);
        
        left.assign(bufferSize, 0.0);
        thresh = 0.3;
        bang = false;
    }
    
    void audioIn(float * input, int bufferSize, int nChannels) {
        float curVol = 0.0;
        
        int numCounted = 0;
        
        for (int i = 0; i < bufferSize; i++){
            left[i]		= input[i]*0.5;
            
            curVol += left[i] * left[i];
            numCounted++;
        }
        
        curVol /= (float)numCounted;
        curVol = sqrt( curVol );
        
        smoothedVol *= 0.93;
        smoothedVol += 0.07 * curVol;
        
        bufferCounter++;
        
        updateState();
    }
};

#endif
