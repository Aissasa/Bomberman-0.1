#include <windows.h>
#include "Player.h"
#include "InputManager.h"
#include "SpriteManager.h"
#include "CollisionManager.h"
#include "LevelManager.h"
#include "Bomb.h"
#include "LevelManager.h"

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

	updateActions(milliseconds);
	updateAnimation(milliseconds);
	render();
}

//---------------------------------------------------------------------------------------------------------------------
void PlayerC::placeBomb()
{
	Coord2D center = { mCurrentPosition.x + mCurrentAnimation->sprites[mCurrentAnimation->currentSpriteIndex].width / 2,
		mCurrentPosition.y + mCurrentAnimation->sprites[mCurrentAnimation->currentSpriteIndex].height / 2 };

	TileCoor_t bombTile = CollisionManagerC::GetInstance()->getTileCoorFromPosition(center);
	Coord2D tileCoor = CollisionManagerC::GetInstance()->getTilePosition(bombTile);
	bool tileOccupied = LevelManagerC::GetInstance()->tileOccupiedByBomb(tileCoor);
	if (LevelManagerC::GetInstance()->getCurrentNumberBombs() < mBaseMaxBombNumber + mPerks->bombUp && !tileOccupied)
	{
		BombC* newBomb = new BombC(tileCoor, mBaseBombRange + mPerks->fire, mPerks->remote);

		LevelManagerC::GetInstance()->addBomb(*newBomb);
	}
}

//---------------------------------------------------------------------------------------------------------------------
void PlayerC::triggerBombs()
{
	if (mPerks->remote)
	{
		// call lvl manager to explode all of the current bombs
	}
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
	mCanMoveHorizontally = !CollisionManagerC::GetInstance()->checkCharacterCollisionWithBlocks(potentialPos, { mCurrentDirection.x, 0 });
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

//---------------------------------------------------------------------------------------------------------------------
void PlayerC::updateActions(DWORD milliseconds)
{
	static int16_t placeBombCoolDownTimer = BTN_COOLDOWN_TIMER;
	static int16_t triggerBombCoolDownTimer = BTN_COOLDOWN_TIMER;

	placeBombCoolDownTimer = placeBombCoolDownTimer - (uint32_t)milliseconds;
	triggerBombCoolDownTimer = triggerBombCoolDownTimer - (uint32_t)milliseconds;

	if (InputManagerC::GetInstance()->placeBombKeyPressed() && placeBombCoolDownTimer < 0)
	{
		placeBombCoolDownTimer = BTN_COOLDOWN_TIMER;
		placeBomb();
		return;
	}
	if (InputManagerC::GetInstance()->explodeBombsKeyPressed() && triggerBombCoolDownTimer < 0)
	{
		triggerBombCoolDownTimer = BTN_COOLDOWN_TIMER;
		triggerBombs();
	}
}

// todo in death add delay to animation timer, and set dying to true
