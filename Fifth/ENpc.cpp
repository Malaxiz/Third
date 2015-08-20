//
//  ENpc.cpp
//  Fifth
//
//  Created by Didrik Munther on 20/08/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "ENpc.h"
#include "EMovable.h"

ENpc::ENpc(CEntity* parent) : EComponent(parent) {
    
}

void ENpc::onLoop(CInstance* instance) {
    if(_target != nullptr) {
        if(_parent->body.getX() > _target->body.getX()) {
            EMovable* movable = _parent->getComponent<EMovable>();
            if(movable) {
                movable->goLeft();
            } else {
                _parent->body.velX -= 1;
            }
        }
        else if(_parent->body.getX() < _target->body.getX()) {
            EMovable* movable = _parent->getComponent<EMovable>();
            if(movable) {
                movable->goRight();
            } else {
                _parent->body.velX += 1;
            }
        }
        if(_parent->collisionRight || _parent->collisionLeft) {
            EMovable* movable = _parent->getComponent<EMovable>();
            if(movable) {
                movable->jump();
            }
        }
    }
}

void ENpc::onRender(CWindow* window, CCamera* camera) {
    
}

void ENpc::renderAdditional(CWindow* window, CCamera* camera) {
    
}

bool ENpc::collisionLogic(CEntity* target, CInstance* instance, CollisionSides collisionSides) {
    return true;
}

void ENpc::setTarget(CEntity* target) {
    
}

CEntity* ENpc::getTarget() {
    
}
