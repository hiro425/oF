//
//  createAction.cpp
//  visualBand_fork
//
//  Created by 川口大志 on 2014/09/11.
//
//

#include "createAction.h"



void randomizeXYZ(property &p, ofVec3f minP, ofVec3f maxP) {
    for (int i = 0; i < p.points.size(); i++) {
        p.distPoints[i] = ofVec3f(
                                  ofRandom(minP.x, maxP.x),
                                  ofRandom(minP.y, maxP.y),
                                  ofRandom(minP.z, maxP.z));
    }
}

void rotationXZ(property &p, float radius) {
    for (int i = 0; i < p.points.size(); i++) {
        p.distPoints[i] = ofVec3f(
                                  p.screen.x + p.div.x/2 + radius*cos(ofGetElapsedTimef()+i),
                                  p.screen.y + p.div.y/4,
                                  radius*sin(ofGetElapsedTimef()+i));
    }
}

void rotationXY(property &p, float radius) {
    for (int i = 0; i < p.points.size(); i++) {
        float radius = p.div.x/4 * p.mags[i].x/100;
        p.distPoints[i] = ofVec3f(
                                  (p.screen.x + p.div.x/2) + radius*cos(ofGetElapsedTimef()+i),
                                  (p.screen.y + p.div.y/2) + radius*sin(ofGetElapsedTimef()+i),
                                  p.points[i].z);
    }
}

void gathering(property &p, ofVec3f pos) {
    for (int i = 0; i < p.points.size(); i++) {
        float distX = ofRandom(pos.x-10, pos.x+10);
        float distY = ofRandom(pos.y-10, pos.y+10);
        float distZ = ofRandom(pos.z-10, pos.z+10);;
        p.distPoints[i] = ofVec3f(distX, distY, distZ);
        
    }
}

void comingZ(property &p) {
    for (int i = 0; i < p.points.size(); i++) {
        float dist = ofDist(p.screen.x + p.div.x/4, p.screen.y + p.div.y/4, p.points[i].x, p.points[i].y);
        p.distPoints[i] = ofVec3f(
                                  (p.screen.x + p.div.x/2) + dist*cos(ofGetElapsedTimef()+i),
                                  (p.screen.y + p.div.y/2) + dist*sin(ofGetElapsedTimef()+i),
                                  p.points[i].z + 10);
        
    }
}

// ------------------------------------------------------

void createAction::setup(property &p) {
    prop = p;

}

void createAction::update(property &p) {
    prop = p;
    if (!p.points.size()) return;
    switch (p.aType) {
        case CP_COME_CONSTANT:
            for (int i = 0; i < p.points.size(); i++) {
                float dist = ofDist(p.screen.x + p.div.x/2, p.screen.y + p.div.y/2, p.points[i].x, p.points[i].y);
                p.distPoints[i] = ofVec3f(
                                        (p.screen.x + p.div.x/2) + dist*cos(ofGetElapsedTimef()+i),
                                        (p.screen.y + p.div.y/2) + dist*sin(ofGetElapsedTimef()+i),
                                        p.points[i].z + 200);
            }
            break;
        case CP_FLOW_X:
            for (int i = 0; i < p.points.size(); i++) {
                p.distPoints[i] = ofVec3f(
                                        p.points[i].x - 10,
                                        p.points[i].y,
                                        p.points[i].z);
            }            
            break;
        case CP_FLOW_X_ROTATE:
            for (int i = 0; i < p.points.size(); i++) {
                p.distPoints[i] = ofVec3f(
                                        p.points[i].x - 10,
                                        p.points[i].y,
                                        p.points[i].z);
            }
            p.angle = p.angle + 0.2*p.mags[0].x;
            break;
        case CP_FLOW_X_SLIDE:
            for (int i = 0; i < p.points.size(); i++) {
                p.distPoints[i] = ofVec3f(
                                          p.points[i].x - 10,
                                          p.points[i].y,
                                          p.points[i].z);
            }
            p.angle = p.angle + 0.2*p.mags[0].x;
            break;
        case CP_FLOW_X_SIN:
            for (int i = 0; i < p.points.size(); i++) {
                p.distPoints[i] = ofVec3f(
                                        p.points[i].x - 10,
                                        p.points[i].y,
                                        p.points[i].z);
            }
            p.angle += 5;
            break;
        case CP_ROTATE:
            for (int i = 0; i < p.points.size(); i++) {
                float radius = p.div.x/4 * p.mags[i].x/30;
                p.distPoints[i] = ofVec3f(
                                        p.screen.x + p.div.x/2 + radius*cos(ofGetElapsedTimef()+i),
                                        p.screen.y + p.div.y/4,
                                        radius*sin(ofGetElapsedTimef()+i));
            }
            break;
        case CP_ROTATE_HORIZONTAL:
            for (int i = 0; i < p.points.size(); i++) {
                //float dist = ofDist(screen.x + p.div.x/2, screen.y + p.div.y/2, points[i].x, points[i].y);
                float radius = p.div.x/4 * p.mags[i].x/30;
                p.distPoints[i] = ofVec3f(
                                        (p.screen.x + p.div.x/2) + radius*cos(ofGetElapsedTimef()+i),
                                        (p.screen.y + p.div.y/2) + radius*sin(ofGetElapsedTimef()+i),
                                        p.points[i].z);
            }
            break;
        case CP_SPREAD:
            for (int i = 0; i < p.points.size(); i++) {
                float dist = ofDist(p.screen.x + p.div.x/2, 0, p.points[i].x, p.points[i].z);
                p.distPoints[i] = ofVec3f(
                                        (p.screen.x + p.div.x/2) + (dist + 700)*cos(ofGetElapsedTimef()+i),
                                        p.points[i].y,
                                        (dist + 700)*sin(ofGetElapsedTimef()+i));
            }
            break;
        case CP_SPREAD_BY_ONE:
     
             for (int i = 0; i < p.points.size(); i++) {
                 float dist = ofDist(p.screen.x + p.div.x/2, 0, p.points[i].x, p.points[i].z);
                 p.distPoints[i] = ofVec3f(
                                           (p.screen.x + p.div.x/2) + (dist + 700)*cos(ofGetElapsedTimef()+i),
                                           p.points[i].y,
                                           (dist + 700)*sin(ofGetElapsedTimef()+i));
             }
     
            break;
        default:
            break;
    }
    
    for (int i = 0; i < p.points.size(); i++) {
        ofVec3f speed = (p.distPoints[i] - p.points[i]) * 0.3;
        p.points[i] += speed;
        
    }
    p.vbo.setVertexData(&p.points[0], p.points.size(), GL_DYNAMIC_DRAW);
    p.vbo.setNormalData(&p.mags[0], p.mags.size(), GL_DYNAMIC_DRAW);
    p.vbo.setColorData(&p.colors[0], p.colors.size(), GL_DYNAMIC_DRAW);
    p.vbo.setIndexData(&p.faces[0], p.faces.size(), GL_DYNAMIC_DRAW);
    
}
void createAction::callback(property &p) {
    if (!p.points.size()) return;
    switch (p.aType) {
            
        case CP_SLIDE:
            randomizeXYZ(p, ofVec3f(0, p.screen.y, -700), ofVec3f(p.screen.x+p.div.x, p.screen.y+p.div.y, 700));
            break;
            
        case CP_ROTATE:
            rotationXZ(p, p.div.x/4 * p.mags[0].x/ 50);
            break;
            
        case CP_ROTATE_HORIZONTAL:
            rotationXY(p, p.div.x/4 * p.mags[0].x/50);
            break;
            
        case CP_GATHER:
            gathering(p, ofVec3f((p.screen.x+p.div.x)/2, (p.screen.y+p.div.y)/2, 0));
            break;
            
        case CP_COME:
            comingZ(p);
            break;
            
        case CP_COME_CONSTANT:
            break;
        case CP_FLOW_X:
            break;
        case CP_FLOW_X_ROTATE:
            break;
        case CP_FLOW_X_SIN:
            break;
        case CP_FLOW_X_SLIDE:
            randomizeXYZ(p, ofVec3f(0, p.screen.y, -700), ofVec3f(p.screen.x+p.div.x, p.screen.y+p.div.y, 700));
            break;
        case CP_SPREAD:
            break;
        case CP_SPREAD_BY_ONE:
            /*
            if (p.points.size()>20) {
                animationFlg = true;
                addFlg = false;
            }
            else if (p.points.size() == 0) {
                animationFlg = false;
                addFlg = true;
            }
            if (animationFlg) {
                bool finishAnimation = true;
                for (int i = 0; i < p.points.size(); i++) {
                    if (p.animated[i] == false) {
                        p.animated[i] = true;
                        finishAnimation = false;
                        float dist = ofDist(screen.x + p.div.x/2, 0, p.points[i].x, p.points[i].z)*2;
                        p.distPoints[i] = ofVec3f(
                                                     (screen.x + p.div.x/2) + (dist + 700)*cos(ofGetElapsedTimef()+i),
                                                     (screen.y + p.div.y/2) + (dist + 700)*sin(ofGetElapsedTimef()+i),//points[i].y,
                                                     p.points[i].z);//(dist + 700)*sin(ofGetElapsedTimef()+i));
                        break;
                    }
                }
                if (finishAnimation) {
                    clear();
                    animationFlg = false;
                    addFlg = true;
                }
            }
            */
            break;
        case CP_BIT_NOISE:
            //if (noiseFlg) bitNoiseFlg = !bitNoiseFlg;
            break;
        default:
            break;
    }
    
    p.vbo.setVertexData(&p.points[0], p.points.size(), GL_DYNAMIC_DRAW);
    p.vbo.setNormalData(&p.mags[0], p.mags.size(), GL_DYNAMIC_DRAW);
    p.vbo.setColorData(&p.colors[0], p.colors.size(), GL_DYNAMIC_DRAW);
    p.vbo.setIndexData(&p.faces[0], p.faces.size(), GL_DYNAMIC_DRAW);
}



