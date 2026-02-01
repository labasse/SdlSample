#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

#include "Settings.h"

#include "RotativeLevel.h"

#define CIRCUMFERENCE_TILECOUNT 36
#define RADIUS0	367.f
#define RADIUS1 415.f

#define PLATFORM_FACE_WIDTH 72.f
#define PLATFORM_SIDE_WIDTH 48.f

const char *RotativeLevel::Level1[] = {
	"                                    ",
	"           |===|                    ",
	"         |==/~\\===|                 ",
	"         |OO[ ]OOO======|           ",
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
	{ CHAR_WINDOW0    , TILEINDEX_WND0		 , TILEFLAG_SPECIALBCK},
	{ CHAR_WINDOW1    , TILEINDEX_WND1		 , TILEFLAG_SPECIALBCK},
	{ CHAR_DOOR0      , TILEINDEX_DOOR0		 },
	{ CHAR_DOOR1      , TILEINDEX_DOOR1		 , TILEFLAG_SPECIALBCK},
	{ CHAR_DOOR2      , TILEINDEX_DOOR2		 },
	{ CHAR_DOOR3      , TILEINDEX_DOOR3		 },
	{ CHAR_DOOR4      , TILEINDEX_DOOR4		 },
	{ CHAR_STONEFLOOR , TILEINDEX_RELIEF_STONEFLOOR , TILEFLAG_PLATFORM },
	{ CHAR_STONEWALL  , TILEINDEX_RELIEF_STONEWALL  , TILEFLAG_SOLID    }
};

RotativeLevel::RotativeLevel(SDL_Texture* tilesheet, const Parallax& parallax) : 
	Level(tilesheet, parallax)
{
	RegisterEmptyTile(CHAR_EMPTY);
	RegisterEmptyTile(CHAR_FINISH);
	for (auto& gen : Predefined)
	{
		RegisterTileType(gen.GetSymbol(), &gen);
	}
}

RotativeLevel::TileGen::TileGen(char symbol, int tileIndex, int flags) :
	symbol(symbol),
	tileInstance(tileIndex, flags),
	tileRelief(tileIndex, flags)
{
}

Tile* RotativeLevel::TileGen::NewTile()
{
	return tileInstance.Is(TILEFLAG_ROTATIVE_RELIEF) ? &tileRelief : &tileInstance;
}

void RotativeLevel::DrawLevelColumn(const Renderer& renderer, size_t col) const
{
	col %= CIRCUMFERENCE_TILECOUNT;
	float lookAtX = renderer.GetLookAtWorld().x;
	float angle = (float)(2. * M_PI / (double)CIRCUMFERENCE_TILECOUNT);
	float s0 = sinf((col - lookAtX) * angle);
	float s1 = sinf((col + 1.f - lookAtX) * angle);
	float offsets[4] = {
		s0* RADIUS0,
		s1* RADIUS0,
		s1* RADIUS1,
		s0* RADIUS1
	};

	for (size_t row = 0; row < (int)GetHeight(); ++row)
	{
		auto tile = GetTile(col, row);

		if (tile->Is(TILEFLAG_ROTATIVE))
		{
			static_cast<const RotativeTile*>(tile)->Render(
				renderer, GetTileSheet(), 
				(float)row, offsets
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
