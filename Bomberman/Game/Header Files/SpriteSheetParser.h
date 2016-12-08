#pragma once

#include "GameDataStructures.h"
#include "RenderingDataStructures.h"
#include "rapidjson/document.h"

class SpriteSheetParserC
{
public:
	static SpriteSheetParserC* CreateInstance();
	static SpriteSheetParserC* GetInsatnce() { return sInsatnce; };
	~SpriteSheetParserC() {};

	SpriteSheet_t parseSpriteSheetJson(char8_t* filePath,const GLfloat textureMap);

protected:
	static SpriteSheetParserC* sInsatnce;
	SpriteSheetParserC() {};

	Sprite_t populateASprite(const SpriteSheet_t& sps, const rapidjson::Value& frames, uint16_t i, float_t sortingLayer);
	Animation_t populateAnAnimation(const SpriteSheet_t& sps, const rapidjson::Value& anims, uint16_t i);
};
