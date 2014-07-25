#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//just set up the openFrameworks stuff
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofBackground(255);

    fft.setup();
    fft.setNumFFTBins(256);
    fftDebugMode = false;
    lowD.thresh = 0.4;
    midD.thresh = 0.3;
    highD.thresh = 0.3;
    commonD.thresh = 3;
    
    numWaves = (int)fft.getNumFFTbins()/12;
    
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
    camAngle = 0;

    pointLight.setDiffuseColor( ofColor(255.f, 255.f, 255.f));
	pointLight.setSpecularColor( ofColor(255.f, 255.f, 255.f));
	pointLight.setPointLight();
    pointLight.setPosition(ofGetWidth()/2, ofGetHeight()/4*3, -30);
    
    setupSPK();
}

void ofApp::setupSPK() {
    ofDisableArbTex();
	sprite.loadImage("image.jpg");
	ofEnableArbTex();

	sys.setup();
	
	group.setup(sys);
	group.setColor(ofxSPK::RangeC(ofColor(255, 255), ofColor(255, 255)),
				   ofxSPK::RangeC(ofColor(0, 0), ofColor(255, 0)));
	
	group.setLifeTime(0.5, 15);
	group.setFriction(0.1);
	group.setSize(0, ofxSPK::RangeF(10, 100));
	
	group.setGravity(ofVec3f(0, -10, 0));
	group.setMass(0.1, 1);
	
	rot.setup(SPK::Vortex::create(SPK::Vector3D(0, ofGetHeight()),
								  SPK::Vector3D(0, 1, 0),
								  200,
								  10), group);
	
	group.reserve(10000);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    updateFFT();
    ofVec3f sumOfAllPoints(0,0,0);
    
    for (int i = 1; i < pointArray.size(); i++) {
        ofVec3f point(
                      ofGetWidth()*1.2,
                      ofGetHeight()/10*9+ fft.getSpectrum()[i] + sin(i * 0.15 + ofGetElapsedTimef() ) * 30.0,
                      -30*i);
        pointArray[i].push_back(point);
        
        for (int j=0; j < pointArray[i].size(); j++) {
            pointArray[i][j].x -= 5;
            if (pointArray[i][j].x < 0) {
                //pointArray[i][j].y +=1;
                //pointArray[i].erase(pointArray[i].begin());
            }
            if (pointArray[i][j].x < -ofGetWidth()*0.2) {
                pointArray[i].erase(pointArray[i].begin());
            }
        }
   
        for (int i = 25; i < fft.getNumFFTbins(); i++) {
            if (/*i>3 && */fft.getSpectrum()[i] > commonD.thresh) {
                particlePos.push_back(ofPoint(ofGetWidth()/8*7, ofGetHeight()/5*3-fft.getSpectrum()[i]*5, -30*i));
            }
        }

    }
    updateSPK();
    //cam.setPosition(ofVec3f(cam.getPosition().x, cam.getPosition().y, cam.getPosition().z));
    //cam.lookAt(ofVec3f(cam.getPosition().x, cam.getPosition().y, -30),ofVec3f(0,-1,0));
    
    //cam.lookAt(ofVec3f(0,-1,0),ofVec3f(0,0,1));

}

void ofApp::updateSPK() {
    //group.emitRandom(10, ofVec3f(ofGetMouseX(), ofGetMouseY()));
	
    for (int i = 0; i < particlePos.size(); i++) {
        group.emitRandom(2, ofVec3f(particlePos[i].x, particlePos[i].y, particlePos[i].z));
    }
    
	sys.update();
    
	ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

void ofApp::updateFFT() {
    fft.update();
    
    lowD.vol  = fft.getLowVal() * 0.03;
    midD.vol  = fft.getMidVal() * 0.045;
    highD.vol = fft.getHighVal() * 0.03;
    
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
    
    //cam.begin();
    
    ofEnableAlphaBlending();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofEnableLighting();
    pointLight.enable();
    
    ofDrawAxis(100);
    ofDrawSphere(ofGetWidth()/2, 200, 10);
	ofSetColor(190, 255, 255, 127);
    for (int i = 0; i < pointArray.size(); i++) {
        for (int j = 1; j < pointArray[i].size(); j++) {
            //find this point and the next point
            ofVec3f thisPoint = pointArray[i][j-1];
            ofVec3f nextPoint = pointArray[i][j];
            
            //get the direction from one to the next.
            //the ribbon should fan out from this direction
            ofVec3f direction = (nextPoint - thisPoint);
            
            //get the distance from one point to the next
            float distance = direction.length();
            
            //get the normalized direction. normalized vectors always have a length of one
            //and are really useful for representing directions as opposed to something with length
            ofVec3f unitDirection = direction.normalized();
            
            //find both directions to the left and to the right
           // ofVec3f toTheLeft = unitDirection.getRotated(-90, ofVec3f(0,0,1));
            //ofVec3f toTheRight = unitDirection.getRotated(90, ofVec3f(0,0,1));
            
            //use the map function to determine the distance.
            //the longer the distance, the narrower the line.
            //this makes it look a bit like brush strokes
            //float thickness = ofMap(distance, 0, 6, 2, 2, true);
            
            //calculate the points to the left and to the right
            //by extending the current point in the direction of left/right by the length
            //ofVec3f leftPoint = thisPoint+toTheLeft*thickness;
            //ofVec3f rightPoint = thisPoint+toTheRight*thickness;
            int pointSize = pointArray[i].size();
            float x = pointArray[i][j].x;
            ofSetColor(190, 255, 255, 255*(pointSize-j)/pointSize*2 * (x+100)/ofGetWidth());
            ofSetLineWidth(3);
            //add these points to the triangle strip
            //mesh.addVertex(ofVec3f(leftPoint.x, leftPoint.y, leftPoint.z));
            //mesh.addVertex(ofVec3f(rightPoint.x, rightPoint.y, rightPoint.z));
            ofLine(thisPoint, nextPoint);
        }

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
    
    //cam.end();
    
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
	sys.draw();
	ofDisablePointSprites();
	sprite.unbind();
    
    ofSetColor(255, 127);
    for (int i = 0; i < particlePos.size(); i++) {
        ofDrawSphere(particlePos[i].x, particlePos[i].y,particlePos[i].z,  10);
    }

    particlePos.clear();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch (key) {
        case 'd':
            fftDebugMode = !fftDebugMode;
            break;
        case 'z':
            camAngle += 1;
            cam.setPosition(
                            cam.getPosition().x,
                            cam.getPosition().y,
                            cam.getPosition().z + 1//750 + sin(camAngle)
                            );
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
