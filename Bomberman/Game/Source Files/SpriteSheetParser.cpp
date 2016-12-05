#include "stdlib.h"
#include "stdio.h"
#include "assert.h"
#include "SpriteSheetParser.h"
#include <rapidjson/istreamwrapper.h>
#include <fstream>

using namespace std;
using namespace rapidjson;

//---------------------------------------------------------------------------------------------------------------------
void SpriteSheetParserC::ParseJson(char8_t* filePath)
{
	// put the file in a stream
	FILE* jsonFile = fopen(filePath, "rb");

	assert(jsonFile);

	ifstream ifs(filePath);
	IStreamWrapper isw(ifs);

	// parse the json
	Document jsonDoc;
	jsonDoc.ParseStream(isw);

	// make sure it is a valid json, and it has frames array in it
	assert(jsonDoc.IsObject());

	const Value& frames = jsonDoc["frames"];
	assert(frames.IsArray());

	numSprites = frames.Size();
	// note if sprites with multiple rows are gonna be added, change this
	texXUnit = 1.0f / numSprites;
	texYUnit = 1.0f;

	// store sprites
	sprites = (Sprite_t*)malloc(numSprites * sizeof(Sprite_t));
	for (SizeType i = 0; i < numSprites; i++) // Uses SizeType instead of size_t
	{
		sprites[i] = populateASprite(frames, i);
	}

	// store animations
	const Value& anims = jsonDoc["Animations"];
	assert(anims.IsArray());

	numAnimations = anims.Size();
	animations = (Animation_t*)malloc(sizeof(Animation_t)*numAnimations);
	for (SizeType i = 0; i < numAnimations; i++) // Uses SizeType instead of size_t
	{
		animations[i] = populateAnAnimation(anims, i);
	}

	fclose(jsonFile);
}

//---------------------------------------------------------------------------------------------------------------------
Sprite_t * SpriteSheetParserC::getSprites()
{
	return sprites;
}

//---------------------------------------------------------------------------------------------------------------------
Animation_t * SpriteSheetParserC::getAnimations()
{
	return animations;
}

//---------------------------------------------------------------------------------------------------------------------
uint32_t SpriteSheetParserC::getNumSprites()
{
	return numSprites;
}

//---------------------------------------------------------------------------------------------------------------------
uint32_t SpriteSheetParserC::getNumAnimations()
{
	return numAnimations;
}

//---------------------------------------------------------------------------------------------------------------------
float_t SpriteSheetParserC::getTexXUnit()
{
	return texXUnit;
}

//---------------------------------------------------------------------------------------------------------------------
float_t SpriteSheetParserC::getTexYUnit()
{
	return texYUnit;
}

//---------------------------------------------------------------------------------------------------------------------
Sprite_t SpriteSheetParserC::populateASprite(const Value& frames, uint16_t i)
{
	Sprite_t newSprite;

	newSprite.width = (float_t)frames[i]["frame"]["w"].GetInt();
	newSprite.height = (float_t)frames[i]["frame"]["h"].GetInt();
	newSprite.xId = (float_t)frames[i]["frame"]["x"].GetInt() / newSprite.width;
	newSprite.yId = (float_t)frames[i]["frame"]["y"].GetInt() / newSprite.height;
	newSprite.sortingLayer = 0; // default

	return newSprite;
}

//---------------------------------------------------------------------------------------------------------------------
Animation_t SpriteSheetParserC::populateAnAnimation(const Value& anims, uint16_t i)
{
	Animation_t newAnimation;

	uint16_t nameLength = anims[i]["Name"].GetStringLength();
	newAnimation.name = (const char8_t*)malloc(nameLength);
	newAnimation.name = anims[i]["Name"].GetString();

	uint16_t animationSpriteCount = anims[i]["Sprites"].Size();
	newAnimation.sprites = (Sprite_t*)malloc(sizeof(Sprite_t) * animationSpriteCount);
	for (SizeType j = 0; j < animationSpriteCount; j++)
	{
		uint16_t index = anims[i]["Sprites"][j].GetInt();
		newAnimation.sprites[j] = sprites[index];
	}

	newAnimation.currentSprite = &newAnimation.sprites[0];

	return newAnimation;
}
