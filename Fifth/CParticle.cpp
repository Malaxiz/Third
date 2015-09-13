//
//  CParticle.cpp
//  Third
//
//  Created by Didrik Munther on 19/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CParticle.h"


CParticle::CParticle(Box rect, SDL_Color color, int livingTime /* = 5 */) :
CEntity(rect, color), _creationTime(SDL_GetTicks()), _livingTime(livingTime) {
    addProperty(EntityProperty::FLIP_FREEZED);
    collisionLayer = LAYER1;
}

CParticle::CParticle(Box rect, std::string spriteKey, int livingTime /* = 5 */) :
CEntity(rect, spriteKey), _creationTime(SDL_GetTicks()), _livingTime(livingTime) {
    addProperty(EntityProperty::FLIP_FREEZED);
    collisionLayer = LAYER1;
}

void CParticle::_init() {
}

void CParticle::renderAdditional(CWindow* window, CCamera* camera, RenderFlags renderFlags) {
    CEntity::renderAdditional(window, camera, renderFlags);
    
}

void CParticle::_doLogic() {
    CEntity::_doLogic();
    
//    if(collisionBottom) {
//        body.velX /= 2;
//        if(body.velX > 0)
//            if(body.velX < 0.000001)
//                body.velX = 0;
//        if(body.velX < 0)
//            if(body.velX < -0.000001)
//                body.velX = 0;
//    }
    
    if(_livingTime > 0 && SDL_GetTicks() - _creationTime > _livingTime * 1000)
        _toRemove = true;
    
}

bool CParticle::_collisionLogic(CEntity* target, CollisionSides collisionSides) {
    bool parentCollision = CEntity::_collisionLogic(target, collisionSides);
    bool collision = true;
    
    return parentCollision && collision;
}