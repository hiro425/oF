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
            ofSetLineWidth(mags[0].x/10);
            vbo.drawElements(GL_LINE_STRIP, points.size());
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
        case CP_CIRCLE:
            for (int i = 0; i < points.size(); i++) {
                ofSetColor(colors[i].r*255, colors[i].g*255, colors[i].b*255);
                ofCircle(points[i].x, points[i].y, points[i].z, mags[i].x);
            }
        default:
            break;
    }
}
