#pragma once

#include "baseTypes.h"
#include <windows.h>											// Header File For Windows
#include <gl\gl.h>											// Header File For The OpenGL32 Library

typedef struct Sprite
{
	float_t width;
	float_t height;
	float_t xId;
	float_t yId;
	float_t sortingLayer;

	GLfloat textureMap;

}Sprite_t;

typedef struct Animation
{
	const char8_t* name;
	Sprite_t* sprites;
	Sprite_t* currentSprite;

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