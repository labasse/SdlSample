#include "RotativeReliefTile.h"

#define SIZE_FACE	74
#define SIZE_SIDE	54

RotativeReliefTile::RotativeReliefTile(int tileIndex, int flags) :
	RotativeTile(TILEINDEX_BGSTONE, flags),
	reliefIndex(tileIndex),
	rcFace(),
	rcSide(),
	rcBack()
{ }

void RotativeReliefTile::PreCalcTileRects(const TileSheet& sheet)
{
	RotativeTile::PreCalcTileRects(sheet);
	rcFace = sheet.FromTileIndex(reliefIndex + 1, SIZE_FACE);
	rcSide = sheet.FromTileIndex(reliefIndex + 1, SIZE_SIDE, SIZE_FACE);
	rcBack = sheet.FromTileIndex(reliefIndex);
}

void RotativeReliefTile::Render(const Renderer& renderer, const TileSheet& sheet, float row, float x[4]) const
{
	if (Is(TILEFLAG_DRAWBGTILE))
	{
		RotativeTile::Render(renderer, sheet, row, x);
	}
	auto tx = sheet.GetTexture();

	renderer.RenderTileScaledX(tx, rcSide, x[0], x[3], row);
	renderer.RenderTileScaledX(tx, rcSide, x[2], x[1], row);
	renderer.RenderTileScaledX(tx, rcFace, x[3], x[2], row);
	renderer.RenderTileScaledX(tx, rcBack, x[1], x[0], row);
}
