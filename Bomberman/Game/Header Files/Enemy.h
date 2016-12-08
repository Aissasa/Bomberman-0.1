#pragma once

#include "Character.h"

class EnemyC : public CharacterC
{
public:
	EnemyC() : CharacterC::CharacterC() {};
	~EnemyC() {};

	void init() {};
	void update(DWORD milliseconds) {};


private:
	
	void interpolate();
	// choose nexttile

};
