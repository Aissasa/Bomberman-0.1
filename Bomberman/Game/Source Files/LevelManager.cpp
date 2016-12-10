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

	mPerkConsumed = false;

	mPlayer = new PlayerC();
	Coord2D playerInitiCoor;
	playerInitiCoor.x = (float_t)mCurrentMap->playerSpawnTile.x * currentMap->tileWidth;
	playerInitiCoor.y = (float_t)mCurrentMap->playerSpawnTile.y * currentMap->tileHeight;

	mPlayer->init(playerInitiCoor);

	mCurrentLevelState = LevelStates_t::Play;
}

//---------------------------------------------------------------------------------------------------------------------
void LevelManagerC::update(DWORD milliseconds)
{
	switch (mCurrentLevelState)
	{
		case LevelStates_t::Play:
			handlePlayState(milliseconds);
			break;

		case LevelStates_t::Pause:
			handlePauseState(milliseconds);
			break;

		case LevelStates_t::Cleared:
			handleClearedState(milliseconds);
			break;

		case LevelStates_t::Failed:
			handleFailedState(milliseconds);
			break;

	}
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
void LevelManagerC::addToFadingBlocks(FadingSoftBlockC * softBlock)
{
	mFadingSoftBlocksVect.push_back(softBlock);
}

//---------------------------------------------------------------------------------------------------------------------
void LevelManagerC::addToDestroyedBlocks(FadingSoftBlockC * softBlock)
{
	mDestroyedSoftBlocksVect.push_back(softBlock);
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
void LevelManagerC::handlePlayState(DWORD milliseconds)
{
	if (mPlayer->getIsDead())
	{
		mCurrentLevelState = LevelStates_t::Failed;
		return;
	}

	if (CollisionManagerC::GetInstance()->checkCharacterCollisionWithDoor(mPlayer->getCurrentPostion()))
	{
		mCurrentLevelState = LevelStates_t::Cleared;
		return;
	}

	mPlayer->update(milliseconds);

	if (!mPerkConsumed && CollisionManagerC::GetInstance()->checkCharacterCollisionWithPerk(mPlayer->getCurrentPostion()))
	{
		mPerkConsumed = true;
		mPlayer->addPerk(mCurrentMap->perkTile.spriteIndex);
	}

	// update fading blocks
	uint16_t fadingSoftBlocksVectLength = (uint16_t)mFadingSoftBlocksVect.size();
	for (uint16_t i = 0; i < fadingSoftBlocksVectLength; i++)
	{
		FadingSoftBlockC* currentFadingSoftBlock = mFadingSoftBlocksVect[i];
		currentFadingSoftBlock->update(milliseconds);
	}

	// update bombsAE
	uint16_t bombsAEVectLength = (uint16_t)mBombsAEVect.size();
	for (uint16_t i = 0; i < bombsAEVectLength; i++)
	{
		BombAEC* currentBombAE = mBombsAEVect[i];
		currentBombAE->update(milliseconds);
	}

	// update bombs
	uint16_t bombsVectLength = (uint16_t)mBombsVect.size();
	for (uint16_t i = 0; i < bombsVectLength; i++)
	{
		mBombsVect[i].update(milliseconds);
	}

	explodeBombs();
	destroyVanishingBombsAE();
	destroyFadingBlocks();
}

//---------------------------------------------------------------------------------------------------------------------
void LevelManagerC::handlePauseState(DWORD milliseconds)
{
}

//---------------------------------------------------------------------------------------------------------------------
void LevelManagerC::handleClearedState(DWORD milliseconds)
{
}

//---------------------------------------------------------------------------------------------------------------------
void LevelManagerC::handleFailedState(DWORD milliseconds)
{
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

	while (tempRange <= bomb.getRange())
	{
		// right
		if (!rightIsDone)
		{
			tempTile = centerTile;
			tempTile.x += tempRange;
			if (tempRange<bomb.getRange())
			{
				handleBombAEResult(bomb, tempTile, &rightIsDone, BombAEType_t::Horizontal);
			}
			else
			{
				handleBombAEResult(bomb, tempTile, &rightIsDone, BombAEType_t::Right);
			}
		}

		// left
		if (!leftIsDone)
		{
			tempTile = centerTile;
			tempTile.x -= tempRange;
			if (tempRange < bomb.getRange())
			{
				handleBombAEResult(bomb, tempTile, &rightIsDone, BombAEType_t::Horizontal);
			}
			else
			{
				handleBombAEResult(bomb, tempTile, &rightIsDone, BombAEType_t::Left);
			}
		}

		// top
		if (!topIsDone)
		{
			tempTile = centerTile;
			tempTile.y += tempRange;
			if (tempRange < bomb.getRange())
			{
				handleBombAEResult(bomb, tempTile, &rightIsDone, BombAEType_t::Vertical);
			}
			else
			{
				handleBombAEResult(bomb, tempTile, &rightIsDone, BombAEType_t::Top);
			}
		}

		// bottom
		if (!bottomIsDone)
		{
			tempTile = centerTile;
			tempTile.y -= tempRange;
			if (tempRange < bomb.getRange())
			{
				handleBombAEResult(bomb, tempTile, &rightIsDone, BombAEType_t::Vertical);
			}
			else
			{
				handleBombAEResult(bomb, tempTile, &rightIsDone, BombAEType_t::Bottom);
			}
		}

		tempRange++;
	}
}

//---------------------------------------------------------------------------------------------------------------------
void LevelManagerC::handleBombAEResult(BombC& bomb, TileCoor_t & tile, bool * done, BombAEType_t bombAEType)
{
	uint32_t spriteIndex = mCurrentMap->blocksLayer[tile.x][tile.y];
	if (spriteIndex == (uint32_t)SpriteIndicesInMap_t::solidBlock || spriteIndex == (uint32_t)SpriteIndicesInMap_t::softBlock)
	{
		*done = true;

		if (spriteIndex == (uint32_t)SpriteIndicesInMap_t::softBlock)
		{
			mFadingSoftBlocksVect.push_back(new FadingSoftBlockC(CollisionManagerC::GetInstance()->getPositionFromTileCoor(tile)));
			mCurrentMap->blocksLayer[tile.x][tile.y] = (uint32_t)SpriteIndicesInMap_t::none;
		}
	}
	else
	{
		mBombsAEVect.push_back(new BombAEC(CollisionManagerC::GetInstance()->getPositionFromTileCoor(tile), 
										   bomb.getAfterEffectAnimation(bombAEType)));

		//if (hitBomb)
		//{
		//	explodebomb
		//}
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

//---------------------------------------------------------------------------------------------------------------------
void LevelManagerC::destroyFadingBlocks()
{
	if (!mDestroyedSoftBlocksVect.empty())
	{
		uint16_t destroyedSoftBlocksVectLength = (uint16_t)mDestroyedSoftBlocksVect.size();

		for (uint16_t i = 0; i < destroyedSoftBlocksVectLength; i++)
		{
			uint16_t fadingSoftBlocksVectLength = (uint16_t)mFadingSoftBlocksVect.size();
			for (uint16_t j = 0; j < fadingSoftBlocksVectLength; i++)
			{
				if (mFadingSoftBlocksVect[j] == mDestroyedSoftBlocksVect[i])
				{
					mFadingSoftBlocksVect.erase(mFadingSoftBlocksVect.begin() + j);

					FadingSoftBlockC* temp = mDestroyedSoftBlocksVect[i];
					delete temp;
					break;
				}
			}
		}
		mDestroyedSoftBlocksVect.clear();
	}
}


