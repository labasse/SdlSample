#ifndef _DARKBACKDECORATOR_H_
#define _DARKBACKDECORATOR_H_

#include "RotativeTile.h"

class DarkBackDecorator : public RotativeTile
{
public:
	DarkBackDecorator(const RotativeTile& inner, size_t backOffset, int flags);

	void Render(const Renderer& renderer, const TileSheet& sheet, float row, float x[4]) const override;

	void PreCalcTileRects(const TileSheet& sheet) override;
private:
	const RotativeTile& innerTile;
	SDL_FRect rcBack;
};

#endif // _DARKBACKDECORATOR_H_
