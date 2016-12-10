#pragma once

#include "baseTypes.h"
#include "RenderingDataStructures.h"
#include <vector>

#define BOMB_AE_ANIMATION_TIME 50

typedef enum class BombAEType
{
	Bottom,
	Center,
	Horizontal,
	Left,
	Right,
	Top,
	Vertical

}BombAEType_t;

class BombAEC
{
public:
	BombAEC(Coord2D position, Animation_t& anim );
	~BombAEC() {};

	void update(DWORD milliseconds);
	void render();

	Coord2D getPosition() { return mPosition; };

private:

	void updateAnimation(DWORD milliseconds);

	Coord2D mPosition;
	Animation_t* mAnimation;

	int32_t mAnimationTimer;

};

typedef std::vector<BombAEC> Bombs_AE_Vect_t;
typedef std::vector<BombAEC*> Bombs_AE_Ptr_Vect_t;
