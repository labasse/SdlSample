#ifndef LEVELROTATIVE_H
#define LEVELROTATIVE_H

#include "Level.h"

class RotativeLevel : public Level
{
public:
	static const char *Level1[];

	RotativeLevel(SDL_Texture* tilesheet, const SDL_FPoint& worldLevelOffset);
	
	void Render(const Renderer& renderer) override;
protected:
	int GetSpriteIndexFromChar(char c) const override;
private:
	SDL_FPoint worldLevelOffset;
};

#endif // LEVELROTATIVE_H