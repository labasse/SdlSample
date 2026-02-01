#ifndef LEVELROTATIVE_H
#define LEVELROTATIVE_H

#include "Level.h"
#include "RotativeTile.h"
#include "RotativeReliefTile.h"

class RotativeLevel : public Level
{
public:
	static const char *Level1[];

	RotativeLevel(SDL_Texture* tilesheet);
	
	void Render(const Renderer& renderer) const override;
	void RenderPreview(const Renderer& renderer, const SDL_FPoint &Offset);

	void Normalize(float& x, float& y) const override;
private:
	struct TileGen : public TileGenerator {
		TileGen(char symbol, int tileIndex, int flags = 0);

		Tile* NewTile() override;

		inline char GetSymbol() const { return symbol; }
	private:
		char symbol;
		RotativeTile tileInstance;
		RotativeReliefTile tileRelief;
	};

	void DrawLevelColumn(const Renderer& renderer, size_t col) const;
//	void DrawTile	 (const Renderer& renderer, size_t row, float offsetX, float width, int tileIndex) const;
//	void DrawPlatform(const Renderer& renderer, size_t col, size_t row) const;

	inline void NextTileSource(SDL_FRect& rcsNext, const SDL_FRect& rcs, float width) const {
		rcsNext.x = rcs.x + rcs.w;
		rcsNext.y = rcs.y;
		rcsNext.w = width;
		rcsNext.h = rcs.h;
	}
	static TileGen Predefined[];
};

#endif // LEVELROTATIVE_H