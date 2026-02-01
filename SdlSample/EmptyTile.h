#ifndef _EMPTY_TYPE_H
#define _EMPTY_TYPE_H

#include "Tile.h"

class EmptyTile : public Tile
{
public:
	static inline EmptyTile* GetInstance() { return &instance; }
private:
	EmptyTile();
	
	static EmptyTile instance;
};

#endif // _EMPTY_TILE_H
