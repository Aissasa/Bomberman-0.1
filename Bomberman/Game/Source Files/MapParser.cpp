#include "MapParser.h"
#include <rapidjson/istreamwrapper.h>
#include <fstream>

using namespace std;
using namespace rapidjson;

MapParserC* MapParserC::sInstance = nullptr;

//---------------------------------------------------------------------------------------------------------------------
MapParserC * MapParserC::createInstance()
{
	if (sInstance == nullptr)
	{
		sInstance = new MapParserC();
	}

	return sInstance;
}

//---------------------------------------------------------------------------------------------------------------------
Map_t MapParserC::parseMap(char8_t * filePath)
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

	return basicMap;
}

//---------------------------------------------------------------------------------------------------------------------
void MapParserC::buildAndPopulateLayersInBasicMap(Map_t& map, const Value& layers)
{
	// build layers
	uint32_t** bgLayer = (uint32_t**)malloc(map.width * sizeof(uint32_t*));
	uint32_t** itemsLayer = (uint32_t**)malloc(map.width * sizeof(uint32_t*));
	uint32_t** blocksLayer = (uint32_t**)malloc(map.width * sizeof(uint32_t*));

	for (uint16_t x = 0; x < map.width; x++)
	{
		bgLayer[x] = (uint32_t*)malloc(map.height * sizeof(uint32_t));
		itemsLayer[x] = (uint32_t*)malloc(map.height * sizeof(uint32_t));
		blocksLayer[x] = (uint32_t*)malloc(map.height * sizeof(uint32_t));
	}

	// populate bg layer and blocks layer
	for (uint16_t y = 0; y < map.height; y++)
	{
		for (uint16_t x = 0; x < map.width; x++)
		{
			uint16_t newY = map.height - y - 1;
			uint32_t number = layers[0]["data"][newY * map.width + x].GetInt();
			bgLayer[x][y] = layers[0]["data"][newY * map.width + x].GetInt();
			number = layers[1]["data"][newY * map.width + x].GetInt();
			blocksLayer[x][y] = layers[1]["data"][newY * map.width + x].GetInt();
			itemsLayer[x][y] = 0;
		}
	}

	// assign layers
	map.bgLayer = bgLayer;
	map.itemsLayer = itemsLayer;
	map.blocksLayer = blocksLayer;
}
