#pragma once

#include "Character.h"

#define BASE_SPEED 1.2
#define SPEED_INCREMENT 0.4

typedef struct Perks
{
	uint16_t bombUp;
	uint16_t fire;
	uint16_t skate;

	bool remote;
	bool passBomb;
	bool passSoftBlocks;

}Perks_t;

class PlayerC : public CharacterC
{
public:
	PlayerC() : CharacterC::CharacterC() {};
	~PlayerC() {};

	void init(Coord2D initialCoor);
	void update(DWORD milliseconds);

	void PlaceBomb(Coord2D position, bool remoteActive);
	void TriggerBombs();

private:

	void setAnimations();
	void updatePosition(DWORD milliseconds);
	//void updateAnimation(DWORD milliseconds);
	//void animate(DWORD milliseconds, Animation_t& potentialAnimation);

	uint32_t mCurrentMaxBombNumber;
};
