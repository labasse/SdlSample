#include "RotativeReliefTile.h"

#define SIZE_FACE	72
#define SIZE_SIDE	48

RotativeReliefTile::RotativeReliefTile(int tileIndex, int flags) :
	RotativeTile(TILEINDEX_BGSTONE, flags),
	tileRelief(tileIndex)
{ }

void RotativeReliefTile::RenderRelief(const Renderer& renderer, const TileSheet& sheet, size_t firstX, float row, float x[4]) const
{
	auto srcSide = sheet.FromTileIndex(tileRelief, SIZE_SIDE, SIZE_FACE);

	if (x[0] < x[3])
		renderer.RenderTileScaledX(sheet.GetTexture(), srcSide, x[0], x[3], row);
	if (x[2] < x[1])
		renderer.RenderTileScaledX(sheet.GetTexture(), srcSide, x[2], x[1], row);
	renderer.RenderTileScaledX(
		sheet.GetTexture(),
		sheet.FromTileIndex(tileRelief, SIZE_FACE, 0),
		x[firstX], x[firstX - 1], row
	);
}


void RotativeReliefTile::RenderFront(const Renderer& renderer, const TileSheet& sheet, float row, float x[4]) const
{
	if (Is(TILEFLAG_PLATFORM))
	{
		RotativeTile::RenderFront(renderer, sheet, row, x);
	}
	RenderRelief(renderer, sheet, 3, row, x);
}

void RotativeReliefTile::RenderBack(const Renderer& renderer, const TileSheet& sheet, float row, float x[4]) const
{
	if (Is(TILEFLAG_PLATFORM))
	{
		RotativeTile::RenderBack(renderer, sheet, row, x);
	}
	RenderRelief(renderer, sheet, 1, row, x);
};
