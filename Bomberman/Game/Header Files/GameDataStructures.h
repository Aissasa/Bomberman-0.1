#pragma once

#include "baseTypes.h"

typedef struct Map
{
	uint32_t width;
	uint32_t height;

	uint32_t tileWidth;
	uint32_t tileHeight;

	uint32_t** bgLayer;
	uint32_t** itemsLayer;
	uint32_t** blocksLayer;

}Map_t;

