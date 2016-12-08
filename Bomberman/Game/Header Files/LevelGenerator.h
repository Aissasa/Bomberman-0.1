#pragma once

#include "baseTypes.h"
#include "GameDataStructures.h"

#define MIN_NUM_SOFT_BLOCKS 80
#define MAX_NUM_SOFT_BLOCKS 120

class LevelGeneratorC
{
public:
	// todo add level parsing
	static LevelGeneratorC* CreateInstance();
	static LevelGeneratorC* GetInstance() { return sInstance; };
	~LevelGeneratorC() {};

	void generateLevel(Map_t* basicMap);

private:

	static LevelGeneratorC* sInstance;
	LevelGeneratorC() {};

	void generateSoftBlocks(Map_t* basicMap);
	void generatePerk(Map_t* basicMap);
	void generateDoor(Map_t* basicMap);

	bool sameTile(TileCoor_t first, TileCoor_t second);

};
