#ifndef _VISIBLE_TILE_H
#define _VISIBLE_TILE_H

#include "Tile.h"

#define TILEFLAG_PLANAR		(1<<16)

class PlanarTile : public Tile
{
public:
	PlanarTile(int tileIndex, int flags);

	inline int GetTileIndex() const { return tileIndex; }
private:
	int tileIndex;
};

#endif // _VISIBLE_TILE_H

