#include "DarkBackDecorator.h"

DarkBackDecorator::DarkBackDecorator(const RotativeTile& inner, size_t backOffset, int flags) :
	RotativeTile(
		(flags&TILEFLAG_SPECIALBCK) 
		? inner.GetTileIndex() + backOffset 
		: TILEINDEX_TILE_BCK, 
		flags
	),
	innerTile(inner),
	rcBack()
{ }

void DarkBackDecorator::PreCalcTileRects(const TileSheet& sheet)
{
	rcBack = sheet.FromTileIndex(GetTileIndex());
}

void DarkBackDecorator::Render(const Renderer& renderer, const TileSheet& sheet, float row, float x[4]) const
{
	innerTile.Render(renderer, sheet, row, x);
	if (!ShouldRenderFront(x))
		renderer.RenderTileScaledX(sheet.GetTexture(), rcBack, x[1], x[0], row);
}
