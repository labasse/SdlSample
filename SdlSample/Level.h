#ifndef _LEVEL_H_
#define _LEVEL_H_

#include <vector>
#include "Renderer.h"
#include "TileSheet.h"
#include "EmptyTile.h"
#include "Parallax.h"

#define LEVEL_TILE_SIZE		64
#define LEVEL_TILE_COLUMNS	5

class Level
{
public:
	Level(SDL_Texture* tilesheet, const Parallax& parallax);
	virtual ~Level();

	void Load(const char *tiles[]);

	inline const Tile* GetTile(size_t col, size_t row) const;

	inline bool IsTile(size_t col, size_t row, int flags) const { return GetTile(col, row)->Is(flags); }
	inline bool IsTileAround(const SDL_FPoint& pos, int dx, int dy, int flags) const;

	inline void		GetStart(size_t& col, size_t& row) const { col = startCol; row = startRow; }
	inline size_t	GetWidth () const { return width; }
	inline size_t	GetHeight() const { return height; }

	virtual void Render(const Renderer& renderer) const = 0;
	virtual void Normalize(float& x, float &y) const = 0;

	static const SDL_Point HERE;
	static const SDL_Point NORTH;
	static const SDL_Point WEST;
	static const SDL_Point SOUTH;
	static const SDL_Point EAST;
	static const SDL_Point NORTHWEST;
	static const SDL_Point NORTHEAST;
	static const SDL_Point SOUTHWEST;
	static const SDL_Point SOUTHEAST;
protected:
	struct LoadContext {
		Tile* previous = nullptr;
	} defaultLoadContext;

	struct TileGenerator {
		virtual Tile* NewTile(size_t col, size_t row, LoadContext& context) = 0;
	};
	virtual LoadContext& GetLineLoadContext(size_t row, const char *line);

	void RegisterTileType(char symbol, TileGenerator* gen);
	inline void RegisterEmptyTile(char symbol) { RegisterTileType(symbol, &emptyGen); }

	inline TileSheet&		GetTileSheet()		 { return tilesheet; }
	inline const TileSheet& GetTileSheet() const { return tilesheet; }
	inline const Parallax&  GetParallax () const { return parallax; }
private:
	struct EmptyTileGen : public TileGenerator {
		Tile* NewTile(size_t, size_t, LoadContext&) override { return EmptyTile::GetInstance(); }
	} emptyGen;
	TileSheet tilesheet;
	const Parallax &parallax;
	std::vector<TileGenerator*> genByChar;
	std::vector<std::vector<Tile*>> tiles;
	size_t width;
	size_t height;
	size_t startCol;
	size_t startRow;
};

const Tile* Level::GetTile(size_t col, size_t row) const
{
	return row < height && col < width
		? tiles[row][col]
		: EmptyTile::GetInstance(); 
}

inline bool Level::IsTileAround(const SDL_FPoint& pos, int dx, int dy, int flags) const
{
	float x = pos.x + (float)dx;
	float y = pos.y + (float)dy;

	Normalize(x, y);
	return IsTile((size_t)x, (size_t)y, flags);
}

#define CHAR_START		 'x'
#define CHAR_FINISH		 '!'
#define CHAR_BGSTONE	 'O'
#define CHAR_CRATE		 '#'
#define CHAR_LADDER		 'H'
#define CHAR_PALE		 '^'
#define CHAR_WOODENFLOOR '-'
#define CHAR_STONEFLOOR  '='
#define CHAR_STONEWALL   '|'
#define CHAR_WINDOW0	 '('
#define CHAR_WINDOW1	 ')'
#define CHAR_DOOR0		 '/'
#define CHAR_DOOR1		 '~'
#define CHAR_DOOR2		 '\\'
#define CHAR_DOOR3		 '['
#define CHAR_DOOR4		 ']'
#define CHAR_EMPTY		 ' '

#endif // _LEVEL_H_
