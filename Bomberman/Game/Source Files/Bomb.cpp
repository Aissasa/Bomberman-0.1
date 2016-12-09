#include <windows.h>
#include "Bomb.h"
#include "SpriteManager.h"
#include "LevelManager.h"

uint16_t BombC::sCurrentId = 0;
//---------------------------------------------------------------------------------------------------------------------
BombC::BombC(Coord2D position, uint16_t range, bool remoteControlled)
{
	mPosition = position;
	mRange = range;
	mRemoteControlled = remoteControlled;

	mExplosionTimer = BOMB_EXPLOSION_TIME;

	mAnimation = (Animation_t*)malloc(sizeof(Animation_t));
	*mAnimation = SpriteManagerC::GetInstance()->getAnimations(SpriteSheetType_t::Bomb)[0];
	mAnimation->timeFromSpriteToSprite = BOMB_ANIMATION_TIME;
	mAnimationTimer = mAnimation->timeFromSpriteToSprite;

	mId = sCurrentId++;
}

//---------------------------------------------------------------------------------------------------------------------
void BombC::update(DWORD milliseconds)
{
	updateAnimation(milliseconds);
	render();
	if (!mRemoteControlled)
	{
		mExplosionTimer -= milliseconds;
		if (mExplosionTimer < 0)
		{
			explode();
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------
void BombC::render()
{
	SpriteManagerC::GetInstance()->addBombToRender(&mAnimation->sprites[mAnimation->currentSpriteIndex], mPosition);
}

//---------------------------------------------------------------------------------------------------------------------
void BombC::explode()
{
	// createae, and send them to lvl manager
	LevelManagerC::GetInstance()->addToTobeRemovedArray(*this);

}

//---------------------------------------------------------------------------------------------------------------------
void BombC::updateAnimation(DWORD milliseconds)
{
	// note make animation as a class instead of a struct, and add these redundant methods to it
	mAnimationTimer -= milliseconds;
	if (mAnimationTimer <= 0)
	{
		mAnimationTimer = mAnimation->timeFromSpriteToSprite;
		mAnimation->currentSpriteIndex = (mAnimation->currentSpriteIndex + 1) % mAnimation->numSprites;
	}
}
