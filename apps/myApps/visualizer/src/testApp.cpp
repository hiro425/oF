#include "testApp.h"
bool isRand = false;
//--------------------------------------------------------------
void testApp::setup(){
    int bufferSize = 256;
    soundStream.setup(this, 0, 1, 44100, bufferSize, 4);
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    
    left.assign(bufferSize, 0.0);
    
    for(int i = 0; i < objectNum; i++){
        Primitive drawObject;
        drawObject.drawMode = i % 2;
        prims.push_back(drawObject);
    }
    
    ofSetSphereResolution(2);
    
    thresh = 0.2; //ランダマイズのための音量の閾値
    bang           = false;
    bHide          = true;
    bOrbits        = false;
    orientPosRatio = 0.5;
    makeMode       = 0;
    bShader        = true;
    bShaderTog     = true;
    bPrims         = false;
    bPrimsTog      = false;
    
    setupCam();
    setupPanel();
    setupShader();
    
}

void testApp::setupPanel() {
    panel.setup("panel");
    panel.add(pos.set("pos",ofVec3f(0,0,0),ofVec3f(0,0,0),ofVec3f(300,300,300)));
    panel.add(size.set("scale",ofVec3f(50,60,80),ofVec3f(0,0,0),ofVec3f(1000,1000,1000)));
    panel.add(rotate.set("rotate",ofVec3f(0,0,100),ofVec3f(0,0,0),ofVec3f(360.0,360.0,360.0)));
    panel.add(bGlobalRotate.setup("bGlobalRotate",false));
    panel.add(thresh.set("thresh", 0.2, 0.0, 1.0));
    panel.add(color.set("color",ofColor(255,255,255,255),ofColor(0,0,0,0),ofColor(255,255,255,255)));
    panel.add(bFillTog.setup("bFIll",false));
    panel.add(bReverseTog.setup("bReverseTog",false));
}

void testApp::setupCam() {
    cameraMode = 0;
    camAngle = 0;
    camSpeed = 10;
	   
    // そのうち、カメラの連続きりかえとかやりたい・・・ので、配列で管理してる
    cam[0].resetTransform();
	cam[0].setFov(60);
	cam[0].clearParent();

    camInitPos.set(ofVec3f(ofGetWidth()/2, ofGetHeight()/2, 750));
	cam[0].setPosition(camInitPos->x, camInitPos->y, camInitPos->z);
    
    camMovedPos.set(ofVec3f(0,0,0));
}

void testApp::setupShader() {
    preFragName = "";
    shaderMode  = "Intro";
    prevMode    = "";
    //listIndex   = 0;
    
    //shaderContents(shaderMode);
    //int rand = int(ofRandom(0, fragContentList.size()));
    //fragIndex = fragContentList[rand];
    //fragIndex = 2;
    postFragName = ".frag";
    loadShader();
}

//--------------------------------------------------------------
void testApp::update(){
    updateWhenOverThreshold();
    updateValue();
    makePrims();
    updateCamera();
}

void testApp::updateWhenOverThreshold() {
    if(bang == false && smoothedVol > thresh){
        bang = true; //音量がthreshold超えたらthreshold以下になるまでは処理しないよ！
        
        if (bPrimsTog && !bShaderTog) {
            if (!prims.size()) {
                for(int i = 0; i < objectNum; i++){
                    Primitive drawObject;
                    drawObject.drawMode = i % 2;
                    prims.push_back(drawObject);
                }
            }
            bShader = false;
            bPrims = true;
            
            
            if(isRand)randomiseAll();
            
            if((int)ofRandom(0,1))rotAxisMode = !rotAxisMode;
            
            if(bFillTog) bFill.x = !bFill.x;
            
            if(bReverseTog) bReverse = !bReverse;
            
            if(bGlobalRotate){
                globalRotate.set(ofRandom(0,360),
                                 ofRandom(0,360),
                                 ofRandom(0,360));
            }
        }
        else if (!bPrimsTog && bShaderTog) {
            if (prims.size()) {
                prims.clear();
            }
            bShader = true;
            bPrims = false;
            //fragIndex++;
            loadShader();
        }
        
    }
}

void testApp::updateValue() {
    if(bFill.x && ofGetFrameNum() % 5 == 0){
        bFill.y = !bFill.y;
    }
    
    if(bGlobalRotate == false){
        globalRotate.set(0,0,0);
    }
    
    rotate.set(ofVec3f(rotate->x,rotate->y,rotate->z));
    globalRotate.update();
    
    if(smoothedVol < thresh){
        bang = false;
    }

    float aVal = 1000;
    if(bFill.y == 1)aVal = 30; //ofFillかましてると明る過ぎるので適当にアルファ下げるよ
    
    color.set(ofColor(color->r,color->g,color->b,(int)(smoothedVol * aVal)));
    
    currentColor = color;
    if(bReverse)currentColor.set(0,0,0,(int)(smoothedVol * aVal));
    else currentColor.set(color->r,color->g,color->b,(int)(smoothedVol * aVal));
}

void testApp::makePrims(){
    switch (makeMode) {
        case 0:
            makePrimsSimply();
            break;
        case 1:
            makePrimsDynamic();
            break;
        case 2:
            makeOnePrim();
            break;
        default:
            makePrimsDynamic();
            break;
    }
}

void testApp::makePrimsSimply() {
    float cVol = smoothedVol * 0.06;
    
    for(int i = 0; i <prims.size(); i++){
        prims[i].audioVal = smoothedVol;
        
        prims[i].pos.x = (i - prims.size() * 0.5) * cVol * pos->x + ofGetWidth() * orientPosRatio;
        prims[i].pos.y = (i - prims.size() * 0.5) * cVol * pos->y + ofGetHeight() * orientPosRatio;
        prims[i].pos.z = (i - prims.size() * 0.5) * cVol * pos->z;
        
        // make orbit
        if (bOrbits) {
            /*
            if (i%3 == 0) {
                if (prims[i].pos.x > 0) prims[i].pos.x = abs(prims[i].pos.x)*3;
                else prims[i].pos.x = abs(prims[i].pos.x)*-3;
            }
             */
            
            if (i%8 == 1) {
                if (prims[i].pos.x > 0) prims[i].pos.x = abs(prims[i].pos.x)*3;
                else prims[i].pos.x = abs(prims[i].pos.x)*-3;
            }
            else if (i%4 == 1) {
                if (prims[i].pos.y > 0) prims[i].pos.y = abs(prims[i].pos.y)*3;
                else prims[i].pos.y = abs(prims[i].pos.y)*-3;
            }
            else if (i%2 == 1) {
                if (prims[i].pos.z > 0) prims[i].pos.z = abs(prims[i].pos.z)*3;
                else prims[i].pos.z = abs(prims[i].pos.z)*-3;
            }
            /*
            else if (i%9 == 0)  prims[i].pos.x *= -1;
            else if (i%6 == 0) prims[i].pos.y *= -1;
            else if (i%3 == 0) prims[i].pos.z *= -1;
             */
        }
        
        prims[i].rotate.x = i * cVol * rotate->x;
        prims[i].rotate.y = i * cVol * rotate->y;
        prims[i].rotate.z = i * cVol * rotate->z;
        
        prims[i].size.x = i * cVol * size->x + 20;
        prims[i].size.y = i * cVol * size->y + 20;
        prims[i].size.z = i * cVol * size->z + 20;
        
        prims[i].color = currentColor;
        
        prims[i].bFill = bFill.y;
        prims[i].rotAxisMode = rotAxisMode;
        prims[i].update();
    }
}

void testApp::makeOnePrim() {
    float cVol = smoothedVol * 0.06;
    
    color.set(ofColor(color->r,color->g,color->b,255));
    prims[0].audioVal = cVol;
    prims[0].pos.x = cVol * pos->x * 2.0 + ofGetWidth() * 0.5;
    prims[0].pos.y = cVol * pos->y * 2.0 + ofGetHeight() * 0.5;
    prims[0].pos.z = cVol * pos->z * 2.0;
    
    
    prims[0].rotate.x = cVol * rotate->x * 2.0;
    prims[0].rotate.y = cVol * rotate->y * 2.0;
    prims[0].rotate.z = cVol * rotate->z * 2.0;
    
    prims[0].size.x = cVol * size->x * 10 + 10;
    prims[0].size.y = cVol * size->y * 10 + 10;
    prims[0].size.z = cVol * size->z * 10 + 10;
    
    prims[0].color = color;
    prims[0].bFill = bFill.x;
    prims[0].update();
}

void testApp::makePrimsDynamic() {
    float cVol = smoothedVol * 0.06;
    
    color.set(ofColor(color->r,color->g,color->b,255));
    int repeat = 20;
    
    for(int i = 0; i < prims.size(); i++){
        
        prims[i].audioVal = cVol;
        prims[i].pos.x = (i - prims.size() * 0.5) * cVol * pos->x * 2.0 + ofGetWidth() * 0.5;
        prims[i].pos.y = (i - prims.size() * 0.5) * cVol * pos->y * 2.0 + ofGetHeight() * 0.5;
        prims[i].pos.z = (i - prims.size() * 0.5) * cVol * pos->z * 2.0;
        
        
        prims[i].rotate.x = i * cVol * rotate->x * 2.0;
        prims[i].rotate.y = i * cVol * rotate->y * 2.0;
        prims[i].rotate.z = i * cVol * rotate->z * 2.0;
        
        prims[i].size.x = i * cVol * size->x * 10 + 40;
        prims[i].size.y = i * cVol * size->y * 10 + 40;
        prims[i].size.z = i * cVol * size->z * 10 + 40;
        prims[i].drawMode = 0;
        prims[i].color = currentColor;
        prims[i].bFill = bFill.x;
        prims[i].update();
    }
}

void testApp::updateCamera() {
    if (cameraMode) {
        if      (cameraMode == 1) camRotationXZ();
        else if (cameraMode == 2) camRotationYZ();
        else                      camRotationXYZ();
    }
    else {
        cam[0].setPosition(camInitPos->x, camInitPos->y, camInitPos->z);
    }
    cam[0].lookAt(ofVec3f(camInitPos->x, camInitPos->y, 0));
}

void testApp::camRotationXZ() {
    float cVol = smoothedVol * 0.06;
    camAngle = camAngle + (camSpeed * cVol);

    camMovedPos.set(ofVec3f(
                            camInitPos->x + (camInitPos->z * cos(camAngle)),
                            camInitPos->y,
                            camInitPos->z * sin(camAngle)
                            ));
    cam[0].setPosition(ofVec3f(
                               camMovedPos->x,
                               camMovedPos->y,
                               camMovedPos->z
                               )
                       );

}

void testApp::camRotationYZ() {
    float cVol = smoothedVol * 0.06;
    camAngle = camAngle + (camSpeed * cVol);
    camMovedPos.set(ofVec3f(
                            camInitPos->x,
                            camInitPos->y + (camInitPos->z * cos(camAngle)),
                            camInitPos->z * sin(camAngle)
                            ));
    cam[0].setPosition(ofVec3f(
                               camMovedPos->x,
                               camMovedPos->y,
                               camMovedPos->z
                               )
                       );

}

void testApp::camRotationXYZ() {
    float cVol = smoothedVol * 0.06;
    camAngle = camAngle + (camSpeed * cVol);
    camMovedPos.set(ofVec3f(
                            camInitPos->x + (camInitPos->z * cos(camAngle)),
                            camInitPos->y + (camInitPos->z * cos(camAngle)),
                            camInitPos->z * sin(camAngle)
                            ));
    cam[0].setPosition(ofVec3f(
                               camMovedPos->x,
                               camMovedPos->y,
                               camMovedPos->z
                               )
                       );

}

void testApp::loadShader() {
    
    
    if (prevMode != shaderMode) {
        shaderContents(shaderMode);
        listIndex = 0;
    }
    else {
        listIndex++;
        if (listIndex > fragContentList.size()-1) listIndex = 0;
    }
    prevMode = shaderMode;
    
    fragIndex = fragContentList[listIndex];
    
    sprintf(charIndex, "%d", fragIndex);
    fragFile = preFragName + charIndex + postFragName;
    
    ofFile file(fragFile);
    if(!file.exists()){
        ofLogError("The file " + fragFile + " is missing");
        fragIndex = 1;
        sprintf(charIndex, "%d", fragIndex);
        fragFile = preFragName + charIndex + postFragName;
    }
    cout << "frag index :" << fragFile << "\n";
    shader1.load("test.vert", fragFile);
}


//--------------------------------------------------------------
void testApp::draw(){
    cam[0].begin();
    
    //細かい設定して、
    //primsの配列をがーーーーーーーーーとかく
    //ofBackgroundGradient(ofFloatColor(1,1,1,1), ofFloatColor(127,127,127,127));
    ofEnableAlphaBlending();
    if(bReverse == false){
        ofBackground(0,0,0,0);
        ofEnableBlendMode(OF_BLENDMODE_ADD);
    }
    else {
        ofBackground(150,150,150);
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    }
    
    ofPushMatrix();
    ofTranslate(ofGetWidth()*0.5, ofGetHeight()*0.5);
    ofRotateX(globalRotate.x);
    ofRotateY(globalRotate.y);
    ofRotateZ(globalRotate.z);
    if(!bHide)ofDrawAxis(750);
    ofTranslate(-ofGetWidth()*0.5, -ofGetHeight()*0.5);
    
    if (bPrims) {
        for(int i = 0; i <prims.size(); i++){
            prims[i].draw();
        }
    }

    ofPopMatrix();
    
    cam[0].end();
    
    if (bShader) {
        drawShader();
    }
    
    if(bHide == false){
        panel.draw();
    }
    
}

void testApp::drawShader() {
    float resolution[2];
    resolution[0] = ofGetWindowWidth();
    resolution[1] = ofGetWindowHeight();
    
    float time = ofGetElapsedTimef();
    float mousePoint[2];
    mousePoint[0] = mouseX;
    mousePoint[1] = mouseY;
    
    shader1.begin();
    shader1.setUniform1f("time", time);
    shader1.setUniform2fv("resolution", resolution);
    shader1.setUniform1f("vol", smoothedVol);
    shader1.setUniform2fv("mouse", mousePoint);

    glEnable(GL_DEPTH_TEST);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(ofGetWidth(), 0);
    glVertex2f(ofGetWidth(), ofGetHeight());
    glVertex2f(0, ofGetHeight());
    glEnd();
    
    shader1.end();
}

//--------------------------------------------------------------
void testApp::randomiseAll(){
    
    int baseNum = 25;
    
    int rand = (int)ofRandom(0,5);
    //pos.set(ofVec3f(100,0,0));
    //rotate.set(ofVec3f(0,100,0));
    //size.set(ofVec3f(ofRandom(0,300),ofRandom(0,300),ofRandom(0,300)));
    
    if(rand == 0)pos.set(ofVec3f(baseNum*2,0,0));
    else if(rand == 1)pos.set(ofVec3f(0,baseNum*2,0));
    else if(rand == 2)pos.set(ofVec3f(0,0,baseNum*2));
    else if(rand == 3)pos.set(ofVec3f(ofRandom(baseNum,baseNum*4),ofRandom(baseNum,baseNum*4),ofRandom(baseNum,baseNum*4)));
    else if(rand == 4)pos.set(ofVec3f(0,0,0));
    
    rand = (int)ofRandom(0,5);
    if(rand == 0)rotate.set(ofVec3f(baseNum*2,0,0));
    else if(rand == 1)rotate.set(ofVec3f(0,baseNum*2,0));
    else if(rand == 2)rotate.set(ofVec3f(0,0,baseNum*2));
    else if(rand == 3)rotate.set(ofVec3f(ofRandom(0,baseNum*2),ofRandom(0,baseNum*2),ofRandom(0,baseNum*2)));
    else if(rand == 4)rotate.set(ofVec3f(0,0,0));
    
    rand = (int)ofRandom(0,4);
    if(rand == 0)size.set(ofVec3f(ofRandom(baseNum*2,baseNum*6),0,0));
    else if(rand == 1)size.set(ofVec3f(0,ofRandom(baseNum*2,baseNum*6),0));
    else if(rand == 2)size.set(ofVec3f(0,0,ofRandom(baseNum*2,baseNum*6)));
    else if(rand == 3)size.set(ofVec3f(ofRandom(0,baseNum*6),ofRandom(0,baseNum*6),ofRandom(0,baseNum*6)));
    
    /*
    rand = (int)ofRandom(0,4);
    if(rand == 0)color.set(ofColor(ofRandom(0,127),ofRandom(0,255),ofRandom(127,255),ofRandom(127,255)));
    else if(rand == 1)color.set(ofColor(ofRandom(0,255),ofRandom(0,255),ofRandom(0,255),ofRandom(0,255)));
    else if(rand == 2)color.set(ofColor(ofRandom(0,255),ofRandom(0,255),ofRandom(0,255),ofRandom(0,255)));
    else if(rand == 3)color.set(ofColor(ofRandom(0,255),ofRandom(0,255),ofRandom(0,255),ofRandom(0,255)));
    */
    color.set(ofColor(ofRandom(0,127),ofRandom(0,255),ofRandom(127,255),ofRandom(127,255)));
    
    camSpeed = ofRandom(5, 10);
    rand = (int)ofRandom(0,6);
    if      (rand == 1) cameraMode = 1;
    else if (rand == 2) cameraMode = 2;
    else if (rand == 3) cameraMode = 3;
    else                cameraMode = 0;
}


//--------------------------------------------------------------
void testApp::audioIn(float * input, int bufferSize, int nChannels){
	
	float curVol = 0.0;
	
	// samples are "interleaved"
	int numCounted = 0;
    
	//lets go through each sample and calculate the root mean square which is a rough way to calculate volume
	for (int i = 0; i < bufferSize; i++){
		left[i]		= input[i] * 5;//*0.5;
        
		curVol += left[i] * left[i];
		numCounted++;
	}
	
	//this is how we get the mean of rms :)
	curVol /= (float)numCounted;
	
	// this is how we get the root of rms :)
	curVol = sqrt( curVol );
	
	smoothedVol *= 0.93;
	smoothedVol += 0.07 * curVol;
	
	bufferCounter++;
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    cout << "key " << key << "\n";
    
    if( key == 'a' ){
		isRand = !isRand;
        cout << "isRand: " << isRand << "\n";
	}
    
    if( key == 'h' ){
		bHide = !bHide;
	}

    if( key == 'i' ){
        loadShader();
	}
    
    if( key == 'z' ){
		bGlobalRotate = !bGlobalRotate;
	}
    
    if( key == 'x' ){
		bFillTog = !bFillTog;
	}
    
    if( key == 'c' ){
		bReverseTog = !bReverseTog;
	}
    
    if( key == 'o' ){
		bOrbits = !bOrbits;
	}
    
    if( key == 'p' ){
		orientPosRatio += 0.25;
        if (orientPosRatio == 1) {
            orientPosRatio = 0.25;
        }
	}

    if( key == 's' ){
		bPrimsTog  = !bPrimsTog;
        bShaderTog = !bShaderTog;
	}

    if( key == '1' ) shaderMode  = "Intro";
	if( key == '2' ) shaderMode  = "Amero";
    if( key == '3' ) shaderMode  = "Sabi1";
    if( key == '4' ) shaderMode  = "Sabi2";
    if( key == '5' ) shaderMode  = "Bmero";
    if( key == '6' ) shaderMode  = "End";
    
    if( key == 't' ){
        
	}
    
    if( key == ' ' ){
		//ofSetWindowPosition(0, 1280);
        makeMode++;
        if (makeMode > 1) makeMode = 0;
	}
    
}

void testApp::shaderContents(char *s) {
    
    map<string, int> mapShaderType;
    mapShaderType.insert(make_pair(
                                   "Intro", 1
                                   ));
    mapShaderType.insert(make_pair(
                                   "Amero", 2
                                   ));
    mapShaderType.insert(make_pair(
                                   "Sabi1", 3
                                   ));
    mapShaderType.insert(make_pair(
                                   "Sabi2", 4
                                   ));
    mapShaderType.insert(make_pair(
                                   "Bmero", 5
                                   ));
    mapShaderType.insert(make_pair(
                                   "End", 6
                                   ));
    
    int type_id = mapShaderType[s];
    
    fragContentList.clear();
    switch (type_id) {
        case 1:
            fragContentList.push_back(2);
            fragContentList.push_back(15);
            fragContentList.push_back(22);
             break;
        case 2:
            fragContentList.push_back(4);
            fragContentList.push_back(5); //bMero
            break;
        case 3:
            fragContentList.push_back(9);
            //fragContentList.push_back(16);  //質感が違いすぎる
            //fragContentList.push_back(10); //弱いパーティクル
            //fragContentList.push_back(1); //弱いパーティクル

            fragContentList.push_back(14);

            break;
        case 4:
            
            //fragContentList.push_back(8);// wood. あわない
            fragContentList.push_back(11);
            //fragContentList.push_back(19);// あわない
            fragContentList.push_back(20);
            //fragContentList.push_back(27);  // 沢谷かすぎる
            fragContentList.push_back(21); // sabi end


            break;
        case 5:
            fragContentList.push_back(6);
            fragContentList.push_back(7);
            fragContentList.push_back(17);
            fragContentList.push_back(23);
            fragContentList.push_back(26);
            fragContentList.push_back(25);
            //fragContentList.push_back(13);
            fragContentList.push_back(25);

            break;
        case 6:
            fragContentList.push_back(3);
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

