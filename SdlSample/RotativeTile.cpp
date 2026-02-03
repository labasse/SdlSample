#include "RotativeTile.h"

RotativeTile::RotativeTile(size_t tileIndex, int flags) :
	Tile(flags | TILEFLAG_ROTATIVE),
	tileIndex(tileIndex),
	rc()
{ }

void RotativeTile::PreCalcTileRects(const TileSheet& sheet)
{
	rc = sheet.FromTileIndex(tileIndex);
}

void RotativeTile::Render(const Renderer& renderer, const TileSheet& sheet, float row, float x[4]) const
{
	bool front;
	int min = (front = ShouldRenderFront(x)) ? 0 : 1;

	renderer.RenderTileScaledX(sheet.GetTexture(), rc, x[min], x[1 - min], row, !front && Is(TILEFLAG_ASYMMETRIC));
}
