#ifndef _ROTATIVE_PLATFORM_TILE_H_
#define _ROTATIVE_PLATFORM_TILE_H_

#include "RotativeTile.h"

#define TILEFLAG_ROTATIVE_RELIEF	(TILEFLAG_PLATFORM|TILEFLAG_SOLID)

class RotativeReliefTile : public RotativeTile
{
public:
	RotativeReliefTile(int tileIndex, int flags);

	void RenderFront(const Renderer& renderer, const TileSheet& sheet, float row, float x[4]) const override;
	void RenderBack (const Renderer& renderer, const TileSheet& sheet, float row, float x[4]) const override;
private:
	void RenderRelief(const Renderer& renderer, const TileSheet& sheet, size_t firstX, float row, float x[4]) const;

	size_t tileRelief;
};

#endif // _ROTATIVE_PLATFORM_TILE_H_