#pragma once

#include "RenderingDataStructures.h"
#include "rapidjson/document.h"

class SpriteSheetParserC
{
public:
	SpriteSheetParserC() {};
	~SpriteSheetParserC() {};

	void ParseJson(char8_t* filePath);

	// accessors
	Sprite_t* getSprites();
	Animation_t* getAnimations();
	uint32_t getNumSprites();
	uint32_t getNumAnimations();
	float_t getTexXUnit();
	float_t getTexYUnit();

private:
	
	Sprite_t* sprites;
	Animation_t* animations;

	uint32_t numSprites;
	uint32_t numAnimations;
	float_t texXUnit;
	float_t texYUnit;

	Sprite_t populateASprite(const rapidjson::Value& frames, uint16_t i);
	Animation_t populateAnAnimation(const rapidjson::Value& anims, uint16_t i);
};
