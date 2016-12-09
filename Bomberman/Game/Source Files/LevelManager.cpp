#include <windows.h>
#include "assert.h"
#include "LevelManager.h"
#include "LevelGenerator.h"
#include "CollisionManager.h"
#include <algorithm>

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
	playerInitiCoor.x = (float_t)mCurrentMap->playerSpawnTile.x * currentMap->tileWidth;
	playerInitiCoor.y = (float_t)mCurrentMap->playerSpawnTile.y * currentMap->tileHeight;

	mPlayer->init(playerInitiCoor);
}

//---------------------------------------------------------------------------------------------------------------------
void LevelManagerC::update(DWORD milliseconds)
{
	mPlayer->update(milliseconds);

	uint16_t bombsVectLength = (uint16_t)mBombsVect.size();
	for (uint16_t i = 0; i < bombsVectLength; i++)
	{
		mBombsVect[i].update(milliseconds);
	}

	removeDestroyedBombs();
}

//---------------------------------------------------------------------------------------------------------------------
void LevelManagerC::addBomb(BombC& bomb)
{
	mBombsVect.push_back(bomb);
}

//---------------------------------------------------------------------------------------------------------------------
void LevelManagerC::addToTobeRemovedArray(BombC& bomb)
{
	//mBombsVect.erase(mBombsVect.begin());
	mBombsToRemoveVect.push_back(bomb);
}

//---------------------------------------------------------------------------------------------------------------------
uint16_t LevelManagerC::getCurrentNumberBombs()
{
	return (uint16_t)mBombsVect.size();
}

//---------------------------------------------------------------------------------------------------------------------
bool LevelManagerC::tileOccupiedByBomb(Coord2D tileCoor)
{
	if (!mBombsVect.empty())
	{
		uint16_t bombsVectLength = (uint16_t)mBombsVect.size();
		for (uint16_t i = 0; i < bombsVectLength; i++)
		{
			if (mBombsVect[i].getPosition().x == tileCoor.x && mBombsVect[i].getPosition().y == tileCoor.y)
			{
				return true;
			}
		}
	}
	return false;
}

//---------------------------------------------------------------------------------------------------------------------
void LevelManagerC::removeDestroyedBombs()
{
	if (!mBombsToRemoveVect.empty())
	{
		uint16_t bombsToRemoveVectLength = (uint16_t)mBombsToRemoveVect.size();

		for (uint16_t i = 0; i < bombsToRemoveVectLength; i++)
		{
			uint16_t bombsVectLength = (uint16_t)mBombsVect.size();

			for (uint16_t j = 0; j < bombsVectLength; i++)
			{
				if (mBombsVect[j].getId() == mBombsToRemoveVect[i].getId())
				{
					mBombsVect.erase(mBombsVect.begin() + j);
					break;
				}
			}
		}

		mBombsToRemoveVect.clear();
	}
}


