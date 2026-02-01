#include "EmptyTile.h"

EmptyTile EmptyTile::instance;

EmptyTile::EmptyTile() : Tile(TILEFLAG_NONE)
{}
