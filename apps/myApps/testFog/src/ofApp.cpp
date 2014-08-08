#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    gui.setup("fog control"); // most of the time you don't need a name
    
	gui.add(density.setup( "density", 0.0035f, 0.0f , 0.5f ));
	gui.add(fog_start.setup( "fog start", 10.0f, 0.0f, 255.0f ));
	gui.add(fog_end.setup( "fog end", 50.0f, 0.0f, 255.0f ));
	gui.add(r.setup( "redColor", 0.1f, 0, 1 ));
	gui.add(g.setup( "greenColor", 0.2f, 0, 1 ));
	gui.add(b.setup( "blueColor", 0.5f, 0, 1 ));
	gui.add(fogFilter.setup( "switch fog",1, 0,2));
    
    
    ofDisableArbTex();
    
    ofEnableLighting();
    
    fogShader.load("shaders/FogShaderTex");
    
    color.loadImage("textures/pond1.jpg");
    
    fogShader.begin();
    
    
    fogShader.setUniformTexture( "baseMap", color, color.getTextureReference().getTextureData().textureID );
    
    fogShader.end();
    
    
    bHide = true;
    fog.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
    fogColor.set(r,g,b) ;//= {r,g,b,1.0f};   // Fog Color
    
    
    fog.setfogColor(fogColor);
    fog.setFogMode(fogFilter);
    fog.setDensity(density);
    fog.setFogStartEnd(fog_start,fog_end);
    fog.enableCoordinate(true);
    fog.enable();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofEnableLighting();
    
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);                       // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                        // Type of Depth Test to perform
    
    
    
    glShadeModel(GL_SMOOTH);
    
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    cam.begin();
    fogShader.begin();
    
    
    ofBox(20);
    
    
    fogShader.end();
    
    cam.end();
    
    
    ofDisableLighting();
    glDisable(GL_DEPTH_TEST);
    
    
    
    if( bHide ){
        
        fog.disable();
		gui.draw();
        
    }
    
    ofSetColor(220,10,0);
    ofDrawBitmapString("I'm a string not coloured by the fog!", 10,720);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if( key == 'h' ){
		bHide = !bHide;
	}
	if(key == 's') {
		gui.saveToFile("settings.xml");
	}
	if(key == 'l') {
		gui.loadFromFile("settings.xml");
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
