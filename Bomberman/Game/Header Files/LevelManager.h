#pragma once

#include "baseTypes.h"
#include "GameDataStructures.h"

// todo add a level info struct

class LevelManagerC
{
public:
	static LevelManagerC* CreateInstance();
	static LevelManagerC* GetInstance() { return sInstance; };
	~LevelManagerC() {};

	void init(Map_t* currentMap);
	void update(DWORD milliseconds);

private:

	static LevelManagerC* sInstance;
	LevelManagerC() {};

	Map_t* mCurrentMap;
	// enemies
	// bombs  : manage their own animation by calling the renderer(in their update that is called in this update)
	// perk pos and door pos
	// lvl generator(enemies, soft blocks(with min and max), perk, door), knows the restricted locations
	// (player spawn position perimeter)
	// lvl manager passes the enemies number, types 
	// todo lvl reader later with info about enemy types, numbers, and perk

};
