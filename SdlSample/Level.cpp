#include <algorithm>
#include <cassert>
#include "Level.h"
#include "Settings.h"

#define CHAR_START		 'x'
#define CHAR_FINISH		 '!'
#define CHAR_BGSTONE	 'O'
#define CHAR_CRATE		 '#'
#define CHAR_LADDER		 'H'
#define CHAR_PALE		 '^'
#define CHAR_WOODENFLOOR '-'
#define CHAR_STONEFLOOR  '='
#define CHAR_STONEWALL   '|'
#define CHAR_WINDOW0	 '('
#define CHAR_WINDOW1	 ')'
#define CHAR_DOOR0		 '/'
#define CHAR_DOOR1		 '\\'
#define CHAR_DOOR2		 '['
#define CHAR_DOOR3		 ']'
#define CHAR_EMPTY		 ' '

#define FREQ_BRACKET 5
#define FREQ_WEB     7

#define TILE_SOLID_COL 0
#define TILE_SOLID_ROW 4

const char* Level::Test[] = {
	"#                                                                                             ",
	"#                                                                                             ",
	"#                 ########                                                                    ",
	"#                                                                                             ",
	"###     ######               ##                                                               ",
	"|                           #####                                             !               ",
	"|                  ------------------------H----|===|                         H              |",
	"|   x              OOOOOO|OOOOOOOOOOOOOOOOOHOOOO|OOO|                         H              |",
	"|========|        |===|OOOOO---OOOOOO---OOOHOOO|=OOO====|                     H              |",
	"|OOOOOOOO|        |OOO|OOOOOOOO=====OOOO()OHOOO|=OOOOOOO|                     H              |",
	"|OOO|||||||||||||||OOO|OOOOOOOOOOOOOOOOOOOOHOO|OOOOOOOOO|------------------------------------|",
	"|OOO|             |OOOO------------------------OO/\\OOOOOOOOOOOOO()OOOOOOOOOOOOOOOOOOOOOOOOOOO|",
	"|OOO|             |OOOOOOOOOOOOOOOOOOOOOOOOOOOOOO[]O||||||||||||||||||||||||||||||||||||||||||",
	"|OOO|             |OOOOOOOOO()OOOOOO^^^^OOOOOO|=====|                                         ",
	"|OOO|             |OOOOOOOOO===================OOOOO|                                         ",
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
	
	width = strlen(tiles[0]);
	for (auto row = tiles; *row != nullptr; ++row, ++height) {
		auto& line = tileSprites.emplace_back();

		line.reserve(width);
		ASSERT(width == strlen(*row));
		for (const char *p = *row; *p; ++p) {
			switch (*p) {
			case CHAR_START:
				startCol = (size_t)(p - *row);
				startLine = height;
				[[fallthrough]];
			case CHAR_FINISH : 
			case CHAR_EMPTY  : line.push_back(-1); break;
			case CHAR_BGSTONE: line.push_back( 6); break;
			case CHAR_WOODENFLOOR: line.push_back(rand() % FREQ_BRACKET ?  1 :  2); break;
			case CHAR_STONEFLOOR : line.push_back(rand() % FREQ_WEB     ? 11 : 23); break;
			case CHAR_DOOR0    : line.push_back(3); break;
			case CHAR_DOOR1    : line.push_back(4); break;
			case CHAR_DOOR2    : line.push_back(8); break;
			case CHAR_DOOR3    : line.push_back(9); break;
			case CHAR_STONEWALL: line.push_back(10); break;
			case CHAR_WINDOW0  : line.push_back(13); break;
			case CHAR_WINDOW1  : line.push_back(14); break;
			case CHAR_CRATE	   : line.push_back(20); break;
			case CHAR_LADDER   : line.push_back(21); break;
			case CHAR_PALE     : line.push_back(24); break;
			default:
				ASSERT(false);
			}
		}
	}
	startRow = height - startLine - 1;
}

void Level::Render(const Renderer& renderer)
{
	SDL_FRect rcs = { 0.f, 0.f, (float)LEVEL_TILE_SIZE, (float)LEVEL_TILE_SIZE };

	for (int row = 0; row < (int)height; ++row) {
		for(int col = 0; col < (int)width; ++col) {
			auto tileIndex = tileSprites[height - row - 1][col];
			if (tileIndex >= 0)
			{
				rcs.x = (float)(tileIndex % 5) * rcs.w;
				rcs.y = (float)(tileIndex / 5) * rcs.h;
				renderer.Render(tilesheet, rcs, (float)col, (float)row);
			}
		}
	}
}
