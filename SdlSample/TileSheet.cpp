#include <algorithm>

#include "TileSheet.h"

TileSheet::TileSheet(SDL_Texture* tilesheet, int defaultTileSize, int columnNum) : 
	tilesheet(tilesheet),
	defaultTileSize(defaultTileSize),
	columnNum(columnNum)
{

}

SDL_FRect TileSheet::FromTileColRow(size_t col, size_t row, size_t specialWidth, size_t offsetX) const
{
	return {
		static_cast<float>(col * defaultTileSize + offsetX),
		static_cast<float>(row * defaultTileSize),
		static_cast<float>(specialWidth > 0 ? specialWidth : defaultTileSize),
		static_cast<float>(defaultTileSize)
	};
}

TileSheet::Anim::Anim(size_t row, Uint64 frameDelay, size_t frameCount, bool loop) : 
	row(row),
	frameDelay(frameDelay),
	frameCount(frameCount),
	loop(loop)
{ }

size_t TileSheet::Anim::GetFrame(Uint64 deltaTime) const
{
	size_t frame = (int)deltaTime / frameDelay;
	return loop ? frame % frameCount : std::min(frame, frameCount - 1);
}

