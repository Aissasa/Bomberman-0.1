#pragma once

#include "baseTypes.h"

typedef struct TileCoor
{
	uint16_t x;
	uint16_t y;

}TileCoor_t;

typedef struct TileWithSprite
{
	TileCoor_t tileCoor;
	uint32_t spriteIndex;

}TileWithSprite_t;

typedef struct Map
{
	TileWithSprite_t perkTile;
	TileWithSprite_t doorTile;

	TileCoor_t playerSpawnTile;
	TileCoor_t* restictedTiles;

	uint32_t width;
	uint32_t height;

	uint32_t tileWidth;
	uint32_t tileHeight;

	uint32_t** bgLayer;
	uint32_t** blocksLayer;

	uint16_t numRestrictedTiles;

}Map_t;

typedef enum class SpriteIndicesInSpriteSheet
{
	door = 2,
	softBlock = 9,
	solidBlock = 16,

}SpriteIndicesInSpriteSheet_t;

typedef enum class SpriteIndicesInMap
{
	door = 3,
	softBlock = 10,
	solidBlock = 17,

}SpriteIndicesInMap_t;


typedef enum class PerksIndicesInSpriteSheet
{
	bombUp = 1,
	fire = 3,
	passBomb = 5,
	passSoftBlock = 6,
	remote = 7,
	skate = 8

}PerksIndicesInSpriteSheet_t;

typedef enum class PerksIndicesInMap
{
	bombUp = 2,
	fire = 4,
	passBomb = 6,
	passSoftBlock = 7,
	remote = 8,
	skate = 9

}PerksIndicesInMap_t;