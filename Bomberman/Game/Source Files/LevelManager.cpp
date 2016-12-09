#include <windows.h>
#include "assert.h"
#include "LevelManager.h"
#include "LevelGenerator.h"
#include "CollisionManager.h"

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
	CollisionManagerC::CreateInstance();

	LevelGeneratorC::GetInstance()->generateLevel(mCurrentMap);
	CollisionManagerC::GetInstance()->init(mCurrentMap);

	mPlayer = new PlayerC();
	Coord2D playerInitiCoor;
	playerInitiCoor.x = (float_t) mCurrentMap->playerSpawnTile.x * currentMap->tileWidth;
	playerInitiCoor.y = (float_t) mCurrentMap->playerSpawnTile.y * currentMap->tileHeight;

	mPlayer->init(playerInitiCoor);
}

//---------------------------------------------------------------------------------------------------------------------
void LevelManagerC::update(DWORD milliseconds)
{
	mPlayer->update(milliseconds);
}


