//
//  CBody.h
//  Third
//
//  Created by Didrik Munther on 19/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CBody__
#define __Third__CBody__

#include <stdio.h>

#include "Define.h"

class CBody {
    
public:
    CBody(Box rect);
    void onLoop();
    
    float velX, velY;
    
    int getX();
    int getY();
    int getW();
    int getH();
    
    void setBox(int x, int y, int w, int h) {
        _rect.x = x;
        _rect.y = y;
        _rect.w = w;
        _rect.h = h;
    }
    
    void setPosition(int x, int y) {
        setBox(x, y, _rect.w, _rect.h);
    }
    
    void setDimension(int w, int h) {
        setBox(_rect.x, _rect.y, w, h);
    }
    
    Position getCenter() { return Position{getX() + getW() / 2, getY() + getH() / 2}; }
    
    Box _rect;
    Box _previousRect;
    
};

#endif /* defined(__Third__CBody__) */
