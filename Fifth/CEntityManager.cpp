//
//  CEntityManager.cpp
//  Third
//
//  Created by Didrik Munther on 19/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include <iostream>
#include "CEntityManager.h"
#include "CCamera.h"

CEntityManager::CEntityManager() : entityID(0), renderFlags(0) {
}

CEntity* CEntityManager::addEntity(sf::IntRect rect, sf::Color color, std::string name /* = "" */) {
    if(name == "") {
        std::string id = "5:" + std::to_string(++entityID);
        _EntityVector[id] = new CEntity(rect, color);
        return _EntityVector[id];
    } else {
        _EntityVector[name] = new CEntity(rect, color);
        return _EntityVector[name];
    }
}

CEntity* CEntityManager::addEntity(sf::IntRect rect, std::string spriteKey, std::string name /* = "" */) {
    if(name == "") {
        std::string id = "5:" + std::to_string(++entityID);
        _EntityVector[id] = new CEntity(rect, spriteKey);
        return _EntityVector[id];
    } else {
        _EntityVector[name] = new CEntity(rect, spriteKey);
        return _EntityVector[name];
    }
}

void CEntityManager::addEntity(CEntity* entity, std::string name /* = "" */) {
    if(name == "") {
        std::string id = "5:" + std::to_string(++entityID);
        _EntityVector[id] = entity;
    } else
        _EntityVector[name] = entity;
}

CEntity* CEntityManager::getEntity(std::string name) {
    if(name == "") {
        std::cout << "Couldn't get entity because it had no name.\n";
        return nullptr;
    } else if(_EntityVector.find(name) == _EntityVector.end()) {
        std::cout << "Couldn't get entity: " << name << ", because it doesn't exist.";
        return nullptr;
    } else {
        return _EntityVector[name];
    }
}

CEntity* CEntityManager::getEntityAtCoordinate(int x, int y) {
    for (auto &i: _EntityVector) {
        if(i.second->coordinateCollision(x, y, 1, 1))
           return i.second;
    }
    return nullptr;
}

std::string CEntityManager::getNameOfEntity(CEntity *entity) {
    for (auto &i: _EntityVector) {                              // There must be a better way of doing this
        if(i.second == entity)
            return i.first;
    }
}

void CEntityManager::addParticle(sf::IntRect rect, sf::Color color, int livingTime) {
    _ParticleVector.push_back(new CParticle(rect, color, livingTime));
}

void CEntityManager::addParticle(CParticle *particle) {
    _ParticleVector.push_back(particle);
}

void CEntityManager::addParticleEmitter(sf::IntRect rect, sf::Color color, int type, int amount, int frequency, int livingTime, int particleLivingTime, ParticleVelocity velocity) {
    _ParticleEmitterVector.push_back(new CParticleEmitter(rect, color, type, amount, frequency, livingTime, particleLivingTime, velocity));
}

void CEntityManager::addGuiText(CGuiText* guiText) {
    _GuiTextVector.push_back(guiText);
}

void CEntityManager::addRenderFlag(int renderFlag) {
    renderFlags |= renderFlag;
}
void CEntityManager::removeRenderFlag(int renderFlag) {
    if(renderFlags & renderFlag) toggleRenderFlag(renderFlag);
}
void CEntityManager::toggleRenderFlag(int renderFlag) {
    renderFlags ^= renderFlag;
}

void CEntityManager::onRender(CWindow* window, CCamera* camera) {
    for (auto &i: _ParticleVector)
        i->onRender(window, camera, renderFlags);
    
    for (auto &i: _EntityVector)
        i.second->onRender(window, camera, renderFlags);
    
    for (auto &i: _EntityVector)
        i.second->renderAdditional(window, camera, renderFlags);
    
    for (auto &i: _GuiTextVector)
        i->onRender(window, camera);
}

void CEntityManager::onLoop() {
    {
        auto i = _EntityVector.begin();
        while(i != _EntityVector.end()) {
            auto target = (*i).second;
            target->onLoop(&_EntityVector);
            
            if(target->isDead()) {
                auto offset = target->getSprite()->getOffset();
                CSprite* tempSprite1 = new CSprite(target->getSprite()->getSpriteSheet(),
                                                    sf::IntRect{
                                                       offset->left,
                                                       offset->top,
                                                       offset->width / 2,
                                                       offset->height / 2});
                CSprite* tempSprite2 = new CSprite(target->getSprite()->getSpriteSheet(),
                                                   sf::IntRect{
                                                       offset->left + offset->width / 2,
                                                       offset->top,
                                                       offset->width / 2,
                                                       offset->height / 2});
                CSprite* tempSprite3 = new CSprite(target->getSprite()->getSpriteSheet(),
                                                   sf::IntRect{
                                                       offset->left,
                                                       offset->top + offset->height / 2,
                                                       offset->width / 2,
                                                       offset->height / 2});
                CSprite* tempSprite4 = new CSprite(target->getSprite()->getSpriteSheet(),
                                                   sf::IntRect{
                                                       offset->left + offset->width / 2,
                                                       offset->top + offset->height / 2,
                                                       offset->width / 2,
                                                       offset->height / 2});
                std::string sprite1 = CAssetManager::addSprite(tempSprite1);
                std::string sprite2 = CAssetManager::addSprite(tempSprite2);
                std::string sprite3 = CAssetManager::addSprite(tempSprite3);
                std::string sprite4 = CAssetManager::addSprite(tempSprite4);
                
                int explosionForce = 10;
                int tempForRand = 5;
                int livingTime = 2;
                
                CParticle* tempParticle1 = new CParticle(sf::IntRect{
                                                            target->body.getX(),
                                                            target->body.getY(),
                                                            target->body.getW() / 2,
                                                            target->body.getH() / 2},
                                                         sprite1,
                                                         livingTime);
                tempParticle1->body.velX = rand() % explosionForce - tempForRand;
                tempParticle1->body.velY = rand() % explosionForce - tempForRand;
                if(target->hasProperty(EntityProperty::FLIP)) tempParticle1->addProperty(EntityProperty::FLIP);
                CParticle* tempParticle2 = new CParticle(sf::IntRect{
                                                            target->body.getX() + target->body.getW() / 2,
                                                            target->body.getY(),
                                                            target->body.getW() / 2,
                                                            target->body.getH() / 2},
                                                         sprite2,
                                                         livingTime);
                tempParticle2->body.velX = rand() % explosionForce - tempForRand;
                tempParticle2->body.velY = rand() % explosionForce - tempForRand;
                if(target->hasProperty(EntityProperty::FLIP)) tempParticle2->addProperty(EntityProperty::FLIP);
                CParticle* tempParticle3 = new CParticle(sf::IntRect{
                                                            target->body.getX(),
                                                            target->body.getY() + target->body.getH() / 2,
                                                            target->body.getW() / 2,
                                                            target->body.getH() / 2},
                                                         sprite3,
                                                         livingTime);
                tempParticle3->body.velX = rand() % explosionForce - tempForRand;
                tempParticle3->body.velY = rand() % explosionForce - tempForRand;
                if(target->hasProperty(EntityProperty::FLIP)) tempParticle3->addProperty(EntityProperty::FLIP);
                CParticle* tempParticle4 = new CParticle(sf::IntRect{
                                                            target->body.getX() + target->body.getW() / 2,
                                                            target->body.getY() + target->body.getH() / 2,
                                                            target->body.getW() / 2,
                                                            target->body.getH() / 2},
                                                         sprite4,
                                                         livingTime);
                tempParticle4->body.velX = rand() % explosionForce - tempForRand;
                tempParticle4->body.velY = rand() % explosionForce - tempForRand;
                if(target->hasProperty(EntityProperty::FLIP)) tempParticle4->addProperty(EntityProperty::FLIP);
                
                addParticle(tempParticle1);
                addParticle(tempParticle2);
                addParticle(tempParticle3);
                addParticle(tempParticle4);
                
                _DeadEntitiesVector[(*i).first] = (*i).second;
                //target->toRemove = true;
                _EntityVector.erase(i++);
            }
            
            
            if(target->toRemove) {
                delete target;
                _EntityVector.erase(i++);
            } else
                ++i;
        }
    }
    
    {
        auto i = _ParticleEmitterVector.begin();
        while(i != _ParticleEmitterVector.end()) {
            (*i)->onLoop(this);
            if((*i)->toRemove) {
                delete *i;
                _ParticleEmitterVector.erase(std::remove(_ParticleEmitterVector.begin(), _ParticleEmitterVector.end(), (*i)), _ParticleEmitterVector.end());
            } else
                ++i;
        }
    }
    
    {
        auto i = _ParticleVector.begin();
        while(i != _ParticleVector.end()) {
            (*i)->onLoop(&_EntityVector);
            if((*i)->toRemove) {
                delete *i;
                _ParticleVector.erase(std::remove(_ParticleVector.begin(), _ParticleVector.end(), (*i)), _ParticleVector.end());
            } else
                ++i;
        }
    }
    
    {
        auto i = _GuiTextVector.begin();
        while(i != _GuiTextVector.end()) {
            (*i)->onLoop();
            if((*i)->toRemove) {
                delete *i;
                _GuiTextVector.erase(std::remove(_GuiTextVector.begin(), _GuiTextVector.end(), (*i)), _GuiTextVector.end());
            } else
                ++i;
        }
    }
}

void CEntityManager::onCleanup() {
    
    entityCleanup();
    particleEmitterCleanup();
    particleCleanup();
}

void CEntityManager::entityCleanup() {
    {
        auto i = _EntityVector.begin();
        while(i != _EntityVector.end()) {
            delete i->second;
            _EntityVector.erase(i++->first);
        }
        _EntityVector.clear();
    }
    
    {
        auto i = _DeadEntitiesVector.begin();
        while(i != _DeadEntitiesVector.end()) {
            delete i->second;
            _DeadEntitiesVector.erase(i++->first);
        }
        _DeadEntitiesVector.clear();
    }
}

void CEntityManager::particleEmitterCleanup() {
    auto i = _ParticleEmitterVector.begin();
    while(i != _ParticleEmitterVector.end()) {
        delete *i;
        i = _ParticleEmitterVector.erase(i);
    }
    _ParticleEmitterVector.clear();
}

void CEntityManager::particleCleanup() {
    auto i = _ParticleVector.begin();
    while(i != _ParticleVector.end()) {
        delete *i;
        i = _ParticleVector.erase(i);
    }
    _ParticleVector.clear();
}

void CEntityManager::guiTextCleanup() {
    auto i = _GuiTextVector.begin();
    while(i != _GuiTextVector.end()) {
        delete *i;
        i = _GuiTextVector.erase(i);
    }
    _GuiTextVector.clear();
}