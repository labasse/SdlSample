#ifndef _LEVEL_H_
#define _LEVEL_H_

#include <SDL3/SDL.h>
#include <vector>
#include "Renderer.h"

#define LEVEL_TILE_SIZE 64

class Level
{
public:
	static const char* Test[];
	enum TileType {
		UNKNOWN = 0,
		EMPTY	=1<<0,
		LADDER	=1<<1,
		PLATFORM=1<<2,
		SOLID	=1<<3
	};

	Level(SDL_Texture* tilesheet, const char* tiles[]);

	inline TileType GetTile (size_t col, size_t row) const;
	inline bool IsTile(size_t col, size_t row, TileType flags) const { return (GetTile(col, row) & flags) != 0; }

	inline void		GetStart(size_t& col, size_t& row) const { col = startCol; row = startRow; }
	inline size_t	GetWidth () const { return width; }
	inline size_t	GetHeight() const { return height; }

	void Render(const Renderer& renderer);
private:
	std::vector<std::vector<int>> tileSprites;
	SDL_Texture* tilesheet;
	TileType types[0x7f];
	const char** tiles;
	size_t width;
	size_t height;
	size_t startCol;
	size_t startRow;
};

Level::TileType Level::GetTile(size_t col, size_t row) const
{
	return row < height 
		? types[tiles[height - row - 1][col]]
		: TileType::EMPTY; 
}

inline Level::TileType operator|(Level::TileType a, Level::TileType b)
{
	return (Level::TileType)((int)a | (int)b);
}

#endif // _LEVEL_H_
