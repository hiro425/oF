#include "ofApp.h"







//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(12);
    
    vidGrabber.setDeviceID(0);
    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(640,480);
    
    colorImg.allocate(640,480);
	grayImage.allocate(640,480);
	prevGrayImage.allocate(320,240);
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofBackground(50, 0);
    
    ofDisableArbTex(); // we need GL_TEXTURE_2D for our models coords.
    
    bAnimate = false;
    bAnimateMouse = false;
    animationPosition = 0;
    
    model.loadModel("astroBoy_walk.dae", true);
    model.setPosition(ofGetWidth() * 0.75, (float)ofGetHeight() * 0.75 , 0);
    model.setRotation(0, 125, .0, 1.0, .0);
    model.setLoopStateForAllAnimations(OF_LOOP_NORMAL);
    model.playAllAnimations();
    //if(!bAnimate) {
        model.setPausedForAllAnimations(true);
    //}
    
    /*
    kinect.setRegistration(true);
	kinect.init();
	kinect.open();
    */
}

//--------------------------------------------------------------
void ofApp::update(){
	ofBackground(100,100,100);
    
    vidGrabber.update();
    bNewFrame = vidGrabber.isFrameNew();
    
	if (bNewFrame){
        grayImage.resize(640, 480);
        prevGrayImage.resize(640, 480);
        
        colorImg.setFromPixels(vidGrabber.getPixels(), 640,480);

        //オプティカルフロー用のサイズ
        CvSize VIDEO_SIZE=cvSize(640.0,480.0);
        
        int count=150;//検出するポイントの最大数
        
        static IplImage *eig=cvCreateImage(VIDEO_SIZE, IPL_DEPTH_32F, 1);
        static IplImage *temp=cvCreateImage(VIDEO_SIZE, IPL_DEPTH_32F, 1);
        
        static CvPoint2D32f *corners1=(CvPoint2D32f*)cvAlloc(count *sizeof(CvPoint2D32f));
        static CvPoint2D32f *corners2=(CvPoint2D32f*)cvAlloc(count *sizeof(CvPoint2D32f));
        
        static IplImage *prev_pyramid = cvCreateImage(cvSize(640.0+8, 480.0 / 3), IPL_DEPTH_8U, 1);
        static IplImage *curr_pyramid = cvCreateImage(cvSize(640.0+8, 480.0 / 3), IPL_DEPTH_8U, 1);
        
        static char *status =(char*)cvAlloc(count);
        
        //グレースケール変換
        grayImage = colorImg;
        
        //特徴点を抽出
        float block_size= 10;//検出するポイント間の最短距離
        
        //　コーナー検出
        cvGoodFeaturesToTrack(
                              grayImage.getCvImage(),
                              //prevGrayImage.getCvImage(),
                              eig,
                              temp,
                              corners1,
                              &count,
                              0.001,
                              block_size
                              );

        //grayImageとprevGrayImageについてオプティカルフロー
        cvCalcOpticalFlowPyrLK(
                               grayImage.getCvImage(),
                               prevGrayImage.getCvImage(),
                               //grayImage.getCvImage(),
                               curr_pyramid,
                               prev_pyramid,
                               //curr_pyramid,
                               corners1,
                               corners2,
                               count,
                               cvSize(10, 10),
                               4,
                               status,
                               NULL,
                               cvTermCriteria(CV_TERMCRIT_EPS|CV_TERMCRIT_ITER, 64, 0.01),
                               0
                               );
        
        
        //grayImageをprevGrayImageにコピー
        prevGrayImage=grayImage;
        
        grayImage.resize(320, 240);
        prevGrayImage.resize(320, 240);
        
        //検出できたフローに対するループ
        for (int i=0; i<count; i++) {
            if(status[i]){
                //始点
                ofVec2f to =ofVec2f(corners1[i].x,corners1[i].y);
                //終点
                ofVec2f from =ofVec2f(corners2[i].x,corners2[i].y);
                //std::cout << "corner2[" << i << "].x : " << corners2[i].x << endl;
                //取得したフローで何かしらの処理をする
                updateFlowPoint(to,from);
            }
        }
        
        //FlowLineの更新　削除
        vector<FlowLine*>::iterator it= flow_lines.begin();
        while (it != flow_lines.end()) {
            FlowLine *line = *it;
            //更新
            line->update();
            
            //オブジェクトの存在チェック
            if(line->alive() == false){
                it =flow_lines.erase(it);
                delete line;
            }else{
                it++;
            }
        }
        
	}
    updateModel();
    
}

void ofApp::updateModel() {
    model.update();
    
   // animationPosition = abs(sin(PI*2 + ofGetElapsedTimef()));
    
    if (animationPosition >= 1.0) {
        animationPosition = 0.0;
    }
    
    animationPosition += 0.1;
    model.setPosition(model.getPosition().x - 5, model.getPosition().y, model.getPosition().z);
    
    std::cout << "y: " << model.getPosition().y << endl;
    
    //if(bAnimateMouse) {
        model.setPositionForAllAnimations(animationPosition);
    //}
    
    mesh = model.getCurrentAnimatedMesh(0);
    
    
}


//一番近い点を持ったオブジェクトを探すためにソートをかける関数オブジェクト
struct sort_by_distance {
    sort_by_distance(ofVec2f pos){
        this->pos =pos;
    }
    bool operator()(const FlowLine* a, const FlowLine* b){
        float len_a = (a->pos - pos).lengthSquared();
        float len_b = (b->pos - pos).lengthSquared();
        return len_a < len_b;
    }
    
    ofVec2f pos;
};


void ofApp::updateFlowPoint(ofVec2f to, ofVec2f from){
    //始点と終点のベクトル距離を求める
    float len=(from -to).length();
    
    //距離がいい感じだったら
    if(len > 1 && len <50){
        //終点周辺のピクセルの色を取得
        CvScalar c =cvGet2D(colorImg.getCvImage() , (int)to.y, (int)to.x );
        
        //周囲の色を取得する
        //ofVec4f color = ofVec4f(c.val[0]/255.0f, c.val[1]/255.0f);
        ofVec4f color = ofVec4f(ofVec2f(c.val[0]/255.0f, c.val[1]/255.0f));
        
        //temp_linesの中のオブジェクトを始点に近い順にソート
        sort(flow_lines.begin(), flow_lines.end(), sort_by_distance(from));
        
        if(flow_lines.empty()||(flow_lines[0]->pos -from ).length()>30){
            //temp_linesがからか、支店の近くにオブジェクトがない場合新しく追加
            FlowLine *line = new FlowLine();
            line->pos = to;
            line->alpha = 0;
            line->color = color;
            FlowPoint point;
            point.pos = to;
            point.color=color;
            line->points.push_back(point);
            flow_lines.push_back(line);
            
        }
        else{
            //近いオブジェクトが見つかったので色や位置を更新
            FlowLine *line = flow_lines[0];
            line->color = color;
            line->pos = to;
            line->alpha += (1 - line->alpha) * 0.1;
            
        }
    }
}


//--------------------------------------------------------------
void ofApp::draw(){
    
	// draw the incoming, the grayscale, the bg and the thresholded difference
	ofSetHexColor(0xffffff);
	colorImg.draw(0,0);
	//grayImage.draw(720,20);

    
    //FlowLineの描画
    vector<FlowLine*>::iterator it= flow_lines.begin();
    while (it != flow_lines.end()) {
        FlowLine *line = *it;
        //更新
        line->drow();
        it++;
        
    }
    
    drawModel();
}

void ofApp::drawModel() {
    ofSetColor(255);
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
	ofEnableDepthTest();
    
    glShadeModel(GL_SMOOTH); //some model / light stuff
    light.enable();
    ofEnableSeparateSpecularLight();
    
    
    model.setScale(.3, .3, .3);
    ofPushMatrix();
    //ofTranslate(model.getPosition().x+100, model.getPosition().y, 0);
    //ofRotate(-mouseX, 0, 1, 0);
    //ofTranslate(-model.getPosition().x, -model.getPosition().y, 0);
    
    model.drawFaces();
    ofPopMatrix();
    
    if(ofGetGLProgrammableRenderer()){
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
    }
    glEnable(GL_NORMALIZE);
    /*
    ofPushMatrix();
    ofTranslate(model.getPosition().x-300, model.getPosition().y, 0);
    //ofRotate(-mouseX, 0, 1, 0);
    ofTranslate(-model.getPosition().x, -model.getPosition().y, 0);
    
    ofxAssimpMeshHelper & meshHelper = model.getMeshHelper(0);
    
    ofMultMatrix(model.getModelMatrix());
    ofMultMatrix(meshHelper.matrix);
    
    ofMaterial & material = meshHelper.material;
    if(meshHelper.hasTexture()){
        meshHelper.getTexturePtr()->bind();
    }
    material.begin();
    //mesh.drawWireframe();
    material.end();
    if(meshHelper.hasTexture()){
        meshHelper.getTexturePtr()->unbind();
    }
    ofPopMatrix();
    */
    if(ofGetGLProgrammableRenderer()){
    	glPopAttrib();
    }
    
    ofDisableDepthTest();
    light.disable();
    ofDisableLighting();
    ofDisableSeparateSpecularLight();

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