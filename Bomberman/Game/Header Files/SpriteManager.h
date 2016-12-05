#pragma once

#include "SpriteSheetParser.h"
#include "PropsSpriteSheetParser.h"
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

#define X_MAP_OFFSET 20
#define Y_MAP_OFFSET 15


struct VertexFormatPos3Tex2
{   // Custom vertex format for position+normal
	float tu, tv;             ///< texture coordinates	>
	float px, py, pz;        ///< untransformed (world-space) position>
};

struct TileCoor
{
	uint16_t x;
	uint16_t y;
};

enum MapLayer
{
	Bg,
	Items,
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
	void renderCharacter(const Coord2D& position);
	void shutdown();

private:
	static SpriteManagerC *sInstance;
	SpriteManagerC() {};

	//GLuint mCurrentSpriteTextureMap;

	GLuint mBaromSpriteTextureMap;
	GLuint mBombSpriteTextureMap;
	GLuint mBombAESpriteTextureMap;
	GLuint mMCSpriteTextureMap;
	GLuint mPropsSpriteTextureMap;

	SpriteSheetParserC* mBaromSpriteSheetParser;
	SpriteSheetParserC* mBombSpriteSheetParser;
	SpriteSheetParserC* mBombAESpriteSheetParser;
	SpriteSheetParserC* mMCSpriteSheetParser;
	SpriteSheetParserC* mPropsSpriteSheetParser;

	Map_t mCurrentMap;

	DWORD mLastUpdateTime;
	DWORD mCurrentTime;

	//VertexFormatPos3Tex2 *mVertexArray;

	void renderMap();
	void renderTile(const TileCoor& tile, MapLayer mapLayer);
	void renderSingleSprite(const VertexFormatPos3Tex2& texTopLeftQuadBottomRight, const VertexFormatPos3Tex2& texBottomLeftQuadBottom,
							const VertexFormatPos3Tex2& texBottomRightQuadTopLeft, const VertexFormatPos3Tex2& texTopRightQuadTopRight);

};
