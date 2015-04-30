//
//  CEntityManager.h
//  Third
//
//  Created by Didrik Munther on 19/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CEntityManager__
#define __Third__CEntityManager__

#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <memory>
#include <vector>
#include "CEntity.h"
#include "CParticle.h"
#include "CParticleEmitter.h"
#include "CSprite.h"
#include <map>
#include <string>
#include "CAssetManager.h"
#include "CGuiText.h"

class CCamera;

class CEntityManager {
    
public:
    CEntityManager();
    
    CEntity* addEntity(SDL_Rect rect, SDL_Color color, std::string name = "");
    CEntity* addEntity(SDL_Rect rect, std::string spriteKey, CAssetManager* assetManager, std::string name = "");
    void addEntity(CEntity* entity, std::string name = "");
    void addParticle(SDL_Rect rect, SDL_Color color, int livingTime);
    void addParticleEmitter(SDL_Rect rect, SDL_Color color, int amount, int frequency, int livingTime, int particleLivingTime, float velocity);
    void addGuiText(CGuiText* guiText);
    
    void onRender(sf::RenderWindow* window, CCamera* camera);
    void onLoop();
    void onCleanup();
    
    void entityCleanup();
    void particleEmitterCleanup();
    void particleCleanup();
    void guiTextCleanup();
    
    int entityID;
    
    int renderFlags;
    void addRenderFlag(int renderFlag);
    void removeRenderFlag(int renderFlag);
    void toggleRenderFlag(int renderFlag);
    
private:
    std::map<std::string, CEntity*> EntityVector;
    std::vector<CParticle*> ParticleVector;
    std::vector<CParticleEmitter*> ParticleEmitterVector;
    std::vector<CGuiText*> GuiTextVector;
    
};

#endif /* defined(__Third__CEntityManager__) */
