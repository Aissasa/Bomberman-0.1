#pragma once

#include "baseTypes.h"
#include "RenderingDataStructures.h"

typedef struct CharacterAnimations
{
	Animation_t* idleUp;
	Animation_t* idleDown;
	Animation_t* idleRight;
	Animation_t* idleLeft;

	Animation_t* moveUp;
	Animation_t* moveDown;
	Animation_t* moveRight;
	Animation_t* moveLeft;

}CharacterAnimations_t;


class CharacterC
{
public:
	CharacterC() {};
	~CharacterC() {};

	virtual void init() {};
	virtual void update(DWORD milliseconds) {};

	// todo add accessors here
	Coord2D getCurrentPostion() { return mCurrentPosition; };

protected:
	
	Coord2D mCurrentPosition;
	float_t mBaseSpeed;

	CharacterAnimations_t* mCharAnimations;
	Animation_t mCurrentAnimation;

	void render();

	// todo cut corners: start going up while walking right
};
