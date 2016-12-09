#pragma once

#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H
class InputManagerC
{
public:
	static InputManagerC *CreateInstance();
	static InputManagerC *GetInstance() {return sInstance;};
	void update();

	Coord2D getPlayerDirection();
	bool placeBombKeyPressed();
	bool explodeBombsKeyPressed();

private:
	InputManagerC(){};
	void keyProcess();

	static InputManagerC *sInstance;
	
};
#endif