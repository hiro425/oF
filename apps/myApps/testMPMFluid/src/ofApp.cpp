#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("OpenFrameworks MPM Fluid Demo");
	ofBackground(0,0,0);
	
	bEpsCapture = false;
	bFullscreen = false;
	bShowControlPanel = true;
	ofSetFullscreen(bFullscreen);
    
	fluid.setup(100000);
	scaleFactor = 6.4;
    
    setupControlPanel();
}

void ofApp::setupControlPanel(){
    
	gui.setup("MPM Fluid Demo", 10, 10, 350, 640);
	gui.addPanel(" MPM Simulation Parameters", 1);
	gui.addLabel("'f' to toggle fullscreen \n'h' to toggle this panel \n'p' to print postscript");
	
	gui.addSlider("# Particles",	"N_PARTICLES",		100000/4,  1000, 100000, true);
	gui.addSlider("Density",		"DENSITY",			5.0,	0, 30.0,	false);
	gui.addSlider("Stiffness",		"STIFFNESS",		0.5,	0, 2.0,		false);
	gui.addSlider("Bulk Viscosity",	"BULK_VISCOSITY",	3.0,	0, 10.0,	false);
	gui.addSlider("Elasticity",		"ELASTICITY",		1.0,	0, 4.0,		false);
	gui.addSlider("Viscosity",		"VISCOSITY",		1.0,	0, 4.0,		false);
	gui.addSlider("Yield Rate",		"YIELD_RATE",		1.0,	0, 2.0,		false);
	gui.addSlider("Gravity",		"GRAVITY",			0.002,	0, 0.02,	false);
	gui.addSlider("Smoothing",		"SMOOTHING",		1.0,	0, 3.0,		false);
	
	gui.addToggle("Do Obstacles?",	"DO_OBSTACLES",		true);
	
	// Now, this horizontal gradient in the Density parameter is just for yuks.
	// It demonstrates that spatial variations in the Density parameter can yield interesting results.
	// For an interesting experiment, try making Density proportional to the luminance of a photograph.
	gui.addToggle("Horizontal Density Gradient?",	"DENSITY_GRADIENT", false);
	
	// This can eliminate screen tearing.
	gui.addToggle("Vertical Sync?",					"VERTICAL_SYNC", true);
	
	gui.loadSettings("settings/controlPanelSettings.xml");
}

//--------------------------------------------------------------
void ofApp::update(){
	bool bSync = gui.getValueB("VERTICAL_SYNC", 0);
	ofSetVerticalSync (bSync);
    
	fluid.numParticles	 = gui.getValueI("N_PARTICLES", 0);
	fluid.densitySetting = gui.getValueF("DENSITY", 0);
	fluid.stiffness      = gui.getValueF("STIFFNESS", 0);
	fluid.bulkViscosity  = gui.getValueF("BULK_VISCOSITY", 0);
	fluid.elasticity     = gui.getValueF("ELASTICITY", 0);
	fluid.viscosity      = gui.getValueF("VISCOSITY", 0);
	fluid.yieldRate      = gui.getValueF("YIELD_RATE", 0);
	fluid.bGradient	     = gui.getValueB("DENSITY_GRADIENT", 0);
	fluid.bDoObstacles   = gui.getValueB("DO_OBSTACLES", 0);
	fluid.gravity		 = gui.getValueF("GRAVITY", 0);
	fluid.smoothing		 = gui.getValueF("SMOOTHING", 0);
    
	fluid.scaleFactor	= scaleFactor;
	
	fluid.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	fluid.draw();
    
	//-----------------
	outputPostscript();
}

void ofApp::outputPostscript(){
	// When the user presses 'p', save out the image as an EPS (PostScript) file.
	// Each particle is rendered as a small circle.
	
	if(bEpsCapture){
        //		epsOutput.beginEPS("fluid-output.ps");
        //		epsOutput.fill();
        //		epsOutput.setColor(0x000000);
        //		epsOutput.setLineWidth(0.04); //mm!
		
		int printAreaInMillimeters = 11.0 * 2.54 * 10.0;
		float pageScale = printAreaInMillimeters / (fluid.getGridSizeX()-5);
		ofxMPMParticle *p;
		
		for (int ip=0; ip<fluid.getParticles().size(); ip++) {
			p = fluid.getParticles()[ip];
			float x0 = p->x;
			float y0 = p->y;
			
			if ((x0>0) && (x0<fluid.getGridSizeX()) && (y0>0) && (y0<fluid.getGridSizeY())){
                //				epsOutput.circle (pageScale*x0, pageScale*y0, 0.1);
			}
			
		}
		
        //		epsOutput.endEPS();
		bEpsCapture = false;
		
	}
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
