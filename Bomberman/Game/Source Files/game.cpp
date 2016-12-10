#define GAME_CPP
#include <windows.h>											// Header File For Windows
#include <stdio.h>												// Header File For Standard Input / Output
#include <stdarg.h>												// Header File For Variable Argument Routines
#include <math.h>												// Header File For Math Operations
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include "glut.h"
#include "baseTypes.h"
#include "openglframework.h"	
#include "random.h"
#include "openGLStuff.h"
#include "game.h"
#include "InputManager.h"
#include "SpriteManager.h"
#include "LevelManager.h"

// Declarations
const char8_t CGame::mGameTitle[]="Bomberman 0.1";
CGame* CGame::sInstance=NULL;

//---------------------------------------------------------------------------------------------------------------------
BOOL Initialize (GL_Window* window, Keys* keys)					// Any OpenGL Initialization Goes Here
{
	initOpenGLDrawing(window,keys,0.0f, 0.0f, 0.0f);
	CGame::CreateInstance();
	CGame::GetInstance()->init();
	return TRUE;						
}

//---------------------------------------------------------------------------------------------------------------------
CGame *CGame::CreateInstance()
{
	if (sInstance == NULL)
	{
		sInstance = new CGame();
	}

	return sInstance;
}

//---------------------------------------------------------------------------------------------------------------------
void CGame::init()
{
	InputManagerC::CreateInstance();
	SpriteManagerC::CreateInstance();
	LevelManagerC::CreateInstance();

	SpriteManagerC::GetInstance()->init();
	LevelManagerC::GetInstance()->init(SpriteManagerC::GetInstance()->getMap());

}

//---------------------------------------------------------------------------------------------------------------------
void CGame::UpdateFrame(DWORD milliseconds)			
{
	InputManagerC::GetInstance()->update();
	LevelManagerC::GetInstance()->update(milliseconds);
}

//---------------------------------------------------------------------------------------------------------------------
void CGame::DrawScene(void)											
{
	startOpenGLDrawing();
	SpriteManagerC::GetInstance()->renderSprites();
}

//---------------------------------------------------------------------------------------------------------------------
void CGame::shutdown()
{
}

//---------------------------------------------------------------------------------------------------------------------
void CGame::DestroyGame(void)
{
}