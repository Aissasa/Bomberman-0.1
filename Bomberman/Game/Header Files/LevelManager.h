#pragma once

#include "baseTypes.h"
#include "GameDataStructures.h"
#include "Player.h"
#include "Bomb.h"
#include "BombAE.h"
#include "FadingSoftBlock.h"

//---------------------------------------------------------------------------------------------------------------------
typedef enum class LevelStates
{
	Play,
	Pause,
	Cleared,
	Failed
}LevelStates_t;

//---------------------------------------------------------------------------------------------------------------------
class LevelManagerC
{
public:
	static LevelManagerC* CreateInstance();
	static LevelManagerC* GetInstance() { return sInstance; };
	~LevelManagerC() {};

	void init(Map_t* currentMap);
	void update(DWORD milliseconds);

	void addBomb(BombC& bomb);
	void addToExplodingBombs(BombC& bomb);

	void addToVanishingBombsAE(BombAEC* bombAE);

	void addToFadingBlocks(FadingSoftBlockC* softBlock);
	void addToDestroyedBlocks(FadingSoftBlockC* softBlock);

	Bombs_Vect_t getBombsVect() { return mBombsVect; };
	Bombs_AE_Ptr_Vect_t getBombsAEVect() { return mBombsAEVect; };
	uint16_t getCurrentNumberBombs();
	bool tileOccupiedByBomb(Coord2D tileCoor);


private:

	static LevelManagerC* sInstance;
	LevelManagerC() {};

	LevelStates_t mCurrentLevelState;

	Map_t* mCurrentMap;
	PlayerC* mPlayer;

	bool mPerkConsumed;

	Bombs_Vect_t mBombsVect;
	Bombs_Vect_t mExplodingBombsVect;

	Bombs_AE_Ptr_Vect_t mBombsAEVect;
	Bombs_AE_Ptr_Vect_t mVanishingBombsAEVect;

	Fading_SoftBlock_Ptr_Vect_t mFadingSoftBlocksVect;
	Fading_SoftBlock_Ptr_Vect_t mDestroyedSoftBlocksVect;

	void handlePlayState(DWORD milliseconds);
	void handlePauseState(DWORD milliseconds);
	void handleClearedState(DWORD milliseconds);
	void handleFailedState(DWORD milliseconds);

	void explodeBombs();
	void createBombAE(BombC& bomb);
	void handleBombAEResult(BombC& bomb, TileCoor_t& tile, bool* done, BombAEType_t bombAEType);

	void destroyVanishingBombsAE();
	void destroyFadingBlocks();

};
