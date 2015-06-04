//
//  CPlayer.cpp
//  Third
//
//  Created by Didrik Munther on 14/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CPlayer.h"
#include "Define.h"
#include <iostream>

CPlayer::CPlayer(sf::IntRect rect, sf::Color color) :
    CLiving(rect, color) {
        _initMovementValues();
}

CPlayer::CPlayer(sf::IntRect rect, std::string spriteKey) :
    CLiving(rect, spriteKey) {
        CPlayer(rect, sf::Color{255, 255, 255, 255});
        _initMovementValues();
}

void CPlayer::_initMovementValues() {
    maxSpeed = 10.0f;
    jumpPower = 20.0f;
    accelerationX = 1.5f;
    accelerationY = 100.0f;
    stoppingAccelerationX = accelerationX * 2;
    sneakSpeed = (float)maxSpeed / 2.0f;
}



void CPlayer::_doLogic() {
    CLiving::_doLogic();
}