#include <algorithm>
#include "Settings.h"

#include "PlanarLevel.h"

#define FREQ_BRACKET 5
#define FREQ_WEB     7

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
	"|OOO|             |OOOO------------------------O/~\\OOOOOOOOOOOOO()OOOOOOOOOOOOOOOOOOOOOOOOOOO|",
	"|OOO|             |OOOOOOOOOOOOOOOOOOOOOOOOOOOOO[O]O||||||||||||||||||||||||||||||||||||||||||",
	"|OOO|             |OOOOOOOOO()OOOOOO^^^^OOOOOO|=====|                                         ",
	"|OOO|             |OOOOOOOOO===================OOOOO|                                         ",
	nullptr
};

PlanarLevel::TileGen PlanarLevel::Predefined[] = {
	{ CHAR_LADDER	  , TILEINDEX_LADDER     , TILEFLAG_CLIMBABLE },
	{ CHAR_WOODENFLOOR, TILEINDEX_WOODENFLOOR, TILEFLAG_PLATFORM  , FREQ_BRACKET },
	{ CHAR_STONEFLOOR , TILEINDEX_STONEFLOOR , TILEFLAG_PLATFORM  , FREQ_WEB     },
	{ CHAR_CRATE	  , TILEINDEX_CRATE      , TILEFLAG_SOLID     },
	{ CHAR_STONEWALL  , TILEINDEX_STONEWALL  , TILEFLAG_SOLID     },
	{ CHAR_BGSTONE    , TILEINDEX_BGSTONE	},
	{ CHAR_DOOR0	  , TILEINDEX_DOOR0		},
	{ CHAR_DOOR1	  , TILEINDEX_DOOR1		},
	{ CHAR_DOOR2	  , TILEINDEX_DOOR2		},
	{ CHAR_DOOR3	  , TILEINDEX_DOOR3 	},
	{ CHAR_DOOR4	  , TILEINDEX_DOOR4		},
	{ CHAR_WINDOW0	  , TILEINDEX_WND0		},
	{ CHAR_WINDOW1	  , TILEINDEX_WND1		},
	{ CHAR_PALE		  , TILEINDEX_PALE		}
};

PlanarLevel::TileGen::TileGen(char symbol, int tileIndex, int flags, int freqAlt) :
	symbol(symbol),
	tileInstance(tileIndex, flags),
	tileInstanceAlt(tileIndex + 1, flags),
	freqAlt(freqAlt)
{
}

Tile* PlanarLevel::TileGen::NewTile(size_t, size_t, LoadContext&)
{
	return (freqAlt == 0 || (rand() % freqAlt)) ? &tileInstance : &tileInstanceAlt;
}

PlanarLevel::PlanarLevel(SDL_Texture* tilesheet, const Parallax& parallax) :
	Level(tilesheet, parallax)
{ 
	RegisterEmptyTile(CHAR_EMPTY);
	RegisterEmptyTile(CHAR_START);
	RegisterEmptyTile(CHAR_FINISH);
	for (auto& gen : Predefined)
	{
		RegisterTileType(gen.symbol, &gen);
	}
}

void PlanarLevel::Render(const Renderer& renderer) const
{
	SDL_Rect rcArea;

	renderer.GetVisibleArea(rcArea);
	GetParallax().Render(renderer, renderer.GetLookAtWorld().x, .0f);

	size_t lastcol = std::min(GetWidth (), (size_t)(rcArea.x + rcArea.w));
	size_t lastrow = std::min(GetHeight(), (size_t)(rcArea.y + rcArea.h));

	for (int row = std::max(0, rcArea.y); row < lastrow; ++row) {
		for (int col = std::max(0, rcArea.x); col < lastcol; ++col) {
			auto tile = GetTile(col, row);

			if (tile->Is(TILEFLAG_PLANAR)){
				auto sheet = GetTileSheet();
				auto index = static_cast<const PlanarTile*>(tile)->GetTileIndex();

				renderer.RenderTile(
					sheet.GetTexture(),	
					sheet.FromTileIndex(index), 
					(float)col, (float)row
				);
			}
		}
	}
}

void PlanarLevel::Normalize(float& x, float& y) const
{
	// No normalization needed for planar levels
}
