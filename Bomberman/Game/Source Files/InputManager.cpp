#include <windows.h>											// Header File For Windows
#include "baseTypes.h"
#include <gl/gl.h>														// Header File For The OpenGL32 Library
#include <gl/glu.h>														// Header File For The GLu32 Library
#include "glut.h"
#include "openglframework.h"											// Header File For The NeHeGL Basecode
#include "InputManager.h"

InputManagerC* InputManagerC::sInstance = NULL;

InputManagerC *InputManagerC::CreateInstance()
{
	if (sInstance == NULL)
	{
		sInstance = new InputManagerC();
	}

	return sInstance;
}

void InputManagerC::init()
{
}

void InputManagerC::update()
{
	keyProcess();
}

Coord2D InputManagerC::getPlayerDirection()
{
	Coord2D coor = {0, 0};

	// make sure they are pressed together
	if (!(g_keys->keyDown[VK_LEFT] && g_keys->keyDown[VK_RIGHT]))
	{
		if (g_keys->keyDown[VK_LEFT])
		{
			coor.x = -1;
		}

		if (g_keys->keyDown[VK_RIGHT])
		{
			coor.x = 1;
		}
	}

	// make sure they are pressed together
	if (!(g_keys->keyDown[VK_UP] && g_keys->keyDown[VK_DOWN]))
	{
		if (g_keys->keyDown[VK_DOWN])
		{
			coor.y = -1;
		}

		if (g_keys->keyDown[VK_UP])
		{
			coor.y = 1;
		}
	}

	return coor;

}

void InputManagerC::keyProcess()
{
	//handles keyboard processing
	if (g_keys->keyDown[VK_ESCAPE])								// Is ESC Being Pressed?
	{
		TerminateApplication(g_window);						// Terminate The Program
	}

	if (g_keys->keyDown[VK_F1])									// Is F1 Being Pressed?
	{
		ToggleFullscreen(g_window);							// Toggle Fullscreen Mode
	}
}
