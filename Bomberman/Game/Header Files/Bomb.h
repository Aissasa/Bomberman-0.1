#pragma once

#include "baseTypes.h"
#include "RenderingDataStructures.h"
#include <vector>

#define BOMB_EXPLOSION_TIME 3000
#define BOMB_ANIMATION_TIME 300

class BombC
{
public:

	BombC(Coord2D position ,uint16_t range, bool remoteControlled);
	~BombC() {};

	void update(DWORD milliseconds);
	void render();
	void explode();

	uint16_t getId() { return mId; };
	Coord2D getPosition() { return mPosition; };

private:
	static uint16_t sCurrentId;

	void updateAnimation(DWORD milliseconds);

	Coord2D mPosition;
	Animation_t* mAnimation;

	int32_t mAnimationTimer;
	int32_t mExplosionTimer;

	uint16_t mId;

	uint16_t mRange;
	bool mRemoteControlled;

};

typedef std::vector<BombC> Bombs_Vect_t;

