#pragma once

#include "GameDataStructures.h"
#include "rapidjson/document.h"

class MapParserC
{
public:

	static MapParserC* CreateInstance();
	static MapParserC* GetInstance() { return sInstance; }
	~MapParserC() {};

	Map_t parseMapJson(char8_t* filePath);

private:
	static MapParserC* sInstance;
	MapParserC() {};
	void buildAndPopulateLayersInBasicMap(Map_t& map, const rapidjson::Value & layers);
	void buildAndPopulateRestrictedTiles(Map_t& map, const rapidjson::Value & restrictedTiles);

};
