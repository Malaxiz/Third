//
//  CParticle.h
//  Third
//
//  Created by Didrik Munther on 19/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CParticle__
#define __Third__CParticle__

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "CEntity.h"

class CParticle : public CEntity {
    
public:
    CParticle(sf::IntRect rect, sf::Color color);
    CParticle(sf::IntRect rect, sf::Color color, int livingTime);
private:
    
    sf::Clock clock;
    
    sf::Color color;
    int creationTime;
    int livingTime;
    
    virtual void doLogic();
};


#endif /* defined(__Third__CParticle__) */
