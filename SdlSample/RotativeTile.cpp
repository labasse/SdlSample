#include <algorithm>

#include "RotativeTile.h"

RotativeTile::RotativeTile(int tileIndex, int flags) :
	Tile(flags | TILEFLAG_ROTATIVE),
	tileIndex(tileIndex)
{ }

void RotativeTile::Render(const Renderer& renderer, const TileSheet& sheet, float row, float x[4]) const
{
	size_t firstX;
	size_t index;

	if (x[0] < x[1])
	{
		firstX = 0;
		index = tileIndex;
	}
	else
	{
		firstX = 1;
		index = Is(TILEFLAG_SPECIALBCK) 
			? tileIndex + sheet.GetColumnNum() 
			: TILEINDEX_TILE_BCK;
	}
	renderer.RenderTileScaledX(
		sheet.GetTexture(), 
		sheet.FromTileIndex(index), 
		x[firstX], row,
		x[1-firstX] - x[firstX]
	);
}
