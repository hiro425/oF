#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){	
	
	ofSetBackgroundAuto(false);
	ofBackground(0, 0, 0);
	
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	
    ofDisableArbTex();
	ofLoadImage(texture, "dot.png");
    //ofLoadImage(texture, "image.jpg");
	
	for (int i = 0; i < 400; i++){
		particle myParticle;
        myParticle.setInitialCondition(
                                       ofRandom(ofGetWidth()/10*4,ofGetWidth()/10*6),
                                       ofRandom(ofGetHeight()/10*4,ofGetHeight()/10*6),
                                       ofRandom(-100,100),0,0,0);
		particles.push_back(myParticle);
        float origSize = ofRandom(5, 100);
        origSizes.push_back(origSize);
        
        if        (i < 400/3) colors.push_back(ofFloatColor(0.1, 0.5,1));
        else if (i < 400/3*2) colors.push_back(ofFloatColor(0.1,0.5,0.1));
        else                  colors.push_back(ofFloatColor(1,1,1));
        
        vector<ofVec3f> point;
        pastPoints.push_back(point);
	}
    
    // ---------------------------------------------------------
    cam.resetTransform();
	cam.setFov(60);
	cam.clearParent();
    cam.setPosition(ofGetWidth()/2, ofGetHeight()/2, 750);
    camAngle = 90.0;
    
    pointLight.setDiffuseColor( ofColor(255.f, 255.f, 255.f));
	pointLight.setSpecularColor( ofColor(255.f, 255.f, 255.f));
	pointLight.setPointLight();
    pointLight.setPosition(ofGetWidth()/2, ofGetHeight()/2, 0);
    
    // ---------------------------------------------------------
    fft.setup();
    fft.setNumFFTBins(32);
    lowD.thresh = 0.25;
    midD.thresh = 0.25;
    highD.thresh = 0.25;
    commonD.thresh = 5;
    
    // ---------------------------------------------------------
#ifdef TARGET_OPENGLES
    shader.load("shaders_gles/shader");
#else
    shader.load("shaders/shader");
#endif
}

//--------------------------------------------------------------
void testApp::update(){
	
	for (int i = 0; i < particles.size(); i++){
        
        float coheDist = particles[i].cohesion.distance;
        float sepeDist = particles[i].seperation.distance;
        float coheSt = particles[i].cohesion.strength;
        float sepeSt = particles[i].seperation.strength;
        
        if (
            (i < particles.size()/3 && lowD.bang)
            || (i >= particles.size()/3 && i < particles.size()/3*2 && midD.bang)
            || (i >= particles.size()/3*2 && highD.bang))
        {
            coheDist = 1;
            coheSt = 0.1;
            sepeDist = 200;
            sepeSt = 0.7;

        }
        else {
            sepeDist = 10;
            sepeSt = 0.1;
            coheDist = 100;
            coheSt = 0.5;
        }
        
        if (coheDist > 1000) coheDist = 1000;
        else if (coheDist < 1) coheDist = 1;
        if (sepeDist > 200) sepeDist = 200;
        else if (sepeDist < 1) sepeDist = 1;

        if (coheSt > 1) coheSt = 1;
        else if (coheSt < 0.1) coheSt = 0.1;
        if (sepeSt > 1) sepeSt = 1;
        else if (sepeSt < 0.1) sepeSt = 0.1;
        
        particles[i].cohesion.distance = coheDist;
		particles[i].alignment.distance = 50;
		particles[i].seperation.distance = sepeDist;
		particles[i].cohesion.strength = coheSt;
		particles[i].alignment.strength = 0.1;
		particles[i].seperation.strength = sepeSt;
		particles[i].damping = 0.01;
        
	}
	
	
	// on every frame 
	// we reset the forces
	// add in any forces on the particle
	// perfom damping and
	// then update

	for (int i = 0; i < particles.size(); i++){
		particles[i].resetForce();
	}
	for (int i = 0; i < particles.size(); i++){
		for (int j = 0; j < particles.size(); j++){
			if (i != j){
				particles[i].addForFlocking(particles[j]);
			}
		}
	}
	for (int i = 0; i < particles.size(); i++){
		particles[i].addFlockingForce();
		particles[i].addDampingForce();
		particles[i].update();
	}
	
	
	// wrap torroidally.
    /*
	for (int i = 0; i < particles.size(); i++){
		ofVec3f pos = particles[i].pos;
		if (pos.x < 0) pos.x = ofGetWidth();
		if (pos.x > ofGetWidth()) pos.x = 0;
		if (pos.y < 0) pos.y = ofGetHeight();
		if (pos.y > ofGetHeight()) pos.y = 0;
        if (pos.z < -300) pos.z = 300;
		if (pos.z > 300) pos.z = -300;
		particles[i].pos = pos;
        
	}
    */
    

    
    for (int i = 0; i < particles.size(); i++) {
        ofVec3f p = particles[i].pos;

        points.push_back(p);
        float size;
        
        if      (i < particles.size()/3)   size = origSizes[i] * lowD.vol;
        else if (i < particles.size()/3*2) size = origSizes[i] * midD.vol;
        else                               size = origSizes[i] * highD.vol;
 
        sizes.push_back(ofVec3f(size));
    }
    for (int i = 0; i < points.size(); i++) {
        pastPoints[i].push_back(points[i]);
        if (pastPoints[i].size() > 10) {
            pastPoints[i].erase(pastPoints[i].begin());
        }
        
    }
    vector<ofIndexType> faces;
    for (int i = 0; i < points.size(); i++) {
        faces.push_back(i);
    }
    
    vbo.setVertexData(&points[0], points.size(), GL_DYNAMIC_DRAW);
	vbo.setNormalData(&sizes[0], points.size(), GL_DYNAMIC_DRAW);
	vbo.setColorData(&colors[0], points.size(), GL_DYNAMIC_DRAW);
    //vbo.setIndexData(&faces[0], points.size(), GL_DYNAMIC_DRAW);

    cam.lookAt(ofVec3f(ofGetWidth()/2, ofGetHeight()/2, -0));
    if (rotateMode) {
        camAngle += 0.5;
        float radian = camAngle * PI / 180;
        cam.setPosition(
                        ofGetWidth()/2 + 750 * cos(radian),
                        cam.getPosition().y,
                        750 * sin(radian)
                        );
    }
    updateFFT();
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

void testApp::updateFFT() {
    fft.update();
    
    lowD.vol  = fft.getLowVal()  * 0.05;
    midD.vol  = fft.getMidVal()  * 0.05;
    highD.vol = fft.getHighVal() * 0.05;
    
    checkBang(lowD);
    checkBang(midD);
    checkBang(highD);
    
}

void testApp::checkBang(bandwidthData &d) {
    if((d.bang == false && d.vol > d.thresh )) {
        d.bang = true;
    }
    if(d.vol < d.thresh){
        d.bang = false;
    }
}
//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(0, 0, 0, 23);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
	
    glDepthMask(GL_FALSE);
	
	//ofSetColor(255, 100, 90);
    
    cam.begin();
    
    ofEnableAlphaBlending();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofEnableLighting();
    pointLight.enable();

    for (int i = 0; i<points.size(); i++) {
        if (sizes[i].x > 10) {
            ofVbo v;
            v.setVertexData(&pastPoints[i][0], pastPoints[i].size(), GL_DYNAMIC_DRAW);
            
            
            vector<ofIndexType> faces;
            for (int j = 0; j < pastPoints[i].size(); j++) {
                faces.push_back(j);
            }
            
            vector<ofFloatColor> vcolors;
            for (int j = 0; j < pastPoints[i].size(); j++) {
                vcolors.push_back(ofFloatColor(colors[i].r, colors[i].g, colors[i].b, 0.2));
            }
            
            v.setColorData(&vcolors[0], points.size(), GL_DYNAMIC_DRAW);
            
            ofSetLineWidth(sizes[i].x/10);
            v.setIndexData(&faces[0], pastPoints[i].size(), GL_DYNAMIC_DRAW);
            
            v.drawElements(GL_LINE_STRIP, pastPoints[i].size());
        }
    }
  
    ofEnablePointSprites();
    shader.begin();
    
    texture.bind();
	vbo.draw(GL_POINTS, 0, (int)points.size());
	texture.unbind();

    shader.end();
    ofDisablePointSprites();
	ofDisableBlendMode();
/*
    ofSetColor(0,255,255,50);
    for (int i = 0; i < pastPoints.size(); i++) {
        ofSetLineWidth(sizes[i].x/10);
        for (int j = 1; j < pastPoints[i].size(); j++) {
            ofLine(
                   pastPoints[i][j-1].x,
                   pastPoints[i][j-1].y,
                   pastPoints[i][j-1].z,
                   pastPoints[i][j].x,
                   pastPoints[i][j].y,
                   pastPoints[i][j].z);
        }
        
    }
  */
    

    
    points.clear();
    sizes.clear();
    
	/*
	for (int i = 0; i < particles.size(); i++){
        if (i < particles.size()/3) {
            ofSetColor(36,139,255);
        }
        else if (i < particles.size()/3*2) {
            ofSetColor(34,139,34);
        }
        else {
            ofSetColor((float)i/particles.size()*255,(float)i/particles.size()*255,255);
        }

		particles[i].draw();
	}
	*/
    
    
    pointLight.disable();
    ofDisableLighting();
    
    glDepthMask(GL_TRUE);
    
    cam.end();
    
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
	switch (key){
			
		case ' ':
			// reposition everything: 
			for (int i = 0; i < particles.size(); i++){
				particles[i].setInitialCondition(ofRandom(0,ofGetWidth()),ofRandom(0,ofGetHeight()),ofRandom(-100,100),0,0,0);
			}
			break;
        case 'z':
            rotateMode = !rotateMode;
            break;
	}
	
	
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){ 
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
	if (x < 500){
		particles.erase(particles.begin());
		particle myParticle;
		float randomAngle = ofRandom(0, TWO_PI);
		myParticle.setInitialCondition(x,y,0,cos(randomAngle),sin(randomAngle),0);
		particles.push_back(myParticle);
	}
	
	
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(){
	
}
