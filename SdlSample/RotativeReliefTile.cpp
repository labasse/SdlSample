#include "RotativeReliefTile.h"

#define SIZE_FACE	72
#define SIZE_SIDE	48

RotativeReliefTile::RotativeReliefTile(int tileIndex, int flags) :
	RotativeTile(TILEINDEX_BGSTONE, flags),
	tileRelief(tileIndex)
{
}

void RotativeReliefTile::Render(const Renderer& renderer, const TileSheet& sheet, float row, float x[4]) const
{
	size_t faceX;
	size_t sideX;
	size_t offsetX;

	if(Is(TILEFLAG_PLATFORM))
	{
		RotativeTile::Render(renderer, sheet, row, x);
	}
	if (x[3] < x[2])
	{
		faceX = 3;
		sideX = x[0] < x[3] ? 0 : 2;
		offsetX = 0;
	}
	else
	{
		faceX = 1;
		sideX = x[0] < x[3] ? 0 : 2;
		offsetX = SIZE_FACE + SIZE_SIDE;
	}
	auto srcSide = sheet.FromTileIndex(tileRelief, SIZE_SIDE, SIZE_FACE);

	if(x[0] < x[3])
		renderer.RenderTileScaledX(sheet.GetTexture(), srcSide, x[0], row, x[3] - x[0]);
	if(x[2] < x[1])
		renderer.RenderTileScaledX(sheet.GetTexture(), srcSide, x[2], row, x[1] - x[2]);

	renderer.RenderTileScaledX(
		sheet.GetTexture(),
		sheet.FromTileIndex(tileRelief, SIZE_FACE, offsetX),
		x[faceX], row, x[faceX-1] - x[faceX]
	);
}