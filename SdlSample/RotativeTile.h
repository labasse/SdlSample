#ifndef _ROTATIVE_TILE_H_
#define _ROTATIVE_TILE_H_

#include "Tile.h"
#include "TileSheet.h"
#include "Renderer.h"

#define TILEFLAG_ROTATIVE	(1<<16)
#define TILEFLAG_SPECIALBCK (1<<17)
#define TILEFLAG_DRAWBGTILE (1<<18)
#define TILEFLAG_ASYMMETRIC	(1<<19)

class RotativeTile : public Tile
{
public:
	RotativeTile(size_t tileIndex, int flags);
	
	/// Renders the rotative tile at the specified column and row with given x-coordinates.
	/// @param renderer The renderer to use for rendering.
	/// @param col The tile column in the map.
	/// @param row The tile row in the map.
	/// @param x An array of four float values representing the x-coordinates for rendering.
	/// @remark
	/// The x-coordinates follow this scheme (top view) :
	///  		0 -- 1
	///    	   //    \\
	///        3 ---- 2
	virtual void Render(const Renderer& renderer, const TileSheet& sheet, float row, float x[4]) const;

	virtual void PreCalcTileRects(const TileSheet& sheet);

	inline const SDL_FRect& GetTileRect () const { return rc; }
	inline size_t			GetTileIndex() const { return tileIndex; }
protected:
	static inline bool ShouldRenderFront(float x[4]) { return x[0] < x[1]; }
private:
	size_t tileIndex;
	SDL_FRect rc;
};

#endif // _ROTATIVE_TILE_H_