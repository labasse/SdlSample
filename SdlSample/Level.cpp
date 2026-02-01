#include <algorithm>
#include <cassert>
#include "Level.h"
#include "Settings.h"

const SDL_Point Level::HERE		 = {  0,  0 };
const SDL_Point Level::NORTH	 = {  0, -1 };
const SDL_Point Level::WEST		 = { -1,  0 };
const SDL_Point Level::SOUTH	 = {  0,  1 };
const SDL_Point Level::EAST		 = {  1,  0 };
const SDL_Point Level::NORTHWEST = { -1, -1 };
const SDL_Point Level::NORTHEAST = {  1, -1 };
const SDL_Point Level::SOUTHWEST = { -1,  1 };
const SDL_Point Level::SOUTHEAST = {  1,  1 };

Level::Level(SDL_Texture* tilesheet, const Parallax& parallax) : 
	tilesheet(tilesheet, LEVEL_TILE_SIZE, LEVEL_TILE_COLUMNS), 
	parallax(parallax),
	genByChar(0x7f, nullptr),
	width(SIZE_MAX),
	height(0),
	startCol(0),
	startRow(0)
{ }

Level::~Level()
{
	for (auto& line : tiles) {
		for (auto tile : line) {
			if (tile->Is(TILEFLAG_ALLOCATED)) {
				delete tile;
			}
		}
	}
}

void Level::Load(const char* map[])
{
	width = strlen(map[0]);
	for (auto row = map; *row != nullptr; ++row, ++height) {
		auto& line = tiles.emplace_back();

		line.reserve(width);
		ASSERT(width == strlen(*row));
		for (const char* p = *row; *p; ++p) {
			if (*p == CHAR_START)
			{
				startCol = (size_t)(p - *row);
				startRow = height;
			}
			ASSERT(genByChar[*p]);
			line.push_back(genByChar[*p]->NewTile());
		}
	}
}

void Level::RegisterTileType(const char symbol, TileGenerator* gen)
{
	ASSERT(symbol >= 0 && symbol < 0x7f);
	ASSERT(gen && genByChar[symbol] == nullptr);
	genByChar[(size_t)symbol] = gen;
}
