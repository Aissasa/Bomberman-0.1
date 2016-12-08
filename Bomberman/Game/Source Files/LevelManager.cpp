#include <windows.h>
#include "assert.h"
#include "LevelManager.h"
#include "LevelGenerator.h"

LevelManagerC* LevelManagerC::sInstance = nullptr;

//---------------------------------------------------------------------------------------------------------------------
LevelManagerC * LevelManagerC::CreateInstance()
{
	if (sInstance == nullptr)
	{
		sInstance = new LevelManagerC();
	}

	return sInstance;
}

//---------------------------------------------------------------------------------------------------------------------
void LevelManagerC::init(Map_t* currentMap)
{
	mCurrentMap = currentMap;

	LevelGeneratorC::CreateInstance();
	LevelGeneratorC::GetInstance()->generateLevel(mCurrentMap);

	mPlayer = new PlayerC();
	Coord2D coor;
	coor.x = (float_t) mCurrentMap->playerSpawnTile.x * currentMap->tileWidth;
	coor.y = (float_t) mCurrentMap->playerSpawnTile.y * currentMap->tileHeight;

	mPlayer->init(coor);
}

//---------------------------------------------------------------------------------------------------------------------
void LevelManagerC::update(DWORD milliseconds)
{
	mPlayer->update(milliseconds);
}


