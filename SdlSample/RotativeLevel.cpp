#include "Settings.h"

#include "RotativeLevel.h"

#define PLATFORM_WIDTH 72
#define WALL_WIDTH     LEVEL_TILE_SIZE

const char *RotativeLevel::Level1[] = {
	"| | | | | | | | | | | | | | | | | | ",
	"O|O|O|O|O|O|O|O|O|O|O|O|O|O|O|O|O|O|",
	"OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO",
	"OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO",
	"xOOO------OOOOOOOOOOOOOOOOO------OOO",
	"H---OOOOOO-----------------OOOOOO---",
	"HOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO",
	"HOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO",
	"HOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO",
	nullptr
};

struct {
	int widthWall;
	int widthPlatform;
	int depthPlatform;
} rotatedColumns[] = {
	{  0,  0, 48 },
	{ 11, 13, 46 },
	{ 21, 25, 44 },
	{ 31, 36, 39 },
	{ 39, 46, 34 },
	{ 47, 55, 28 },
	{ 53, 63, 20 },
	{ 58, 68, 12 },
	{ 60, 71,  4 }
};

RotativeLevel::RotativeLevel(SDL_Texture* tilesheet, const SDL_FPoint &worldLevelOffset) : 
	Level(tilesheet),
	worldLevelOffset(worldLevelOffset)
{

}

int RotativeLevel::GetSpriteIndexFromChar(char c) const
{
	switch (c)
	{
	case CHAR_START      :
	case CHAR_FINISH     :
	case CHAR_EMPTY      : return -1;
	case CHAR_BGSTONE    : return  6;
	case CHAR_STONEFLOOR : return rand() % FREQ_WEB ? 11 : 23;
	case CHAR_STONEWALL  : return 10;
	case CHAR_LADDER     : return 21;
	default:
		ASSERT(false);
		return -1;
	}
}

void RotativeLevel::Render(const Renderer& renderer)
{
	/* for (int row = 0; row < (int)GetHeight(); ++row) {
		for(int col = 0; col < (int)GetWidth(); ++col) {
			auto tileIndex = GetTileSpriteIndex(col, row);
			if (tileIndex >= 0) {
				SDL_FRect rcs = { 
					(float)(tileIndex % 8) * LEVEL_TILE_SIZE, 
					(float)(tileIndex / 8) * LEVEL_TILE_SIZE, 
					(float)LEVEL_TILE_SIZE, 
					(float)LEVEL_TILE_SIZE 
				};
				float worldX = worldLevelOffset.x + col * PLATFORM_WIDTH;
				float worldY = worldLevelOffset.y + row * LEVEL_TILE_SIZE;
				int absCol = (int)col;
				if (absCol < GetWidth() / 2)
					worldX += rotatedColumns[absCol].depthPlatform;
				else
					worldX += rotatedColumns[(int)GetWidth() - absCol - 1].depthPlatform;
				renderer.RenderRotativeTile(
					GetTileSheet(), rcs,
					worldX,
					worldY
				);
			}
		}
	}*/
}
