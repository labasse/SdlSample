#ifndef LEVELROTATIVE_H
#define LEVELROTATIVE_H

#include "Level.h"
#include "RotativeTile.h"
#include "RotativeReliefTile.h"
#include "DarkBackDecorator.h"

class RotativeLevel : public Level
{
public:
	static const char *Level1[];

	RotativeLevel(SDL_Texture* tilesheet, const Parallax& parallax);
	
	void Render(const Renderer& renderer) const override;
	void RenderPreview(const Renderer& renderer, const SDL_FPoint &Offset);

	void Normalize(float& x, float& y) const override;
private:
	struct RotativeLoadContext : public LoadContext {
		bool darkBack = true;
	} loadContext;
	struct TileGen : public TileGenerator {
		TileGen(char symbol, int tileIndex, int flags = 0);

		void OnRegisteredBy(const Level& level) override;
		Tile* NewTile(size_t, size_t, LoadContext&) override;

		char symbol;
	private:
		RotativeTile		tileInstance;
		RotativeReliefTile	tileRelief;
		DarkBackDecorator	tileDarkBack;
	};

	LoadContext& GetLineLoadContext(size_t row, const char* line) override;

	void DrawLevelColumn(const Renderer& renderer, size_t col) const;

	inline void NextTileSource(SDL_FRect& rcsNext, const SDL_FRect& rcs, float width) const {
		rcsNext.x = rcs.x + rcs.w;
		rcsNext.y = rcs.y;
		rcsNext.w = width;
		rcsNext.h = rcs.h;
	}
	static TileGen Predefined[];
};

#endif // LEVELROTATIVE_H