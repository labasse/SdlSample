#include "PlanarTile.h"

PlanarTile::PlanarTile(size_t tileIndex, int flags) :
	Tile(flags | TILEFLAG_PLANAR),
	tileIndex(tileIndex),
	rc{ 0.f, 0.f, 0.f, 0.f }
{ }
