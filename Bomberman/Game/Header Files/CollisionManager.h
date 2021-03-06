#pragma once

#include "baseTypes.h"
#include "GameDataStructures.h"
#include "Bomb.h"
#include "BombAE.h"

class CollisionManagerC
{
public:
	~CollisionManagerC() {};

	static CollisionManagerC* CreateInstance();
	static CollisionManagerC* GetInstance() { return sInstance; };

	void init(Map_t* map) { mCurrentMap = map; };

	bool checkCharacterCollisionWithBlocks(const Coord2D& position, const Coord2D& direction);
	bool checkCharacterCollisionWithBombs(const Coord2D& position, const Coord2D& direction, Bombs_Vect_t& bombsVect);
	bool checkCharacterCollisionWithCharacters(const Coord2D& sourcePosition, const Coord2D& targetPosition);
	bool checkCharacterCollisionWithBombsAE(const Coord2D& position, const Bombs_AE_Ptr_Vect_t bombsAEVect);
	bool checkCharacterCollisionWithDoor(const Coord2D& position);
	bool checkCharacterCollisionWithPerk(const Coord2D& position);


	Coord2D getPositionFromTileCoor(const TileCoor_t& tile);
	TileCoor_t getTileCoorFromPosition(const Coord2D& coor);

private:
	static CollisionManagerC* sInstance;
	static const int16_t sMarginForBlocksCol;
	static const int16_t sMarginForBombAECol;
	static const int16_t sMarginForPerksCol;
	static const int16_t sMarginForDoorCol;
	CollisionManagerC() {};
	Map_t* mCurrentMap;

	bool boxCollision(const Coord2D& source, const Coord2D& target, int16_t toleranceMargin);

};
