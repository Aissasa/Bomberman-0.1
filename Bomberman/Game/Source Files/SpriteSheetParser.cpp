#include "stdlib.h"
#include "stdio.h"
#include "assert.h"
#include <windows.h>											// Header File For Windows
#include <gl\gl.h>											// Header File For The OpenGL32 Library
#include "SpriteSheetParser.h"
#include <rapidjson/istreamwrapper.h>
#include <fstream>
#include "string.h"

using namespace std;
using namespace rapidjson;


SpriteSheetParserC* SpriteSheetParserC::sInsatnce = nullptr;

//---------------------------------------------------------------------------------------------------------------------
SpriteSheetParserC * SpriteSheetParserC::CreateInstance()
{
	if (sInsatnce == nullptr)
	{
		sInsatnce = new SpriteSheetParserC();
	}

	return sInsatnce;
}

//---------------------------------------------------------------------------------------------------------------------
SpriteSheet_t SpriteSheetParserC::parseSpriteSheetJson(char8_t* filePath,const GLfloat textureMap)
{
	SpriteSheet_t sps;
	float_t sortingLayer = -20;

	sps.textureMap = textureMap;

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

	if (jsonDoc.HasMember("sortingLayer"))
	{
		sortingLayer = jsonDoc["sortingLayer"].GetFloat();
	}

	const Value& frames = jsonDoc["frames"];
	assert(frames.IsArray());

	sps.numSprites = frames.Size();
	// note if sprite sheets with multiple rows are gonna be added, change this
	sps.texXUnit = 1.0f / sps.numSprites;
	sps.texYUnit = 1.0f;

	// store sprites
	sps.sprites = (Sprite_t*)malloc(sps.numSprites * sizeof(Sprite_t));
	for (SizeType i = 0; i < sps.numSprites; i++) // Uses SizeType instead of size_t
	{
		sps.sprites[i] = populateASprite(sps, frames, i, sortingLayer);
	}

	// store animations
	const Value& anims = jsonDoc["Animations"];
	assert(anims.IsArray());

	sps.numAnimations = anims.Size();
	sps.animations = (Animation_t*)malloc(sizeof(Animation_t)*sps.numAnimations);
	for (SizeType i = 0; i < sps.numAnimations; i++) // Uses SizeType instead of size_t
	{
		sps.animations[i] = populateAnAnimation(sps, anims, i);
	}

	fclose(jsonFile);

	return sps;
}

//---------------------------------------------------------------------------------------------------------------------
Sprite_t SpriteSheetParserC::populateASprite(const SpriteSheet_t& sps, const Value& frames, uint16_t i, float_t sortingLayer)
{
	Sprite_t newSprite;

	newSprite.width = (float_t)frames[i]["frame"]["w"].GetInt();
	newSprite.height = (float_t)frames[i]["frame"]["h"].GetInt();
	newSprite.xId = (float_t)frames[i]["frame"]["x"].GetInt() / newSprite.width;
	newSprite.yId = (float_t)frames[i]["frame"]["y"].GetInt() / newSprite.height;
	if (sortingLayer < -10)
	{
		newSprite.sortingLayer = frames[i]["sortingLayer"].GetFloat(); 
	}
	else
	{
		newSprite.sortingLayer = sortingLayer; // default for all sprites
	}
	newSprite.textureMap = sps.textureMap;

	return newSprite;
}

//---------------------------------------------------------------------------------------------------------------------
Animation_t SpriteSheetParserC::populateAnAnimation(const SpriteSheet_t& sps, const Value& anims, uint16_t i)
{
	Animation_t newAnimation;

	//uint16_t nameLength = anims[i]["Name"].GetStringLength();
	//newAnimation.name = (char8_t*)malloc(nameLength+2);
	//strncpy(newAnimation.name, anims[i]["Name"].GetString(), nameLength+1);
	//newAnimation.name = anims[i]["Name"].GetString();

	newAnimation.numSprites = anims[i]["Sprites"].Size();

	newAnimation.sprites = (Sprite_t*)malloc(sizeof(Sprite_t) * newAnimation.numSprites);
	for (SizeType j = 0; j < newAnimation.numSprites; j++)
	{
		uint16_t index = anims[i]["Sprites"][j].GetInt();
		newAnimation.sprites[j] = sps.sprites[index];
	}

	newAnimation.timeFromSpriteToSprite = ANIMATIONS_TIME;
	newAnimation.currentSpriteIndex = 0;

	return newAnimation;
}
