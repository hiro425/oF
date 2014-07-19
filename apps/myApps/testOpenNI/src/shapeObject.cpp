//
//  shapeObject.cpp
//  testOpenNI
//
//  Created by 川口大志 on 2014/07/09.
//
//

#include "shapeObject.h"

shapeObject::shapeObject() {
    pos   = ofPoint(ofGetWidth()/2, ofGetHeight()/2);
    dim   = 80.0;
    speed = ofPoint(0,0);
    phaseSpeed = ofRandom(0.1,0.5);
    phase = 0;
    canSeparate= false;
    createTime = ofGetElapsedTimef();
}

shapeObject::shapeObject(string MODE) {
    init();
}

void shapeObject::init() {
    pos   = ofPoint(ofRandom(0.0, ofGetWidth()), -ofGetHeight()/2);
    dim   = ofRandom(50, 100);
    speed = ofPoint(ofRandom(-10, 10), ofRandom(-10, 10));
    gravity = 1.00;
    friction = 0.999;
    
    phaseSpeed = ofRandom(0.1,0.5);
    phase = 0;
    
    canSeparate= false;
    createTime = ofGetElapsedTimef();
}

void shapeObject::_init() {
    
    poses.push_back(ofVec3f(ofRandom(0.0, ofGetWidth()), -50, 0));
    //poses.push_back(ofVec3f(ofRandom(0.0, ofGetWidth()), ofRandom(0,ofGetHeight()), 0));
    dims.push_back(ofVec3f(ofRandom(25, 50)));
    speeds.push_back(ofPoint(ofRandom(-1, 1), ofRandom(-10, 1)));
    colors.push_back(ofFloatColor(255,255,255,255));
    decayFlag.push_back(false);
    gravity = 0.01;
    friction = 0.999;
    
    phaseSpeeds.push_back(ofRandom(0.1,0.5));
    phases.push_back(0);
    
    canSeparate= false;
    createTime = ofGetElapsedTimef();
}

void shapeObject::setup(string filename, int num) {
	
	ofBackgroundHex(0x000000);
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	
	// load the texure
	ofDisableArbTex();
	ofLoadImage(texture, filename);
		
	// randomly add a point on a sphere
	int   _num = num;
	for(int i = 0; i<_num; i++ ) {
        _init();
        
	}
	
	// upload the data to the vbo
	int total = (int)poses.size();
	vbo.setVertexData(&poses[0], total, GL_DYNAMIC_DRAW);
	vbo.setNormalData(&dims[0], total, GL_DYNAMIC_DRAW);
    vbo.setColorData(&colors[0], total, GL_DYNAMIC_DRAW);
	
	
	// load the shader
#ifdef TARGET_OPENGLES
    shader.load("shaders_gles/shader");
#else
    shader.load("shaders/shader");
#endif
    
    _setupLamp();
}

void shapeObject::_setupLamp() {

	// load the texure
	ofDisableArbTex();
	//ofLoadImage(lampTexture, "texture/light11.png");
    ofLoadImage(lampTexture, "image.jpg");
    
}
void shapeObject::_addLamp(ofVec3f p, ofVec3f d) {
    lampPoses.push_back(p);
    lampDims.push_back(d);
    lampDimsOrig.push_back(d);
    lampSpeeds.push_back(ofVec3f(ofRandom(-1, 1),ofRandom(-1,1), ofRandom(-1, 1)));
    lampColors.push_back(ofFloatColor(196, 150, 3));
    createFlag.push_back(true);
}

void shapeObject::draw() {
    ofSetColor(33, 63, 255, 100);
    ofCircle(pos.x, pos.y, moveDim);
    
    ofSetColor(255,0,0,200);
    ofCircle(pos.x, pos.y, dim/10);
    
    ofSetColor(33, 63, 255);
}

void shapeObject::draw(float vol) {
    ofSetColor(33, 63, 255, 100);
    float moveRad = dim/10 + dim*vol*10;
    ofCircle(pos.x, pos.y, moveRad);
    
    ofSetColor(33, 63,255, 200);
    ofCircle(pos.x, pos.y, dim/10);
    
    ofSetColor(33, 63, 255);
    
	glDepthMask(GL_FALSE);
	
	ofSetColor(255, 100, 90);
    
}

void shapeObject::_draw(float vol) {
	
    glDepthMask(GL_FALSE);
    
	// this makes everything look glowy :)
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofEnablePointSprites();
	
	// bind the shader and camera
	// everything inside this function
	// will be effected by the shader/camera
	shader.begin();
	
	// bind the texture so that when all the points
	// are drawn they are replace with our dot image
	texture.bind();
	vbo.draw(GL_POINTS, 0, (int)poses.size());
	texture.unbind();
    
    ofSetColor(196, 150, 3);
    lampTexture.bind();
    lamps.draw(GL_POINTS, 0, (int)lampPoses.size());
    lampTexture.unbind();
	

    
	shader.end();
	
	ofDisablePointSprites();
	ofDisableBlendMode();
    
    glDepthMask(GL_TRUE);
    
    
    
	
}

//--------------------------------------------------------------
void shapeObject::addPoint(float x, float y, float z) {
	ofVec3f p(x, y, z);
	points.push_back(p);
	
	// we are passing the size in as a normal x position
	float size = ofRandom(5, 50);
	sizes.push_back(ofVec3f(size));
}

void shapeObject::update() {
    
    moveDim = dim + sin(phase) * dim/4;
    phase += phaseSpeed;
    if (phase > TWO_PI) {
        phase -= TWO_PI;
    }
    
    pos += speed;
    
    if (pos.x > ofGetWidth() - dim || pos.x < dim) {
        speed.x *= -1;
    }
    
    if (pos.y > ofGetHeight() - dim || pos.y < dim) {
        speed.y *= -1;
    }
    
    //cout << "createTIme : " << createTime << endl;
    
    if ( ofGetElapsedTimef() - createTime > 1 ) {
        canSeparate = true;
    }
    
}

void shapeObject::update(string MODE) {
    speed *= friction;
    speed.y += gravity;
    
    pos.x += speed.x;
    pos.y += speed.y;
    
    if (pos.y > ofGetHeight()) {
        init();
    }
    
    if (pos.x < 0) {
        pos.x = ofGetWidth();
    }
    if (pos.x > ofGetWidth()) {
        pos.x = 1;
    }
}

void shapeObject::_update(float vol) {
    for (int i = 0; i < poses.size(); i++) {
        speeds[i] *= friction;
        speeds[i].y += gravity;
        poses[i].x += speeds[i].x;
        poses[i].y += speeds[i].y;
        
        if (poses[i].y > ofGetHeight()) {
            poses[i]  = ofVec3f(ofRandom(0.0, ofGetWidth()), -50, 0);
           dims.push_back(ofVec3f(ofRandom(25, 50)));
            speeds[i] = ofPoint(ofRandom(-1, 1), ofRandom(-10, 1));
            colors[i] = ofFloatColor(255,255,255,255);
        }
        
        if (poses[i].x < 0) {
            poses[i].x = ofGetWidth();
        }
        if (poses[i].x > ofGetWidth()) {
            poses[i].x = 1;
        }
        
        if (decayFlag[i] == true) {
            colors[i].a *= 0.5;
            cout << "colors.a : " << colors[i].a << endl;
            if (colors[i].a < 0.1) {
                colors[i].a = 0;
                _addLamp(poses[i], dims[i]);
                decayFlag[i] = false;
            }

        }
    }

    for (int i = 0; i < lampPoses.size(); i++) {
        /*lampSpeeds[i] *= friction;
        lampSpeeds[i].y += gravity;*/
        lampPoses[i].x += lampSpeeds[i].x;
        lampPoses[i].y += lampSpeeds[i].y;
        //lampPoses[i].z += lampSpeeds[i].z;
        
        if (lampPoses[i].x < 0) {
            lampPoses[i].x = ofGetWidth();
        }
        if (lampPoses[i].x > ofGetWidth()) {
            lampPoses[i].x = 1;
        }
        
        lampDims[i] = lampDimsOrig[i] + lampDimsOrig[i] * vol* 30;
        /*if (createFlag[i] == true) {
            lampColors[i].a *= 5;
        }
        
        if (lampColors[i].a >= 255 ) {
            createFlag[i] = false;
        }*/
        
    }
    
    int total = (int)poses.size();
	vbo.setVertexData(&poses[0], total, GL_DYNAMIC_DRAW);
	vbo.setNormalData(&dims[0], total, GL_DYNAMIC_DRAW);
    vbo.setColorData(&colors[0], total, GL_DYNAMIC_DRAW);
    
    total = (int)lampPoses.size();
	lamps.setVertexData(&lampPoses[0], total, GL_DYNAMIC_DRAW);
	lamps.setNormalData(&lampDims[0], total, GL_DYNAMIC_DRAW);
    //lamps.setColorData(&lampColors[0], total, GL_DYNAMIC_DRAW);
}

bool shapeObject::_touchEvent(ofPoint handPos) {
    for (int i = 0; i<poses.size(); i++) {
        float distDim    = ofDist(poses[i].x, poses[i].y, handPos.x, handPos.y);
        
        if (distDim - dims[i].length()/2 < 0  && !decayFlag[i]) {
            //_addLamp(poses[i], dims[i]);
            /*
            poses[i]  = ofVec3f(ofRandom(0.0, ofGetWidth()), -ofGetHeight()/2, 0);
            dims[i]   = ofVec3f(ofRandom(50, 100));
            speeds[i] = ofPoint(ofRandom(-10, 10), ofRandom(-10, 10));
            */
             decayFlag[i] = true;
            
        }
        
    }
    
}

void shapeObject::setPos(ofPoint _pos) {
    pos = _pos;
}

ofPoint shapeObject::getPos() {
    return pos;
}

void shapeObject::setDim(float _dim) {
    dim = _dim;
}

float shapeObject::getDim() {
    return dim;
}

void shapeObject::setSpeed(ofPoint _speed) {
    speed = _speed;
}

ofPoint shapeObject::getSpeed() {
    return speed;
}