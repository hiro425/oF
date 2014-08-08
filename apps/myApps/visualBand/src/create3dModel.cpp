//
//  create3dModel.cpp
//  visualBand
//
//  Created by 川口大志 on 2014/08/07.
//
//

#include "create3dModel.h"

void create3dModel::init(int w, int h, const ofVec2f &sc, const ofVec3f &v, ActionType a) {
    width   = w;
    height  = h;
    aType   = a;
    screen  = sc;
    initPos = v;
    addFlg  = true;
    noiseFlg = false;
    
    setColorType(CP_MONO);
    mags.clear();
    ofDisableArbTex();
	//model.loadModel("dog/dog.3ds");
    //models.push_back(model);
	//model.setPosition(ofGetWidth()*.5, ofGetHeight() * 0.25, 0);
    ofxAssimpModelLoader *m = new ofxAssimpModelLoader;
    m->loadModel("models/monster-animated-character-X.X");
    models.push_back(m);
    bitNoiseFlg = false;
    
}


void create3dModel::draw() {
    drawWithMesh();
}

void create3dModel::drawWithMesh(){
    for (int i = 0; i < models.size() && i < 10; i++) {

        ofVec3f scale = models[i]->getScale()/2;
        ofVec3f position = points[i]; //model.getPosition();
        models[i]->setPosition(position.x, position.y, position.z);
        float normalizedScale = models[i]->getNormalizedScale();
        ofVboMesh mesh = models[i]->getMesh(0);
        ofTexture texture;
        ofxAssimpMeshHelper& meshHelper = models[i]->getMeshHelper( 0 );
        
        bool bHasTexture = meshHelper.hasTexture();
        if( bHasTexture ) {
            //texture = models[i]->getTextureForMesh(0);
        }
        
        ofMaterial material = models[i]->getMaterialForMesh(0);
        
        ofPushMatrix();
        
        ofTranslate(position);
        ofRotate(90,1,0,0);
        ofRotate(90,0,0,1);
        //ofRotate(90,0,0,1);
        
        // normalizedScale *= mags[0].x / 50;
        ofScale(normalizedScale, normalizedScale, normalizedScale);
        ofScale(scale.x,scale.y,scale.z);
        
        //modify mesh with some noise
        //float liquidness = 5;
        //float amplitude =  500/100.0;
        //float speedDampen = 5;
        vector<ofVec3f>& verts = mesh.getVertices();
        
        float seed = ofRandom(10);
        for(unsigned int j = 0; j < verts.size(); j++){
            //verts[j].x += ofSignedNoise(verts[j].x/liquidness, verts[j].y/liquidness,verts[j].z/liquidness, ofGetElapsedTimef()/speedDampen)*amplitude;
            //verts[j].y += ofSignedNoise(verts[j].z/liquidness, verts[j].x/liquidness,verts[j].y/liquidness, ofGetElapsedTimef()/speedDampen)*amplitude;
            //verts[j].z += ofSignedNoise(verts[j].y/liquidness, verts[j].z/liquidness,verts[j].x/liquidness, ofGetElapsedTimef()/speedDampen)*amplitude;
            
            if (bitNoiseFlg && j%2 ==0) {
                float noise = (ofNoise(seed))*2;
                verts[j].x *= noise;
                verts[j].y *= noise;
                verts[j].z *= noise;
                seed += 0.5;
            }
            /*
            if (bitNoiseFlg && j%6 ==0) {
                verts[j].x = ofRandom(screen.x, screen.x + width);
                verts[j].y = ofRandom(screen.y, screen.y + height);
                verts[j].z = ofRandom(screen.x + width, screen.x + width);
            }
             */
        }
        
        
        //draw the model manually
        if(bHasTexture) texture.bind();
        material.begin();
        //mesh.drawWireframe(); //you can draw wireframe too
        mesh.drawFaces();
        material.end();
        if(bHasTexture) texture.unbind();
        
        ofPopMatrix();
        
    }
    
}