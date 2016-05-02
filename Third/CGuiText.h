//
//  CGuiText.h
//  Third
//
//  Created by Didrik Munther on 22/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CGuiText__
#define __Third__CGuiText__

#include <stdio.h>
#include <string>

#include "Define.h"


class CWindow;
class CCamera;

class CGuiText {
public:
    CGuiText(int x, int y, std::string text, std::string fontKey);
    
    virtual void onLoop() {  }
    virtual void onRender(CWindow* window, CCamera* camera, RenderFlags renderFlags) {  }
    
    bool toRemove() { return _toRemove; }
    
protected:
    std::string _fontKey;
    std::string _text;
    int _x, _y;
    
    int _livingTime;         // Seconds
    int _creationTime;
    
    bool _toRemove;
};

#endif /* defined(__Third__CGuiText__) */
