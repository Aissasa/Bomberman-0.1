#pragma once

#include "RenderingDataStructures.h"
#include "SpriteSheetParser.h"
#include "MapParser.h"

#define BAROM_SPRITE_SHEET_PATH "Game/Resources/SpriteSheets/BaromSpriteSheet.png"
#define BOMB_SPRITE_SHEET_PATH "Game/Resources/SpriteSheets/BombSpriteSheet.png"
#define BOMB_AE_SPRITE_SHEET_PATH "Game/Resources/SpriteSheets/BombAESpriteSheet.png"
#define MC_SPRITE_SHEET_PATH "Game/Resources/SpriteSheets/MCSpriteSheet.png"
#define PROPS_SPRITE_SHEET_PATH "Game/Resources/SpriteSheets/PropsSpriteSheet.png"

#define BAROM_JSON_PATH "Game/Resources/Jsons/Barom.json"
#define BOMB_JSON_PATH "Game/Resources/Jsons/Bomb.json"
#define BOMB_AE_JSON_PATH "Game/Resources/Jsons/BombAE.json"
#define MC_JSON_PATH "Game/Resources/Jsons/MC.json"
#define PROPS_JSON_PATH "Game/Resources/Jsons/Props.json"

#define BASIC_MAP_JSON_PATH "Game/Resources/Jsons/BasicMap.json"


struct VertexFormatPos3Tex2
{   // Custom vertex format for position+normal
	float tu, tv;             ///< texture coordinates	>
	float px, py, pz;        ///< untransformed (world-space) position>
};

enum class BasicMapLayer
{
	Bg,
	Blocks
};

class SpriteManagerC
{
public:
	static SpriteManagerC *CreateInstance();
	static SpriteManagerC *GetInstance() { return sInstance; };
	~SpriteManagerC() {};

	void init();
	void updateSprites(DWORD milliseconds);
	void renderSprites();
	void shutdown();

	Map_t* getMap();
	Animation_t* getAnimations(SpriteSheetType_t spshType);
	void addPlayerToRender(const Sprite_t* sprite, Coord2D pos);
	void addBombToRender(const Sprite_t* sprite, Coord2D pos);
	void addBombAEToRender(const Sprite_t* sprite, Coord2D pos);

private:
	static SpriteManagerC *sInstance;
	SpriteManagerC() {};

	GLuint mBaromSpriteTextureMap;
	GLuint mBombSpriteTextureMap;
	GLuint mBombAESpriteTextureMap;
	GLuint mMCSpriteTextureMap;
	GLuint mPropsSpriteTextureMap;

	SpriteSheet_t mCurrentSpriteSheet;

	SpriteSheet_t mBaromSpriteSheet;
	SpriteSheet_t mBombSpriteSheet;
	SpriteSheet_t mBombAESpriteSheet;
	SpriteSheet_t mMCSpriteSheet;
	SpriteSheet_t mPropsSpriteSheet;

	Map_t mCurrentMap;

	Renderable_Sprite_Vect_t mPlayerRenderableSprite;
	Renderable_Sprite_Vect_t mBombsRenderableSpritesVect;
	Renderable_Sprite_Vect_t mBombsAERenderableSpritesVect;

	DWORD mLastUpdateTime;
	DWORD mCurrentTime;

	void renderBasicMap();
	void renderItems();
	void renderPlayer(); // note replace render enemies and player with render char where u pass an enum
	// void render enemies
	void renderBombs();
	void renderBombsAE();
	void renderBasicMapTile(const TileCoor_t& tile, BasicMapLayer mapLayer);
	void renderCharacter(const Sprite_t* spriteToRender, const Coord2D& position);
	void renderSingleSprite(const Sprite_t& sprite, Coord2D pos);
	void renderTexture(const VertexFormatPos3Tex2& texTopLeftQuadBottomRight, const VertexFormatPos3Tex2& texBottomLeftQuadBottom,
							const VertexFormatPos3Tex2& texBottomRightQuadTopLeft, const VertexFormatPos3Tex2& texTopRightQuadTopRight);

};
