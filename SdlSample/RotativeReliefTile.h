#ifndef _ROTATIVE_PLATFORM_TILE_H_
#define _ROTATIVE_PLATFORM_TILE_H_

#include "RotativeTile.h"

#define TILEFLAG_ROTATIVE_RELIEF	(TILEFLAG_PLATFORM|TILEFLAG_SOLID)

class RotativeReliefTile : public RotativeTile
{
public:
	RotativeReliefTile(int tileIndex, int flags);

	void Render(const Renderer& renderer, const TileSheet& sheet, float row, float x[4]) const override;

	void PreCalcTileRects(const TileSheet& sheet) override;
private:
	void RenderRelief(const Renderer& renderer, const TileSheet& sheet, size_t firstX, float row, float x[4]) const;

	int reliefIndex;
	SDL_FRect rcFace;
	SDL_FRect rcSide;
	SDL_FRect rcBack;
};

#endif // _ROTATIVE_PLATFORM_TILE_H_