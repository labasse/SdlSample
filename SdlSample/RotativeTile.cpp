#include "RotativeTile.h"

RotativeTile::RotativeTile(int tileIndex, int flags) :
	Tile(flags | TILEFLAG_ROTATIVE),
	tileIndex(tileIndex)
{ }

void RotativeTile::RenderFront(const Renderer& renderer, const TileSheet& sheet, float row, float x[4]) const
{
	renderer.RenderTileScaledX(sheet.GetTexture(), sheet.FromTileIndex(tileIndex), x[0], x[1], row);
}

void RotativeTile::RenderBack(const Renderer& renderer, const TileSheet& sheet, float row, float x[4]) const
{
	size_t index = Is(TILEFLAG_SPECIALBCK)
		? tileIndex + sheet.GetColumnNum()
		: TILEINDEX_TILE_BCK;
	renderer.RenderTileScaledX(sheet.GetTexture(), sheet.FromTileIndex(index), x[1], x[0], row);
}
