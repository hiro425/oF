#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetLogLevel( OF_LOG_VERBOSE );
	ofSetFrameRate(999);
	ofSetVerticalSync( false );
	   
	fontWorld.loadFont( "Fonts/DIN.otf", 18, true, false, true );
    
	oculusRift.init( 1280, 800, 4 );
	oculusRift.setPosition( 0,-30,0 );

	lastUpdateTime = ofGetElapsedTimef();
    dif = false;
    grow = false;
    proc = false;
    spi = false;
    scat = true;
    
    // audio
    int bufferSize = 256;
    soundStream.setup(this, 0, 1, 44100, bufferSize, 4);
    left.assign(bufferSize, 0.0);
    
    // point light
    radius		= 180.f;
	center.set(ofGetWidth()*.5, ofGetHeight()*.5, 0);
    pointLight.setDiffuseColor( ofColor(255.f, 255.f, 255.f));
	pointLight.setSpecularColor( ofColor(255.f, 255.f, 255.f));
	pointLight.setPointLight();

}

//--------------------------------------------------------------
void ofApp::update(){
	float currTime = ofGetElapsedTimef();
	float frameDeltaTime = currTime - lastUpdateTime;
	lastUpdateTime = currTime;
	
	if(		 ofGetKeyPressed('i') ) { oculusRift.setInterOcularDistance( oculusRift.getInterOcularDistance() + 0.001f ); }
	else if( ofGetKeyPressed('o') ) { oculusRift.setInterOcularDistance( oculusRift.getInterOcularDistance() - 0.001f ); }
	else if( ofGetKeyPressed('k') ) { oculusRift.setInterOcularDistance( oculusRift.getInterOcularDistance() + 1.0f ); }
	else if( ofGetKeyPressed('l') ) { oculusRift.setInterOcularDistance( oculusRift.getInterOcularDistance() - 1.0f ); }
	
	if(	ofGetKeyPressed(OF_KEY_UP) )    { oculusRift.dolly(  30.0f * frameDeltaTime ); }
	if( ofGetKeyPressed(OF_KEY_DOWN) )  { oculusRift.dolly( -30.0f * frameDeltaTime ); }
	if( ofGetKeyPressed(OF_KEY_LEFT) )  { oculusRift.truck(  30.0f * frameDeltaTime ); }
	if( ofGetKeyPressed(OF_KEY_RIGHT) ) { oculusRift.truck( -30.0f * frameDeltaTime ); }
    
    pointLight.setPosition(100, 300, 100);

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
	oculusRift.beginRenderSceneLeftEye();
    ofEnableLighting();
    pointLight.enable();

    drawSceneGeometry();

    ofDisableLighting();
	ofSetColor( pointLight.getDiffuseColor() );
	//pointLight.draw();
    
    pointLight.disable();
	oculusRift.endRenderSceneLeftEye();
	
    /////////////////
	oculusRift.beginRenderSceneRightEye();
    ofEnableLighting();
    pointLight.enable();

    drawSceneGeometry();

    ofDisableLighting();
	ofSetColor( pointLight.getDiffuseColor() );
	//pointLight.draw();

    oculusRift.endRenderSceneRightEye();
	
	ofSetColor( 255 );
	oculusRift.draw( ofVec2f(0,0), ofVec2f( ofGetWidth(), ofGetHeight() ) );
    
    
}

//--------------------------------------------------------------
void ofApp::drawSceneGeometry()
{

 	//ofSetColor(120);
	ofSetColor(0, 255,255);
    
	ofPushMatrix();
    ofRotate(90, 0, 0, -1);
    ofDrawGridPlane(500.0f, 40.0f, false );
	ofPopMatrix();
	
    if (dif) sphereDiffuse();
    if (grow) sphereGrow();
    if (proc) sphereProceed();
    if (spi) sphereSpiral();
    if (scat) sphereScatter();

}

void ofApp::sphereDiffuse() {
    ofSetColor( 255, 255, 255 );
	ofPushMatrix();
    ofTranslate( ofPoint(0,27,0) );
    
    int objNum = 20;
    for( int i = 0; i < objNum; i ++ )
    {
        //ofSetColor(255, 190+ 65/objNum*i,3 + 252/objNum*i);
        //ofSetColor(255/objNum*i , 190/objNum*i,3/objNum*i);
        ofSetColor(0, 255/objNum*i,255/objNum*i);
        float tox = (2+ 100 * smoothedVol) * sin(PI*2 + 360/objNum*(i));
        float toz = (2+ 100 * smoothedVol) * cos(PI*2 + 360/objNum*(i));
        ofFill();
        //ofDrawBox( ofPoint(tox,0,toz), 5+200* smoothedVol);
        ofDrawSphere( ofPoint(tox,0,toz), 1);//+200* smoothedVol);
        //ofCircle(ofPoint(tox,0,toz), 1);//+200* smoothedVol);
    }
	ofPopMatrix();
    
}

void ofApp::sphereGrow() {
    ofSetColor( 255, 255, 255 );
	ofPushMatrix();
    ofSetColor(255,210,3);
    ofFill();
    ofDrawSphere( ofPoint(85,220,85), 80* smoothedVol);
	ofPopMatrix();
}

void ofApp::sphereProceed() {
    ofSetColor( 255, 255, 255 );
	ofPushMatrix();
    ofTranslate( ofPoint(0,25,0) );
    int objNum = 20;
    for( int i = 0; i < 20; i ++ )
    {
        ofSetColor(0, 255/objNum*i,255/objNum*i);
        float time = ofGetElapsedTimef() *0.5 + smoothedVol*2;
        //if (i%2==0) time *= -1;
        
        float tox = 2 + 10 * sin(PI*2 + 360/20*(i) + time);
        float toz = 2 + 10 * cos(PI*2 + 360/20*(i) + time);
        
        ofFill();
        ofDrawSphere( ofPoint(tox,0,toz), 1);
    }
	ofPopMatrix();
}

void ofApp::sphereSpiral() {
    ofSetColor( 255, 255, 255 );
	ofPushMatrix();
    ofTranslate( ofPoint(0,25,0) );
    int objNum = 100;
    for( int i = 0; i < 100; i ++ )
    {
        ofSetColor(127 + 128/objNum*i, 255,255);
        //float time = ofGetElapsedTimef() *0.1 + smoothedVol*2;
        float time = 0;
        //if (i%2==0) time *= -1;
        
        float effi = (1+i) * smoothedVol;
        
        float tox = 2 + 100/effi * sin(PI*2 + 360/100*effi);
        float toz = 2 + 100/effi * cos(PI*2 + 360/100*effi);
        float toy = 5 * effi;
        
        ofFill();
        ofDrawSphere( ofPoint(tox,toy,toz), 1);
    }
	ofPopMatrix();
}

void ofApp::sphereScatter() {
    ofSetColor( 255, 255, 255 );
	ofPushMatrix();
    ofTranslate( ofPoint(0,25,0) );
    int objNum = 20;
    for( int i = 0; i < 20; i ++ )
    {
        //ofSetColor(255/objNum*i, 255,255);
        //float time = ofGetElapsedTimef() *0.1 + smoothedVol*2;
        float time = 0;
        //if (i%2==0) time *= -1;
        
        float effi = (1+i) * smoothedVol;
        
        float tox = 2 + 100/effi * sin(PI*2 + 360/20*effi);
        float toz = 2 + 100/effi * cos(PI*2 + 360/20*effi);
        float toy = 10 * effi;
        
        ofFill();
        ofDrawSphere( ofPoint(tox,toy,toz), 1);
    }
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == '1') {
        dif = !dif;
    }
    if (key == '2') {
        grow = !grow;
    }

    if (key == '3') {
        proc = !proc;
    }

    if (key == '4') {
        scat = !scat;
    }

    if (key == '5') {
        spi = !spi;
    }
    if (key == '6') {
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

void ofApp::audioIn(float * input, int bufferSize, int nChannels){
	
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
