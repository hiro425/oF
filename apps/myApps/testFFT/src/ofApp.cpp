#include "ofApp.h"
#include "math.h"
//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	ofSetFrameRate(999);
	fft.setup();
    
    //setupPrims();

    lowD.thresh = 0.4;
    midD.thresh = 0.3;
    highD.thresh = 0.3;
    
    lowD.bang = midD.bang = highD.bang = false;
    
    pointLight.setDiffuseColor( ofColor(255.f, 255.f, 255.f));
	pointLight.setSpecularColor( ofColor(255.f, 255.f, 255.f));
	pointLight.setPointLight();
    pointLight.setPosition(ofGetWidth()/2, ofGetHeight()/2, 300);
    
    shader1.load("test.vert", "particle/1_ringingLight.frag");
    
}

void ofApp::setupPrims() {
    ofSetSphereResolution(2);
    for(int i = 0; i < objectNum; i++){
        Primitive lowObject, midObject, highObject;
        lowObject.drawMode = i % 2;
        midObject.drawMode = i % 2 + 1;
        highObject.drawMode = i % 2 + 1;
        lowPrims.push_back(lowObject);
        midPrims.push_back(midObject);
        highPrims.push_back(highObject);
    }
    lowData.pos.set("pos",ofVec3f(0,0,0),ofVec3f(0,0,0),ofVec3f(300,300,300));
    lowData.size.set("scale",ofVec3f(50,60,80),ofVec3f(0,0,0),ofVec3f(1000,1000,1000));
    lowData.rotate.set("rotate",ofVec3f(0,0,100),ofVec3f(0,0,0),ofVec3f(360.0,360.0,360.0));
    lowData.color.set("color",ofColor(255,255,255,255),ofColor(0,0,0,0),ofColor(255,255,255,255));
    
    midData.pos.set("pos",ofVec3f(0,0,0),ofVec3f(0,0,0),ofVec3f(300,300,300));
    midData.size.set("scale",ofVec3f(50,60,80),ofVec3f(0,0,0),ofVec3f(1000,1000,1000));
    midData.rotate.set("rotate",ofVec3f(0,0,100),ofVec3f(0,0,0),ofVec3f(360.0,360.0,360.0));
    midData.color.set("color",ofColor(255,255,255,255),ofColor(0,0,0,0),ofColor(255,255,255,255));
    
    highData.pos.set("pos",ofVec3f(0,0,0),ofVec3f(0,0,0),ofVec3f(300,300,300));
    highData.size.set("scale",ofVec3f(50,60,80),ofVec3f(0,0,0),ofVec3f(1000,1000,1000));
    highData.rotate.set("rotate",ofVec3f(0,0,100),ofVec3f(0,0,0),ofVec3f(360.0,360.0,360.0));
    highData.color.set("color",ofColor(255,255,255,255),ofColor(0,0,0,0),ofColor(255,255,255,255));
    
    bGlobalRotate.setup("bGlobalRotate",false);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    fft.update();

    lowD.vol  = fft.getLowVal() * 0.03;
    midD.vol  = fft.getMidVal() * 0.045;
    highD.vol = fft.getHighVal() * 0.03;
    
    checkBang(lowD);
    checkBang(midD);
    checkBang(highD);
    
    //makePrims();
    
}

void ofApp::checkBang(bandwidthData &d) {
    if((d.bang == false && d.vol > d.thresh )) {
        d.bang = true;
    }
    if(d.vol < d.thresh){
        d.bang = false;
    }
}

void ofApp::makePrims(){
    
    if(bGlobalRotate == false){
        globalRotate.set(0,0,0);
    }
    lowRotate.set(ofVec3f(lowRotate->x,lowRotate->y,lowRotate->z));
    globalRotate.update();

    insertPrimsData(lowPrims, lowData, lowD.vol, 0.5);
    insertPrimsData(midPrims, midData, midD.vol, 0.25);
    insertPrimsData(highPrims, highData, highD.vol, 0.75);
    
}

void ofApp::insertPrimsData(vector<Primitive> &p, ofApp::primsData &d, float vol, float orientRatio) {
    for(int i = 0; i < p.size(); i++){
        p[i].audioVal = vol;
        
        p[i].pos.x = (i - p.size() * 0.5) * vol * d.pos->x + ofGetWidth() * orientRatio;
        p[i].pos.y = (i - p.size() * 0.5) * vol * d.pos->y + ofGetHeight()* orientRatio;
        p[i].pos.z = (i - p.size() * 0.5) * vol * d.pos->z;
        
        p[i].rotate.x = i * vol * d.rotate->x;
        p[i].rotate.y = i * vol * d.rotate->y;
        p[i].rotate.z = i * vol * d.rotate->z;
        
        p[i].size.x = i * vol * d.size->x + 20;
        p[i].size.y = i * vol * d.size->y + 20;
        p[i].size.z = i * vol * d.size->z + 20;
        
        //p[i].color = currentColor;
        d.color.set(ofColor(d.color->r,d.color->g,d.color->b,(int)(fft.getSmoothedUnScaledLoudestValue()*100)));
        p[i].color = d.color;
        
        p[i].rotAxisMode = rotAxisMode;
        p[i].update();
        
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofEnableAlphaBlending();
    ofBackground(0,0,0,0);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofEnableLighting();
    pointLight.enable();
    
    ofSetColor(255);
    
    //drawTwoRings();
    
    for (int i = 0; i < 360; i+=15) {
        
        //float moveDim = ofGetElapsedTimef()/2;
        //if (midD.bang) midD.moveDim++;
        if (midD.bang && i%2==0) midD.moveDim++;
        
        float rad = i*PI/180;
        float posx = ofGetWidth()/2 + ofGetWidth()/10 *cos(rad + midD.moveDim);
        float posz = ofGetHeight()/2;
        float posy = ofGetHeight()/2 + ofGetWidth()/10 *sin(rad + midD.moveDim);
        
        //ofDrawSphere(posx, posy, posz, 10);
        ofPushMatrix();
        ofTranslate(posx, posy, posz);
        ofRotate(120, 1, 1, 1);
        ofDrawCone(0, -20-500*midD.vol, 0, 10, 20+1000*midD.vol);
        ofPopMatrix();
        
    }
    /*
    for (int i = 0; i < 8; i++) {
        float lastx = -999;
        float lasty = -999;
        float ynoise = ofRandom(10);
        
        float orientX = 10;
        float orientY = 0;

        float y;

        float limitX = orientX + ofGetWidth()*hVol;
        
        for (int x = orientX; x < limitX ; x++) {
            y = orientY + ofNoise(ynoise) * 80;
            if (lastx > -999) {
                ofLine(x, y, lastx, lasty);
            }
            lastx = x;
            lasty = y;
            ynoise += 0.1;
        }
        
    }*/
    float ynoise = ofRandom(10);
    //float y;
    float x;
    
//    float orientX = ofGetWidth()/2;
//    float orientY = 0;

    float orientY = ofGetHeight()/2;
    float orientX = 10;

//    float limitX    = orientX + ofGetWidth()*hVol;
//    float limitMinX = orientX - ofGetWidth()*hVol;

    float multiplyVal = 80;
    //if (highD.bang) multiplyVal = 500;
    
    float limitY    = 0 + ofGetHeight()/2*highD.vol*3.5;
    float limitMinY = ofGetHeight() - ofGetHeight()/2*highD.vol*3.5;
    
    float lastx = -999;
    float lasty = -999;
    for (int y = 0; y < limitY ; y++) {
        
        float xNoise = ofNoise(ynoise);
        x = orientX + xNoise * multiplyVal;
        if (lasty > -999) {
            ofLine(x, y, lastx, lasty);
        }
        ynoise += 0.1;
        x = orientX + ofNoise(ynoise) * multiplyVal;
        if (lasty > -999) {
            ofLine(x, y, lastx, lasty);
        }
        lastx = x;
        lasty = y;
        ynoise += 0.1;
        
        //if (y%2==0) xNoise *= -1;

        orientX += xNoise;
    }
    

    orientX = 10;
    float lastMinX = -999;
    float lastMinY = -999;
    for (int y = ofGetHeight(); y > limitMinY ; y--) {
        float xNoise = ofNoise(ynoise);
        x = orientX + ofNoise(ynoise) * multiplyVal;
        if (lastMinY > -999) {
            ofLine(x, y, lastMinX, lastMinY);
        }
        ynoise += 0.1;
        x = orientX + ofNoise(ynoise) * multiplyVal;
        if (lastMinY > -999) {
            ofLine(x, y, lastMinX, lastMinY);
        }
        lastMinX = x;
        lastMinY = y;
        ynoise += 0.1;
        orientX += xNoise;
    }
    
    float lastx2 = -999;
    float lasty2 = -999;
    orientX = ofGetWidth()-100;
    for (int y = 0; y < limitY ; y++) {
        float xNoise = ofNoise(ynoise);
        x = orientX + ofNoise(ynoise) * multiplyVal;
        if (lasty2 > -999) {
            ofLine(x, y, lastx2, lasty2);
        }
        ynoise += 0.1;
        x = orientX + ofNoise(ynoise) * multiplyVal;
        if (lasty2 > -999) {
            ofLine(x, y, lastx2, lasty2);
        }
        
        lastx2 = x;
        lasty2 = y;
        ynoise += 0.1;
        orientX -=xNoise;
    }

    float lastMinX2 = -999;
    float lastMinY2 = -999;
    orientX = ofGetWidth()-100;
    for (int y = ofGetHeight(); y > limitMinY ; y--) {
        float xNoise = ofNoise(ynoise);
        x = orientX + ofNoise(ynoise) * multiplyVal;
        if (lastMinY2 > -999) {
            ofLine(x, y, lastMinX2, lastMinY2);
        }
        ynoise += 0.1;
        x = orientX + ofNoise(ynoise) * multiplyVal;
        if (lastMinY2 > -999) {
            ofLine(x, y, lastMinX2, lastMinY2);
        }
        lastMinX2 = x;
        lastMinY2 = y;
        ynoise += 0.1;
        orientX -=xNoise;
    }

    if (highD.bang) {
        orientX = ofGetWidth()/2-50;
        x = orientX + ofNoise(ynoise) * multiplyVal;
        if (lastx > -999) {
            ofLine(x, orientY, lastx, lasty);
        }
        x = orientX + ofNoise(ynoise) * multiplyVal;
        if (lastMinX > -999) {
            ofLine(x, orientY, lastMinX, lastMinY);
        }
        orientX = ofGetWidth()/2-30;
        x = orientX + ofNoise(ynoise) * multiplyVal;
        if (lastx2 > -999) {
            ofLine(x, orientY, lastx2, lasty2);
        }
        x = orientX + ofNoise(ynoise) * multiplyVal;
        if (lastMinX2 > -999) {
            ofLine(x, orientY, lastMinX2, lastMinY2);
        }

    }
    
    drawShader();
    pointLight.disable();
    ofDisableLighting();
    
}



void ofApp::drawShader() {
    /*
    if (lowD.bang) {
        shader1.load("test.vert", "particle/6_diffuseLightOffAxisHard.frag");
    }
    else {
        shader1.load("test.vert", "particle/6_diffuseLightOffAxisHard.frag");
    }*/
        
    float resolution[2];
    resolution[0] = ofGetWindowWidth();
    resolution[1] = ofGetWindowHeight();
    
    float time = ofGetElapsedTimef();
    
    shader1.begin();
    shader1.setUniform1f("time", time);
    shader1.setUniform2fv("resolution", resolution);
    shader1.setUniform1f("vol", lowD.vol/3);
    
    glEnable(GL_DEPTH_TEST);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(ofGetWidth(), 0);
    glVertex2f(ofGetWidth(), ofGetHeight());
    glVertex2f(0, ofGetHeight());
    glEnd();
    
    shader1.end();
}

void ofApp::drawTwoRings() {
    //additional---------------------------------
    ofPushMatrix();
    ofTranslate(ofGetWidth()/8*3, 0);
    //-------------------------------------------
    
    //ofDrawSphere(0, ofGetHeight()/2, -ofGetHeight()/2, 10+250*lVol);
    for (int i = 0; i < 360; i+=15) {
        
        //float moveDim = ofGetElapsedTimef()/2;
        //if (midD.bang) midD.moveDim++;
        if (lowD.bang && i%2==0) lowD.moveDim++;
        
        float rad = i*PI/180;
        float posx = ofGetWidth()/12 *cos(rad + lowD.moveDim);
        float posz = ofGetHeight()/2;
        float posy = ofGetHeight()/2 + ofGetWidth()/10 *sin(rad + lowD.moveDim);
        
        //ofDrawSphere(posx, posy, posz, 10);
        ofPushMatrix();
        ofTranslate(posx, posy, posz);
        ofRotate(120, 1, 1, 1);
        ofDrawCone(0, -20-500*midD.vol, 0, 10, 20+1000*midD.vol);
        ofPopMatrix();
        
    }
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(ofGetWidth()/8*5, 0);
    //ofDrawSphere(0, ofGetHeight()/2,-ofGetHeight()/2, 10+250*lVol);
    for (int i = 0; i < 360; i+=15) {
        
        //float moveDim = ofGetElapsedTimef()/2;
        //if (midD.bang) midD.moveDim++;
        if (lowD.bang && i%2==0) lowD.moveDim++;
        
        float rad = i*PI/180;
        float posx = ofGetWidth()/12 *cos(rad + lowD.moveDim);
        float posz = ofGetHeight()/2;
        float posy = ofGetHeight()/2 + ofGetWidth()/10 *sin(rad + lowD.moveDim);
        
        //ofDrawSphere(posx, posy, posz, 10);
        ofPushMatrix();
        ofTranslate(posx, posy, posz);
        ofRotate(120, 1, 1, 1);
        ofDrawCone(0, -20-500*midD.vol, 0, 10, 20+1000*midD.vol);
        ofPopMatrix();
        
    }
    
    ofPopMatrix();
}

void ofApp::drawPrims() {
    ofPushMatrix();
    ofTranslate(ofGetWidth()*0.5, ofGetHeight()*0.5);
    ofRotateX(globalRotate.x);
    ofRotateY(globalRotate.y);
    ofRotateZ(globalRotate.z);
    
    ofTranslate(-ofGetWidth()*0.5, -ofGetHeight()*0.5);
    
    for(int i = 0; i <lowPrims.size(); i++){
        lowPrims[i].draw();
    }
    for(int i = 0; i < midPrims.size(); i++){
        midPrims[i].draw();
    }
    for(int i = 0; i < highPrims.size(); i++){
        highPrims[i].draw();
    }
    
    ofPopMatrix();
}

void ofApp::randomiseAll(){

    insertRandomiseData(lowData);
    insertRandomiseData(midData);
    insertRandomiseData(highData);
    //color.set(ofColor(ofRandom(0,127),ofRandom(0,255),ofRandom(127,255),ofRandom(127,255)));
    
}

void ofApp::insertRandomiseData(ofApp::primsData &p) {
    int baseNum = 5;

    int rand = (int)ofRandom(0,5);
    
    if(rand == 0)p.pos.set(ofVec3f(baseNum*2,0,0));
    else if(rand == 1)p.pos.set(ofVec3f(0,baseNum*2,0));
    else if(rand == 2)p.pos.set(ofVec3f(0,0,baseNum*2));
    else if(rand == 3)p.pos.set(ofVec3f(ofRandom(baseNum,baseNum*4),ofRandom(baseNum,baseNum*4),ofRandom(baseNum,baseNum*4)));
    else if(rand == 4)p.pos.set(ofVec3f(0,0,0));
    
    rand = (int)ofRandom(0,5);
    if(rand == 0)p.rotate.set(ofVec3f(baseNum*2,0,0));
    else if(rand == 1)p.rotate.set(ofVec3f(0,baseNum*2,0));
    else if(rand == 2)p.rotate.set(ofVec3f(0,0,baseNum*2));
    else if(rand == 3)p.rotate.set(ofVec3f(ofRandom(0,baseNum*2),ofRandom(0,baseNum*2),ofRandom(0,baseNum*2)));
    else if(rand == 4)p.rotate.set(ofVec3f(0,0,0));
    
    rand = (int)ofRandom(0,4);
    if(rand == 0)p.size.set(ofVec3f(ofRandom(baseNum*2,baseNum*6),0,0));
    else if(rand == 1)p.size.set(ofVec3f(0,ofRandom(baseNum*2,baseNum*6),0));
    else if(rand == 2)p.size.set(ofVec3f(0,0,ofRandom(baseNum*2,baseNum*6)));
    else if(rand == 3)p.size.set(ofVec3f(ofRandom(0,baseNum*6),ofRandom(0,baseNum*6),ofRandom(0,baseNum*6)));
    
    /*
     rand = (int)ofRandom(0,4);
     if(rand == 0)color.set(ofColor(ofRandom(0,127),ofRandom(0,255),ofRandom(127,255),ofRandom(127,255)));
     else if(rand == 1)color.set(ofColor(ofRandom(0,255),ofRandom(0,255),ofRandom(0,255),ofRandom(0,255)));
     else if(rand == 2)color.set(ofColor(ofRandom(0,255),ofRandom(0,255),ofRandom(0,255),ofRandom(0,255)));
     else if(rand == 3)color.set(ofColor(ofRandom(0,255),ofRandom(0,255),ofRandom(0,255),ofRandom(0,255)));
     */
    
    p.color.set(ofColor(ofRandom(0,127),ofRandom(0,255),ofRandom(127,255),ofRandom(127,255)));
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key=='q'){
        fft.setVolumeRange(100);
        fft.setNormalize(false);
    }
    if(key=='r'){
        fft.setNormalize(true);
    }
    
    if(key=='i'){
        randomiseAll();
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
