//
//  createObject.cpp
//  visualBand
//
//  Created by 川口大志 on 2014/08/07.
//
//

#include "createObject.h"

createObject::createObject() {
    angle = 0.0;
    addFlg = true;
    animationFlg = false;
}

void createObject::setup(
                         ofVec2f d,
                         ofVec2f s,
                         ofVec3f minp,
                         ofVec3f maxp,
                         ofVec2f size,
                         DrawType t,
                         ActionType a,
                         ColorType c ){
    prop.div = d;
    prop.screen = s;
    prop.minAddPos = minp;
    prop.maxAddPos = maxp;
    prop.minMaxSize = size;
    prop.dType = t;
    prop.aType = a;
    setColorType(c);
    prop.angle = 0.0;
    
    updateAddFlg = false;
    addFlg  = true;
    noiseFlg = false;
    prop.mags.clear();
    
    cAction.setup(prop);
    cPrimitive.setup(prop);
}

void createObject::_add() {
    if (!addFlg) {
        return;
    }
    prop.points.push_back(ofVec3f(
                                  ofRandom(prop.minAddPos.x, prop.maxAddPos.x),
                                  ofRandom(prop.minAddPos.y, prop.maxAddPos.y),
                                  ofRandom(prop.minAddPos.z, prop.maxAddPos.z)
                                  ));
    prop.distPoints.push_back(ofVec3f(
                                      ofRandom(prop.minAddPos.x, prop.maxAddPos.x),
                                      ofRandom(prop.minAddPos.y, prop.maxAddPos.y),
                                      ofRandom(prop.minAddPos.z, prop.maxAddPos.z)
                                      ));
    prop.colors.push_back(ofFloatColor(ofRandom(minColor.r, maxColor.r),
                                       ofRandom(minColor.g, maxColor.g),
                                       ofRandom(minColor.b, maxColor.b),
                                       ofRandom(minColor.a, maxColor.a)));
    prop.faces.push_back(getIndex());
    
    ofVec3f mag = ofVec3f(ofRandom(prop.minMaxSize.x, prop.minMaxSize.y));
    prop.mags.push_back(mag);
    prop.origMags.push_back(mag.x);
    prop.animated.push_back(false);
    
    prop.vbo.setVertexData(&prop.points[0], prop.points.size(), GL_DYNAMIC_DRAW);
    prop.vbo.setNormalData(&prop.mags[0], prop.mags.size(), GL_DYNAMIC_DRAW);
    prop.vbo.setColorData(&prop.colors[0], prop.colors.size(), GL_DYNAMIC_DRAW);
    prop.vbo.setIndexData(&prop.faces[0], prop.faces.size(), GL_DYNAMIC_DRAW);
}

void createObject::_add(const ofVec3f &mag) {
    if (!addFlg) {
        return;
    }
    prop.points.push_back(ofVec3f(
                                  ofRandom(prop.minAddPos.x, prop.maxAddPos.x),
                                  ofRandom(prop.minAddPos.y, prop.maxAddPos.y),
                                  ofRandom(prop.minAddPos.z, prop.maxAddPos.z)
                                  ));
    prop.distPoints.push_back(ofVec3f(
                                      ofRandom(prop.minAddPos.x, prop.maxAddPos.x),
                                      ofRandom(prop.minAddPos.y, prop.maxAddPos.y),
                                      ofRandom(prop.minAddPos.z, prop.maxAddPos.z)
                                      ));
    prop.colors.push_back(ofFloatColor(ofRandom(minColor.r, maxColor.r),
                                       ofRandom(minColor.g, maxColor.g),
                                       ofRandom(minColor.b, maxColor.b),
                                       ofRandom(minColor.a, maxColor.a)));
    prop.faces.push_back(getIndex());
    
    prop.mags.push_back(mag);
    prop.origMags.push_back(mag.x);
    prop.animated.push_back(false);
    
    prop.vbo.setVertexData(&prop.points[0], prop.points.size(), GL_DYNAMIC_DRAW);
    prop.vbo.setNormalData(&prop.mags[0], prop.mags.size(), GL_DYNAMIC_DRAW);
    prop.vbo.setColorData(&prop.colors[0], prop.colors.size(), GL_DYNAMIC_DRAW);
    prop.vbo.setIndexData(&prop.faces[0], prop.faces.size(), GL_DYNAMIC_DRAW);
}

void createObject::clear() {
    prop.points.clear();
    prop.distPoints.clear();
    prop.colors.clear();
    prop.faces.clear();
    prop.mags.clear();
    prop.origMags.clear();
    prop.animated.clear();
}

void createObject::eraseOld() {
    prop.points.erase(prop.points.begin());
    prop.distPoints.erase(prop.distPoints.begin());
    prop.colors.erase(prop.colors.begin());
    //faces.erase(faces.begin());
    prop.faces.pop_back();
    prop.mags.erase(prop.mags.begin());
    prop.origMags.erase(prop.origMags.begin());
}

int createObject::getIndex() {
    return prop.faces.size();
}

void createObject::setDrawType(DrawType d) {
    prop.dType = d;
}

void createObject::setActionType(ActionType a) {
    prop.aType = a;
}

void createObject::setColorType(ColorType c) {
    cType = c;
    
    float minR;
    float maxR;
    float minG;
    float maxG;
    float minB;
    float maxB;
    float minA;
    float maxA;
    
    switch (cType) {
        case CP_MONO:
            minColor = ofFloatColor(1.0, 1.0, 1.0, 1.0);
            maxColor = ofFloatColor(1.0, 1.0, 1.0, 1.0);
            
            for (int i = 0; i < prop.colors.size(); i++) {
                float rand = ofRandom(1);
                prop.colors[i] = ofFloatColor(rand,
                                         rand,
                                         rand,
                                         rand);
            }
            break;
        case CP_GRAY:
            minColor = ofFloatColor(1.0, 1.0, 1.0, 0.3);
            maxColor = ofFloatColor(1.0, 1.0, 1.0, 0.3);
            
            for (int i = 0; i < prop.colors.size(); i++) {
                prop.colors[i] = ofFloatColor(ofRandom(minColor.r, maxColor.r),
                                              ofRandom(minColor.g, maxColor.g),
                                              ofRandom(minColor.b, maxColor.b),
                                              ofRandom(minColor.a, maxColor.a));
            }
            break;
        case CP_HIGHLIGHT:
            minR = 255. / 255.;
            maxR = 255. / 255.;
            minG = 251. / 255.;
            maxG = 251. / 255.;
            minB = 204. / 255.;
            maxB = 204. / 255.;
            minA = 127. /255.;
            maxA = 255. / 255.;
            minColor = ofFloatColor(minR, minG, minB, minA);
            maxColor = ofFloatColor(maxR, maxG, maxB, maxA);
            for (int i = 0; i < prop.colors.size(); i++) {
                prop.colors[i] = ofFloatColor(ofRandom(minColor.r, maxColor.r),
                                              ofRandom(minColor.g, maxColor.g),
                                              ofRandom(minColor.b, maxColor.b),
                                              ofRandom(minColor.a, maxColor.a));
            }
            break;
        case CP_FIRE:
            minR = 127. / 255.;
            maxR = 255. / 255.;
            minG = 0. / 255.;
            maxG = 50. / 255.;
            minB = 0. / 255.;
            maxB = 0. / 255.;
            minA = 127. /255.;
            maxA = 255. / 255.;
            minColor = ofFloatColor(minR, minG, minB, minA);
            maxColor = ofFloatColor(maxR, maxG, maxB, maxA);
            for (int i = 0; i < prop.colors.size(); i++) {
                prop.colors[i] = ofFloatColor(ofRandom(minColor.r, maxColor.r),
                                         ofRandom(minColor.g, maxColor.g),
                                         ofRandom(minColor.b, maxColor.b),
                                         ofRandom(minColor.a, maxColor.a));
            }
            break;
        case CP_SEA:
            minR = 0. / 255.;
            maxR = 0. / 255.;
            minG = 0. / 255.;
            maxG = 50. / 255.;
            minB = 127. / 255.;
            maxB = 255. / 255.;
            minA = 127. /255.;
            maxA = 255. / 255.;
            minColor = ofFloatColor(minR, minG, minB, minA);
            maxColor = ofFloatColor(maxR, maxG, maxB, maxA);
            for (int i = 0; i < prop.colors.size(); i++) {
                prop.colors[i] = ofFloatColor(ofRandom(minColor.r, maxColor.r),
                                         ofRandom(minColor.g, maxColor.g),
                                         ofRandom(minColor.b, maxColor.b),
                                         ofRandom(minColor.a, maxColor.a));
            }
            break;
        case CP_FOREST:
            minR = 0. / 255.;
            maxR = 0. / 255.;
            minG = 127. / 255.;
            maxG = 255. / 255.;
            minB = 0. / 255.;
            maxB = 50. / 255.;
            minA = 127. /255.;
            maxA = 255. / 255.;
            minColor = ofFloatColor(minR, minG, minB, minA);
            maxColor = ofFloatColor(maxR, maxG, maxB, maxA);
            for (int i = 0; i < prop.colors.size(); i++) {
                prop.colors[i] = ofFloatColor(ofRandom(minColor.r, maxColor.r),
                                         ofRandom(minColor.g, maxColor.g),
                                         ofRandom(minColor.b, maxColor.b),
                                         ofRandom(minColor.a, maxColor.a));
            }
            break;
        case CP_RANDOM:
            minColor = ofFloatColor(ofRandom(1),ofRandom(1), ofRandom(1), ofRandom(1));
            maxColor = ofFloatColor(ofRandom(1), ofRandom(1), ofRandom(1), ofRandom(1));
            for (int i = 0; i < prop.colors.size(); i++) {
                prop.colors[i] = ofFloatColor(ofRandom(minColor.r, maxColor.r),
                                         ofRandom(minColor.g, maxColor.g),
                                         ofRandom(minColor.b, maxColor.b),
                                         ofRandom(minColor.a, maxColor.a));
            }
            break;
        default:
            break;
    }
    
    prop.vbo.setColorData(&prop.colors[0], prop.colors.size(), GL_DYNAMIC_DRAW);
}

ofColor createObject::getColor() {
    int r =maxColor.r*255;
    int g =maxColor.g*255;
    int b =maxColor.b*255;  
    int a =maxColor.a*255;
    
    return ofColor(r, g,b,a);
}

void createObject::updatePos(int index, const ofVec3f &v) {
    prop.points[index] = v;
    prop.vbo.setVertexData(&prop.points[0], prop.points.size(), GL_DYNAMIC_DRAW);
}

void createObject::updateMags(float mag) {
    for (int i = 0; i < prop.mags.size(); i++) {
        prop.mags[i] = ofVec3f(prop.origMags[i] * mag);
    }
}

void createObject::updateDrawBins(vector <float> &d) {
    prop.drawBins = d;
    //cout << prop.drawBins.size() << endl;
}


void createObject::update() {
    if (!prop.points.size()) return;
    while (prop.points.size() > 800) eraseOld();
    if (prop.points[0].x < prop.screen.x - 1000
        || prop.points[0].x > (prop.screen.x + prop.div.x + 1000)
        || prop.points[0].y < prop.screen.y - 1000
        || prop.points[0].y > (prop.screen.y + prop.div.y + 1000))
    {
        eraseOld();
    }
   
    float seed = ofRandom(-1, 1);
    float multiply = 5.0;
    
    for (int i = 0; i < prop.points.size(); i++) {
        ofVec3f speed = (prop.distPoints[i] - prop.points[i]) * 0.3;
        prop.points[i] += speed;
        
        if (noiseFlg) {
            seed += 0.1;
            float noise = ofNoise(seed) - 0.51;
            
            if (i%3==0) {
                prop.points[i].z = prop.points[i].z + noise * multiply * prop.mags[i].x/2;
                prop.points[i].x = prop.points[i].x + noise * multiply * prop.mags[i].x/2;
            }
            else if(i%3==1) {
                prop.points[i].x = prop.points[i].x + noise * multiply * prop.mags[i].x/2;
                prop.points[i].y = prop.points[i].y + noise * multiply * prop.mags[i].x/2;
            }
            else {
                prop.points[i].y = prop.points[i].y + noise * multiply * prop.mags[i].x/2;
                prop.points[i].z = prop.points[i].z + noise * multiply * prop.mags[i].x/2;
            }
        }
    }
    
    cAction.update(prop);
    
    switch (prop.aType) {
        case CP_FLOW_X:
            if ( (prop.mags[0].x > 10) && (ofGetElapsedTimeMillis()%10 == 0) ) _add();
            //cout << prop.mags[0].x << endl;
            break;
        default:
            break;
    }
    
    switch (prop.dType) {
        case CP_CYLINDER_SPECTRUM:
            cout << prop.vol << endl;
            if (prop.vol > 0.5 && (ofGetElapsedTimeMillis()%10 == 0) ) {
                for (int i = 0; i < 10/*prop.drawBins.size()*/; i++) {
                    prop.minAddPos.z = prop.maxAddPos.z = -700 + i * 140;
                    prop.minAddPos.y = prop.maxAddPos.y;
                    
                    //_add(ofVec3f(prop.drawBins[i]));
                    /*
                    if (prop.drawBins[i] == 1) {
                        _add(ofVec3f(1,1,1));
                    }
                    else {
                        _add(ofVec3f(0,0,0));
                    }
                    */
                    
                }
            }
            break;
            
        default:
            break;
    }
    
}

void createObject::callback() {
    cAction.callback(prop);
    //_add();
    
    switch (prop.dType) {
        case CP_CYLINDER_SPECTRUM:
            for (int i = 0; i < 10/*prop.drawBins.size()*/; i++) {
                prop.minAddPos.z = prop.maxAddPos.z = -700 + i * 140;
                prop.minAddPos.y = prop.maxAddPos.y;
                
                //_add(ofVec3f(prop.drawBins[i]));
                
                if (prop.drawBins[i] == 1) {
                    _add(ofVec3f(1,1,1));
                }
                else {
                    _add(ofVec3f(0,0,0));
                }
                
            }
            break;
        default:
            _add();
            break;
    }
}

void createObject::draw() {
    cPrimitive.draw(prop);
}



