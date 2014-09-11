//
//  createPrimitive.cpp
//  visualBand
//
//  Created by 川口大志 on 2014/08/03.
//
//

#include "createPrimitive.h"

void createPrimitive::init(int w, int h, const ofVec2f &sc, const ofVec3f &v, DrawType t, ActionType a) {
    width   = w;
    height  = h;
    dType   = t;
    aType   = a;
    screen  = sc;
    initPos = v;
    updateAddFlg = false;
    addFlg  = true;
    noiseFlg = false;
    particleFlg = false;
    
    setColorType(CP_MONO);
    mags.clear();
    ofDisableArbTex();
	ofLoadImage(texture, "dot.png");
#ifdef TARGET_OPENGLES
    shader.load("shaders_gles/shader");
#else
    shader.load("shaders/shader");
#endif
}

void createPrimitive::draw() {
    if(!points.size()) return;
    switch (dType) {
        case CP_POINT:
            glDepthMask(GL_FALSE);
            ofEnableAlphaBlending();
            ofEnableBlendMode(OF_BLENDMODE_ALPHA);
            ofEnablePointSprites();
            shader.begin();
            texture.bind();
            vbo.draw(GL_POINTS, 0, (int)points.size());
            texture.unbind();
            
            shader.end();
            ofDisablePointSprites();
            glDepthMask(GL_TRUE);
            
            break;
        case CP_LINE:
            //ofSetLineWidth(mags[0].x/10);
            ofSetLineWidth(20);
            vbo.drawElements(GL_LINE_STRIP, points.size());
            break;
        case CP_RECT:
            ofNoFill();
            for (int i = 0; i < points.size(); i++) {
                ofPushMatrix();
                ofTranslate(points[i].x-mags[i].x/2, points[i].y-mags[i].x/4);
                ofRotate(-angle, 0, 0, 1);
                //ofRect(0, 0, mags[i].x, mags[i].x/2);
                ofDrawBox(0, 0, 0, mags[i].x, mags[i].x, mags[i].x);
                ofPopMatrix();
            }
            break;
        case CP_RECT_ROUNDED:
            for (int i = 0; i < points.size(); i++) {
                ofRectRounded(points[i].x-mags[i].x/2, points[i].y-mags[i].x/4, mags[i].x, mags[i].x/2, mags[i].x/4);
            }
            break;
        case CP_ELLIPPSE:
            for (int i = 0; i < points.size(); i++) {
                ofEllipse(points[i].x, points[i].y, mags[i].x, mags[i].x/2);
                //ofRect(points[i].x-mags[i].x/2, points[i].y-mags[i].x/2, mags[i].x, mags[i].x);
            }
            break;
        case CP_LINE_IND:
            ofSetLineWidth(10);
            for (int i = 0; i < points.size(); i++) {
                ofLine(points[i].x, points[i].y, points[i].x, points[i].y+mags[i].x);
            }
            break;
        case CP_SPHERE:
            for (int i = 0; i < points.size(); i++) {
                ofSetColor(colors[i].r*255, colors[i].g*255, colors[i].b*255);
                ofDrawSphere(points[i].x, points[i].y, points[i].z, mags[i].x);
            }
        
            if (particleFlg) {
                for (int i = 0; i < mags.size(); i++) {
                    mags[i] *= 10.0;
                }
                vbo.setNormalData(&mags[0], mags.size(), GL_DYNAMIC_DRAW);
                
                glDepthMask(GL_FALSE);
                ofEnableAlphaBlending();
                ofEnableBlendMode(OF_BLENDMODE_ALPHA);
                ofEnablePointSprites();
                shader.begin();
                texture.bind();
                vbo.draw(GL_POINTS, 0, (int)points.size());
                texture.unbind();
                
                shader.end();
                ofDisablePointSprites();
                glDepthMask(GL_TRUE);

            }
            break;
        case CP_TRIANGLE:
            vbo.drawElements(GL_TRIANGLE_STRIP, points.size());
            break;
        case CP_TRIANGLE_IND:
            angle+=5;
            ofNoFill();
            ofSetLineWidth(10);
            for (int i = 0; i < points.size(); i++) {
                ofPushMatrix();
                ofTranslate(points[i].x, points[i].y);
                ofRotate(angle, 0, 0, 1);
                
                ofTriangle(0,
                           -mags[i].x,
                           mags[i].x/-2,
                           mags[i].x/2*sqrt(3),
                           mags[i].x/2,
                           mags[i].x/2*sqrt(3));
                 
                //ofDrawCone(0, mags[i].x/-2,points[i].z, mags[i].x/2, mags[i].x);
                ofPopMatrix();
            }
            break;
        case CP_CIRCLE:
            for (int i = 0; i < points.size(); i++) {
                ofSetColor(colors[i].r*255, colors[i].g*255, colors[i].b*255);
                ofCircle(points[i].x, points[i].y, points[i].z, mags[i].x/2);
            }
        default:
            break;
    }
}
