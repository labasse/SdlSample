#ifndef _VISIBLE_TILE_H
#define _VISIBLE_TILE_H

#include "Tile.h"
#include "TileSheet.h"

#define TILEFLAG_PLANAR		(1<<16)

class PlanarTile : public Tile
{
public:
	PlanarTile(size_t tileIndex, int flags);

	inline void PreCalcTileRects(const TileSheet& sheet)	{ rc = sheet.FromTileIndex(tileIndex); }
	inline const SDL_FRect& GetTileRect() const				{ return rc; }
private:
	size_t tileIndex;
	SDL_FRect rc;
};

#endif // _VISIBLE_TILE_H

