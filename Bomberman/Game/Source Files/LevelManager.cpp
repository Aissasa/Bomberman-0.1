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

	uint16_t bombsAEVectLength = (uint16_t)mBombsAEVect.size();
	for (uint16_t i = 0; i < bombsAEVectLength; i++)
	{
		BombAEC* currentBomb = mBombsAEVect[i];
		currentBomb->update(milliseconds);
	}

	uint16_t bombsVectLength = (uint16_t)mBombsVect.size();
	for (uint16_t i = 0; i < bombsVectLength; i++)
	{
		mBombsVect[i].update(milliseconds);
	}

	explodeBombs();
	destroyVanishingBombsAE();
}

//---------------------------------------------------------------------------------------------------------------------
void LevelManagerC::addBomb(BombC& bomb)
{
	mBombsVect.push_back(bomb);
}

//---------------------------------------------------------------------------------------------------------------------
void LevelManagerC::addToExplodingBombs(BombC& bomb)
{
	mExplodingBombsVect.push_back(bomb);
}

//---------------------------------------------------------------------------------------------------------------------
void LevelManagerC::addToVanishingBombsAE(BombAEC * bombAE)
{
	mVanishingBombsAEVect.push_back(bombAE);
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
void LevelManagerC::explodeBombs()
{
	if (!mExplodingBombsVect.empty())
	{
		uint16_t explodingBombsVectLength = (uint16_t)mExplodingBombsVect.size();

		for (uint16_t i = 0; i < explodingBombsVectLength; i++)
		{

			createBombAE(mExplodingBombsVect[i]);

			uint16_t bombsVectLength = (uint16_t)mBombsVect.size();

			for (uint16_t j = 0; j < bombsVectLength; i++)
			{
				if (mBombsVect[j].getId() == mExplodingBombsVect[i].getId())
				{
					mBombsVect.erase(mBombsVect.begin() + j);
					break;
				}
			}
		}

		mExplodingBombsVect.clear();
	}
}

//---------------------------------------------------------------------------------------------------------------------
void LevelManagerC::createBombAE(BombC & bomb)
{
	mBombsAEVect.push_back(new BombAEC(bomb.getPosition(), bomb.getAfterEffectAnimation(BombAEType_t::Center)));
	TileCoor_t centerTile = CollisionManagerC::GetInstance()->getTileCoorFromPosition(bomb.getPosition());

	uint16_t tempRange = 1;
	TileCoor_t tempTile = centerTile;

	bool rightIsDone = false;
	bool leftIsDone = false;
	bool topIsDone = false;
	bool bottomIsDone = false;

	while (tempRange < bomb.getRange())
	{
		// right
		if (!rightIsDone)
		{
			tempTile.x += tempRange;
			mBombsAEVect.push_back(new BombAEC(CollisionManagerC::GetInstance()->getPositionFromTileCoor(tempTile), bomb.getAfterEffectAnimation(BombAEType_t::Horizontal)));
		}

		// left
		if (!leftIsDone)
		{
			tempTile = centerTile;
			tempTile.x -= tempRange;
			mBombsAEVect.push_back(new BombAEC(CollisionManagerC::GetInstance()->getPositionFromTileCoor(tempTile), bomb.getAfterEffectAnimation(BombAEType_t::Horizontal)));
		}

		// top
		if (!topIsDone)
		{
			tempTile = centerTile;
			tempTile.y += tempRange;
			mBombsAEVect.push_back(new BombAEC(CollisionManagerC::GetInstance()->getPositionFromTileCoor(tempTile), bomb.getAfterEffectAnimation(BombAEType_t::Vertical)));
		}

		// bottom
		if (!bottomIsDone)
		{
			tempTile = centerTile;
			tempTile.y -= tempRange;
			mBombsAEVect.push_back(new BombAEC(CollisionManagerC::GetInstance()->getPositionFromTileCoor(tempTile), bomb.getAfterEffectAnimation(BombAEType_t::Vertical)));
		}

		tempRange++;
	}

	if (!rightIsDone)
	{
		tempTile = centerTile;
		tempTile.x += tempRange;
		mBombsAEVect.push_back(new BombAEC(CollisionManagerC::GetInstance()->getPositionFromTileCoor(tempTile), bomb.getAfterEffectAnimation(BombAEType_t::Right)));
	}
	if (!leftIsDone)
	{
		tempTile = centerTile;
		tempTile.x -= tempRange;
		mBombsAEVect.push_back(new BombAEC(CollisionManagerC::GetInstance()->getPositionFromTileCoor(tempTile), bomb.getAfterEffectAnimation(BombAEType_t::Left)));
	}
	if (!topIsDone)
	{
		tempTile = centerTile;
		tempTile.y += tempRange;
		mBombsAEVect.push_back(new BombAEC(CollisionManagerC::GetInstance()->getPositionFromTileCoor(tempTile), bomb.getAfterEffectAnimation(BombAEType_t::Top)));
	}
	if (!bottomIsDone)
	{
		tempTile = centerTile;
		tempTile.y -= tempRange;
		mBombsAEVect.push_back(new BombAEC(CollisionManagerC::GetInstance()->getPositionFromTileCoor(tempTile), bomb.getAfterEffectAnimation(BombAEType_t::Bottom)));
	}
}

//---------------------------------------------------------------------------------------------------------------------
void LevelManagerC::destroyVanishingBombsAE()
{
	if (!mVanishingBombsAEVect.empty())
	{
		uint16_t vanishingBombsAEVectLength = (uint16_t)mVanishingBombsAEVect.size();

		for (uint16_t i = 0; i < vanishingBombsAEVectLength; i++)
		{
			uint16_t bombsAEVectLength = (uint16_t)mBombsAEVect.size();
			for (uint16_t j = 0; j < bombsAEVectLength; i++)
			{
				if (mBombsAEVect[j] == mVanishingBombsAEVect[i])
				{
					mBombsAEVect.erase(mBombsAEVect.begin() + j);

					BombAEC* temp = mVanishingBombsAEVect[i];
					delete temp;
					break;
				}
			}
		}

		mVanishingBombsAEVect.clear();
	}
}


