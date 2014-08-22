#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    int bufferSize = 256;
    //soundStream.setDeviceID(6);
    soundStream.setup(this, 0, 1, 44100, bufferSize, 4);
    //left.assign(bufferSize, 0.0);
    //soundStream.listDevices();
    ch1.assign(bufferSize/2, 0.0);
    ch2.assign(bufferSize/2, 0.0);
    
    //ofSetVerticalSync(true);
    //ofSetFrameRate(60);
    ofEnableDepthTest();

    ofSetSphereResolution(64);
    
    dMode = VB_PRIMITIVE;
    
    createPrimitive prim1, prim2, prim3, prim4;
    prim1.init(ofGetWidth()/2, ofGetHeight(), ofVec2f(ofGetWidth()/4,0), ofVec3f(ofGetWidth()/2, ofGetHeight()/2, 0),   CP_SPHERE, CP_GATHER);
    prim2.init(ofGetWidth()/2, ofGetHeight(), ofVec2f(0,0),              ofVec3f(ofGetWidth()/4, ofGetHeight()/2, 0),   CP_NO_DRAW, CP_GATHER);
    prim3.init(ofGetWidth()/2, ofGetHeight(), ofVec2f(ofGetWidth()/2,0), ofVec3f(ofGetWidth()/4*3, ofGetHeight()/2, 0), CP_NO_DRAW, CP_GATHER);
    primitives.push_back(prim1);
    primitives.push_back(prim2);
    primitives.push_back(prim3);
    
    prim4.init(ofGetWidth()*3, ofGetHeight()*3, ofVec2f(-ofGetWidth(),-ofGetHeight()), ofVec3f(ofGetWidth()/2, ofGetHeight()/2, 0),   CP_POINT, CP_NO_ACTION);
    for (int i = 0; i < 200; i++) {
        prim4.addRandom();
    }
    primitives.push_back(prim4);

    for (int i = 0; i < primitives.size(); i++) {
        primitives[i].add();
        bandwidthData primD;
        primD.thresh = 0.25;
        primD.c = &primitives[i];
        primD.vol = 0.0;
        primsD.push_back(primD);
    }

    create3dModel model1, model2, model3;
    model1.init( ofGetWidth()/2,ofGetHeight(), ofVec2f(ofGetWidth()/4,0), ofVec3f(ofGetWidth()/2, ofGetHeight()/2, 0), CP_BIT_NOISE);
    model2.init( ofGetWidth()/2,ofGetHeight(), ofVec2f(0,0),              ofVec3f(ofGetWidth()/4, ofGetHeight()/2, 0), CP_BIT_NOISE);
    model3.init( ofGetWidth()/2,ofGetHeight(), ofVec2f(ofGetWidth()/2,0), ofVec3f(ofGetWidth()/4*3, ofGetHeight()/2, 0), CP_BIT_NOISE);
    models.push_back(model1);
    //models.push_back(model2);
    //models.push_back(model3);
    for (int i = 0; i < models.size(); i++) {
        models[i].add();
        bandwidthData modelD;
        modelD.thresh = 0.25;
        modelD.c = &models[i];
        modelD.vol = 0.0;
        modelsD.push_back(modelD);
    }
    
    ofBackgroundHex(0x000000);
    camAngle   = 90.0;
    rotateMode = false;
    decay = 3.0;
    camRadius = ofGetWidth()/4*3;
    cam.resetTransform();
	cam.setFov(60);
	cam.clearParent();
    camPos = ofVec3f(ofGetWidth()/2, ofGetHeight()/2, camRadius);
    camDistPos = camPos;
    cam.setPosition(camPos);
    
    diffuseColor = ofColor(255.f, 255.f, 255.f);
    specularColor = ofColor(255.f, 255.f, 255.f);
    
    pointLight.setDiffuseColor( diffuseColor );
	pointLight.setSpecularColor( specularColor );
	pointLight.setPointLight();
    pointLight.setPosition(ofGetWidth(), ofGetHeight(), 1080);
    darkFlg = false;
    
    post.init(ofGetWidth(), ofGetHeight());
    post.createPass<DofAltPass>()->setEnabled(true);           //0
    post.createPass<NoiseWarpPass>()->setEnabled(false);
    post.createPass<EdgePass>()->setEnabled(false);
    post.createPass<PixelatePass>()->setEnabled(false);
    post.createPass<RGBShiftPass>()->setEnabled(false);
    post.createPass<ZoomBlurPass>()->setEnabled(false);
    post.createPass<RimHighlightingPass>()->setEnabled(false);
    
    warpFlg = false;
    flashFlg = true;
    forceBang = false;
    
    backImage.loadImage("dot.png");
    
}


//--------------------------------------------------------------
void ofApp::update(){
    
    for (int i = 0; i < primitives.size(); i++) {
        primitives[i].updateMags(10 * primsD[i].vol);
        checkBang(primsD[i]);
        primitives[i].update();
    }
    primitives[3].updateMags(1);
    primitives[3].update();
    
    for (int i = 0; i < models.size(); i++) {
        models[i].updateMags(10 * modelsD[i].vol);
        checkBang(modelsD[i]);
        models[i].update();
    }

    if (rotateMode) {
        
        float dist = ofDist(camDistPos.x, camDistPos.y, camPos.x,camPos.y);
        if (primsD[0].bang && dist < 50) {
            float posx, posy, posz;
            camAngle = ofRandom(0, 360);
            decay = ofRandom(0.05, 0.3);
            posx = ofGetWidth()/2 + camRadius*cos(camAngle * PI /180);
            posy = ofGetHeight()/2 + camRadius*cos(camAngle * PI /180);
            posz = camRadius*sin(camAngle * PI /180);
            camDistPos = ofVec3f(posx, posy, posz);
        }
        ofVec3f speed = (camDistPos - camPos) * decay;
        camPos += speed;
        cam.setPosition(camPos);
    }
    cam.setPosition(camPos);
    cam.lookAt(ofVec3f(ofGetWidth()/2, ofGetHeight()/2, 0));
    
    
    if (darkFlg) {
        if (primsD[0].bang) {
            darkFlg = true;
            diffuseColor = ofColor(255,255,255);
            specularColor = ofColor(255,255,255);
            pointLight.setDiffuseColor(diffuseColor);
            pointLight.setSpecularColor(specularColor);
        }
        else {
            diffuseColor = ofColor(175,175,175);
            specularColor = ofColor(175,175,175);
            pointLight.setDiffuseColor(diffuseColor);
            pointLight.setSpecularColor(specularColor);
        }
    }
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

void ofApp::checkBang(bandwidthData &d) {
    if((d.bang == false && d.vol > d.thresh ) || forceBang) {
        d.bang = true;
        forceBang = false;
        d.c->callback();
    }
    if(d.vol < d.thresh){
        d.bang = false;
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    //cam.begin();
    
    post.begin(cam);
    ofEnableLighting();
    ofEnableAlphaBlending();
    //ofEnableBlendMode(OF_BLENDMODE_ADD);
    pointLight.enable();
    
    //spheres.draw();
    //triangles.draw();
    
    if (dMode == VB_PRIMITIVE) {
        for (int i = 0; i < primitives.size(); i++) {
            primitives[i].draw();
        }
    }
    else if (dMode == VB_MODEL) {
        for (int i = 0; i < models.size(); i++) {
            models[i].draw();
        }
    }
    
    ofPushMatrix();
    ofSetLineWidth(0.1);
    ofNoFill();
    ofRotate(80, 1, 0, 0);
    
    float posx = 1000, posy = 500,posz = -1, radius = 1;
    float seed = ofRandom(10);
    for (int i = 0; i < 100; i++) {
        float noise = ofNoise(seed) * 5; //* primsD[0].vol*100;
        //float noise = (1-pow(ofRandom(1), 5)) * primsD[0].vol*50;
        
        ofSetColor(255, 255, 255, (int)noise*20);
        //ofCircle(posx, posy, posz-i*noise, radius *noise*20);
        seed += 0.5;
    }
    ofPopMatrix();
    
    ofPushMatrix();
    ofSetColor(primitives[0].getColor(), 127);
    backImage.setAnchorPercent(0.5, 0.5);
    backImage.draw(ofGetWidth()/2, ofGetHeight()/2, -3000, ofGetWidth()*8, ofGetHeight()*4);
    backImage.draw(ofGetWidth(), ofGetHeight(), 3000, ofGetWidth()*4, ofGetHeight()*4);
    ofPopMatrix();
    
    pointLight.disable();
    ofDisableLighting();
    if (flashFlg) {
        flashFlg = false;
        ofPushMatrix();
        ofFill();
        ofSetColor(primitives[0].getColor(), 255);
        ofDrawPlane(ofGetWidth()/2, ofGetHeight()/2, camRadius-10, ofGetWidth(), ofGetHeight());
        ofPopMatrix();
    }
    
    ofDisableBlendMode();
    post.end();

}


//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
	
	float curVolCh1 = 0.0;
    float curVolCh2 = 0.0;
    
	int   numCountedCh1 = 0;
    int   numCountedCh2 = 0;
    
	for (int i = 0; i < bufferSize; i++){
        if (i%2 == 0) {
            ch1[i/2]   = input[i] * 5;
            curVolCh1 += ch1[i/2] * ch1[i/2];
            numCountedCh1++;
            
        }
        else if (i%2 == 1) {
            ch2[(i-1)/2] = input[i] * 5;
            curVolCh2   += ch2[(i-1)/2] * ch2[(i-1)/2];
            numCountedCh2++;
        }
	}
	
	curVolCh1 /= (float)numCountedCh1;
	curVolCh1  = sqrt( curVolCh1 );
	curVolCh2 /= (float)numCountedCh2;
	curVolCh2  = sqrt( curVolCh2 );

    /*
    sphereD.vol *= 0.93;
    sphereD.vol += 0.07 * curVolCh1;
    
    triangleD.vol *= 0.93;
    triangleD.vol += 0.07 * curVolCh2;
    */
    for (int i = 0; i < primsD.size(); i++) {
        primsD[i].vol *= 0.93;
        if (i==0) {
            primsD[i].vol += 0.07 * curVolCh1;
        }
        else {
            primsD[i].vol += 0.07 * curVolCh2;

        }

    }
    for (int i = 0; i < modelsD.size(); i++) {
        modelsD[i].vol *= 0.93;
        modelsD[i].vol += 0.07 * curVolCh1;
        
    }

	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    for (int i = 0; i < primitives.size(); i++) {
        switch (key) {
            case 'a':
                primitives[i].addFlg = !primitives[i].addFlg;
                break;
            case 'b':
                primsD[i].c->callback();
                break;
            case 'c':
                primitives[i].clear();
                break;
            case 'n':
                primitives[i].noiseFlg = !primitives[i].noiseFlg;
                models[0].noiseFlg = !models[0].noiseFlg;
                
                break;
            case 'p':
                primitives[i].setDrawType(CP_POINT);
                break;
            case 'T':
                primitives[i].setDrawType(CP_TRIANGLE);
                primitives[3].setDrawType(CP_POINT);
                break;
            case 'l':
                //primitives[i].setDrawType(CP_LINE);
                break;
            case 's':
                primitives[i].setDrawType(CP_SPHERE);
                break;
            case 'C':
                primitives[i].setActionType(CP_COME_CONSTANT);
                break;
            
            case 'R':
                primitives[i].setActionType(CP_ROTATE);
                break;
            case 'S':
                forceBang = true;
                primitives[i].setActionType(CP_SLIDE);
                break;
            case 'G':
                forceBang = true;
                primitives[i].setActionType(CP_GATHER);
                break;
            case 'H':
                primitives[i].setActionType(CP_ROTATE_HORIZONTAL);
                break;
            case 'q':
                primitives[i].setColorType(CP_MONO);
                break;
            case 'w':
                primitives[i].setColorType(CP_FIRE);
                break;
            case 'e':
                primitives[i].setColorType(CP_SEA);
                break;
            case 'r':
                primitives[i].setColorType(CP_FOREST);
                break;
            case 't':
                primitives[i].setColorType(CP_RANDOM);
                break;
        }
    }
    primitives[3].noiseFlg = false;
    primitives[3].setActionType(CP_NO_ACTION);
    float r,g,b;
    switch (key) {
        case ' ':
            rotateMode = !rotateMode;
            if (!rotateMode) camPos = ofVec3f(ofGetWidth()/2, ofGetHeight()/2, camRadius);
            break;
        case '1':
            primitives[0].setDrawType(CP_SPHERE);
            primitives[1].setDrawType(CP_LINE);
            primitives[2].setDrawType(CP_LINE);
            
            primitives[0].setActionType(CP_GATHER);
            primitives[1].setActionType(CP_GATHER);
            primitives[2].setActionType(CP_GATHER);
            primsD[0].c->callback();
            primsD[1].c->callback();
            primsD[2].c->callback();

            rotateMode = false;
            camPos = ofVec3f(ofGetWidth()/2, ofGetHeight()/2, camRadius);
            break;
        case '2':
            primitives[1].setDrawType(CP_LINE);
            primitives[2].setDrawType(CP_LINE);
            primitives[0].particleFlg = false;
            primitives[1].particleFlg = false;
            primitives[2].particleFlg = false;
            primitives[1].setActionType(CP_SPREAD);
            primitives[2].setActionType(CP_SPREAD);
            rotateMode = false;
            camPos = ofVec3f(ofGetWidth()/2, ofGetHeight()/2, camRadius);
            break;
        case '3':
            primitives[1].setDrawType(CP_TRIANGLE);
            primitives[2].setDrawType(CP_TRIANGLE);
            break;
        case '4':
            dMode = VB_PRIMITIVE;
            primitives[0].setDrawType(CP_SPHERE);
            primitives[1].setDrawType(CP_SPHERE);
            primitives[2].setDrawType(CP_SPHERE);
            primitives[0].setActionType(CP_SLIDE);
            primitives[1].setActionType(CP_SLIDE);
            primitives[2].setActionType(CP_SLIDE);
            primsD[0].c->callback();
            primsD[1].c->callback();
            primsD[2].c->callback();
            break;
        case 'E':
            post[2]->setEnabled(!post[2]->getEnabled());
            break;
        case 'f':
            forceBang = true;
            break;
        case 'F':
            flashFlg = true;
            break;
        case 'L':
            darkFlg = !darkFlg;
            break;
        case 'M':
            dMode = VB_MODEL;
            flashFlg = true;
            break;
        case 'N':
            flashFlg = true;
            dMode = VB_PRIMITIVE;
            break;
        case 'P':
            primitives[1].setActionType(CP_SPREAD);
            primitives[2].setActionType(CP_SPREAD);
            break;
        case 'W':
            post[1]->setEnabled(!post[1]->getEnabled());
            break;
        case 'Z':
            post[4]->setEnabled(!post[5]->getEnabled());
            break;
        case '0':
            post[6]->setEnabled(!post[6]->getEnabled());
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
