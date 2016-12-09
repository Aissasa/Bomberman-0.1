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
	SpriteSheetParserC::CreateInstance();
	MapParserC::CreateInstance();

	// note adapt to the screen size when drawing sprites
	/* load an image file directly as a new OpenGL texture */
	mBaromSpriteTextureMap = SOIL_load_OGL_texture(BAROM_SPRITE_SHEET_PATH, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, NULL);
	mBombSpriteTextureMap = SOIL_load_OGL_texture(BOMB_SPRITE_SHEET_PATH, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, NULL);
	mBombAESpriteTextureMap = SOIL_load_OGL_texture(BOMB_AE_SPRITE_SHEET_PATH, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, NULL);
	mMCSpriteTextureMap = SOIL_load_OGL_texture(MC_SPRITE_SHEET_PATH, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, NULL);
	mPropsSpriteTextureMap = SOIL_load_OGL_texture(PROPS_SPRITE_SHEET_PATH, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, NULL);

	mBaromSpriteSheet = SpriteSheetParserC::GetInsatnce()->parseSpriteSheetJson(BAROM_JSON_PATH, mBaromSpriteTextureMap);
	mBombSpriteSheet = SpriteSheetParserC::GetInsatnce()->parseSpriteSheetJson(BOMB_JSON_PATH, mBombSpriteTextureMap);
	mBombAESpriteSheet = SpriteSheetParserC::GetInsatnce()->parseSpriteSheetJson(BOMB_AE_JSON_PATH, mBombAESpriteTextureMap);
	mMCSpriteSheet = SpriteSheetParserC::GetInsatnce()->parseSpriteSheetJson(MC_JSON_PATH, mMCSpriteTextureMap);
	mPropsSpriteSheet = SpriteSheetParserC::GetInsatnce()->parseSpriteSheetJson(PROPS_JSON_PATH, mPropsSpriteTextureMap);

	mCurrentMap = MapParserC::GetInstance()->parseMapJson(BASIC_MAP_JSON_PATH);
}

//---------------------------------------------------------------------------------------------------------------------
void SpriteManagerC::updateSprites(DWORD milliseconds)
{
}

//---------------------------------------------------------------------------------------------------------------------
void SpriteManagerC::renderSprites()
{
	glClear(GL_DEPTH_BUFFER_BIT);

	renderBasicMap();
	renderItems();
	renderPlayer();
	renderBombs();
}

//---------------------------------------------------------------------------------------------------------------------
void SpriteManagerC::renderBasicMap()
{
	TileCoor_t currentTile;
	BasicMapLayer currentMapLayer = BasicMapLayer::Bg;

	for (uint16_t y = 0; y < mCurrentMap.height; y++)
	{
		for (uint16_t x = 0; x < mCurrentMap.width; x++)
		{
			currentTile = { x,y };

			// render bg tile
			currentMapLayer = BasicMapLayer::Bg;
			renderBasicMapTile(currentTile, currentMapLayer);

			// render blocks tile
			currentMapLayer = BasicMapLayer::Blocks;
			renderBasicMapTile(currentTile, currentMapLayer);

		}
	}
}

//---------------------------------------------------------------------------------------------------------------------
void SpriteManagerC::renderItems()
{
	// render perk
	Sprite_t currentSprite = mPropsSpriteSheet.sprites[mCurrentMap.perkTile.spriteIndex];

	Coord2D tilePos;
	tilePos.x = mCurrentMap.tileWidth * mCurrentMap.perkTile.tileCoor.x;
	tilePos.y = mCurrentMap.tileHeight * mCurrentMap.perkTile.tileCoor.y ;

	renderSingleSprite(currentSprite, tilePos);

	// render door
	currentSprite = mPropsSpriteSheet.sprites[mCurrentMap.doorTile.spriteIndex];

	tilePos;
	tilePos.x = mCurrentMap.tileWidth * mCurrentMap.doorTile.tileCoor.x;
	tilePos.y = mCurrentMap.tileHeight * mCurrentMap.doorTile.tileCoor.y;

	mCurrentSpriteSheet = mPropsSpriteSheet;
	renderSingleSprite(currentSprite, tilePos);
}

//---------------------------------------------------------------------------------------------------------------------
void SpriteManagerC::renderPlayer()
{
	mCurrentSpriteSheet = mMCSpriteSheet;
	renderCharacter(&mPlayerRenderableSprite.sprite, mPlayerRenderableSprite.position);
}

//---------------------------------------------------------------------------------------------------------------------
void SpriteManagerC::renderBombs()
{
	mCurrentSpriteSheet = mBombSpriteSheet;
	uint16_t vectLength = (uint16_t)mBombsRenderableSpritesVect.size();
	for (uint16_t i = 0; i < vectLength; i++)
	{
		RenderableSprite_t renderableSprite = mBombsRenderableSpritesVect.back();
		renderSingleSprite(renderableSprite.sprite, renderableSprite.position);
		mBombsRenderableSpritesVect.pop_back();
	}
}

//---------------------------------------------------------------------------------------------------------------------
void SpriteManagerC::renderBasicMapTile(const TileCoor_t & tile, BasicMapLayer mapLayer)
{
	uint32_t currentSpriteIndex;

	switch (mapLayer)
	{
		case BasicMapLayer::Bg:
			currentSpriteIndex = mCurrentMap.bgLayer[tile.x][tile.y];
			break;

		case BasicMapLayer::Blocks:
			currentSpriteIndex = mCurrentMap.blocksLayer[tile.x][tile.y];
			break;

		default:
			currentSpriteIndex = 0;
			break;
	}

	if (currentSpriteIndex > 0)
	{
		Sprite_t currentSprite = mPropsSpriteSheet.sprites[--currentSpriteIndex];

		Coord2D tilePos;
		tilePos.x = mCurrentMap.tileWidth * tile.x ;
		tilePos.y = mCurrentMap.tileHeight * tile.y ;

		mCurrentSpriteSheet = mPropsSpriteSheet;
		renderSingleSprite(currentSprite, tilePos);
	}

}

//---------------------------------------------------------------------------------------------------------------------
void SpriteManagerC::renderCharacter(const Sprite_t* spriteToRender, const Coord2D & position)
{
	Sprite_t sprite = *spriteToRender;
	renderSingleSprite(sprite, position);
}

//---------------------------------------------------------------------------------------------------------------------
void SpriteManagerC::renderSingleSprite(const Sprite_t& sprite, Coord2D pos)
{
	glBindTexture(GL_TEXTURE_2D, sprite.textureMap);

	glBegin(GL_QUADS);
	

	GLfloat texXUnit = mCurrentSpriteSheet.texXUnit;
	GLfloat texYUnit = mCurrentSpriteSheet.texYUnit;

	VertexFormatPos3Tex2 TLBR = { sprite.xId*texXUnit, sprite.yId*texYUnit,pos.x, pos.y + mCurrentMap.tileHeight, sprite.sortingLayer };

	VertexFormatPos3Tex2 BLBL = { sprite.xId*texXUnit, (sprite.yId + 1)*texYUnit, pos.x, pos.y, sprite.sortingLayer };

	VertexFormatPos3Tex2 BRTL = { (sprite.xId + 1)*texXUnit, (sprite.yId + 1)*texYUnit, pos.x + mCurrentMap.tileWidth, pos.y + 0, sprite.sortingLayer };

	VertexFormatPos3Tex2 TRTR = { (sprite.xId + 1)*texXUnit, sprite.yId*texYUnit, pos.x + mCurrentMap.tileWidth, pos.y + mCurrentMap.tileHeight, sprite.sortingLayer };

	renderTexture(TLBR, BLBL, BRTL, TRTR);

	glEnd();
}


//---------------------------------------------------------------------------------------------------------------------
void SpriteManagerC::renderTexture(const VertexFormatPos3Tex2& texTopLeftQuadBottomRight, const VertexFormatPos3Tex2& texBottomLeftQuadBottom,
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

//---------------------------------------------------------------------------------------------------------------------
void SpriteManagerC::shutdown()
{
	// todo free and delete stuff
}

//---------------------------------------------------------------------------------------------------------------------
Map_t * SpriteManagerC::getMap()
{
	return &mCurrentMap;
}

//---------------------------------------------------------------------------------------------------------------------
Animation_t * SpriteManagerC::getAnimations(SpriteSheetType_t spshType)
{
	switch (spshType)
	{
		case SpriteSheetType_t::Barom:
			return mBaromSpriteSheet.animations;

		case SpriteSheetType_t::Bomb:
			return mBombSpriteSheet.animations;

		case SpriteSheetType_t::BombAE:
			return mBombAESpriteSheet.animations;

		case SpriteSheetType_t::MC:
			return mMCSpriteSheet.animations;

		case SpriteSheetType_t::Props:
			return mPropsSpriteSheet.animations;

		default:
			return nullptr;
	}
}

//---------------------------------------------------------------------------------------------------------------------
void SpriteManagerC::setPlayerRendParameters(const Sprite_t * sprite, Coord2D pos)
{
	mPlayerRenderableSprite.position = pos;
	mPlayerRenderableSprite.sprite = *sprite;
}

//---------------------------------------------------------------------------------------------------------------------
void SpriteManagerC::addBombToRender(const Sprite_t * sprite, Coord2D pos)
{
	RenderableSprite_t renderableSprite;
	renderableSprite.sprite = *sprite;
	renderableSprite.position = pos;

	mBombsRenderableSpritesVect.push_back(renderableSprite);
}
