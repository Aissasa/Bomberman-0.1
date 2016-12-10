#pragma once

#include "baseTypes.h"
#include "RenderingDataStructures.h"
#include <vector>

#define FADING_BLOCK_ANIMATION_TIME 50

class FadingSoftBlockC
{
public:
	FadingSoftBlockC(Coord2D position);
	~FadingSoftBlockC() {};

	void update(DWORD milliseconds);
	void render();

	Coord2D getPosition() { return mPosition; };

private:

	void updateAnimation(DWORD milliseconds);

	Coord2D mPosition;
	Animation_t* mAnimation;

	int32_t mAnimationTimer;

};

typedef std::vector<FadingSoftBlockC> Fading_SoftBlock_Vect_t;
typedef std::vector<FadingSoftBlockC*> Fading_SoftBlock_Ptr_Vect_t;
