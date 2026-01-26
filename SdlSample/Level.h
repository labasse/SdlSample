#ifndef _LEVEL_H_
#define _LEVEL_H_

#include <SDL3/SDL.h>
#include <vector>
#include "Renderer.h"

#define LEVEL_TILE_SIZE 64

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
#define CHAR_DOOR1		 '\\'
#define CHAR_DOOR2		 '['
#define CHAR_DOOR3		 ']'
#define CHAR_EMPTY		 ' '

#define FREQ_BRACKET 5
#define FREQ_WEB     7

class Level
{
public:
	enum TileType {
		UNKNOWN = 0,
		EMPTY	=1<<0,
		LADDER	=1<<1,
		PLATFORM=1<<2,
		SOLID	=1<<3
	};

	Level(SDL_Texture* tilesheet);

	void Load(const char *tiles[]);

	inline TileType GetTileType (size_t col, size_t row) const;
	inline bool IsTile(size_t col, size_t row, TileType flags) const { return (GetTileType(col, row) & flags) != 0; }

	inline void		GetStart(size_t& col, size_t& row) const { col = startCol; row = startRow; }
	inline size_t	GetWidth () const { return width; }
	inline size_t	GetHeight() const { return height; }

	virtual void Render(const Renderer& renderer) = 0;
protected:
	virtual int GetSpriteIndexFromChar(char c) const = 0;
	
	inline int GetTileSpriteIndex(size_t col, size_t row) const { 
		return tileSprites[row][col];
	}
	inline SDL_Texture* GetTileSheet() const { return tilesheet; }
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

Level::TileType Level::GetTileType(size_t col, size_t row) const
{
	return row < height 
		? types[tiles[row][col]]
		: TileType::EMPTY; 
}

inline Level::TileType operator|(Level::TileType a, Level::TileType b)
{
	return (Level::TileType)((int)a | (int)b);
}

#endif // _LEVEL_H_
