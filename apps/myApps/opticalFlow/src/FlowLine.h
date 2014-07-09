//
//  FlowLine.h
//  opticalFlow
//
//  Created by 川口大志 on 2014/06/22.
//
//

#ifndef __opticalFlow__FlowLine__
#define __opticalFlow__FlowLine__

#include "ofMain.h"

struct FlowPoint{
    ofVec3f pos;
    ofVec3f color;
};

//軌跡を描画するためのクラス
class FlowLine{
    
public:
    ofVec3f pos;//軌跡の先頭位置
    ofVec3f color;//軌跡の先頭の色
    deque<FlowPoint> points;//軌跡を構成する点列
    float alpha;//アルファ値
    float rise_speed;//上昇スピード
    
    FlowLine() { rise_speed=ofRandom(0.5,5); }
    
    void update(){
        //軌跡の先頭に現在のポイントを追加
        FlowPoint p;
        //平滑化してる　2割ぐらい一つ前の値をいれるかな？
        p.pos = (points.front().pos*0.2 + pos*0.8);
        
        p.color = (points.front().color*0.2 + color*0.8);
        
        points.push_front(p);
        
        if (points.size() > 2){
            for(int i=1;i<points.size();i++){
                //軌跡がだんだん登って行くように
                points[i].pos.y -= rise_speed;
                
                // 線をなめらかに
                //この平滑化をコメントアウトすると、線分が細切れになる
                //points[i-i].pos = points[i].pos*0.6+points[i-1].pos*0.4;
                
            }
        }
        //軌跡の長さを制限する
        if(points.size()>100)
            points.pop_back();
        //アルファ値をだんだん小さくする
        alpha += -alpha *0.1;
    }
    
    void drow(){
        //軌跡の描画　//OpenGLの関数で描画
        glBegin(GL_LINE_STRIP);
        
        for (int i=0; i<points.size(); i++) {
            FlowPoint &p =points[i];
            float a=1- (float)i / (float)points.size();//だんだん透明に
            glColor4f(p.color.x, p.color.y, p.color.z, a*alpha);
            glVertex2f(p.pos.x,p.pos.y); //(p.pos.v);
            
        }
        glEnd();
    }
    
    //アルファ値が十分に小さいときに消去するためのフラグ用の関数
    bool alive(){
        return alpha > 0.05;
    }
};

#endif /* defined(__opticalFlow__FlowLine__) */
