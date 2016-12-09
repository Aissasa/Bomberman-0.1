#pragma once

#include "baseTypes.h"
#include "GameDataStructures.h"
#include "Player.h"
#include "Bomb.h"


class LevelManagerC
{
public:
	static LevelManagerC* CreateInstance();
	static LevelManagerC* GetInstance() { return sInstance; };
	~LevelManagerC() {};

	void init(Map_t* currentMap);
	void update(DWORD milliseconds);

	void addBomb(BombC& bomb);
	void addToTobeRemovedArray(BombC& bomb);

	uint16_t getCurrentNumberBombs();
	bool tileOccupiedByBomb(Coord2D tileCoor);

private:

	static LevelManagerC* sInstance;
	LevelManagerC() {};

	Map_t* mCurrentMap;
	PlayerC* mPlayer;

	Bombs_Vect_t mBombsVect;
	Bombs_Vect_t mBombsToRemoveVect;
	// todo add enemies

	void removeDestroyedBombs();

};
