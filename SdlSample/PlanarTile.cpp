#include "PlanarTile.h"

PlanarTile::PlanarTile(int tileIndex, int flags) :
	Tile(flags | TILEFLAG_PLANAR),
	tileIndex(tileIndex)
{

}
