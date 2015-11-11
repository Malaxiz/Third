//
//  NFile.cpp
//  Third
//
//  Created by Didrik Munther on 13/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>

#include "NFile.h"
#include "Define.h"
#include "CAssetManager.h"


rapidjson::Document NFile::loadJsonFile(std::string fileName) {
    
    rapidjson::Document errorDoc;
    errorDoc.Parse<0>("{\"JSONParsingError\":\"Unknown reason for error (does the json file have a member called JSONParsingError?)\"}");
    
    std::ifstream file(fileName);
    
    if(!file.is_open()) {             // Todo add error msg logging
        errorDoc["JSONParsingError"] = "Couldn't open file";
        return errorDoc;
    }
    
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    
    rapidjson::Document d;
    
    if(d.Parse<0>(content.c_str()).HasParseError()) {
        errorDoc["JSONParsingError"] = "Couldn't parse file";
        return errorDoc;
    }
    
    return d;
    
}

void NFile::loadAssets(std::string fileName, CInstance* instance) {
    
    log(LogType::ALERT, "Loading map: \"", fileName.c_str(), "\"");
    
    rapidjson::Document d = loadJsonFile(fileName);
    
    if(d.HasMember("JSONParsingError")) {
        NFile::log(LogType::ERROR, "Could not load map \"", fileName, "\". (Reason: \"", d["JSONParsingError"].GetString(), ")");
        return;
    }
    
    loadFonts(&d);
    loadSpriteSheets(&d, instance);
    loadSprites(&d);
    
    log(LogType::SUCCESS, "Loaded map: \"", fileName.c_str(), "\" as \"", d["name"].GetString(), "\"");
}

void NFile::loadFonts(rapidjson::Document* d) {
    if(!d->HasMember("fonts"))
        return;
    
    const rapidjson::Value& fonts = (*d)["fonts"];                                     // Fonts
    for(rapidjson::SizeType i = 0; i < fonts.Size(); i++) {
        const rapidjson::Value& font = fonts[i];
        if(!(font.HasMember("name") && font.HasMember("path") && font.HasMember("size")))
            continue;
        
        CAssetManager::addFont(font["name"].GetString(), font["path"].GetString(), font["size"].GetInt());
    }
}

void NFile::loadSpriteSheets(rapidjson::Document* d, CInstance* instance) {
    if(!d->HasMember("spriteSheets"))
        return;
    
    const rapidjson::Value& spriteSheets = (*d)["spriteSheets"];                       // Sprite sheets
    for(rapidjson::SizeType i = 0; i < spriteSheets.Size(); i++) {
        const rapidjson::Value& spriteSheet = spriteSheets[i];
        if(!(spriteSheet.HasMember("name") && spriteSheet.HasMember("path")))
            continue;
        
        CAssetManager::addSpriteSheet(spriteSheet["name"].GetString(), instance->window.getRenderer(), spriteSheet["path"].GetString());
    }
}

void NFile::loadSprites(rapidjson::Document* d) {
    if(!d->HasMember("sprites"))
        return;
    
    const rapidjson::Value& sprites = (*d)["sprites"];                                 // Sprites
    for(rapidjson::SizeType i = 0; i < sprites.Size(); i++) {
        const rapidjson::Value& sprite = sprites[i];
        if(!(sprite.HasMember("name") && sprite.HasMember("spriteSheetKey") && sprite.HasMember("offsets")))
            continue;
        
        const rapidjson::Value& offsets = sprite["offsets"];
        CAssetManager::addSprite(sprite["name"].GetString(),
                                 sprite["spriteSheetKey"].GetString(),
                                 Box{offsets[0].GetInt(), offsets[1].GetInt(), offsets[2].GetInt(), offsets[3].GetInt()});
    }
}

void NFile::clearFile(std::string fileName) {
    std::ofstream file(fileName);
    file.close();
}
