//
//  CGame.cpp
//  Third
//
//  Created by Didrik Munther on 12/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include <fstream>
#include <cmath>
#include <iomanip>

#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>

#include "Define.h"
#include "CGame.h"
#include "CEntity.h"
#include "CText.h"
#include "CBackground.h"
#include "CCombatText.h"
#include "CAssetManager.h"
#include "CAnimation.h"

#include "NMouse.h"
#include "NSurface.h"
#include "NFile.h"

#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#include "ResourcePath.hpp"
#endif


CGame::CGame()
    : _intro("Third"), instance(this), toRestart(false), ignoreEvents(false)
    , _lastTime(SDL_GetTicks()), _timer(SDL_GetTicks()), _isRunning(true), quickSave("{}")
    , _ns(1000.0f / GAME_INTERVAL), _delta(0), _frames(0), _updates(0), isFocused(true) {
}

int CGame::onExecute() {
    
    switch(_onInit()){
        case -1:
            NFile::log(LogType::ERROR, "Initializing failed!");
            _isRunning = false;
        case 0:
            NFile::log(LogType::SUCCESS, "Initializing succesful!");
    }
    
    NFile::log(LogType::ALERT, "Starting game...");
    
    while(_isRunning) {
        if(toRestart)
            _restart();
        
        while(SDL_PollEvent(&event)){
            _onEvent(&event);
        }
        
        float now = SDL_GetTicks();
        _delta += (now - _lastTime) / _ns;
        _lastTime = now;
        
        while(_delta >= 1) {    // Todo implement variable time step instead of this laggy thing
            if(_delta > 20) {       // To make sure it doesn't freeze
                //NFile::log(LogType::WARNING, "Game exeeding delta limit (", 20, "), cleaning up particles.");
                instance.entityManager.particleCleanup();
            }
            
            _handleKeyStates();
            _onLoop();
            
            _updates++;
            _delta--;
            
            auto sayer = instance.entityManager.getEntity("n:bush");
            if(sayer)
                sayer->say(_title.str() + " Gravity: " + std::to_string(instance.gravity), "TESTFONT", ChatBubbleType::INSTANT_TALK);
        }
        
        _onRender();
        
        _frames++;
        
        if(SDL_GetTicks() - _timer > 1000) {
            _timer += 1000;
            _title.str("");
            _title << _updates << " ups, " << _frames << " fps";
            _updates = 0;
            _frames = 0;
        }
    }
    
    NFile::log(LogType::ALERT, "Ending game...");
    
    return _onCleanup();
    
}

int CGame::_onInit() {
    
    _initRelativePaths();
    NFile::clearFile(LOG_FILE);     // Clear log file
    srand((Uint16)time(nullptr));
    
    NFile::log(LogType::ALERT, "Initializing game...");
    
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        NFile::log(LogType::ERROR, "SDL_Init failed: ", SDL_GetError());
        return -1;
    }
    
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        NFile::log(LogType::ERROR, "IMG_Init failed: ", SDL_GetError());
        return -1;
    }
    
    if(TTF_Init() != 0) {
        NFile::log(LogType::ERROR, "TTF_Init failed: ", SDL_GetError());
        return -1;
    }
    
    if(instance.window.onInit(_intro, SCREEN_WIDTH, SCREEN_HEIGHT)) {
        NFile::log(LogType::ERROR, "Window initialization failed!");
        return -1;
    }
    instance.camera->onInit(&instance.window);
    
    _restart();
    
    return 0;
}

void CGame::restart() {
    toRestart = true;
}

void CGame::_restart() {
    CAssetManager::onCleanup();
    instance.closeInstance();
    
    instance.L = luaL_newstate();
    _initLua();

    auto controller = CAssetManager::addLuaScript(instance.L, "resources/scripts/Standard/Controller.lua");
    auto temp = new CEntity(Box{0, 0, 0, 0}, Color{0, 0, 0, 0});
    instance.entityManager.addEntity(temp, "Controller");
    instance.controller = temp;
    temp->addComponent(&instance, controller);
    
    NFile::loadLevel("island.lvl", &instance);
    instance.loadedMap = "island.lvl";
    
    toRestart = false;
}

void CGame::_initRelativePaths() {
    // ----------------------------------------------------------------------------
    // This makes relative paths work in C++ in Xcode by changing directory to the Resources folder inside the .app bundle
    #ifdef __APPLE__
        CFBundleRef mainBundle = CFBundleGetMainBundle();
        CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
        char path[PATH_MAX];
        if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
        {
            // error!
        }
        CFRelease(resourcesURL);
        
        chdir(path);
        _path = path;
        NFile::log(LogType::ALERT, "Current Path: ", path);
    #endif
    // ----------------------------------------------------------------------------
}

int CGame::setLuaPath(lua_State* L, const char* path) {
    lua_getglobal( L, "package" );
    lua_getfield( L, -1, "path" ); // get field "path" from table at top of stack (-1)
    std::string cur_path = lua_tostring( L, -1 ); // grab path string from top of stack
    cur_path.append( ";" ); // do your path magic here
    cur_path.append( path );
    lua_pop( L, 1 ); // get rid of the string on the stack we just pushed on line 5
    lua_pushstring( L, cur_path.c_str() ); // push the new one
    lua_setfield( L, -2, "path" ); // set the field "path" in table at -2 with value at top of stack
    lua_pop( L, 1 ); // get rid of package table from top of stack
    return 0; // all done!
}

void CGame::_initLua() {
    luaL_openlibs(instance.L);
    luaL_dofile(instance.L, "resources/scripts/Standard/class.lua");
    luaL_dofile(instance.L, "resources/scripts/Standard/bitwise.lua");
    luaL_dofile(instance.L, "resources/scripts/Standard/standard.lua");
    
    luabridge::getGlobalNamespace(instance.L)
    
        .beginNamespace("game")
            .addFunction("getScript", &CAssetManager::getLuaScript)
            .addFunction("getTime", &CGame::getTime)
            .addFunction("setLuaPath", &CGame::setLuaPath)
            .addFunction("leftMousePressed", &NMouse::leftMouseButtonPressed)
            .addFunction("rightMousePressed", &NMouse::rightMouseButtonPressed)
            .addFunction("getScreenWidth", &CGame::getWidth)
            .addFunction("getScreenHeight", &CGame::getHeight)
            .addFunction("addSprite", (std::string (*)(CSprite*, std::string)) &CAssetManager::addSprite)
            .addFunction("createSprite", (CSprite* (*)(CSpriteSheet*, Box)) &CAssetManager::createSprite)
            .addFunction("log", (void (*)(int, std::string)) &NFile::log)
            .addFunction("writeToFile", (void (*)(std::string, std::string)) &NFile::luaWriteToFile)
            .addFunction("readFile", &NFile::readFromFile)
            .addFunction("clearFile", &NFile::clearFile)
            .addFunction("tileSize", &CGame::tileSize)
        .endNamespace()
    
        .beginClass<CLuaObject>("LuaObject")
            .addFunction("getScript", &CLuaObject::getScript)
        .endClass()
    
        .beginClass<CLuaScript>("LuaScript")
            .addFunction("getName", &CLuaScript::getName)
        .endClass()
    
        .beginClass<CEntity>("Entity")      // Entity doesn't need a constructor, because you should use the entityManager.create.. functions
            .addCFunction("getComponent", &CEntity::getComponent)
            .addFunction("addComponent", &CEntity::addComponent)
            .addData("entityManager", &CEntity::entityManager)
            .addFunction("getThis", &CEntity::getThis)
            .addData("body", &CEntity::body)
            .addData("isDead", &CEntity::isDead)
            .addData("toRemove", &CEntity::toRemove)
            .addData("collisionSides", &CEntity::collisionSides)
            .addData("properties", &CEntity::properties)
            .addFunction("toggleProperty", &CEntity::toggleProperty)
            .addFunction("addProperty", &CEntity::addProperty)
            .addFunction("removeProperty", &CEntity::removeProperty)
            .addFunction("hasProperty", &CEntity::hasProperty)
            .addData("transparency", &CEntity::transparency)
            .addData("angle", &CEntity::angle)
            .addFunction("say", (void (CEntity::*)(std::string, std::string, int)) &CEntity::say)
            .addFunction("addTextObject", &CEntity::addTextObject)
            .addFunction("addCombatText", &CEntity::addCombatText)
            .addFunction("compare", &CEntity::compare)
            .addFunction("setSpriteOnState", &CEntity::setSpriteOnState)
            .addFunction("getSprite", &CEntity::getSprite)
            .addFunction("hasSprite", &CEntity::hasSprite)
            .addFunction("setSprite", &CEntity::setSprite)
            .addFunction("getSpriteFromState", &CEntity::getSpriteFromState)
            .addData("defaultSprite", &CEntity::defaultSprite)
            .addFunction("collision", (bool (CEntity::*)(Box)) &CEntity::coordinateCollision)
        .endClass()
    
        .beginClass<CComponent>("Component")
            .addFunction("renderRect", &CComponent::renderRect)
            .addFunction("renderLine", &CComponent::renderLine)
            .addFunction("renderText", &CComponent::renderText)
            .addFunction("addString", &CComponent::addString)
            .addFunction("addInt", &CComponent::addInt)
            .addFunction("addFloat", &CComponent::addFloat)
            .addCFunction("getRelativeMouse", &CComponent::getRelativeMouse)
            .addCFunction("getMouse", &CComponent::getMouse)
            .addData("instance", &CComponent::instance)
            .addData("camera", &CComponent::tempCamera)
            .addData("object", &CComponent::object)
        .endClass()
    
        .beginClass<Box>("Box")
            .addConstructor<void(*) (int, int, int, int)>()
            .addData("x", &Box::x) // Cannot change these values in lua
            .addData("y", &Box::y)
            .addData("w", &Box::w)
            .addData("h", &Box::h)
        .endClass()
    
        .beginClass<Position>("Position")
            .addConstructor<void(*) (int, int)>()
            .addData("x", &Position::x)
            .addData("y", &Position::y)
        .endClass()
    
        .beginClass<Color>("Color")
            .addConstructor<void(*) (int, int, int, int)>()
        .endClass()
    
        .beginClass<SDL_Point>("SDL_Point")
            .addData("x", &SDL_Point::x)
            .addData("y", &SDL_Point::y)
        .endClass()
    
        .beginClass<CSpriteSheet>("SpriteSheet")
        .endClass()
    
        .beginClass<CSprite>("Sprite")
            .addConstructor<void(*) (CSpriteSheet*, Box)>()
            .addFunction("getSpriteSheet", &CSprite::getSpriteSheet)
            .addFunction("getSource", &CSprite::getSource)
        .endClass()
    
        .beginClass<CGuiText>("GuiText")
            .addConstructor<void(*) (int, int, std::string, std::string)>()
        .endClass()
    
        .beginClass<CCombatText>("CombatText")
            .addConstructor<void(*) (int, int, Color, int, std::string, std::string)>()
        .endClass()
    
        .beginClass<CInstance>("Instance")
            .addData("player", &CInstance::player)
            .addData("controller", &CInstance::controller)
            .addData("game", &CInstance::game)
            .addData("camera", &CInstance::camera)
            .addData("gravity", &CInstance::gravity)
            .addData("isPaused", &CInstance::isPaused)
            .addFunction("loadAssets", &CInstance::loadAssets)
            .addFunction("doLine", &CInstance::doLine)
            .addFunction("loadLevel", &CInstance::loadLevel)
            .addFunction("onSerialize", &CInstance::onSerialize)
        .endClass()
    
        .beginClass<CCamera>("Camera")
            .addFunction("addCameraShake", &CCamera::addCameraShake)
            .addFunction("setTarget", &CCamera::setTarget)
            .addData("cameraSway", &CCamera::cameraSway)
            .addFunction("offsetX", &CCamera::offsetX)
            .addFunction("offsetY", &CCamera::offsetY)
        .endClass()
    
        .beginClass<CGame>("Game")
            .addFunction("restart", &CGame::restart)
            .addData("ignoreEvents", &CGame::ignoreEvents)
            .addData("isRunning", &CGame::_isRunning)
        .endClass()
    
        .beginClass<CBody>("Body")
            .addConstructor<void(*) (Box)>()
            .addData("velX", &CBody::velX)
            .addData("velY", &CBody::velY)
            .addData("box", &CBody::_rect)
            .addFunction("setBox", &CBody::setBox)
            .addFunction("setPosition", &CBody::setPosition)
            .addFunction("setDimension", &CBody::setDimension)
        .endClass()
    
        .beginClass<KeyState>("KeyState")
            .addFunction("hasState", &KeyState::hasState)
        .endClass()
    
        .beginClass<Line>("Line")
            .addConstructor<void(*) (int, int, int, int, Color)>()
        .endClass()
    
        .beginClass<Triangle>("Triangle")
            .addConstructor<void(*) (Line, Line, Line)>()
        .endClass()
    
        .beginClass<CEntityManager>("EntityManager")
            .addFunction("addEntity", &CEntityManager::addEntity)
            .addFunction("addParticle", &CEntityManager::addParticle)
            .addFunction("addTile", &CEntityManager::addTile)
            .addFunction("removeTile", &CEntityManager::removeTile)
            .addFunction("getEntity", &CEntityManager::getEntity)
            .addFunction("getNameOfEntity", &CEntityManager::getNameOfEntity)
            .addFunction("getEntityAtCoordinate", &CEntityManager::getEntityAtCoordinate)
            .addFunction("getEntityCount", &CEntityManager::getEntityCount)
            .addCFunction("getEntities", &CEntityManager::pushEntities)
            .addFunction("createColoredEntity", (CEntity* (CEntityManager::*)(Box, Color)) &CEntityManager::createEntity)
            .addFunction("createSpriteEntity", (CEntity* (CEntityManager::*)(Box, std::string)) &CEntityManager::createEntity)
            .addFunction("toggleRenderFlag", (void (CEntityManager::*)(int)) &CEntityManager::toggleRenderFlag)
            .addFunction("removeRenderFlag", (void (CEntityManager::*)(int)) &CEntityManager::removeRenderFlag)
            .addFunction("addRenderFlag", (void (CEntityManager::*)(int)) &CEntityManager::addRenderFlag)
        .endClass()
    
        .beginClass<CollisionSides>("CollisionSides")
            .addConstructor<void(*) (void)>()
            .addData("top", &CollisionSides::top)
            .addData("bottom", &CollisionSides::bottom)
            .addData("right", &CollisionSides::right)
            .addData("left", &CollisionSides::left)
        .endClass();
    
    lua_getglobal(instance.L, "init");
    lua_pushstring(instance.L, (_path + "/resources/scripts/Standard/").c_str());
    lua_call(instance.L, 1, 0);
    
    lua_getglobal(instance.L, "initJson");
    lua_call(instance.L, 0, 0);
}

int CGame::getTime() {
    return SDL_GetTicks();
}

void CGame::_onLoop() {
    instance.onLoop();
    instance.entityManager.onLoop(&instance);
    instance.camera->onLoop();
}

void CGame::_onRender() {
    SDL_SetRenderDrawColor(instance.window.getRenderer(), 250, 250, 250, 255);
    SDL_RenderClear(instance.window.getRenderer());
    
    instance.entityManager.onRender(&instance.window, instance.camera);
    
    SDL_RenderPresent(instance.window.getRenderer());
}

int CGame::_onCleanup() {
    CAssetManager::onCleanup(EVERYTHING);
    instance.window.onCleanup();
    instance.closeInstance();
    
    NFile::log(LogType::ALERT, "Exiting game.");
    
    return 0;
}