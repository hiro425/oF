#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackgroundHex(0x000000);
    cam.setDistance(100);
    
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            Verts[j * WIDTH + i].set(i-WIDTH/2, j-HEIGHT/2, 0);
            Color[j * WIDTH + i].set(0.5, 0.8, 1.0, 1.0);
        }
    }
    Vbo.setVertexData(Verts, NUM_PARTICLES, GL_DYNAMIC_DRAW);
    Vbo.setColorData(Color, NUM_PARTICLES, GL_DYNAMIC_DRAW);
}

//--------------------------------------------------------------
void ofApp::update(){
    for (int i = 0; i < WIDTH; i++) { // 頂点の座標を更新
        for (int j = 0; j < HEIGHT; j++) {
            float x = sin(i * 0.1 + ofGetElapsedTimef()) * 10.0;
            float y = sin(j * 0.15 + ofGetElapsedTimef()) * 10.0;
            float z = x + y;
            Verts[j * WIDTH + i] = ofVec3f(i - WIDTH/2, j - HEIGHT/2, z);
        }
    }
    Vbo.updateVertexData(Verts, NUM_PARTICLES);
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    cam.begin(); //カメラの回転が適用される範囲(始まり)
    
    glPointSize(2);
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    Vbo.draw(GL_POINTS, 0, NUM_PARTICLES);
    cam.end();  //カメラの回転が適用される範囲(終わり)
    ofPopMatrix();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
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
