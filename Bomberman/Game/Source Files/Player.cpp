#include <windows.h>
#include "Player.h"
#include "InputManager.h"
#include "SpriteManager.h"
#include "CollisionManager.h"

//---------------------------------------------------------------------------------------------------------------------
void PlayerC::init(Coord2D initialCoor)
{
	mCurrentPosition = initialCoor;
	setAnimations();

	mPerks = (Perks_t*)malloc(sizeof(Perks_t));
	mPerks->bombUp = 0;
	mPerks->fire = 0;
	mPerks->skate = 0;
	mPerks->passBomb = false;
	mPerks->passSoftBlocks = false;
	mPerks->remote = false;

	mCurrentDirection = { 0, 0 };
	mCharacterMovement = { false, false, false, false };
	mCanMoveHorizontally = true;
	mCanMoveVertically = true;
	mIsDying = false;

	mBaseSpeed = (float_t)BASE_SPEED;
	mBaseMaxBombNumber = 1;
	mBaseBombRange = 1;
}

//---------------------------------------------------------------------------------------------------------------------
void PlayerC::update(DWORD milliseconds)
{
	if (!mIsDying)
	{
		updateMovementDirection();
		checkCollisions(milliseconds);
		updatePosition(milliseconds);
	}

	updateAnimation(milliseconds);
	render();
}

//---------------------------------------------------------------------------------------------------------------------
void PlayerC::PlaceBomb(Coord2D position, bool remoteActive)
{
}

//---------------------------------------------------------------------------------------------------------------------
void PlayerC::TriggerBombs()
{

}

//---------------------------------------------------------------------------------------------------------------------
void PlayerC::setAnimations()
{
	mCharAnimations = createCharAnimations();

	Animation_t* mcAnimations = SpriteManagerC::GetInstance()->getAnimations(SpriteSheetType_t::MC);

	// note change this with animation name using strstr

	populateAnimation(mCharAnimations->death, mcAnimations[0]);
	populateAnimation(mCharAnimations->idleDown, mcAnimations[1]);
	populateAnimation(mCharAnimations->walkDown, mcAnimations[2]);
	populateAnimation(mCharAnimations->idleLeft, mcAnimations[3]);
	populateAnimation(mCharAnimations->walkLeft, mcAnimations[4]);
	populateAnimation(mCharAnimations->idleRight, mcAnimations[5]);
	populateAnimation(mCharAnimations->walkRight, mcAnimations[6]);
	populateAnimation(mCharAnimations->idleUp, mcAnimations[7]);
	populateAnimation(mCharAnimations->walkUp, mcAnimations[8]);

	mCurrentAnimation = mCharAnimations->idleDown;
	mAnimationTimer = mCurrentAnimation->timeFromSpriteToSprite;
}

//---------------------------------------------------------------------------------------------------------------------
void PlayerC::updateMovementDirection()
{
	mCurrentDirection = InputManagerC::GetInstance()->getPlayerDirection();

	mCharacterMovement.goingLeft = mCurrentDirection.x < 0;
	mCharacterMovement.goingRight = mCurrentDirection.x > 0;
	mCharacterMovement.goingUp = mCurrentDirection.y > 0;
	mCharacterMovement.goingDown = mCurrentDirection.y < 0;

}

//---------------------------------------------------------------------------------------------------------------------
bool PlayerC::checkCollisions(DWORD milliseconds)
{
	// todo add the other collisions logic
	return checkCollisionsWithBlocks(milliseconds);
}

//---------------------------------------------------------------------------------------------------------------------
bool PlayerC::checkCollisionsWithBlocks(DWORD milliseconds)
{
	Coord2D potentialPos = { mCurrentPosition.x + mCurrentDirection.x * mBaseSpeed * milliseconds / 10, mCurrentPosition.y + mCurrentDirection.y * mBaseSpeed * milliseconds / 10 };
	mCanMoveHorizontally = !CollisionManagerC::GetInstance()->checkCharacterCollisionWithBlocks(potentialPos, {mCurrentDirection.x, 0});
	mCanMoveVertically = !CollisionManagerC::GetInstance()->checkCharacterCollisionWithBlocks(potentialPos, { 0, mCurrentDirection.y });

	return mCanMoveHorizontally && mCanMoveVertically;
}

//---------------------------------------------------------------------------------------------------------------------
bool PlayerC::checkCollisionsWithCharacters()
{
	return true;
}

//---------------------------------------------------------------------------------------------------------------------
bool PlayerC::checkCollisionsBombs(DWORD milliseconds)
{
	return true;
}

//---------------------------------------------------------------------------------------------------------------------
bool PlayerC::checkCollisionsWithBombsAE()
{
	return true;
}

//---------------------------------------------------------------------------------------------------------------------
void PlayerC::updatePosition(DWORD milliseconds)
{
	if (mCanMoveHorizontally)
	{
		mCurrentPosition.x += mCurrentDirection.x * mBaseSpeed * milliseconds / 10;
	}

	if (mCanMoveVertically)
	{
		mCurrentPosition.y += mCurrentDirection.y * mBaseSpeed * milliseconds / 10;
	}
}

// todo in death add delay to animation timer, and set dying to true
// urgent when it comes to collision, think about box collision, making a manager that notifies the colliders