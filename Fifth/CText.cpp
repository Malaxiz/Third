//
//  CText.cpp
//  Third
//
//  Created by Didrik Munther on 22/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CText.h"
#include "NSurface.h"

CText::CText(std::string text, int size, std::string fontKey, SDL_Color color) :
_text(text), _size(size), _fontKey(fontKey), _color(color) {
}

void CText::onRender(int x, int y, CWindow* window, CCamera* camera) {
    if(CAssetManager::getFont(_fontKey) != nullptr)
        NSurface::renderText(x - camera->offsetX(), y - camera->offsetY(), this, window);
}

void CText::onRender(int x, int y, CWindow* window) {
    if(CAssetManager::getFont(_fontKey) != nullptr)
        NSurface::renderText(x, y, this, window);
}

TTF_Font* CText::getFont() {
    return CAssetManager::getFont(_fontKey);
}

int CText::getSize() {
    return _size;
}

std::string* CText::getText() {
    return &_text;
}

SDL_Color* CText::getColor() {
    return &_color;
}