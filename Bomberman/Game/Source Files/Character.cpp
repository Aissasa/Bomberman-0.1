#include <windows.h>
#include "Character.h"
#include "SpriteManager.h"
#include "string.h"


//---------------------------------------------------------------------------------------------------------------------
void CharacterC::updateAnimation(DWORD milliseconds)
{
	if (mIsDying)
	{
		if (mCurrentAnimation != mCharAnimations->death)
		{
			mCurrentAnimation = mCharAnimations->death;
		}
		animate(milliseconds, mCurrentAnimation);
	}
	else
	{
		bool animated = false;

		if (mCharacterMovement.goingLeft)
		{
			animated = true;
			animate(milliseconds, mCharAnimations->walkLeft);
		}

		if (mCharacterMovement.goingRight)
		{
			animated = true;
			animate(milliseconds, mCharAnimations->walkRight);
		}

		if (mCharacterMovement.goingUp)
		{
			animated = true;
			animate(milliseconds, mCharAnimations->walkUp);
		}

		if (mCharacterMovement.goingDown)
		{
			animated = true;
			animate(milliseconds, mCharAnimations->walkDown);
		}

		if (!animated)
		{
			if (mCurrentAnimation == mCharAnimations->idleLeft || mCurrentAnimation == mCharAnimations->walkLeft)
			{
				mCurrentAnimation = mCharAnimations->idleLeft;
			}
			else
			{
				if (mCurrentAnimation == mCharAnimations->idleRight || mCurrentAnimation == mCharAnimations->walkRight)
				{
					mCurrentAnimation = mCharAnimations->idleRight;
				}
				else
				{
					if (mCurrentAnimation == mCharAnimations->idleUp || mCurrentAnimation == mCharAnimations->walkUp)
					{
						mCurrentAnimation = mCharAnimations->idleUp;
					}
					else
					{
						if (mCurrentAnimation == mCharAnimations->idleDown || mCurrentAnimation == mCharAnimations->walkDown)
						{
							mCurrentAnimation = mCharAnimations->idleDown;
						}
					}
				}
			}
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------
void CharacterC::animate(DWORD milliseconds, Animation_t* potentialAnimation)
{
	if (mCurrentAnimation == potentialAnimation)
	{
		mAnimationTimer -= milliseconds;
		if (mAnimationTimer <= 0)
		{
			mAnimationTimer = mCurrentAnimation->timeFromSpriteToSprite;
			mCurrentAnimation->currentSpriteIndex = (mCurrentAnimation->currentSpriteIndex + 1) % mCurrentAnimation->numSprites;
		}
	}
	else
	{
		mCurrentAnimation->currentSpriteIndex = 0;
		mCurrentAnimation = potentialAnimation;
		mAnimationTimer = mCurrentAnimation->timeFromSpriteToSprite;
	}
}

//---------------------------------------------------------------------------------------------------------------------
CharacterAnimations_t * CharacterC::createCharAnimations()
{
	CharacterAnimations_t* cAnim = (CharacterAnimations_t*)malloc(sizeof(CharacterAnimations_t));

	cAnim->death = (Animation_t*)malloc(sizeof(Animation_t));
	cAnim->idleDown = (Animation_t*)malloc(sizeof(Animation_t));
	cAnim->idleLeft = (Animation_t*)malloc(sizeof(Animation_t));
	cAnim->idleRight = (Animation_t*)malloc(sizeof(Animation_t));
	cAnim->idleUp = (Animation_t*)malloc(sizeof(Animation_t));
	cAnim->walkDown = (Animation_t*)malloc(sizeof(Animation_t));
	cAnim->walkLeft = (Animation_t*)malloc(sizeof(Animation_t));
	cAnim->walkRight = (Animation_t*)malloc(sizeof(Animation_t));
	cAnim->walkUp = (Animation_t*)malloc(sizeof(Animation_t));

	return cAnim;
}

//---------------------------------------------------------------------------------------------------------------------
void CharacterC::populateAnimation(Animation_t * anim, const Animation_t& sourceAnim)
{
	anim->currentSpriteIndex = sourceAnim.currentSpriteIndex;
	//strncpy(anim->name, sourceAnim.name, strlen(sourceAnim.name) + 1);
	anim->numSprites = sourceAnim.numSprites;
	anim->sprites = sourceAnim.sprites;
	anim->timeFromSpriteToSprite = sourceAnim.timeFromSpriteToSprite;
}

//---------------------------------------------------------------------------------------------------------------------
void CharacterC::render()
{
	//SpriteManagerC::GetInstance()->renderCharacter(&(mCurrentAnimation->sprites[mCurrentAnimation->currentSpriteIndex]), mCurrentPosition);
	SpriteManagerC::GetInstance()->setPlayerRendParameters(&(mCurrentAnimation->sprites[mCurrentAnimation->currentSpriteIndex]), mCurrentPosition);
}

