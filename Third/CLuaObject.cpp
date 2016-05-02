//
//
//  LuaTest4
//
//  Created by Didrik Munther on 16/10/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CLuaObject.h"
#include "CEntity.h"
#include "NFile.h"


CLuaObject::CLuaObject(CEntity* parent, CComponent* component, CLuaScript* script)
    : _script(script)
    , _parent(parent)
    , _object((*script->getObjectCreation())(parent, component))
{
    
}

void CLuaObject::beginCall(std::string function) {
    selectReference(function);
    lua_pushvalue(_script->getState(), -2);
}

void CLuaObject::endCall(int argc, int results) {
    int error = lua_pcall(_script->getState(), argc + 1, results, 0);

    if(error && lua_gettop(_script->getState())) {
        NFile::log(LogType::ERROR, "LuaScript \"", _script->getName(), "\": \"", lua_tostring(_script->getState(), -1), "\"");
    }
}

void CLuaObject::endCall() {
    endCall(0, 0);
}

std::vector<std::pair<luabridge::LuaRef, luabridge::LuaRef>> CLuaObject::getTable(std::string name) {
    std::vector<std::pair<luabridge::LuaRef, luabridge::LuaRef>> toReturn = {};
    auto L = _script->getState();
    
    luabridge::LuaRef table = _object[name];
    if(!table.isNil()) {
        table.push(L);
        push(L, luabridge::Nil ());
        while(lua_next (L, -2)) {
//            luabridge::LuaRef key = luabridge::LuaRef::fromStack (L, -2);
//            luabridge::LuaRef val = luabridge::LuaRef::fromStack (L, -1);
            toReturn.push_back(std::make_pair(luabridge::LuaRef::fromStack (L, -2), luabridge::LuaRef::fromStack (L, -1)));
            lua_pop(L, 1);
        }
    }
    
    return toReturn;
}

void CLuaObject::selectScript() {
    _object.push(_script->getState());
}

void CLuaObject::selectReference(std::string reference) {
    selectScript();
    ((luabridge::LuaRef)_object[reference.c_str()]).push(_script->getState());
}

bool CLuaObject::hasReference(std::string reference) {
    bool toReturn;
    
    if(hasReferences.count(reference))
        toReturn = hasReferences[reference];
    else {
        toReturn = !_object[reference.c_str()].isNil();
        hasReferences[reference] = toReturn;
    }
    
    return toReturn;
}

void CLuaObject::pushThisObject() {
    selectScript();
    lua_pushvalue(_script->getState(), -1);
}

CLuaScript* CLuaObject::getScript() {
    return _script;
}
