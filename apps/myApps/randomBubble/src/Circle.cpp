//
//  Circle.cpp
//  randomBubble
//
//  Created by 川口大志 on 2014/05/10.
//
//

#include "Circle.h"
int Circle::idx;

Circle::Circle() {
    posx   = ofRandom(ofGetWidth() / 2);
    posy   = ofRandom(ofGetHeight() / 2);
    radius = ofRandom(100);
    red    = ofRandom(255);
    green  = ofRandom(255);
    blue   = ofRandom(255);
    xspeed = ofRandom(20) - 10;
    yspeed = ofRandom(10) - 10;
    alph   = 80;
    noFillFlg = false;
    Circle::idx += 1;
    id = idx;
    create_time = time(NULL);
}

Circle::Circle(float x, float y, float r, int w) {
    posx   = x;
    posy   = y;
    radius = r;
    red    = 0;
    green  = 0;
    blue   = 0;
    xspeed = 0;
    yspeed = 0;
    alph   = 10;//ofRandom(80);
    noFillFlg = true;
    Circle::idx += 1;
    id = idx;
    bywho = w;
    create_time = time(NULL);
}

void Circle::addCircles(int add_num) {
    for (int i = 0; i < add_num; i++) {
        Circle c;
        c_array.push_back(c);
    }
    
}

void Circle::drawCircles() {
    for (int i = 0; i < c_array.size(); i++) {
        Circle *c = &c_array[i];
        
        ofFill();
        ofSetColor(c->red, c->green, c->blue, c->alph);
        //ofCircle(c->posx, c->posy, c->radius);
        
        for (int j = 0; j < c->c_array.size(); j++) {
            Circle *cj = &c->c_array[j];
            if (cj->noFillFlg) {
                ofNoFill();
            }
            ofSetLineWidth(ofRandom(0.01, 0.1));
            ofSetColor(cj->red, cj->green, cj->blue, cj->alph);
            ofCircle(cj->posx, cj->posy, cj->radius);
        }
    }
}

float customRandom() {
    return 1 - pow(ofRandom(1), 5);
}

void Circle::updatePos() {
    for (int i = 0; i < c_array.size(); i++) {
        Circle *c = &c_array[i];
        
        c->posx += (ofNoise(customRandom()) * c->xspeed * customRandom());
        c->posy += (ofNoise(customRandom()) * c->yspeed * customRandom());
        
        
        
        if (c->posx > ofGetWidth()) {
            //c->posx = ofGetWidth();
            //c->xspeed *= -1;
            c->posx = 0;
        }
        else if (c->posx < 0) {
            c->posx = ofGetWidth();
            //c->posx = 0;
            //c->xspeed *= -1;
        }
        
        if (c->posy > ofGetHeight()) {
            c->posy = 0;
            //c->posy = ofGetHeight();
            //c->yspeed *= -1;
        }
        else if (c->posy < 0) {
            c->posy = ofGetHeight();
            //c->posy = 0;
            //c->yspeed *= -1;
        }
        
        if (checkTouching(c)) {
            if (c->alph > 0) {
                c->alph--;
            }
        }
        else if (c->alph < 255) {
                c->alph +=2;
        }
        /*
        if (c->alph == 0) {
            
            for (int j = 0; j < c_array.size(); j++) {
                Circle *cj = &c_array[j];
                for (int k = 0; k < cj->c_array.size(); k++) {
                    if(cj->c_array[k].bywho == c->id) {
                        vector<Circle>::iterator p1 = cj->c_array.begin() + k;
                        cj->c_array.erase(p1);
                    }
                }
            }
                        c->c_array.clear();
            vector<Circle>::iterator p2 = c_array.begin() + i;
            c_array.erase(p2);
            //c_array.clear();
        }*/
        
        if (c->create_time && time(NULL) - c->create_time > 2) {
            
            for (int j = 0; j < c_array.size(); j++) {
                Circle *cj = &c_array[j];
                for (int k = 0; k < cj->c_array.size(); k++) {
                    if(cj->c_array[k].bywho == c->id) {
                        vector<Circle>::iterator p1 = cj->c_array.begin() + k;
                        cj->c_array.erase(p1);
                    }
                }
            }
            c->c_array.clear();
            vector<Circle>::iterator p2 = c_array.begin() + i;
            c_array.erase(p2);
            //c_array[i].clear();
        }
        
    }
}

bool Circle::checkTouching(Circle *ob) {
    bool touching = false;
    for (int i = 0; i < c_array.size(); i++) {
        Circle *c = &c_array[i];
        
        if (ob != c) {
            float distance = ofDist(ob->posx, ob->posy, c->posx, c->posy);
            float overlap  = distance - ob->radius - c->radius;
            
            if (overlap < 0) {
                //return true;
                //cout << "distance : " << distance << "\n";
                
                float midx, midy;
                midx = (ob->posx + c->posx) / 2;
                midy = (ob->posy + c->posy) / 2;
                
                float r, g, b;
                r = g = b = 0;
                
                overlap *= -0.5;
                ///*  // ### MODE CHANGE2 ###
                for (int j = 0; j < ob->c_array.size(); j++) {
                    if (ob->c_array[j].bywho == c->id
                        // && ob->id < c->id  // ### MODE CHANGE1 ###
                    ) {
                        ob->c_array[j].posx = midx;
                        ob->c_array[j].posy = midy;
                        ob->c_array[j].radius = overlap;

                    }
                    
                }//*/  // ### MODE CHANGE2 ###
                Circle blcCircle(midx, midy, overlap, c->id);
                ob->c_array.push_back(blcCircle);
                
                touching = true;
            }
            // 重ならなくなったあとのゴミ処理
            else {
                for (int j = 0; j < ob->c_array.size(); j++) {
                    if (ob->c_array[j].bywho == c->id) {
                        vector<Circle>::iterator p = ob->c_array.begin() + j;
                        ob->c_array.erase(p);
                        //cout << "id : " << c->id << "deleted\n";
                    }
                    
                }
            }
        }
    }
    return touching;
}

