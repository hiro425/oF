//
//  createObject.cpp
//  visualBand
//
//  Created by 川口大志 on 2014/08/07.
//
//

#include "createObject.h"

void createObject::add() {
    _add(initPos,
         ofFloatColor(ofRandom(minColor.r, maxColor.r),
                      ofRandom(minColor.g, maxColor.g),
                      ofRandom(minColor.b, maxColor.b),
                      ofRandom(minColor.a, maxColor.a)),
         getIndex(), ofVec3f(ofRandom(5,30)));
}

void createObject::addRandom() {
    _add(ofVec3f(
                 ofRandom(screen.x,width),
                 ofRandom(screen.y, height),
                 ofRandom(-3000, 3000)
                 ),
         ofFloatColor(ofRandom(minColor.r, maxColor.r),
                      ofRandom(minColor.g, maxColor.g),
                      ofRandom(minColor.b, maxColor.b),
                      ofRandom(minColor.a, maxColor.a)),
         getIndex(), ofVec3f(ofRandom(5,30)));
}

void createObject::_add(const ofVec3f &v, const ofFloatColor &c, const ofIndexType &f, const ofVec3f &mag) {
    points.push_back(v);
    distPoints.push_back(v);
    colors.push_back(c);
    faces.push_back(f);
    mags.push_back(mag);
    origMags.push_back(mag.x);
    
    vbo.setVertexData(&points[0], points.size(), GL_DYNAMIC_DRAW);
    vbo.setNormalData(&mags[0], mags.size(), GL_DYNAMIC_DRAW);
    vbo.setColorData(&colors[0], colors.size(), GL_DYNAMIC_DRAW);
    vbo.setIndexData(&faces[0], faces.size(), GL_DYNAMIC_DRAW);
 /*
    ofxAssimpModelLoader *m = new ofxAssimpModelLoader;
    m->loadModel("dog/dog.3ds");
    models.push_back(m);
  */

}

void createObject::clear() {
    points.clear();
    distPoints.clear();
    colors.clear();
    faces.clear();
    mags.clear();
    origMags.clear();
}

void createObject::eraseOld() {
    points.erase(points.begin());
    distPoints.erase(distPoints.begin());
    colors.erase(colors.begin());
    //faces.erase(faces.begin());
    faces.pop_back();
    mags.erase(mags.begin());
    origMags.erase(origMags.begin());
}


int createObject::getIndex() {
    return faces.size();
}

void createObject::setDrawType(DrawType d) {
    dType = d;
}

void createObject::setActionType(ActionType a) {
    aType = a;
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
            minColor = ofFloatColor(1.0, 1.0, 1.0, 0.1);
            maxColor = ofFloatColor(1.0, 1.0, 1.0, 1.0);
            
            for (int i = 0; i < colors.size(); i++) {
                float rand = ofRandom(1);
                colors[i] = ofFloatColor(rand,
                                         rand,
                                         rand,
                                         rand);
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
            for (int i = 0; i < colors.size(); i++) {
                colors[i] = ofFloatColor(ofRandom(minColor.r, maxColor.r),
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
            for (int i = 0; i < colors.size(); i++) {
                colors[i] = ofFloatColor(ofRandom(minColor.r, maxColor.r),
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
            for (int i = 0; i < colors.size(); i++) {
                colors[i] = ofFloatColor(ofRandom(minColor.r, maxColor.r),
                                         ofRandom(minColor.g, maxColor.g),
                                         ofRandom(minColor.b, maxColor.b),
                                         ofRandom(minColor.a, maxColor.a));
            }
            break;
        case CP_RANDOM:
            minColor = ofFloatColor(ofRandom(1),ofRandom(1), ofRandom(1), ofRandom(1));
            maxColor = ofFloatColor(ofRandom(1), ofRandom(1), ofRandom(1), ofRandom(1));
            for (int i = 0; i < colors.size(); i++) {
                colors[i] = ofFloatColor(ofRandom(minColor.r, maxColor.r),
                                         ofRandom(minColor.g, maxColor.g),
                                         ofRandom(minColor.b, maxColor.b),
                                         ofRandom(minColor.a, maxColor.a));
            }
            break;
        default:
            break;
    }
    
    vbo.setColorData(&colors[0], colors.size(), GL_DYNAMIC_DRAW);
}

ofColor createObject::getColor() {
    
    int r =maxColor.r*255;
    int g =maxColor.g*255;
    int b =maxColor.b*255;
    int a =maxColor.a*255;
    
    return ofColor(r, g,b,a);
}

void createObject::updatePos(int index, const ofVec3f &v) {
    points[index] = v;
    vbo.setVertexData(&points[0], points.size(), GL_DYNAMIC_DRAW);
}

void createObject::updateMags(float mag) {
    for (int i = 0; i < mags.size(); i++) {
        mags[i] = ofVec3f(origMags[i] * mag);
    }
}


void createObject::update() {
    while (points.size() > 150) eraseOld();
   
    
    float seed = ofRandom(-1, 1);
    float multiply = 5.0;
    
    for (int i = 0; i < points.size(); i++) {
        ofVec3f speed = (distPoints[i] - points[i]) * 0.3;
        points[i] += speed;
        
        if (noiseFlg) {
            seed += 0.1;
            float noise = ofNoise(seed) - 0.51;
            
            if (i%3==0) {
                points[i].z = points[i].z + noise * multiply * mags[i].x/2;
                points[i].x = points[i].x + noise * multiply * mags[i].x/2;
            }
            else if(i%3==1) {
                points[i].x = points[i].x + noise * multiply * mags[i].x/2;
                points[i].y = points[i].y + noise * multiply * mags[i].x/2;
            }
            else {
                points[i].y = points[i].y + noise * multiply * mags[i].x/2;
                points[i].z = points[i].z + noise * multiply * mags[i].x/2;
            }
            
        }
    }
    switch (aType) {
        case CP_COME_CONSTANT:
            for (int i = 0; i < points.size(); i++) {
                float dist = ofDist(screen.x + width/2, screen.y + height/2, points[i].x, points[i].y);
                distPoints[i] = ofVec3f(
                                        (screen.x + width/2) + dist*cos(ofGetElapsedTimef()+i),
                                        (screen.y + height/2) + dist*sin(ofGetElapsedTimef()+i),
                                        points[i].z + 200);
            }
            if (addFlg) {
                for (int j = 0; j < 3; j++) {
                    _add(
                         ofVec3f(
                                 ofRandom(screen.x, screen.x + width),
                                 ofRandom(screen.y, screen.y + height),
                                 -700
                                 ),
                         ofFloatColor(ofRandom(minColor.r, maxColor.r),
                                      ofRandom(minColor.g, maxColor.g),
                                      ofRandom(minColor.b, maxColor.b),
                                      ofRandom(minColor.a, maxColor.a)),
                         getIndex(),
                         ofVec3f(ofRandom(5,30)));
                }
            }
            break;
        case CP_ROTATE:
            for (int i = 0; i < points.size(); i++) {
                float radius = width/4 * mags[i].x/30;
                distPoints[i] = ofVec3f(
                                        screen.x + width/2 + radius*cos(ofGetElapsedTimef()+i),
                                        screen.y + height/4,
                                        radius*sin(ofGetElapsedTimef()+i));
            }
            break;
        case CP_ROTATE_HORIZONTAL:
            for (int i = 0; i < points.size(); i++) {
                //float dist = ofDist(screen.x + width/2, screen.y + height/2, points[i].x, points[i].y);
                float radius = width/4 * mags[i].x/30;
                distPoints[i] = ofVec3f(
                                        (screen.x + width/2) + radius*cos(ofGetElapsedTimef()+i),
                                        (screen.y + height/2) + radius*sin(ofGetElapsedTimef()+i),
                                        points[i].z);
            }
            break;
        case CP_SPREAD:
            for (int i = 0; i < points.size(); i++) {
                float dist = ofDist(screen.x + width/2, 0, points[i].x, points[i].z);
                distPoints[i] = ofVec3f(
                                        (screen.x + width/2) + (dist + 700)*cos(ofGetElapsedTimef()+i),
                                        points[i].y,
                                        (dist + 700)*sin(ofGetElapsedTimef()+i));
            }
            break;
        default:
            break;
    }
    vbo.setVertexData(&points[0], points.size(), GL_DYNAMIC_DRAW);
    vbo.setNormalData(&mags[0], mags.size(), GL_DYNAMIC_DRAW);
    vbo.setColorData(&colors[0], colors.size(), GL_DYNAMIC_DRAW);
    vbo.setIndexData(&faces[0], faces.size(), GL_DYNAMIC_DRAW);
}

void createObject::callback() {
    float seed = ofRandom(-1, 1);
    float multiply = 5.0;
    
    switch (aType) {
        case CP_SLIDE:
            for (int i = 0; i < points.size(); i++) {
                distPoints[i] = ofVec3f(
                                        ofRandom(screen.x, screen.x + width),
                                        ofRandom(screen.y, screen.y + height),
                                        ofRandom(-700, 700));
            }
            if (addFlg) _add(initPos,
                             ofFloatColor(ofRandom(minColor.r, maxColor.r),
                                          ofRandom(minColor.g, maxColor.g),
                                          ofRandom(minColor.b, maxColor.b),
                                          ofRandom(minColor.a, maxColor.a)),
                             getIndex(),
                             ofVec3f(ofRandom(5,30)));
            break;
        case CP_ROTATE:
            for (int i = 0; i < points.size(); i++) {
                float radius = width/4 * mags[i].x/ 100;
                distPoints[i] = ofVec3f(
                                        screen.x + width/2 + radius*cos(ofGetElapsedTimef()+i),
                                        screen.y + height/4,
                                        radius*sin(ofGetElapsedTimef()+i));
            }
            if (addFlg) _add(initPos,
                             ofFloatColor(ofRandom(minColor.r, maxColor.r),
                                          ofRandom(minColor.g, maxColor.g),
                                          ofRandom(minColor.b, maxColor.b),
                                          ofRandom(minColor.a, maxColor.a)),
                             getIndex(),
                             ofVec3f(ofRandom(5,30)));
            break;
        case CP_ROTATE_HORIZONTAL:
            for (int i = 0; i < points.size(); i++) {
                //float dist = ofDist(screen.x + width/2, screen.y + height/2, points[i].x, points[i].y);
                float radius = width/4 * mags[i].x/100;
                distPoints[i] = ofVec3f(
                                        (screen.x + width/2) + radius*cos(ofGetElapsedTimef()+i),
                                        (screen.y + height/2) + radius*sin(ofGetElapsedTimef()+i),
                                        points[i].z);
                
            }
            if (addFlg) _add(initPos, ofFloatColor(ofRandom(minColor.r, maxColor.r),
                                                   ofRandom(minColor.g, maxColor.g),
                                                   ofRandom(minColor.b, maxColor.b),
                                                   ofRandom(minColor.a, maxColor.a)), getIndex(), ofVec3f(ofRandom(5,30)));
            break;
        case CP_GATHER:
            for (int i = 0; i < points.size(); i++) {
                float distX = ofRandom(initPos.x-10, initPos.x+10);
                float distY = ofRandom(initPos.y-10, initPos.y+10);
                float distZ = ofRandom(initPos.z-10, initPos.z+10);;
                distPoints[i] = ofVec3f(distX, distY, distZ);
                
            }
            if (addFlg) {
                for (int j = 0; j < 10; j++) {
                    _add(initPos, ofFloatColor(ofRandom(minColor.r, maxColor.r),
                                               ofRandom(minColor.g, maxColor.g),
                                               ofRandom(minColor.b, maxColor.b),
                                               ofRandom(minColor.a, maxColor.a)), getIndex(), ofVec3f(ofRandom(5,30)));
                }
            }
            break;
        case CP_COME:
            for (int i = 0; i < points.size(); i++) {
                float dist = ofDist(screen.x + width/2, screen.y + height/2, points[i].x, points[i].y);
                distPoints[i] = ofVec3f(
                                        (screen.x + width/2) + dist*cos(ofGetElapsedTimef()+i),
                                        (screen.y + height/2) + dist*sin(ofGetElapsedTimef()+i),
                                        points[i].z + 300);
                
            }
            if (addFlg) {
                for (int j = 0; j < 3; j++) {
                    _add(
                         ofVec3f(
                                 ofRandom(screen.x, screen.x + width),
                                 ofRandom(screen.y, screen.y + height),
                                 -700
                                 ),
                         ofFloatColor(ofRandom(minColor.r, maxColor.r),
                                      ofRandom(minColor.g, maxColor.g),
                                      ofRandom(minColor.b, maxColor.b),
                                      ofRandom(minColor.a, maxColor.a)),
                         getIndex(),
                         ofVec3f(ofRandom(5,30)));
                }
            }
            break;
        case CP_COME_CONSTANT:
            if (addFlg) {
                for (int j = 0; j < 10; j++) {
                    _add(
                         ofVec3f(
                                 ofRandom(screen.x, screen.x + width),
                                 ofRandom(screen.y, screen.y + height),
                                 -700
                                 ),
                         ofFloatColor(ofRandom(minColor.r, maxColor.r),
                                      ofRandom(minColor.g, maxColor.g),
                                      ofRandom(minColor.b, maxColor.b),
                                      ofRandom(minColor.a, maxColor.a)),
                         getIndex(),
                         ofVec3f(ofRandom(5,30)));
                }
            }
            break;
        case CP_SPREAD:
            if (addFlg) {
                for (int j = 0; j < 10; j++) {
                    _add(
                         ofVec3f(
                                 ofRandom(screen.x, screen.x + width),
                                 ofRandom(screen.y, screen.y + height),
                                 -700
                                 ),
                         ofFloatColor(ofRandom(minColor.r, maxColor.r),
                                      ofRandom(minColor.g, maxColor.g),
                                      ofRandom(minColor.b, maxColor.b),
                                      ofRandom(minColor.a, maxColor.a)),
                         getIndex(),
                         ofVec3f(ofRandom(5,30)));
                }
            }
            break;
        case CP_BIT_NOISE:
            if (noiseFlg) bitNoiseFlg = !bitNoiseFlg;
            break;
        default:
            break;
    }
    
    
}
