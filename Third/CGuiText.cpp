//
//  CGuiText.cpp
//  Third
//
//  Created by Didrik Munther on 22/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CGuiText.h"
#include "NSurface.h"


CGuiText::CGuiText(int x, int y, std::string text, std::string fontKey) :
_x(x), _y(y), _text(text), _fontKey(fontKey), _toRemove(false), _creationTime(SDL_GetTicks()) {
}