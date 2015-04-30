//
//  CCamera.cpp
//  Third
//
//  Created by Didrik Munther on 02/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CCamera.h"
#include <iostream>
#include "Define.h"
#include <math.h>

CCamera::CCamera() :
    offset{0, 0, 0, 0}, cameraSway(10) {
}

void CCamera::onInit(CWindow* window) {
    offset = {0, 0, window->getWidth(), window->getHeight()};
}

void CCamera::onLoop() {
    if (target == nullptr) {
        offset.x = 0;
        offset.y = 0;
        return;
    }
    
    offset.x += (((target->body.getX() + target->body.getW() / 2) - offset.w / 2) - offset.x) / cameraSway;
    offset.y += (((target->body.getY() + target->body.getH() / 2) - offset.h / 2) - offset.y) / cameraSway;
}

int CCamera::offsetX() {
    return (int)floor(offset.x);
}

int CCamera::offsetY() {
    return (int)floor(offset.y);
}

void CCamera::setTarget(CEntity* target) {
    this->target = target;
}

bool CCamera::collision(CEntity* entity) {
    if(offsetX() > entity->body.getX() + entity->body.getW())
        return false;
    if(offsetX() + offset.w < entity->body.getX())
        return false;
    if(offsetY() > entity->body.getY() + entity->body.getH())
        return false;
    if(offsetY() + offset.h < entity->body.getY())
        return false;
    else
        return true;
}

bool CCamera::collision(int x, int y, int w, int h) {
    if(offsetX() > x + w)
        return false;
    if(offsetX() + offset.w < x)
        return false;
    if(offsetY() > y + h)
        return false;
    if(offsetY() + offset.h < y)
        return false;
    else
        return true;
}