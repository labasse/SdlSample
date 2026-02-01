#ifndef _PARALLAX_H_
#define _PARALLAX_H_

#include <vector>
#include "ResourceManager.h"
#include "Renderer.h"

#define NOREPEAT(layer)	(1<<(layer))

class Parallax
{
public:
	Parallax(const ResourceManager& resources, size_t firstLayer, const float layerDepth[], size_t layerCount, int flags);

	void Render(const Renderer& renderer, float centerWorldX, float angleRatio) const;
private:
	std::vector<SDL_Texture*> layers;
	std::vector<float> coefs;
	std::vector<float> circs;
	int flags;
};

#endif // _PARALLAX_H_

