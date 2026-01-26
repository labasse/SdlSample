#include <algorithm>
#include "Settings.h"

#include "PlanarLevel.h"

const char *PlanarLevel::Level0[] = {
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

PlanarLevel::PlanarLevel(SDL_Texture* tilesheet) :
	Level(tilesheet)
{ }

int PlanarLevel::GetSpriteIndexFromChar(char c) const
{
	switch (c)
	{
	case CHAR_START      :
	case CHAR_FINISH     :
	case CHAR_EMPTY      : return -1;
	case CHAR_BGSTONE    : return  6;
	case CHAR_WOODENFLOOR: return rand() % FREQ_BRACKET ? 1 : 2;
	case CHAR_STONEFLOOR : return rand() % FREQ_WEB ? 11 : 23;
	case CHAR_DOOR0      : return  3;
	case CHAR_DOOR1      : return  4;
	case CHAR_DOOR2      : return  8;
	case CHAR_DOOR3      : return  9;
	case CHAR_STONEWALL  : return 10;
	case CHAR_WINDOW0    : return 13;
	case CHAR_WINDOW1    : return 14;
	case CHAR_CRATE      : return 20;
	case CHAR_LADDER     : return 21;
	case CHAR_PALE       : return 24;
	default:
		ASSERT(false);
		return -1;
	}
}

void PlanarLevel::Render(const Renderer& renderer)
{
	SDL_FRect rcs = { 0.f, 0.f, (float)LEVEL_TILE_SIZE, (float)LEVEL_TILE_SIZE };
	SDL_Rect rcArea;

	renderer.GetVisibleArea(rcArea);
	size_t lastcol = std::min(GetWidth (), (size_t)(rcArea.x + rcArea.w));
	size_t lastrow = std::min(GetHeight(), (size_t)(rcArea.y + rcArea.h));
	for (int row = std::max(0, rcArea.y); row < lastrow; ++row) {
		for (int col = std::max(0, rcArea.x); col < lastcol; ++col) {
			auto tileIndex = GetTileSpriteIndex(col, row);

			if (tileIndex >= 0)
			{
				rcs.x = (float)(tileIndex % 5) * rcs.w;
				rcs.y = (float)(tileIndex / 5) * rcs.h;
				renderer.RenderTile(GetTileSheet(), rcs, (float)col, (float)row);
			}
		}
	}
}
