#ifndef _LEVEL_PLANAR_H_
#define _LEVEL_PLANAR_H_

#include "Level.h"
#include "PlanarTile.h"

class PlanarLevel : public Level
{
public:
	static const char* Level0[];

	PlanarLevel(SDL_Texture* tilesheet, const Parallax& parallax);

	void Render(const Renderer& renderer) const override;
	void Normalize(float& x, float& y) const override;
protected:
	struct TileGen : public TileGenerator {
		TileGen(char symbol, int tileIndex, int flags = 0, int freqAlt=0);

		Tile* NewTile() override;

		inline char GetSymbol() const { return symbol; }
	private:	
		char symbol;
		int freqAlt;
		PlanarTile tileInstance;
		PlanarTile tileInstanceAlt;
	};
	static TileGen Predefined[];
};

#endif // _LEVEL_PLANAR_H_
