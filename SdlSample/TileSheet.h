#ifndef _TILESHEET_H
#define _TILESHEET_H

#include <SDL3/SDL.h>

class TileSheet
{
public:
	struct Anim;

	TileSheet(SDL_Texture* tilesheet, int defaultTileSize, int columnNum);

	SDL_FRect FromTileColRow(size_t col, size_t row, size_t specialWidth = 0, size_t offsetX = 0) const;

	inline SDL_FRect FromAnim	   (const Anim& anim, Uint64 deltaTime) const;
	inline SDL_FRect FromTileIndex (size_t index, size_t specialWidth = 0, size_t offsetX = 0) const;

	inline size_t		GetColumnNum() const { return columnNum; }
	inline SDL_Texture* GetTexture  () const { return tilesheet; }
private:
	SDL_Texture* tilesheet;
	int defaultTileSize;
	int columnNum;
public:
	struct Anim
	{
		Anim(size_t row, Uint64 frameDelay, size_t frameCount, bool loop = true);

		size_t GetFrame(Uint64 deltaTime) const;
		inline size_t GetRow() const { return row; }
	private:
		size_t row, frameDelay, frameCount; 
		bool loop;
	};
};

SDL_FRect TileSheet::FromAnim(const Anim& anim, Uint64 deltaTime) const
{
	return FromTileColRow(anim.GetFrame(deltaTime), anim.GetRow());
}

SDL_FRect TileSheet::FromTileIndex(size_t index, size_t specialWidth, size_t offsetX) const
{
	return FromTileColRow(index%columnNum, index/columnNum, specialWidth, offsetX);
}

#endif
