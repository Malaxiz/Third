//
//  CText.cpp
//  Third
//
//  Created by Didrik Munther on 22/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CText.h"
#include "NSurface.h"

CText::CText(std::string text, std::string fontKey, CAssetManager* assetManager, SDL_Color color) :
    text(text), fontKey(fontKey), assetManager(assetManager), color(color) {
}

void CText::onRender(int x, int y, SDL_Renderer *renderer, CCamera* camera) {
    if(assetManager->getFont(fontKey) != nullptr)
        NSurface::renderText(x - camera->offsetX(), y - camera->offsetY(), this, renderer);
}

void CText::onRender(int x, int y, SDL_Renderer *renderer) {
    if(assetManager->getFont(fontKey) != nullptr)
        NSurface::renderText(x, y, this, renderer);
}

TTF_Font* CText::getFont() {
    return assetManager->getFont(fontKey);
}

std::string* CText::getText() {
    return &text;
}

SDL_Color* CText::getColor() {
    return &color;
}