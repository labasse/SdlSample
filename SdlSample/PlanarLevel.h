#ifndef _LEVEL_PLANAR_H_
#define _LEVEL_PLANAR_H_

#include "Level.h"

class PlanarLevel : public Level
{
public:
	static const char *Level0[];

	PlanarLevel(SDL_Texture* tilesheet);

	void Render(const Renderer& renderer) override;
protected:
	int GetSpriteIndexFromChar(char c) const override;
};

#endif // _LEVEL_PLANAR_H_
