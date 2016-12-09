#pragma once

#include "baseTypes.h"
#include "GameDataStructures.h"

class CollisionManagerC
{
public:
	~CollisionManagerC() {};

	static CollisionManagerC* CreateInstance();
	static CollisionManagerC* GetInstance() { return sInstance; };

	void init(Map_t* map) { mCurrentMap = map; };

	bool checkCharacterCollisionWithBlocks(const Coord2D& position, const Coord2D& direction);
	bool checkCharacterCollisionWithBombs(const Coord2D& position, const Coord2D& direction);
	bool checkCharacterCollisionWithCharacters(const Coord2D& position);
	bool checkCharacterCollisionWithBombAE(const Coord2D& position);

	Coord2D getTilePosition(const TileCoor_t& tile);
	TileCoor_t getTileCoorFromPosition(const Coord2D& coor);

private:
	static CollisionManagerC* sInstance;
	static const uint32_t sMargin;
	CollisionManagerC() {};
	Map_t* mCurrentMap;

	bool boxCollision(const Coord2D& source, const Coord2D& target);

};
