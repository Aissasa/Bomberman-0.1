#pragma once

#include "Character.h"

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

	void init() {};
	void update(DWORD milliseconds) {};


	void PlaceBomb(Coord2D position, bool remoteActive);
	void TriggerBombs();

private:
	uint32_t mCurrentMaxBombNumber;
};
