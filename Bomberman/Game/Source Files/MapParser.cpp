#include "MapParser.h"
#include <rapidjson/istreamwrapper.h>
#include <fstream>

using namespace std;
using namespace rapidjson;

MapParserC* MapParserC::sInstance = nullptr;

//---------------------------------------------------------------------------------------------------------------------
MapParserC * MapParserC::CreateInstance()
{
	if (sInstance == nullptr)
	{
		sInstance = new MapParserC();
	}

	return sInstance;
}

//---------------------------------------------------------------------------------------------------------------------
Map_t MapParserC::parseMapJson(char8_t * filePath)
{
	// put the file in a stream
	FILE* jsonFile = fopen(filePath, "rb");

	assert(jsonFile);

	ifstream ifs(filePath);
	IStreamWrapper isw(ifs);

	// parse the json
	Document jsonDoc;
	jsonDoc.ParseStream(isw);

	// make sure it is a valid json, and it has frames array in it
	assert(jsonDoc.IsObject());

	Map_t basicMap;
	basicMap.width = jsonDoc["width"].GetInt();
	basicMap.height = jsonDoc["height"].GetInt();
	basicMap.tileWidth = jsonDoc["tilewidth"].GetInt();
	basicMap.tileHeight = jsonDoc["tileheight"].GetInt();

	const Value& layers = jsonDoc["layers"];
	assert(layers.IsArray());

	buildAndPopulateLayersInBasicMap(basicMap, layers);

	assert(jsonDoc.HasMember("restrictedTiles"));

	const Value& restrictedTiles = jsonDoc["restrictedTiles"];
	assert(restrictedTiles.IsArray());

	buildAndPopulateRestrictedTiles(basicMap, restrictedTiles);
	basicMap.playerSpawnTile = basicMap.restictedTiles[0];

	return basicMap;
}

//---------------------------------------------------------------------------------------------------------------------
void MapParserC::buildAndPopulateLayersInBasicMap(Map_t& map, const Value& layers)
{
	// build layers
	uint32_t** bgLayer = (uint32_t**)malloc(map.width * sizeof(uint32_t*));
	uint32_t** blocksLayer = (uint32_t**)malloc(map.width * sizeof(uint32_t*));

	for (uint16_t x = 0; x < map.width; x++)
	{
		bgLayer[x] = (uint32_t*)malloc(map.height * sizeof(uint32_t));
		blocksLayer[x] = (uint32_t*)malloc(map.height * sizeof(uint32_t));
	}

	// populate bg layer and blocks layer
	for (uint16_t y = 0; y < map.height; y++)
	{
		for (uint16_t x = 0; x < map.width; x++)
		{
			uint16_t newY = map.height - y - 1;
			bgLayer[x][y] = layers[0]["data"][newY * map.width + x].GetInt();
			blocksLayer[x][y] = layers[1]["data"][newY * map.width + x].GetInt();
		}
	}

	// assign layers
	map.bgLayer = bgLayer;
	map.blocksLayer = blocksLayer;
}

//---------------------------------------------------------------------------------------------------------------------
void MapParserC::buildAndPopulateRestrictedTiles(Map_t& map, const rapidjson::Value & restrictedTiles)
{
	uint16_t restrictedTilesCount = restrictedTiles.Size();

	map.numRestrictedTiles = restrictedTilesCount;

	// build restricted tiles
	TileCoor_t* temp = (TileCoor_t*)malloc(restrictedTilesCount * sizeof(TileCoor_t));

	for (uint16_t i = 0; i < restrictedTilesCount; i++)
	{
		TileCoor_t tileCoor;
		tileCoor.x = restrictedTiles[i]["x"].GetInt();
		tileCoor.y = restrictedTiles[i]["y"].GetInt();
		temp[i] = tileCoor;
	}

	map.restictedTiles = temp;
}
