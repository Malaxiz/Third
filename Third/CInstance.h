//
//  CInstance.h
//  Fifth
//
//  Created by Didrik Munther on 06/05/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Fifth__CInstance__
#define __Fifth__CInstance__

#include <stdio.h>

#include "CEntityManager.h"
#include "CWindow.h"
#include "CCamera.h"

extern "C" {
# include "lua/lua.h"
# include "lua/lauxlib.h"
# include "lua/lualib.h"
}

#include "LuaBridge/LuaBridge.h"


class CInstance {
    
public:
    CInstance(CGame* game);
    ~CInstance();
    
    void onLoop();
    
    CCamera* camera;
    CWindow window;
    CEntityManager entityManager;
    
    CEntity* player;
    CEntity* controller;
    
    lua_State* L;
    CGame* game;
    
    float gravity;
    bool isPaused;
    
    void loadAssets(std::string path);
    void doLine(std::string line);
    
    void loadLevel(std::string fileName);
    std::string levelToLoad = "";
    bool doLoadLevel;
    void _loadLevel(std::string fileName);
    
    std::string onSerialize();
    void closeInstance();
    
    std::string loadedMap;
    
};

#endif /* defined(__Fifth__CInstance__) */
