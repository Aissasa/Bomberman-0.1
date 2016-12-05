#pragma once
#include "baseTypes.h"

typedef struct Sprite
{
	float_t width;
	float_t height;
	float_t xId;
	float_t yId;
	float_t sortingLayer;
}Sprite_t;

typedef struct Animation
{
	const char8_t* name;
	Sprite_t* sprites;
	Sprite_t* currentSprite;
}Animation_t;