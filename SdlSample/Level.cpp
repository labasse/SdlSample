#include <algorithm>
#include <cassert>
#include "Level.h"
#include "Settings.h"

#define CHAR_START 'x'
#define CHAR_SOLID '*'
#define CHAR_EMPTY ' '

#define TILE_SOLID_COL 0
#define TILE_SOLID_ROW 4

const char* Level::Test[] = {
	"*                     *",
	"*                     *",
	"*                     *",
	"***                   *",
	"*     *****           *",
	"*            **********",
	"*   x                 *",
	"*************         *",
	"         *            *",
	"         **************",
	nullptr
};

Level::Level(SDL_Texture* tilesheet, const char* tiles[]) : 
	tilesheet(tilesheet), 
	types({ UNKNOWN }), 
	tiles(tiles), 
	width(SIZE_MAX),
	height(0),
	startCol(0),
	startRow(0)
{
	size_t startLine = 0;

	types[CHAR_EMPTY] = EMPTY;
	types[CHAR_START] = EMPTY;
	types[CHAR_SOLID] = SOLID;
	for (auto row = tiles; *row != nullptr; ++row, ++height) {
		auto start = strchr(*row, CHAR_START);

		if(start != nullptr) {
			startCol = (size_t)(start - *row);
			startLine = height;
		}
		width = std::min(width, strlen(*row));
#ifdef _DEBUG
		for (const char *p = *row; *p; ++p) {
			assert(types[*p] != UNKNOWN);
		}
#endif
	}
	startRow = height - startLine - 1;
}

void Level::Render(const Renderer& renderer)
{
	static const SDL_FRect rcsSolid = { 
		(float)(TILE_SOLID_COL*LEVEL_TILE_SIZE), 
		(float)(TILE_SOLID_ROW*LEVEL_TILE_SIZE),
		(float)LEVEL_TILE_SIZE, 
		(float)LEVEL_TILE_SIZE 
	};
	for (int row = 0; row < (int)height; ++row) {
		for(int col = 0; col < (int)width; ++col) {
			if (GetTile(col, row) == SOLID)
			{
				renderer.Render(tilesheet, rcsSolid, (float)col, (float)row);
			}
		}
	}
}
