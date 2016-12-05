#pragma once

#include "baseTypes.h"
#include "SpriteSheetParser.h"

class PropsSpriteSheetParserC : public SpriteSheetParserC
{
public:
	PropsSpriteSheetParserC() :SpriteSheetParserC::SpriteSheetParserC() {};

	//accessors
	static uint32_t getGreenBgIndex() { return sGreenBgIndex; }
	static uint32_t getGreyBgIndex() { return sGreyBgIndex; }
	static uint32_t getHardBlockIndex() { return sHardBlockIndex; }
	static uint32_t getSoftBlockIndex() { return sSoftBlockIndex; }

private:

	static const uint32_t sGreenBgIndex;
	static const uint32_t sGreyBgIndex;
	static const uint32_t sHardBlockIndex;
	static const uint32_t sSoftBlockIndex;

	// note add the perks indices too

};
