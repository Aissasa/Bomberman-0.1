#pragma once

#include "Character.h"
#include "baseTypes.h"

#define BASE_SPEED 1.2
#define SPEED_INCREMENT 0.4
#define BTN_COOLDOWN_TIMER 200

//---------------------------------------------------------------------------------------------------------------------
typedef struct Perks
{
	uint16_t bombUp;
	uint16_t fire;
	uint16_t skate;

	bool remote;
	bool passBomb;
	bool passSoftBlocks;

}Perks_t;

//---------------------------------------------------------------------------------------------------------------------
class PlayerC : public CharacterC
{
public:
	PlayerC() : CharacterC::CharacterC() {};
	~PlayerC() {};

	void init(Coord2D initialCoor);
	void update(DWORD milliseconds);

	void placeBomb();
	void triggerBombs();

	void addPerk(uint32_t perk);

private:

	void setAnimations();
	void updateMovementDirection();
	bool checkCollisions(DWORD milliseconds);
	bool checkCollisionsWithBlocks(DWORD milliseconds);
	bool checkCollisionsWithCharacters();
	bool checkCollisionsBombs(DWORD milliseconds);
	bool checkCollisionsWithBombsAE();
	void updatePosition(DWORD milliseconds);
	void updateActions(DWORD milliseconds);

	Perks_t* mPerks;
	uint16_t mBaseMaxBombNumber;
	uint16_t mBaseBombRange;

	Coord2D mCurrentDirection;

	bool mCanMoveHorizontally;
	bool mCanMoveVertically;
};
