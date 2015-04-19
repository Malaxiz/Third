//
//  NMouse.cpp
//  Third
//
//  Created by Didrik Munther on 16/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "NMouse.h"

int NMouse::absoluteMouseX() {
    int x, y;
    SDL_GetMouseState(&x, &y);
    return x;
}

int NMouse::absoluteMouseY() {
    int x, y;
    SDL_GetMouseState(&x, &y);
    return y;
}

int NMouse::relativeMouseX(CCamera* camera) {
    int x, y;
    SDL_GetMouseState(&x, &y);
    return x + camera->offsetX();
}

int NMouse::relativeMouseY(CCamera* camera) {
    int x, y;
    SDL_GetMouseState(&x, &y);
    return y + camera->offsetY();
}