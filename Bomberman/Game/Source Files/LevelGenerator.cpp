#include <windows.h>
#include "LevelGenerator.h"
#include "random.h"


LevelGeneratorC* LevelGeneratorC::sInstance = nullptr;

//---------------------------------------------------------------------------------------------------------------------
LevelGeneratorC * LevelGeneratorC::CreateInstance()
{
	if (sInstance == nullptr)
	{
		sInstance = new LevelGeneratorC();
	}

	return sInstance;
}

//---------------------------------------------------------------------------------------------------------------------
void LevelGeneratorC::generateLevel(Map_t* basicMap)
{
	generateSoftBlocks(basicMap);
	generatePerk(basicMap);
	generateDoor(basicMap);
}

//---------------------------------------------------------------------------------------------------------------------
void LevelGeneratorC::generateSoftBlocks(Map_t* basicMap)
{
	// starts from bottom left
	uint32_t softBlocksNum = getRangedRandom(MIN_NUM_SOFT_BLOCKS, MAX_NUM_SOFT_BLOCKS);

	while (softBlocksNum > 0)
	{
		TileCoor_t randomTile;

		randomTile.x = getRangedRandom(basicMap->playerSpawnTile.x, basicMap->width - 1);
		randomTile.y = getRangedRandom(1, basicMap->playerSpawnTile.y + 1);

		uint32_t index = basicMap->blocksLayer[randomTile.x][randomTile.y];
		
		if (!index)
		{
			bool restricted = false;
			for (uint16_t i = 0; i < basicMap->numRestrictedTiles; i++)
			{
				restricted = sameTile(randomTile, basicMap->restictedTiles[i]);
				if (restricted)
				{
					break;
				}
			}

			if (restricted)
			{
				continue;;
			}
			else
			{
				basicMap->blocksLayer[randomTile.x][randomTile.y] = ((uint32_t)SpriteIndicesInMap_t::softBlock);
				softBlocksNum--;
			}
		}
	}

}

//---------------------------------------------------------------------------------------------------------------------
void LevelGeneratorC::generatePerk(Map_t* basicMap)
{
	bool itemAdded = false;

	while (!itemAdded)
	{
		TileCoor_t randomTile;

		randomTile.x = getRangedRandom(basicMap->playerSpawnTile.x, basicMap->width - 1);
		randomTile.y = getRangedRandom(1, basicMap->playerSpawnTile.y + 1);

		uint32_t index = basicMap->blocksLayer[randomTile.x][randomTile.y];

		if (index == (uint32_t)SpriteIndicesInMap_t::softBlock)
		{
			itemAdded = true;
			basicMap->perkTile.tileCoor = randomTile;
			basicMap->perkTile.spriteIndex =(uint32_t) PerksIndicesInSpriteSheet_t::fire;
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------
void LevelGeneratorC::generateDoor(Map_t * basicMap)
{
	bool itemAdded = false;

	while (!itemAdded)
	{
		TileCoor_t randomTile;

		randomTile.x = getRangedRandom(basicMap->playerSpawnTile.x, basicMap->width - 1);
		randomTile.y = getRangedRandom(1, basicMap->playerSpawnTile.y + 1);

		if (sameTile(randomTile, basicMap->perkTile.tileCoor))
		{
			continue;
		}

		uint32_t index = basicMap->blocksLayer[randomTile.x][randomTile.y];

		if (index == (uint32_t)SpriteIndicesInMap_t::softBlock)
		{
			itemAdded = true;
			basicMap->doorTile.tileCoor = randomTile;
			basicMap->doorTile.spriteIndex = (uint32_t)SpriteIndicesInSpriteSheet_t::door;
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------
bool LevelGeneratorC::sameTile(TileCoor_t first, TileCoor_t second)
{
	return first.x == second.x && first.y == second.y;
}
