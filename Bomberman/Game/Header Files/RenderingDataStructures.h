#pragma once

#include "baseTypes.h"
#include <windows.h>											// Header File For Windows
#include <gl\gl.h>											// Header File For The OpenGL32 Library
#include <vector>

typedef struct Sprite
{
	float_t width;
	float_t height;
	float_t xId;
	float_t yId;
	float_t sortingLayer;

	GLfloat textureMap;

}Sprite_t;


typedef struct RenderableSprite
{
	Sprite_t sprite;
	Coord2D position;
}RenderableSprite_t;

typedef struct Animation
{
	//char8_t* name;

	uint32_t timeFromSpriteToSprite; //speed
	
	Sprite_t* sprites;
	uint16_t numSprites;	
	uint16_t currentSpriteIndex;


}Animation_t;


typedef struct SpriteSheet
{
	Sprite_t* sprites;
	Animation_t* animations;

	uint32_t numSprites;
	uint32_t numAnimations;
	float_t texXUnit;
	float_t texYUnit;

	GLfloat textureMap;

}SpriteSheet_t;

typedef std::vector<RenderableSprite_t> Renderable_Sprite_Vect_t;