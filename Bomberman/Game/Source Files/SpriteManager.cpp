#include <windows.h>											// Header File For Windows
#include <stdio.h>												// Header File For Standard Input / Output
#include <stdarg.h>												// Header File For Variable Argument Routines
#include <math.h>												// Header File For Math Operations
#include <gl\gl.h>											// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include "glut.h"
#include <assert.h>
#include "baseTypes.h"
#include "openglframework.h"	
#include "SpriteManager.h"
#include "random.h"

#include "soil.h"

SpriteManagerC* SpriteManagerC::sInstance = nullptr;

//---------------------------------------------------------------------------------------------------------------------
SpriteManagerC* SpriteManagerC::CreateInstance()
{
	if (sInstance == nullptr)
	{
		sInstance = new SpriteManagerC();
	}

	return sInstance;
}

//---------------------------------------------------------------------------------------------------------------------
void SpriteManagerC::init()
{
	// note adapt to the screen size when drawing sprites
	/* load an image file directly as a new OpenGL texture */
	mBaromSpriteTextureMap = SOIL_load_OGL_texture(BAROM_SPRITE_SHEET_PATH, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, NULL);
	mBombSpriteTextureMap = SOIL_load_OGL_texture(BOMB_SPRITE_SHEET_PATH, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, NULL);
	mBombAESpriteTextureMap = SOIL_load_OGL_texture(BOMB_AE_SPRITE_SHEET_PATH, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, NULL);
	mMCSpriteTextureMap = SOIL_load_OGL_texture(MC_SPRITE_SHEET_PATH, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, NULL);
	mPropsSpriteTextureMap = SOIL_load_OGL_texture(PROPS_SPRITE_SHEET_PATH, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, NULL);

	mBaromSpriteSheetParser = new SpriteSheetParserC();
	mBombSpriteSheetParser = new SpriteSheetParserC();
	mBombAESpriteSheetParser = new SpriteSheetParserC();
	mMCSpriteSheetParser = new SpriteSheetParserC();
	mPropsSpriteSheetParser = new PropsSpriteSheetParserC();

	mBaromSpriteSheetParser->ParseJson(BAROM_JSON_PATH);
	mBombSpriteSheetParser->ParseJson(BOMB_JSON_PATH);
	mBombAESpriteSheetParser->ParseJson(BOMB_AE_JSON_PATH);
	mMCSpriteSheetParser->ParseJson(MC_JSON_PATH);
	mPropsSpriteSheetParser->ParseJson(PROPS_JSON_PATH);

	MapParserC::createInstance();
	mCurrentMap = MapParserC::getInstance()->parseMap(BASIC_MAP_JSON_PATH);

	// todo add soft blocks, one item and a gate randomly in the map
	// todo init sorting layers (init method after parse)
}

//---------------------------------------------------------------------------------------------------------------------
void SpriteManagerC::updateSprites(DWORD milliseconds)
{
}

//---------------------------------------------------------------------------------------------------------------------
void SpriteManagerC::renderSprites()
{
	renderMap();
}

//---------------------------------------------------------------------------------------------------------------------
void SpriteManagerC::renderMap()
{
	TileCoor currentTile;
	MapLayer currentMapLayer = Bg;

	for (uint16_t y = 0; y < mCurrentMap.height; y++)
	{
		for (uint16_t x = 0; x < mCurrentMap.width; x++)
		{
			currentTile = { x,y };

			// render bg tile
			currentMapLayer = Bg;
			renderTile(currentTile, currentMapLayer);

			// render items tile
			currentMapLayer = Items;
			renderTile(currentTile, currentMapLayer);

			// render blocks tile
			currentMapLayer = Blocks;
			renderTile(currentTile, currentMapLayer);

		}
	}
}

//---------------------------------------------------------------------------------------------------------------------
void SpriteManagerC::renderTile(const TileCoor & tile, MapLayer mapLayer)
{
	Sprite_t* allSprites = mPropsSpriteSheetParser->getSprites();
	float_t sortingLayer;
	uint32_t currentSpriteIndex;

	switch (mapLayer)
	{
		case Bg:
			sortingLayer = -5;
			currentSpriteIndex = mCurrentMap.bgLayer[tile.x][tile.y];
			break;

		case Items:
			sortingLayer = 0;
			currentSpriteIndex = mCurrentMap.itemsLayer[tile.x][tile.y];
			break;

		case Blocks:
			sortingLayer = 5;
			currentSpriteIndex = mCurrentMap.blocksLayer[tile.x][tile.y];
			break;

		default:
			sortingLayer = 0;
			currentSpriteIndex = 0;
			break;
	}

	if (currentSpriteIndex > 0)
	{
		Sprite_t currentSprite = allSprites[--currentSpriteIndex];

		glClear(GL_DEPTH_BUFFER_BIT);
		glBindTexture(GL_TEXTURE_2D, mPropsSpriteTextureMap);

		glBegin(GL_QUADS);

		//glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);

		GLfloat xStartPos = mCurrentMap.tileWidth * tile.x + X_MAP_OFFSET;
		GLfloat yStartPos = mCurrentMap.tileHeight * tile.y + Y_MAP_OFFSET;

		GLfloat texXUnit = mPropsSpriteSheetParser->getTexXUnit();
		GLfloat texYUnit = mPropsSpriteSheetParser->getTexYUnit();

		VertexFormatPos3Tex2 TLBR = { currentSprite.xId*texXUnit, currentSprite.yId*texYUnit,xStartPos, yStartPos + mCurrentMap.tileHeight, sortingLayer };

		VertexFormatPos3Tex2 BLBL = { currentSprite.xId*texXUnit, (currentSprite.yId + 1)*texYUnit, xStartPos, yStartPos, sortingLayer };

		VertexFormatPos3Tex2 BRTL = { (currentSprite.xId + 1)*texXUnit, (currentSprite.yId + 1)*texYUnit, xStartPos + mCurrentMap.tileWidth, yStartPos + 0, sortingLayer };

		VertexFormatPos3Tex2 TRTR = { (currentSprite.xId + 1)*texXUnit, currentSprite.yId*texYUnit, xStartPos + mCurrentMap.tileWidth, yStartPos + mCurrentMap.tileHeight, sortingLayer };

		renderSingleSprite(TLBR, BLBL, BRTL, TRTR);

		glEnd();
	}

	// glDisable(GL_TEXTURE_2D);
	//        
	//glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
	//glColor3f(1.0f, 0.0f, 0.0f); // Red

	//glVertex2f(0.0f, 0.0f);
	//glVertex2f(100, 0.0f);
	//glVertex2f(100, 100);     //  so that the normal (front-face) is facing you    
	//glVertex2f(0.0f, 100);     // Define vertices in counter-clockwise (CCW) order
	//glEnd();
	//glFlush();

}

//---------------------------------------------------------------------------------------------------------------------
void SpriteManagerC::renderCharacter(const Coord2D & position)
{
	// todo render char at the pos

}

//---------------------------------------------------------------------------------------------------------------------
void SpriteManagerC::renderSingleSprite(const VertexFormatPos3Tex2& texTopLeftQuadBottomRight, const VertexFormatPos3Tex2& texBottomLeftQuadBottom,
										const VertexFormatPos3Tex2& texBottomRightQuadTopLeft, const VertexFormatPos3Tex2& texTopRightQuadTopRight)
{
	glTexCoord2f(texTopLeftQuadBottomRight.tu, texTopLeftQuadBottomRight.tv);                              // top left of the texture
	glVertex3f(texTopLeftQuadBottomRight.px, texTopLeftQuadBottomRight.py, texTopLeftQuadBottomRight.pz);  // Bottom Right Of The Quad

	glTexCoord2f(texBottomLeftQuadBottom.tu, texBottomLeftQuadBottom.tv);                            // Bottom left of the texture
	glVertex3f(texBottomLeftQuadBottom.px, texBottomLeftQuadBottom.py, texBottomLeftQuadBottom.pz);  // Bottom Left Of The Quad

	glTexCoord2f(texBottomRightQuadTopLeft.tu, texBottomRightQuadTopLeft.tv);                              // Bottom right of the texture
	glVertex3f(texBottomRightQuadTopLeft.px, texBottomRightQuadTopLeft.py, texBottomRightQuadTopLeft.pz);  // Top Left Of The Quad	

	glTexCoord2f(texTopRightQuadTopRight.tu, texTopRightQuadTopRight.tv);                            // Top right of the texture
	glVertex3f(texTopRightQuadTopRight.px, texTopRightQuadTopRight.py, texTopRightQuadTopRight.pz);  // Top Right Of The Quad
}

void SpriteManagerC::shutdown()
{
	// todo free and delete stuff
}
