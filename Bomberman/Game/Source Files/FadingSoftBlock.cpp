#include "FadingSoftBlock.h"
#include "SpriteManager.h"
#include "LevelManager.h"

//---------------------------------------------------------------------------------------------------------------------
FadingSoftBlockC::FadingSoftBlockC(Coord2D position)
{
	mPosition = position;

	mAnimation = (Animation_t*)malloc(sizeof(Animation_t));
	*mAnimation = SpriteManagerC::GetInstance()->getAnimations(SpriteSheetType_t::Props)[1];;
	mAnimation->timeFromSpriteToSprite = FADING_BLOCK_ANIMATION_TIME;
	mAnimationTimer = mAnimation->timeFromSpriteToSprite;
}

//---------------------------------------------------------------------------------------------------------------------
void FadingSoftBlockC::update(DWORD milliseconds)
{
	updateAnimation(milliseconds);
	render();
}

//---------------------------------------------------------------------------------------------------------------------
void FadingSoftBlockC::render()
{
	SpriteManagerC::GetInstance()->addToRender(RenderableSpriteType_t::FadingBlock ,&mAnimation->sprites[mAnimation->currentSpriteIndex], mPosition);
}

//---------------------------------------------------------------------------------------------------------------------
void FadingSoftBlockC::updateAnimation(DWORD milliseconds)
{
	mAnimationTimer -= milliseconds;
	if (mAnimationTimer <= 0)
	{
		if (mAnimation->currentSpriteIndex == mAnimation->numSprites - 1)
		{
			LevelManagerC::GetInstance()->addToDestroyedBlocks(this);
			return;
		}
		mAnimationTimer = mAnimation->timeFromSpriteToSprite;
		mAnimation->currentSpriteIndex = (mAnimation->currentSpriteIndex + 1);
	}
}
