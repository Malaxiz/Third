
//
//  CSprite.h
//  Third
//
//  Created by Didrik Munther on 24/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CSprite__
#define __Third__CSprite__

#include <stdio.h>
#include <iostream>

#include "Define.h"


class CWindow;
class CCamera;
class CSpriteSheet;
enum RenderFlags;

class CSprite {
    
public:
    CSprite(CSpriteSheet* spriteSheet, Box rect);
    
    virtual CSpriteSheet* getSpriteSheet() { return _spriteSheet; }
    Box* getSource() { return &_source; }
    
    virtual void onRender(Box destination, bool flip, int angle, int alpha, CWindow* window, CCamera* camera, RenderFlags renderFlags);
    
private:
    CSpriteSheet* _spriteSheet; // Don't modify from here
    Box _source;
    
};

#endif /* defined(__Third__CSprite__) */
