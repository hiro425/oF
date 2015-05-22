//
//  SlidePoint.h
//  VisualizerComponents
//
//  Created by Hiroshi Kawaguchi on 2015/05/21.
//
//

#ifndef VisualizerComponents_SlidePoint_h
#define VisualizerComponents_SlidePoint_h

class SlidePoint : public ofPoint {
private:
    
    ofPoint p;
    
    float speed;
    
public:
    
    SlidePoint():speed(0.1){
    }
    
    void set(float _px,float _py,float _pz){
        p.x = _px;
        p.y = _py;
        p.z = _pz;
    }
    
    void set(float _px,float _py){
        p.x = _px;
        p.y = _py;
    }
    
    void imSet(float _px,float _py,float _pz){
        p.x = _px;
        p.y = _py;
        p.z = _pz;
        
        x = _px;
        y = _py;
        z = _pz;
    }
    
    void imSet(float _px,float _py){
        p.x = _px;
        p.y = _py;
        
        x = _px;
        y = _py;
        
    }
    
    void update(){
        x += (p.x - x) * speed;
        y += (p.y - y) * speed;
        z += (p.z - z) * speed;
    }
};

#endif
