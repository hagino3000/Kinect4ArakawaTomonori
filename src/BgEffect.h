/*
 *  BgEffect.h
 *  arakawa1
 *
 *  Created by Takashi on 11/01/13.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _BG_EFFECT
#define _BG_EFFECT

#include "ofMain.h"

class BgEffect{
	
public:
    //コンストラクタ
    BgEffect(int direction, float position);
    //デストラクタ
    ~BgEffect();
    //半径の更新
    void update();
    //円を描く
    void draw();
    //アクセサ
    void setPos(float pos);
	//
    float speed;
    //生死の判定
    bool dead;
    //寿命
    float lifeLength;
	
private:
    //プロパティはprivateで宣言
    ofPoint startPos;
	ofPoint endPos;
	int length;
	
	int direction; //0:horizontal, 1:vartical
};

#endif