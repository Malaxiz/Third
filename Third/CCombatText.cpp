//
//  CCombatText.cpp
//  Fifth
//
//  Created by Didrik Munther on 02/06/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CCombatText.h"
#include "NSurface.h"
#include "CText.h"


CCombatText::CCombatText(int x, int y, Color color, int size, std::string text, std::string fontKey) : CGuiText(x, y, text, fontKey) {
    
    _livingTime = 2;
    
    int variator = 100;
    int randomPosX = rand() % variator - variator / 2;
    int randomPosY = rand() % variator - variator / 2;
    _x += randomPosX;
    _y += randomPosY;
    
    _textObj = new CText(text, size, fontKey, color);
}

void CCombatText::onLoop() {
    if(SDL_GetTicks() > _creationTime + _livingTime * 1000)
        _toRemove = true;
    _y--;
    _x += floor(cos(_y * 25) * 2); // Use _y as an incrementor here
}

void CCombatText::onRender(CWindow* window, CCamera* camera, RenderFlags renderFlags) {    
    if(renderFlags & RenderFlags::RENDER_COMBAT_TEXT)
        _textObj->onRender(_x, _y, window, camera);
}