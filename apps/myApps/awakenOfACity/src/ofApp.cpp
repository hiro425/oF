#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//just set up the openFrameworks stuff
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofBackground(255);
    ofSetSphereResolution(4);

    fft.setup();
    fft.setNumFFTBins(32); //adjustment bulve
    fftDebugMode = false;
    lowD.thresh = 0.4;
    midD.thresh = 0.3;
    highD.thresh = 0.3;
    commonD.thresh = 5;
    
    numWaves = (int)fft.getNumFFTbins();
    
    for (int i = 0; i < numWaves; i++) {
        vector<ofVec3f> point;
        pointArray.push_back(point);
    }
    
    //
    ofBackgroundHex(0x000000);
    cam.resetTransform();
	cam.setFov(60);
	cam.clearParent();
    cam.setPosition(ofGetWidth()/2, ofGetHeight()/2, 750);
    
    //cam.enableOrtho();
    camAngle = 90;
    rotateMode = false;

    pointLight.setDiffuseColor( ofColor(255.f, 255.f, 255.f));
	pointLight.setSpecularColor( ofColor(255.f, 255.f, 255.f));
	pointLight.setPointLight();
    pointLight.setPosition(ofGetWidth()/2, ofGetHeight()/4*3, -480);
    
    setupSPK();
}

void ofApp::setupSPK() {
    ofDisableArbTex();
	sprite.loadImage("image.jpg");
	ofEnableArbTex();

	sys1.setup();
	group1.setup(sys1);
	group1.setColor(ofxSPK::RangeC(ofColor(255, 255, 255), ofColor(255, 255, 255)),
                    ofxSPK::RangeC(ofColor(0, 0), ofColor(0, 0)));
	group1.setLifeTime(0.5, 10);
	group1.setFriction(0.1);
	group1.setSize(0, ofxSPK::RangeF(10, 50));
	group1.setGravity(ofVec3f(0, -10, 0));
	group1.setMass(0.1, 1);
	rot1.setup(SPK::Vortex::create(SPK::Vector3D(0, 0, 0),
								  SPK::Vector3D(0, 0, 0.5),
								  100,
								  100), group1);
	group1.reserve(4000);
    
    sys2.setup();
	group2.setup(sys2);
	group2.setColor(ofxSPK::RangeC(ofColor(255, 255, 160), ofColor(255, 255, 160)),
                    ofxSPK::RangeC(ofColor(0, 0), ofColor(0, 0)));
	group2.setLifeTime(0.5, 10);
	group2.setFriction(0.1);
	group2.setSize(0, ofxSPK::RangeF(10, 50));
	group2.setGravity(ofVec3f(0, -10, 0));
	group2.setMass(0.1, 1);
	rot2.setup(SPK::Vortex::create(SPK::Vector3D(0, 0, 0),
                                   SPK::Vector3D(0, 0, 0.5),
                                   100,
                                   100), group2);
	group2.reserve(4000);
    
    sys3.setup();
	group3.setup(sys3);
	group3.setColor(ofxSPK::RangeC(ofColor(196, 150, 3), ofColor(196, 150, 3)),
                    ofxSPK::RangeC(ofColor(0, 0), ofColor(0, 0)));
	group3.setLifeTime(0.5, 10);
	group3.setFriction(0.1);
	group3.setSize(0, ofxSPK::RangeF(10, 50));
	group3.setGravity(ofVec3f(0, -10, 0));
	group3.setMass(0.1, 1);
	rot3.setup(SPK::Vortex::create(SPK::Vector3D(0, 0, 0),
                                   SPK::Vector3D(0, 0, 0.5),
                                   100,
                                   100), group3);
	group3.reserve(4000);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    updateFFT();
    ofVec3f sumOfAllPoints(0,0,0);
    
    for (int i = 0; i < pointArray.size(); i++) {
        ofVec3f point(
                      ofGetWidth()*0.9,
                      ofGetHeight()/6 + fft.getSpectrum()[i] + sin(i * 0.15 + ofGetElapsedTimef() ) * 30.0,
                      -30*i);
        pointArray[i].push_back(point);
        
        for (int j=0; j < pointArray[i].size(); j++) {
            pointArray[i][j].x -= 5;
            if (pointArray[i][j].x < 0) {
                pointArray[i].erase(pointArray[i].begin());
            }
        }
   
        for (int i = 0; i < fft.getNumFFTbins(); i++) {
            spherePos.push_back(ofPoint(ofGetWidth()/8*7, ofGetHeight()/9*2+fft.getSpectrum()[i]*2, -30*i));
            
            if (fft.getSpectrum()[i] > commonD.thresh) {
                if (i < (int)fft.getNumFFTbins()/3) {
                    particlePos1.push_back(ofPoint(ofGetWidth()/8*7, ofGetHeight()/9*2+fft.getSpectrum()[i]*2, -30*i));
                }
                else if (i < (int)fft.getNumFFTbins()/3*2) {
                    particlePos2.push_back(ofPoint(ofGetWidth()/8*7, ofGetHeight()/9*2+fft.getSpectrum()[i]*2, -30*i));
                }
                else {
                    particlePos3.push_back(ofPoint(ofGetWidth()/8*7, ofGetHeight()/9*2+fft.getSpectrum()[i]*2, -30*i));
                }
                    
                
            }
        }

    }
    updateSPK();
    //cam.setPosition(ofVec3f(cam.getPosition().x, cam.getPosition().y, cam.getPosition().z));
    cam.lookAt(ofVec3f(ofGetWidth()/2, ofGetHeight()/2, -510));
    if (rotateMode) {
        camAngle += 0.5;
        float radian = camAngle * PI / 180;
        cam.setPosition(
                        ofGetWidth()/2 + 1260 * cos(radian),
                        cam.getPosition().y,
                        -510 + 1260 * sin(radian)
                        );
    }
    
    //cam.lookAt(ofVec3f(0,-1,0),ofVec3f(0,0,1));

}

void ofApp::updateSPK() {
    //group.emitRandom(10, ofVec3f(ofGetMouseX(), ofGetMouseY()));
	
    for (int i = 0; i < particlePos1.size(); i++) {
        group1.emitRandom(1, ofVec3f(particlePos1[i].x, particlePos1[i].y, particlePos1[i].z));
    }
    for (int i = 0; i < particlePos2.size(); i++) {
        group2.emitRandom(1, ofVec3f(particlePos2[i].x, particlePos2[i].y, particlePos2[i].z));
    }
    for (int i = 0; i < particlePos3.size(); i++) {
        group3.emitRandom(1, ofVec3f(particlePos3[i].x, particlePos3[i].y, particlePos3[i].z));
    }
    
	sys1.update();
    sys2.update();
    sys3.update();
    
	ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

void ofApp::updateFFT() {
    fft.update();
    
    lowD.vol  = fft.getLowVal() * 0.03;
    midD.vol  = fft.getMidVal() * 0.045;
    highD.vol = fft.getHighVal() * 0.045;
    
    checkBang(lowD);
    checkBang(midD);
    checkBang(highD);
    
}

void ofApp::checkBang(bandwidthData &d) {
    if((d.bang == false && d.vol > d.thresh )) {
        d.bang = true;
    }
    if(d.vol < d.thresh){
        d.bang = false;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    cam.begin();
    
    ofEnableAlphaBlending();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofEnableLighting();
    pointLight.enable();
    
    for (int i = 0; i < pointArray.size(); i++) {
        
        ofVbo v;
        int pointSize = pointArray[i].size();
        
        v.setVertexData(&pointArray[i][0], pointArray[i].size(), GL_DYNAMIC_DRAW);
        
        vector<ofIndexType> faces;
        for (int j=0; j < pointSize; j++) {
            faces.push_back(j);
        }
        v.setIndexData(&faces[0], pointSize, GL_DYNAMIC_DRAW);
        
        vector<ofFloatColor> vcolors;
        for (int j=0; j < pointSize; j++) {
            float r = 175. / 255.;
            float g = 223. / 255.;
            float b = 228. / 255.;
            float a = 255. * (pointSize-j)/pointSize*2. * (pointArray[i][j].x+100)/ofGetWidth() / 255.;
            vcolors.push_back(ofFloatColor(r, g, b, a));
        }
        v.setColorData(&vcolors[0], pointSize, GL_DYNAMIC_DRAW);
        ofSetLineWidth(3);
        v.drawElements(GL_LINE_STRIP, pointSize);
    /*
        for (int j = 1; j < pointArray[i].size(); j++) {

            ofVec3f thisPoint = pointArray[i][j-1];
            ofVec3f nextPoint = pointArray[i][j];
            
            
            ofVec3f direction = (nextPoint - thisPoint);
            float distance = direction.length();
            ofVec3f unitDirection = direction.normalized();
            
            int pointSize = pointArray[i].size();
            float x = pointArray[i][j].x;
            ofSetColor(175, 223, 228, 255*(pointSize-j)/pointSize*2 * (x+100)/ofGetWidth());
            ofSetLineWidth(3);
            ofLine(thisPoint, nextPoint);
        }
*/
    }
    /*
    vector<float> spectrum = fft.getSpectrum();
    
    for (int i = 0; i < spectrum.size(); i++) {
        ofPushStyle();
        //ofSetRectMode(OF_RECTMODE_CORNER);
        //ofSetLineWidth(2);
        for(int i=0; i<spectrum.size(); i++){ //for the number of columns
            ofSetColor(100,100,200);
            ofNoFill();
            ofRect(
                   ofGetWidth()*((float)i/spectrum.size()),
                   ofGetHeight()-20,
                   ofGetWidth()/spectrum.size(),
                   -ofMap(spectrum[i], 0, 200, 0, ofGetHeight() -50)
                   );
        }
        ofPopStyle();
    }
*/
    drawSPK();
    
    pointLight.disable();
    ofDisableLighting();
    
    cam.end();
    
    if (fftDebugMode) {
        fft.drawBars();
        fft.drawDebug();
        
    }
}

void ofApp::drawSPK() {

	// sys.debugDraw();
	
	// bind texture, enable point sprite while drawing particles
	sprite.bind();
	ofEnablePointSprites();
	sys1.draw();
    sys2.draw();
    sys3.draw();
	ofDisablePointSprites();
	sprite.unbind();
    
    ofSetColor(10, 255);
    for (int i = 0; i < spherePos.size(); i++) {
        ofDrawBox(spherePos[i].x, spherePos[i].y,spherePos[i].z, 50,5,10);
    }
    spherePos.clear();
    particlePos1.clear();
    particlePos2.clear();
    particlePos3.clear();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    int r;
    int g;
    int b;
    
    switch (key) {
        case 'd':
            fftDebugMode = !fftDebugMode;
            break;
        case 'z':
            rotateMode = !rotateMode;
            break;
        case 'r':
            r = 255 * pointLight.getDiffuseColor().r;
            g = 255 * pointLight.getDiffuseColor().g;
            b = 255 * pointLight.getDiffuseColor().b;
    
            if (r > 196) r--;
            else if (r < 196) r++;
            
            if (g > 100) g--;
            else if (g < 100) g++;

            if (b > 3) b--;
            else if (b < 3) b++;
            
            pointLight.setDiffuseColor( ofColor(r, g, b));
            pointLight.setSpecularColor( ofColor(r, g, b));
            group1.setColor(ofxSPK::RangeC(ofColor(r, g, b), ofColor(r, g, b)),
                            ofxSPK::RangeC(ofColor(0, 0), ofColor(0, 0)));
            group2.setColor(ofxSPK::RangeC(ofColor(r, g, b), ofColor(r, g, b)),
                            ofxSPK::RangeC(ofColor(0, 0), ofColor(0, 0)));
            group3.setColor(ofxSPK::RangeC(ofColor(r, g, b), ofColor(r, g, b)),
                            ofxSPK::RangeC(ofColor(0, 0), ofColor(0, 0)));
            break;
        case 'b':
            r = 255 * pointLight.getDiffuseColor().r;
            g = 255 * pointLight.getDiffuseColor().g;
            b = 255 * pointLight.getDiffuseColor().b;
            
            if (r > 100) r--;
            else if (r < 100) r++;
            
            if (g > 3) g--;
            else if (g < 3) g++;
            
            if (b > 100) b--;
            else if (b < 100) b++;
            
            pointLight.setDiffuseColor( ofColor(r, g, b));
            pointLight.setSpecularColor( ofColor(r, g, b));
            group1.setColor(ofxSPK::RangeC(ofColor(r, g, b), ofColor(r, g, b)),
                            ofxSPK::RangeC(ofColor(0, 0), ofColor(0, 0)));
            group2.setColor(ofxSPK::RangeC(ofColor(r, g, b), ofColor(r, g, b)),
                            ofxSPK::RangeC(ofColor(0, 0), ofColor(0, 0)));
            group3.setColor(ofxSPK::RangeC(ofColor(r, g, b), ofColor(r, g, b)),
                            ofxSPK::RangeC(ofColor(0, 0), ofColor(0, 0)));
            break;
        case 'w':
            r = 255 * pointLight.getDiffuseColor().r;
            g = 255 * pointLight.getDiffuseColor().g;
            b = 255 * pointLight.getDiffuseColor().b;
            
            if (r > 255) r--;
            else if (r < 255) r++;
            
            if (g > 255) g--;
            else if (g < 255) g++;
            
            if (b > 160) b--;
            else if (b < 160) b++;
            
            pointLight.setDiffuseColor( ofColor(r, g, b));
            pointLight.setSpecularColor( ofColor(r, g, b));
            group1.setColor(ofxSPK::RangeC(ofColor(r, g, b), ofColor(r, g, b)),
                            ofxSPK::RangeC(ofColor(0, 0), ofColor(0, 0)));
            group2.setColor(ofxSPK::RangeC(ofColor(r, g, b), ofColor(r, g, b)),
                            ofxSPK::RangeC(ofColor(0, 0), ofColor(0, 0)));
            group3.setColor(ofxSPK::RangeC(ofColor(r, g, b), ofColor(r, g, b)),
                            ofxSPK::RangeC(ofColor(0, 0), ofColor(0, 0)));
            break;
            
        case 'a':
            r = 255 * pointLight.getDiffuseColor().r;
            g = 255 * pointLight.getDiffuseColor().g;
            b = 255 * pointLight.getDiffuseColor().b;
            
            if (r > 255) r--;
            else if (r < 255) r++;
            
            if (g > 255) g--;
            else if (g < 255) g++;
            
            if (b > 255) b--;
            else if (b < 255) b++;
            pointLight.setDiffuseColor( ofColor(r, g, b));
            pointLight.setSpecularColor( ofColor(r, g, b));
            group1.setColor(ofxSPK::RangeC(ofColor(255, 255, 255), ofColor(255, 255, 255)),
                            ofxSPK::RangeC(ofColor(0, 0), ofColor(0, 0)));
            group2.setColor(ofxSPK::RangeC(ofColor(255, 255, 160), ofColor(255, 255, 160)),
                            ofxSPK::RangeC(ofColor(0, 0), ofColor(0, 0)));
            group3.setColor(ofxSPK::RangeC(ofColor(196, 150, 3), ofColor(196, 150, 3)),
                            ofxSPK::RangeC(ofColor(0, 0), ofColor(0, 0)));

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
