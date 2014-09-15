#include "ofApp.h"
#include <dirent.h>

//--------------------------------------------------------------
void ofApp::setup(){
    int bufferSize = 256;
    soundStream.setDeviceID(5);
    soundStream.setup(this, 0, 3, 44100, bufferSize, 4);
    //soundStream.listDevices();
    
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    
    left.assign(bufferSize, 0.0);
    
    datadir = "/Users/kawaguchihiroshi/Developer/git/oF/apps/myApps/shaderSelecter/bin/data/shaders/";
    setupShader();
}

void ofApp::setupShader() {
    presetDir   = "";
    shaderMode  = "";
    prevMode    = "";
    //shaderContents(shaderMode);
    listIndex = 0;
    loadShader();
}

void ofApp::loadShader() {
    shaderContents(shaderMode);
    cout << "size : " << fragContentsList.size() << endl;
    if (listIndex < 0){
        listIndex = fragContentsList.size()-1;
    }
    else if (listIndex > (fragContentsList.size()-1) ) {
        listIndex = 0;
    }
    fragFile = datadir + presetDir + fragContentsList[listIndex];
    
    ofFile file(fragFile);
    if(!file.exists()){
        ofLogError("The file " + fragFile + " is missing");
        setupShader();
    }
    cout << "frag index :" << fragFile << "\n";
    shader1.load("shaders/test.vert", fragFile);
}
//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofEnableAlphaBlending();
    drawShader();
}

void ofApp::drawShader() {
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
    shader1.setUniform1f("vol2", smoothedVol2);
    shader1.setUniform1f("vol3", smoothedVol3);
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
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
	
	float curVol = 0.0;
    float curVol2 = 0.0;
	float curVol3 = 0.0;
    
	// samples are "interleaved"
	int numCounted = 0;
    
	//lets go through each sample and calculate the root mean square which is a rough way to calculate volume
    
	for (int i = 0; i < bufferSize; i++){
        left[i]		= input[i] * 5;//*0.5;
        if (i%3==0) {
            curVol += left[i] * left[i];
            numCounted++;
        }
        else if (i%3==1) {
            curVol2 += left[i] * left[i];
        }
        else if (i%3==2) {
            curVol3 += left[i] * left[i];
        }
     
        
	}
	
	//this is how we get the mean of rms :)
	curVol /= (float)numCounted;
	curVol2 /= (float)numCounted;
    curVol3 /= (float)numCounted;
	
    // this is how we get the root of rms :)
	curVol  = sqrt( curVol );
	curVol2 = sqrt( curVol2 );
    curVol3 = sqrt( curVol3 );
    
	smoothedVol *= 0.93;
	smoothedVol += 0.07 * curVol;
	
    smoothedVol2 *= 0.93;
	smoothedVol2 += 0.07 * curVol2;
    
    smoothedVol3 *= 0.93;
	smoothedVol3 += 0.07 * curVol3;
    
	bufferCounter++;
	
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == OF_KEY_UP) {
        listIndex++;
        loadShader();
    }
    if (key == OF_KEY_DOWN) {
        listIndex--;
        loadShader();
    }
    if( key == '1' ) shaderMode  = "Intro";
}

void ofApp::shaderContents(string s) {
    
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
    mapShaderType.insert(make_pair(
                                   "Test", 9
                                   ));
    
    int type_id = mapShaderType[s];
    
    fragContentsList.clear();
    
    string loaddir;
    char c[256];
    switch (type_id) {
        case 1:
            presetDir = "pjt/";
            break;
        case 2:
            presetDir = "move/";
            break;
        case 3:
            presetDir = "particles/";
            break;
        case 4:
            presetDir = "weird/";
            break;
        case 5:
            presetDir = "cells/";
            break;
        case 6:
            presetDir = "nature/";
            break;
        case 9:
            presetDir = "test/";
            break;
        default:
            presetDir = "";
            break;
            
    }
    
    DIR* dp;
    struct dirent* dent;
    
    loaddir = datadir + presetDir;
    strcpy(c, loaddir.c_str());
    dp = opendir(c);
    if (dp!=NULL) {
        do{
            dent = readdir(dp);
            if (dent!=NULL && strstr(dent->d_name, ".frag")) {
                fragContentsList.push_back(dent->d_name);
            }
        } while (dent!=NULL);
        closedir(dp);
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
