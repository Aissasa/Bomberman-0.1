#pragma once

#include "baseTypes.h"
#include "GameDataStructures.h"
#include "Player.h"

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

	PlayerC* mPlayer;

	// enemies
	// bombs  : manage their own animation by calling the renderer(in their update that is called in this update)

};
