#include "BombAE.h"
#include "SpriteManager.h"
#include "LevelManager.h"

//---------------------------------------------------------------------------------------------------------------------
BombAEC::BombAEC(Coord2D position, Animation_t & anim)
{
	mPosition = position;

	mAnimation = (Animation_t*)malloc(sizeof(Animation_t));
	*mAnimation = anim;
	mAnimation->timeFromSpriteToSprite = BOMB_AE_ANIMATION_TIME;
	mAnimationTimer = mAnimation->timeFromSpriteToSprite;
}

//---------------------------------------------------------------------------------------------------------------------
void BombAEC::update(DWORD milliseconds)
{
	updateAnimation(milliseconds);
	render();
}

//---------------------------------------------------------------------------------------------------------------------
void BombAEC::render()
{
	SpriteManagerC::GetInstance()->addBombAEToRender(&mAnimation->sprites[mAnimation->currentSpriteIndex], mPosition);
}

//---------------------------------------------------------------------------------------------------------------------
void BombAEC::updateAnimation(DWORD milliseconds)
{
	mAnimationTimer -= milliseconds;
	if (mAnimationTimer <= 0)
	{
		if (mAnimation->currentSpriteIndex == mAnimation->numSprites - 1)
		{
			LevelManagerC::GetInstance()->addToVanishingBombsAE(this);
			return;
		}
		mAnimationTimer = mAnimation->timeFromSpriteToSprite;
		mAnimation->currentSpriteIndex = (mAnimation->currentSpriteIndex + 1);
	}
}
