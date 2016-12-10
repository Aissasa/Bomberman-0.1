#include <windows.h>
#include "Bomb.h"
#include "SpriteManager.h"
#include "LevelManager.h"

uint16_t BombC::sCurrentId = 0;

//---------------------------------------------------------------------------------------------------------------------
void BombC::init(Coord2D position, uint16_t range, bool remoteControlled)
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

	setAfterEffectsAnimations();
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
	LevelManagerC::GetInstance()->addToExplodingBombs(*this);
}

//---------------------------------------------------------------------------------------------------------------------
Animation BombC::getAfterEffectAnimation(BombAEType_t aeType)
{
	switch (aeType)
	{
		case BombAEType_t::Bottom:
			return *mAfterEffectAnimations.bottomAE;
		
		case BombAEType_t::Center:
			return *mAfterEffectAnimations.centerAE;

		case BombAEType_t::Horizontal:
			return *mAfterEffectAnimations.horizontalAE;

		case BombAEType_t::Left:
			return *mAfterEffectAnimations.leftAE;

		case BombAEType_t::Right:
			return *mAfterEffectAnimations.rightAE;

		case BombAEType_t::Top:
			return *mAfterEffectAnimations.topAE;

		case BombAEType_t::Vertical:
			return *mAfterEffectAnimations.verticalAE;

		default:
			return *mAfterEffectAnimations.centerAE;
	}
}

//---------------------------------------------------------------------------------------------------------------------
void BombC::setAfterEffectsAnimations()
{
	Animation_t* aeAnimations = SpriteManagerC::GetInstance()->getAnimations(SpriteSheetType_t::BombAE);

	mAfterEffectAnimations.bottomAE = &aeAnimations[0];
	mAfterEffectAnimations.centerAE = &aeAnimations[1];
	mAfterEffectAnimations.horizontalAE = &aeAnimations[2];
	mAfterEffectAnimations.leftAE = &aeAnimations[3];
	mAfterEffectAnimations.rightAE = &aeAnimations[4];
	mAfterEffectAnimations.topAE = &aeAnimations[5];
	mAfterEffectAnimations.verticalAE = &aeAnimations[6];
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
