#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

#include "Settings.h"

#include "RotativeLevel.h"

#define CIRCUMFERENCE_TILECOUNT 36
#define RADIUS0	367.0f
#define RADIUS1 421.2f

const char *RotativeLevel::Level1[] = {
	"                                    ",
	"         |=====|                    ",
	"-----    |==/~\\===|             ----",
	"         |OO[ ]OOO======|  ---      ",
	"         |=====HOOOO()OO|     ||||||",
	"|        |OOOOOHOO========|   |OOOO=",
	"|||   |||=OO|||HOOOOOOOO|||   |||OOO",
	"=O=====O=====O===H=O=====O==H==O====",
	"OOOOOO()OOOOOOOOOHOOOOOOOOOOHOOOOOOO",
	"OOOOOOOOOOOOO/~\\OHOO====OOOOHOO/~\\OO",
	"xOOO|====|OOO[ ]OHOOOOOOOOO|==|[ ]OO",
	"=====OOOO===================OO======",
	"HOOOOOOOOOOOOOOO()OOOOOOOOOOOOOOOOOO",
	"HOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO",
	"HOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO",
	nullptr
};

RotativeLevel::TileGen RotativeLevel::Predefined[] = {
	{ CHAR_LADDER     , TILEINDEX_LADDER     , TILEFLAG_CLIMBABLE },
	{ CHAR_START	  , TILEINDEX_BGSTONE	 },
	{ CHAR_BGSTONE    , TILEINDEX_BGSTONE	 },
	{ CHAR_WINDOW0    , TILEINDEX_WND0		 , TILEFLAG_SPECIALBCK|TILEFLAG_ASYMMETRIC},
	{ CHAR_WINDOW1    , TILEINDEX_WND1		 , TILEFLAG_SPECIALBCK|TILEFLAG_ASYMMETRIC},
	{ CHAR_DOOR0      , TILEINDEX_DOOR0		 , TILEFLAG_ASYMMETRIC},
	{ CHAR_DOOR1      , TILEINDEX_DOOR1		 , TILEFLAG_SPECIALBCK},
	{ CHAR_DOOR2      , TILEINDEX_DOOR2		 , TILEFLAG_ASYMMETRIC},
	{ CHAR_DOOR3      , TILEINDEX_DOOR3		 , TILEFLAG_ASYMMETRIC},
	{ CHAR_DOOR4      , TILEINDEX_DOOR4		 , TILEFLAG_ASYMMETRIC},
	{ CHAR_WOODENFLOOR, TILEINDEX_RELIEF_STONEFLOOR , TILEFLAG_PLATFORM },
	{ CHAR_STONEFLOOR , TILEINDEX_RELIEF_STONEFLOOR , TILEFLAG_PLATFORM|TILEFLAG_DRAWBGTILE },
	{ CHAR_STONEWALL  , TILEINDEX_RELIEF_STONEWALL  , TILEFLAG_SOLID    }
};

RotativeLevel::RotativeLevel(SDL_Texture* tilesheet, const Parallax& parallax) : 
	Level(tilesheet, parallax)
{
	RegisterEmptyTile(CHAR_EMPTY);
	RegisterEmptyTile(CHAR_FINISH);
	for (auto& gen : Predefined)
	{
		RegisterTileType(gen.symbol, &gen);
	}
}

RotativeLevel::TileGen::TileGen(char symbol, int tileIndex, int flags) :
	symbol(symbol),
	tileInstance(tileIndex, flags),
	tileRelief	(tileIndex, flags),
	tileDarkBack(
		(flags&TILEFLAG_ROTATIVE_RELIEF) ? tileRelief : tileInstance, 
		LEVEL_TILE_COLUMNS, flags
	)
{ }

Level::LoadContext& RotativeLevel::GetLineLoadContext(size_t row, const char* line)
{
	loadContext.darkBack = strstr(line, "  ") == nullptr;
	return loadContext;
}

void RotativeLevel::TileGen::OnRegisteredBy(const Level& level)
{
	tileInstance	.PreCalcTileRects(level.GetTileSheet());
	tileRelief		.PreCalcTileRects(level.GetTileSheet());
	tileDarkBack	.PreCalcTileRects(level.GetTileSheet());
}

Tile* RotativeLevel::TileGen::NewTile(size_t col, size_t row, LoadContext& context)
{
	auto& rotContext = static_cast<RotativeLevel::RotativeLoadContext&>(context);
	
	if (rotContext.darkBack)
	{
		return &tileDarkBack;
	}
	return tileInstance.Is(TILEFLAG_ROTATIVE_RELIEF) ? &tileRelief : &tileInstance;
}

void RotativeLevel::DrawLevelColumn(const Renderer& renderer, size_t col) const
{
	col %= CIRCUMFERENCE_TILECOUNT;
	float lookAtX = renderer.GetLookAtWorld().x;
	float angle = (float)(2. * M_PI / (double)CIRCUMFERENCE_TILECOUNT);
	float s0 = sinf((col - lookAtX) * angle);
	float s1 = sinf((col + 1.f - lookAtX) * angle);
	float offsets[] { s0* RADIUS0, s1* RADIUS0, s1* RADIUS1, s0* RADIUS1 };

	for (size_t row = 0; row < (int)GetHeight(); ++row)
	{
		auto tile = GetTile(col, row);

		if (tile->Is(TILEFLAG_ROTATIVE))
		{
			static_cast<const RotativeTile*>(tile)->Render(
				renderer, GetTileSheet(), (float)row, offsets
			);
		}
	}
}

void RotativeLevel::Render(const Renderer& renderer) const
{
	float lookAtX = renderer.GetLookAtWorld().x;
	int col = (int)lookAtX + CIRCUMFERENCE_TILECOUNT;
	int delta = CIRCUMFERENCE_TILECOUNT / 2;

	GetParallax().Render(renderer, 0.f, lookAtX/static_cast<float>(GetWidth()));

	DrawLevelColumn(renderer, col + delta);
	while(--delta > 0)
	{
		DrawLevelColumn(renderer, col - delta);
		DrawLevelColumn(renderer, col + delta);
	}
	DrawLevelColumn(renderer, col);
}

void RotativeLevel::Normalize(float& x, float& y) const
{
	std::ignore = y;
	x += CIRCUMFERENCE_TILECOUNT;
	int ix = static_cast<int>(x);
	float dx = x - static_cast<float>(ix);

	ix = (ix + CIRCUMFERENCE_TILECOUNT) % CIRCUMFERENCE_TILECOUNT;
	x = (float)ix + dx;
}
