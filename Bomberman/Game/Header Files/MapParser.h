#pragma once

#include "GameDataStructures.h"
#include "rapidjson/document.h"

class MapParserC
{
public:

	static MapParserC* createInstance();
	static MapParserC* getInstance() { return sInstance; }
	~MapParserC() {};

	Map_t parseMap(char8_t* filePath);

private:
	static MapParserC* sInstance;
	MapParserC() {};
	void buildAndPopulateLayersInBasicMap(Map_t& map, const rapidjson::Value & layers);
};
