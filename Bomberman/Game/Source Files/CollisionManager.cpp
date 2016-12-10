#include "CollisionManager.h"

CollisionManagerC* CollisionManagerC::sInstance = nullptr;
const uint32_t CollisionManagerC::sMargin = 3; // collision margin with the blocks

//---------------------------------------------------------------------------------------------------------------------
CollisionManagerC * CollisionManagerC::CreateInstance()
{
	if (sInstance == nullptr)
	{
		sInstance = new CollisionManagerC();
	}

	return sInstance;
}

//---------------------------------------------------------------------------------------------------------------------
bool CollisionManagerC::checkCharacterCollisionWithBlocks(const Coord2D & position, const Coord2D& direction)
{
	if (direction.x == 0 && direction.y == 0)
	{
		return false;
	}

	bool colliding = false;

	Coord2D center = { position.x + mCurrentMap->tileWidth / 2, position.y + mCurrentMap->tileHeight / 2 };
	TileCoor_t charTile = getTileCoorFromPosition(center);

	// diagonal
	if (direction.x != 0 && direction.y != 0)
	{
		// note add cutting corners logic here

		TileCoor_t targetTile = charTile;
		targetTile.x += (int16_t)direction.x;
		targetTile.y += (int16_t)direction.y;

		uint32_t spriteIndex = mCurrentMap->blocksLayer[targetTile.x][targetTile.y];
		if (spriteIndex == (uint32_t)SpriteIndicesInMap_t::solidBlock || spriteIndex == (uint32_t)SpriteIndicesInMap_t::softBlock)
		{
			colliding = boxCollision(position, getPositionFromTileCoor(targetTile));
		}
	}
	else
	{
		TileCoor_Vect_t tilesVect;

		// going horiz
		if (direction.x != 0)
		{
			for (int16_t y = -1; y <= 1; y++)
			{
				TileCoor_t newTile = charTile;
				newTile.x += (int16_t)direction.x;
				newTile.y += y;

				tilesVect.push_back(newTile);
			}
		}
		else
		{
			// going vertical
			if (direction.y != 0)
			{
				for (int16_t x = -1; x <= 1; x++)
				{
					TileCoor_t newTile = charTile;
					newTile.x += x;
					newTile.y += (int16_t)direction.y;
					tilesVect.push_back(newTile);
				}
			}
		}

		// check collision
		for (uint16_t i = 0; i < tilesVect.size(); i++)
		{
			TileCoor_t targetTile = tilesVect[i];

			uint32_t spriteIndex = mCurrentMap->blocksLayer[targetTile.x][targetTile.y];
			if (spriteIndex == (uint32_t)SpriteIndicesInMap_t::solidBlock || spriteIndex == (uint32_t)SpriteIndicesInMap_t::softBlock)
			{
				colliding = boxCollision(position, getPositionFromTileCoor(targetTile));
				if (colliding)
				{
					break;
				}
			}
		}
	}

	return colliding;

}

//---------------------------------------------------------------------------------------------------------------------
bool CollisionManagerC::checkCharacterCollisionWithBombs(const Coord2D & position, const Coord2D & direction, Bombs_Vect_t & bombsVect)
{
	if (direction.x == 0 && direction.y == 0)
	{
		return false;
	}

	bool colliding = false;

	Coord2D center = { position.x + mCurrentMap->tileWidth / 2, position.y + mCurrentMap->tileHeight / 2 };
	TileCoor_t charTile = getTileCoorFromPosition(center);

	// note add cutting corners logic here

	TileCoor_t targetTile = charTile;
	targetTile.x += (int16_t)direction.x;
	targetTile.y += (int16_t)direction.y;

	uint16_t vectLength = (uint16_t)bombsVect.size();
	for (uint16_t i = 0; i < vectLength; i++)
	{
		TileCoor_t bombTile = getTileCoorFromPosition(bombsVect[i].getPosition());
		if (bombTile.x == targetTile.x && bombTile.y == targetTile.y)
		{
			colliding = boxCollision(position, getPositionFromTileCoor(targetTile));
			break;
		}
	}
	return colliding;
}

//---------------------------------------------------------------------------------------------------------------------
bool CollisionManagerC::checkCharacterCollisionWithBombsAE(const Coord2D & position, const Bombs_AE_Ptr_Vect_t bombsAEVect)
{
	bool colliding = false;

	uint16_t vectLength = (uint16_t)bombsAEVect.size();
	for (uint16_t i = 0; i < vectLength; i++)
	{
		BombAEC bombAE = *bombsAEVect[i];
		colliding = boxCollision(position, bombAE.getPosition());

		if (colliding)
		{
			break;
		}
	}

	return colliding;
}

//---------------------------------------------------------------------------------------------------------------------
bool CollisionManagerC::boxCollision(const Coord2D & source, const Coord2D & target)
{
	return source.x + sMargin < target.x + mCurrentMap->tileWidth &&
		source.x - sMargin + mCurrentMap->tileWidth > target.x &&
		source.y - sMargin > target.y - mCurrentMap->tileHeight &&
		source.y + sMargin - mCurrentMap->tileHeight < target.y;
}

//---------------------------------------------------------------------------------------------------------------------
Coord2D CollisionManagerC::getPositionFromTileCoor(const TileCoor_t & tile)
{
	Coord2D pos;
	pos.x = (float_t)tile.x * mCurrentMap->tileWidth;
	pos.y = (float_t)tile.y * mCurrentMap->tileHeight;

	return pos;

}

//---------------------------------------------------------------------------------------------------------------------
TileCoor_t CollisionManagerC::getTileCoorFromPosition(const Coord2D & coor)
{
	TileCoor_t tile;
	tile.x = (uint32_t)coor.x / mCurrentMap->tileWidth;
	tile.y = (uint32_t)coor.y / mCurrentMap->tileHeight;

	return tile;
}


