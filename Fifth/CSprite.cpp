//
//  CSprite.cpp
//  Third
//
//  Created by Didrik Munther on 24/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CSprite.h"

CSprite::CSprite(CSpriteSheet* spriteSheet, sf::IntRect rect, std::string shaderKey /* = "" */) : _rect(rect), CRenderable(shaderKey)
{
    _sprite.setTexture(*spriteSheet->getTexture());
    _sprite.setTextureRect(rect);
    
}


sf::IntRect* CSprite::getOffset() {
    return &_rect;
}

sf::Sprite* CSprite::getSprite() {
    return &_sprite;
}