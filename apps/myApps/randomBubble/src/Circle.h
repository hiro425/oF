//
//  Circle.h
//  randomBubble
//
//  Created by 川口大志 on 2014/05/10.
//
//

#ifndef __randomBubble__Circle__
#define __randomBubble__Circle__

#include <iostream>
#include "ofMain.h"
#include "time.h"

class Circle {
    int num;
    vector<Circle> c_array;
    vector<Circle> b_array;
    float posx;
    float posy;
    float radius;
    int red;
    int green;
    int blue;
    float xspeed;
    float yspeed;
    int alph;
    bool checkTouching(Circle *ob);
    bool noFillFlg;
    //Circle *who;
    int bywho;
    static int idx;
    int id;
    time_t create_time;
public:
    Circle();
    //Circle(float x, float y, float r, Circle *c);
    Circle(float x, float y, float r, int w);
    void addCircles(int add_num);
    void drawCircles();
    void updatePos();
};

#endif /* defined(__randomBubble__Circle__) */
