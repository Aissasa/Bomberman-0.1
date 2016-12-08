#include <windows.h>
#include "Player.h"
#include "InputManager.h"
#include "SpriteManager.h"

//---------------------------------------------------------------------------------------------------------------------
void PlayerC::init(Coord2D initialCoor)
{
	mCurrentPosition = initialCoor;
	setAnimations();

	mCharacterMovement = { false, false, false, false };
	mIsDying = false;
	mCurrentMaxBombNumber = 1;
	mBaseSpeed =(float_t) BASE_SPEED;
}

//---------------------------------------------------------------------------------------------------------------------
void PlayerC::update(DWORD milliseconds)
{
	// todo check col before update pos
	if (!mIsDying)
	{
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
void PlayerC::updatePosition(DWORD milliseconds)
{
	Coord2D direction = InputManagerC::GetInstance()->getPlayerDirection();

	mCharacterMovement.goingLeft = direction.x < 0;
	mCharacterMovement.goingRight = direction.x > 0;
	mCharacterMovement.goingUp = direction.y > 0;
	mCharacterMovement.goingDown = direction.y < 0;

	mCurrentPosition.x += direction.x * mBaseSpeed * milliseconds/10;
	mCurrentPosition.y += direction.y * mBaseSpeed * milliseconds/10;

}

// todo in death add delay to animation timer, and set dying to true
// urgent when it comes to collision, think about box collision, making a manager that notifies the colliders