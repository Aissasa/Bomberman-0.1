#pragma once

#include "baseTypes.h"
#include "RenderingDataStructures.h"

#define DEATH_ANIMATION_DELAY 400
#define DEATH_ANIMATION_TIME 250

typedef struct CharacterAnimations
{
	Animation_t* idleUp;
	Animation_t* idleDown;
	Animation_t* idleRight;
	Animation_t* idleLeft;

	Animation_t* walkUp;
	Animation_t* walkDown;
	Animation_t* walkRight;
	Animation_t* walkLeft;

	Animation_t* death;

}CharacterAnimations_t;

typedef struct CharacterMovement
{
	bool goingUp;
	bool goingDown;
	bool goingLeft;
	bool goingRight;

}CharacterMovement_t;


class CharacterC
{
public:
	CharacterC() {};
	~CharacterC() {};

	virtual void init() {};
	virtual void update(DWORD milliseconds) {};

	Coord2D getCurrentPostion() { return mCurrentPosition; };

protected:
	
	Coord2D mCurrentPosition;
	float_t mBaseSpeed;
	bool mIsDying;
	bool mIsDead;

	CharacterAnimations_t* mCharAnimations;
	Animation_t* mCurrentAnimation;
	int32_t mAnimationTimer;

	CharacterMovement_t mCharacterMovement;

	virtual void setAnimations() {};
	virtual void updateMovementDirection() {};
	virtual bool checkCollisions(DWORD milliseconds) { return true; };
	virtual bool checkCollisionsWithBlocks(DWORD milliseconds) { return true; };
	virtual bool checkCollisionsWithCharacters() { return true; };
	virtual bool checkCollisionsBombs(DWORD milliseconds) { return true; };
	virtual bool checkCollisionsWithBombsAE() { return true; };
	virtual void updatePosition(DWORD milliseconds) {};
	
	void updateAnimation(DWORD milliseconds);
	void doMovementAnimation(DWORD milliseconds, Animation_t* potentialAnimation);
	void doDeathAnimation(DWORD milliseconds, Animation_t* potentialAnimation);
	CharacterAnimations_t* createCharAnimations();
	void populateAnimation(Animation_t* anim, const Animation_t& sourceAnim);

	void render();

	// todo cut corners: start going up while walking right
};
