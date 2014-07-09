#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    kinect.setRegistration(true);
    
	kinect.init();
	kinect.open();
	if(kinect.isConnected()) {
		ofLogNotice() << "kinect has been connected." << endl;
	}
    
    colorImg.allocate(kinect.width, kinect.height);
	grayImage.allocate(kinect.width, kinect.height);
	grayThreshNear.allocate(kinect.width, kinect.height);
	grayThreshFar.allocate(kinect.width, kinect.height);
	
	nearThreshold = 255;
	farThreshold = 230;
	bThreshWithOpenCV = true;
	
	ofSetFrameRate(60);
	
	// zero the tilt on startup
	angle = 0;
	kinect.setCameraTiltAngle(angle);
	
	// start from the front
    bDrawPointCloud = false;
    
    ofDisableArbTex(); // we need GL_TEXTURE_2D for our models coords.
    
    bAnimate = false;
    bAnimateMouse = false;
    animationPosition = 0;
    /*
    model.loadModel("astroBoy_walk.dae", true);
    model.setPosition(ofGetWidth() * 0.75, (float)ofGetHeight() * 0.75 , 0);
    model.setRotation(0, 125, .0, 1.0, .0);
    model.setLoopStateForAllAnimations(OF_LOOP_NORMAL);
    model.playAllAnimations();
    //if(!bAnimate) {
        model.setPausedForAllAnimations(true);
    //}
    models.push_back(&model);
     */
    //setupModel(model);
    setupSPK();
}


void ofApp::setupModel(ofxAssimpModelLoader &mod) {
    mod.loadModel("astroBoy_walk.dae", true);
    mod.setPosition(ofGetWidth() * 0.75, (float)ofGetHeight() * 0.75 , 0);
    mod.setRotation(0, 125, .0, 1.0, .0);
    mod.setLoopStateForAllAnimations(OF_LOOP_NORMAL);
    mod.playAllAnimations();
    //if(!bAnimate) {
    mod.setPausedForAllAnimations(true);
    //}
    models.push_back(&mod);
    
}

void ofApp::setupModel(ofxAssimpModelLoader *mod) {
    mod->loadModel("astroBoy_walk.dae", true);
    mod->setPosition(ofGetWidth() * 0.75, (float)ofGetHeight() * 0.75 , 0);
    mod->setRotation(0, 125, .0, 1.0, .0);
    mod->setLoopStateForAllAnimations(OF_LOOP_NORMAL);
    mod->playAllAnimations();
    //if(!bAnimate) {
    mod->setPausedForAllAnimations(true);
    //}
    models.push_back(mod);
    
}

void ofApp::addModel() {
    ofxAssimpModelLoader *mod = new ofxAssimpModelLoader;
    mod->loadModel("astroBoy_walk.dae", true);
    mod->setPosition(ofGetWidth() * 0.75, (float)ofGetHeight() * 0.75 , 0);
    mod->setRotation(0, 125, .0, 1.0, .0);
    mod->setLoopStateForAllAnimations(OF_LOOP_NORMAL);
    mod->playAllAnimations();
    //if(!bAnimate) {
    mod->setPausedForAllAnimations(true);
    //}
    models.push_back(mod);
    
}

void ofApp::setupSPK() {
    ofDisableArbTex();
	sprite.loadImage("image.jpg");
	ofEnableArbTex();
	
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(0);
	
	sys.setup();
	
	group.setup(sys);
	group.setColor(ofxSPK::RangeC(ofColor(255, 255), ofColor(255, 255)),
				   ofxSPK::RangeC(ofColor(0, 0), ofColor(255, 0)));
	
	group.setLifeTime(0.5, 2);
	group.setFriction(0.1);
	group.setSize(0, ofxSPK::RangeF(30, 100));
	
	group.setGravity(ofVec3f(0, -10, 0));
	group.setMass(0.1, 1);
	
	rot.setup(SPK::Vortex::create(SPK::Vector3D(ofGetWidth()/2, ofGetHeight()/2),
								  SPK::Vector3D(0, 1, 0),
								  200,
								  10), group);
	
	group.reserve(10000);
}

//--------------------------------------------------------------
void ofApp::update(){
    ofBackground(1, 1, 1);
	
	kinect.update();
	
	// there is a new frame and we are connected
	if(kinect.isFrameNew()) {
		
		// load grayscale depth image from the kinect source
		grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
		
		// we do two thresholds - one for the far plane and one for the near plane
		// we then do a cvAnd to get the pixels which are a union of the two thresholds
		if(bThreshWithOpenCV) {
			grayThreshNear = grayImage;
			grayThreshFar = grayImage;
			grayThreshNear.threshold(nearThreshold, true);
			grayThreshFar.threshold(farThreshold);
			cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
		} else {
			
			// or we do it ourselves - show people how they can work with the pixels
			unsigned char * pix = grayImage.getPixels();
			
			int numPixels = grayImage.getWidth() * grayImage.getHeight();
			for(int i = 0; i < numPixels; i++) {
				if(pix[i] < nearThreshold && pix[i] > farThreshold) {
					pix[i] = 255;
				} else {
					pix[i] = 0;
				}
			}
		}
		
		// update the cv images
		grayImage.flagImageChanged();
        grayImage.mirror(false, true);
		
		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayImage, 10, (kinect.width*kinect.height)/2, 20, false);
	}
    
    int count = 0;

    for(int y = 0; y < kinect.height; y++) {
		for(int x = 0; x < kinect.width; x++) {
			if(/*kinect.getDistanceAt(x, y) > 472 && kinect.getDistanceAt(x, y) < 600
               &&*/ (int)kinect.getColorAt(x, y).r < 10
               && (int)kinect.getColorAt(x, y).g > 100
               && (int)kinect.getColorAt(x, y).b < 10
               )
            {
                count++;
                particlePos.push_back(ofPoint(1024 - x*2, y*2));
                if (!bInProgress) {
                    bInProgress = true;
                    cout << "bang! " << kinect.getDistanceAt(1024-x, y) << endl;
                    ofxAssimpModelLoader *mod = new ofxAssimpModelLoader;
                    //addModel();
                    //setupModel(mod);
                    cout << "x: " << x << endl;
                    cout << "y: " << y << endl;
                    cout << "color" << kinect.getColorAt(x, y) << endl;
                }
			}

		}
	}
    if (count == 0) {
        bInProgress = false;
    }
    
    //cout << "distance " << least << endl;    
    
    updateModels();
    updateSPK();
    
}

void ofApp::updateModels() {
    //model.update();
    
    if (animationPosition >= 1.0) {
        animationPosition = 0.0;
    }
    // animationPosition = abs(sin(PI*2 + ofGetElapsedTimef()));
    animationPosition += 0.01;

    for (int i = 0; i < models.size(); i++) {
        models[i]->update();
        models[i]->setPosition(
                              models[i]->getPosition().x - 1,
                              models[i]->getPosition().y,
                              models[i]->getPosition().z
                              );
        models[i]->setScale(.3, .3, .3);
        models[i]->setPositionForAllAnimations(animationPosition);
    }
    
    //model.setPosition(model.getPosition().x - 1, model.getPosition().y, model.getPosition().z);
    
    //std::cout << "y: " << model.getPosition().y << endl;
    
    //if(bAnimateMouse) {
    //model.setPositionForAllAnimations(animationPosition);
    //}
    
    //mesh = model.getCurrentAnimatedMesh(0);
}

void ofApp::updateSPK() {
    //group.emitRandom(10, ofVec3f(ofGetMouseX(), ofGetMouseY()));
	
    for (int i = 0; i < particlePos.size(); i++) {
        group.emitRandom(2, ofVec3f(particlePos[i].x, particlePos[i].y));
    }
    
	sys.update();
    
	ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw() {
	
	ofSetColor(255, 255, 255);
	
	if(bDrawPointCloud) {
		easyCam.begin();
		drawPointCloud();
		easyCam.end();
	} else {
		// draw from the live kinect
		//kinect.drawDepth(10, 10, 1024, 768);
		//kinect.draw(420, 10, 400, 300);
		
		grayImage.draw(10, 10, 1024, 768);
		//contourFinder.draw(10, 320, 400, 300);
		
	}
	
	// draw instructions
    
	ofSetColor(255, 255, 255);
	stringstream reportStream;
    
    if(kinect.hasAccelControl()) {
        reportStream << "accel is: " << ofToString(kinect.getMksAccel().x, 2) << " / "
        << ofToString(kinect.getMksAccel().y, 2) << " / "
        << ofToString(kinect.getMksAccel().z, 2) << endl;
    } else {
        reportStream << "Note: this is a newer Xbox Kinect or Kinect For Windows device," << endl
		<< "motor / led / accel controls are not currently supported" << endl << endl;
    }
    
	reportStream << "press p to switch between images and point cloud, rotate the point cloud with the mouse" << endl
	<< "using opencv threshold = " << bThreshWithOpenCV <<" (press spacebar)" << endl
	<< "set near threshold " << nearThreshold << " (press: + -)" << endl
	<< "set far threshold " << farThreshold << " (press: < >) num blobs found " << contourFinder.nBlobs
	<< ", fps: " << ofGetFrameRate() << endl
	<< "press c to close the connection and o to open it again, connection is: " << kinect.isConnected() << endl;
    
    if(kinect.hasCamTiltControl()) {
    	reportStream << "press UP and DOWN to change the tilt angle: " << angle << " degrees" << endl
        << "press 1-5 & 0 to change the led mode" << endl;
    }
    
	ofDrawBitmapString(reportStream.str(), 20, 652);

    drawModels();
    drawSPK();
}

void ofApp::drawModels() {
    ofSetColor(255);
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
	ofEnableDepthTest();
    
    glShadeModel(GL_SMOOTH); //some model / light stuff
    light.enable();
    ofEnableSeparateSpecularLight();


    for (int i = 0; i < models.size(); i++) {
        ofPushMatrix();
        models[i]->drawFaces();
        ofPopMatrix();
    }
    //model.setScale(.3, .3, .3);
    //model.drawFaces();
 
    
    if(ofGetGLProgrammableRenderer()){
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
    }
    glEnable(GL_NORMALIZE);
    
    if(ofGetGLProgrammableRenderer()){
    	glPopAttrib();
    }
    
    ofDisableDepthTest();
    light.disable();
    ofDisableLighting();
    ofDisableSeparateSpecularLight();
}
void ofApp::drawSPK() {
    ofEnableBlendMode(OF_BLENDMODE_ADD);
	
	// sys.debugDraw();
	
	// bind texture, enable point sprite while drawing particles
	sprite.bind();
	ofEnablePointSprites();
	sys.draw();
	ofDisablePointSprites();
	sprite.unbind();
    particlePos.clear();
}

void ofApp::drawPointCloud() {
	int w = 640;
	int h = 480;
	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_POINTS);
	int step = 2;
	for(int y = 0; y < h; y += step) {
		for(int x = 0; x < w; x += step) {
			if(kinect.getDistanceAt(x, y) > 0) {
				mesh.addColor(kinect.getColorAt(x,y));
				mesh.addVertex(kinect.getWorldCoordinateAt(x, y));
			}
		}
	}
	glPointSize(3);
	ofPushMatrix();
	// the projected points are 'upside down' and 'backwards'
	ofScale(1, -1, -1);
	ofTranslate(0, 0, -1000); // center the points a bit
	ofEnableDepthTest();
	mesh.drawVertices();
	ofDisableDepthTest();
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
		case ' ':
			bThreshWithOpenCV = !bThreshWithOpenCV;
			break;
			
		case'p':
			bDrawPointCloud = !bDrawPointCloud;
			break;
			
		case '>':
		case '.':
			farThreshold ++;
			if (farThreshold > 255) farThreshold = 255;
			break;
			
		case '<':
		case ',':
			farThreshold --;
			if (farThreshold < 0) farThreshold = 0;
			break;
			
		case '+':
		case '=':
        case '/':
			nearThreshold ++;
			if (nearThreshold > 255) nearThreshold = 255;
			break;
			
		case '-':
			nearThreshold --;
			if (nearThreshold < 0) nearThreshold = 0;
			break;
			
		case 'w':
			kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
			break;
			
		case 'o':
			kinect.setCameraTiltAngle(angle); // go back to prev tilt
			kinect.open();
			break;
			
		case 'c':
			kinect.setCameraTiltAngle(0); // zero the tilt
			kinect.close();
			break;
			
		case '1':
			kinect.setLed(ofxKinect::LED_GREEN);
			break;
			
		case '2':
			kinect.setLed(ofxKinect::LED_YELLOW);
			break;
			
		case '3':
			kinect.setLed(ofxKinect::LED_RED);
			break;
			
		case '4':
			kinect.setLed(ofxKinect::LED_BLINK_GREEN);
			break;
			
		case '5':
			kinect.setLed(ofxKinect::LED_BLINK_YELLOW_RED);
			break;
			
		case '0':
			kinect.setLed(ofxKinect::LED_OFF);
			break;
			
		case OF_KEY_UP:
			angle++;
			if(angle>30) angle=30;
			kinect.setCameraTiltAngle(angle);
			break;
			
		case OF_KEY_DOWN:
			angle--;
			if(angle<-30) angle=-30;
			kinect.setCameraTiltAngle(angle);
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
