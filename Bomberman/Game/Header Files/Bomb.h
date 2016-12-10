#pragma once

#include "baseTypes.h"
#include "RenderingDataStructures.h"
#include "BombAE.h"
#include <vector>

#define BOMB_EXPLOSION_TIME 3000
#define BOMB_ANIMATION_TIME 300

//---------------------------------------------------------------------------------------------------------------------
typedef struct AfterEffectsAnimations
{
	Animation_t* bottomAE;
	Animation_t* centerAE;
	Animation_t* horizontalAE;
	Animation_t* leftAE;
	Animation_t* rightAE;
	Animation_t* topAE;
	Animation_t* verticalAE;

}AfterEffectsAnimations_t;

//---------------------------------------------------------------------------------------------------------------------
class BombC
{
public:

	BombC() {};
	~BombC() {};

	void init(Coord2D position, uint16_t range, bool remoteControlled);
	void update(DWORD milliseconds);
	void render();
	void explode();

	uint16_t getId() { return mId; };
	uint16_t getRange() { return mRange; };
	Coord2D getPosition() { return mPosition; };

	Animation getAfterEffectAnimation(BombAEType_t aeType);

private:
	static uint16_t sCurrentId;

	void setAfterEffectsAnimations();
	void updateAnimation(DWORD milliseconds);

	AfterEffectsAnimations_t mAfterEffectAnimations;
	Coord2D mPosition;
	Animation_t* mAnimation;

	int32_t mAnimationTimer;
	int32_t mExplosionTimer;

	uint16_t mId;

	uint16_t mRange;
	bool mRemoteControlled;

};

//---------------------------------------------------------------------------------------------------------------------
typedef std::vector<BombC> Bombs_Vect_t;
typedef std::vector<BombC*> Bombs_Ptr_Vect_t;

