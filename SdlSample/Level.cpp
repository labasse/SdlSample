#include <algorithm>
#include <cassert>
#include "Level.h"
#include "Settings.h"

#define TILE_SOLID_COL 0
#define TILE_SOLID_ROW 4

Level::Level(SDL_Texture* tilesheet) : 
	tilesheet(tilesheet), 
	types({ UNKNOWN }), 
	tiles(nullptr), 
	width(SIZE_MAX),
	height(0),
	startCol(0),
	startRow(0)
{
	types[CHAR_EMPTY  ] = 
	types[CHAR_START  ] =
	types[CHAR_FINISH ] =
	types[CHAR_DOOR0  ] =
	types[CHAR_DOOR1  ] =
	types[CHAR_DOOR2  ] =
	types[CHAR_DOOR3  ] =
	types[CHAR_PALE   ] =
	types[CHAR_WINDOW0] =
	types[CHAR_WINDOW1] =
	types[CHAR_BGSTONE] = EMPTY;

	types[CHAR_LADDER] = LADDER;

	types[CHAR_WOODENFLOOR] =
	types[CHAR_STONEFLOOR ] = PLATFORM;

	types[CHAR_CRATE    ] =
	types[CHAR_STONEWALL] = SOLID;
}

void Level::Load(const char* tiles[])
{
	this->tiles = tiles;
	width = strlen(tiles[0]);
	for (auto row = tiles; *row != nullptr; ++row, ++height) {
		auto& line = tileSprites.emplace_back();

		line.reserve(width);
		ASSERT(width == strlen(*row));
		for (const char* p = *row; *p; ++p) {
			if (*p == CHAR_START)
			{
				startCol = (size_t)(p - *row);
				startRow = height;
			}
			line.push_back(GetSpriteIndexFromChar(*p));
		}
	}
}
