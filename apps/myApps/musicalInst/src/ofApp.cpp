#include "ofApp.h"
#include "CustomRect.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofBackgroundHex(0x000000);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
 
    box2d.init();
    box2d.setGravity(0, 0);
    box2d.setFPS(30.0);
    
    particleImage.loadImage("emitter.png");
    dacImage.loadImage("particle.png");
    
    dac.fixture.filter.groupIndex = -1;
    dac.setup(box2d.getWorld(), ofGetWidth()/2, ofGetHeight()/2, 1,1);

}

//--------------------------------------------------------------
void ofApp::update(){
    box2d.update();
    
    for (int i = 0; i < particles.size(); i++) {
        particles[i]->update();
        particles[i]->addAttractionPoint(dac.getPosition(), 0.1);
        for (int j = i+1; j < particles.size(); j++) {
            particles[j]->addAttractionPoint(particles[i]->getPosition(), 0.001);
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    for(int i=0; i<particles.size(); i++) {
        ofVec2f p1 (particles[i]->getPosition());
        for (int j = i + 1; j < particles.size(); j++) {
            ofVec2f p2 (particles[j]->getPosition());
            float dist = p2.distance(p1);
            if (dist < 300) {
                int col = (300 - dist);
                ofSetColor(255, 255, 255, col);
                ofLine(p1.x, p1.y, p2.x, p2.y);
            }
        }
    }
    ofSetColor(255, 255, 255);
    for(int i=0; i<particles.size(); i++) {
        float radius = particles[i]->radius;
        particleImage.draw(particles[i]->getPosition() - radius / 2.0, radius, radius);
        ofLogError("ofxOscMessage") << "[i]" << i << " desu";
        ofLogError("ofxOscMessage") << "particles[i]->getPosition() " << particles[i]->getPosition() << " desu";
    }
    float allAmp = 0;
    for (int i = 0; i < particles.size(); i++) {
        allAmp += particles[i]->amp;
    }
    float dacRadius = sin(ofGetElapsedTimef() * 100.0) * allAmp * 20.0 + allAmp * 40.0 + 40.0;
    ofVec2f offset(dacRadius, dacRadius);
    dacImage.draw(dac.getPosition() - offset, dacRadius * 2.0, dacRadius * 2.0);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    switch (key) {
        case 'f':
            ofToggleFullscreen();
            break;
        case 'c':
            for (int i = 0; i < particles.size(); i++) {
                particles[i]->destroy();
            }
            particles.clear();
    }
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    /*CustomRect rect(particles.size());
    rect.setPhysics(10.0, 0.1, 0.5);
    rect.fixture.filter.groupIndex = -1;
    rect.setup(box2d.getWorld(), mouseX, mouseY, 10, 10);
    particles.push_back(rect);

    
    ofPtr<CustomRect> rect = ofPtr<CustomRect>(new CustomRect)(particles.size());
    rect.get()->setPhysics(10.0,0.1,0.5);
    rect.get()->fixture.filter.groupIndex=-1;
    rect.get()->setup(box2d.getWorld(),mouseX,mouseY,10,10);
    particles.push_back(rect);
    */
    particles.push_back(ofPtr<CustomRect>(new CustomRect(particles.size())));
    particles.back().get()->setPhysics(10.0,0.1,0.5);
    particles.back().get()->fixture.filter.groupIndex=-1;
    particles.back().get()->setup(box2d.getWorld(),mouseX,mouseY,10,10);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    dac.setPosition(ofGetWidth()/2, ofGetHeight()/2);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
