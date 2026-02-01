#include "Parallax.h"
#include "Settings.h"

Parallax::Parallax(const ResourceManager& resources, size_t firstLayer, const float layerDepth[], size_t layerCount, int flags) :
	flags(flags)
{
	ASSERT(layerCount>=2);
	float min = 0.f;
	float max = 0.f;

	for (size_t i = 0; i < layerCount; ++i)
	{
		if (i > 0)
		{
			float delta = layerDepth[i] - layerDepth[i - 1];
			
			ASSERT(delta >= 0.f);
			if (delta > 0.f && (min == 0.f || delta < min))
			{
				min = delta;
			}
		}
		layers.push_back(resources.GetTexture(firstLayer + i));
		coefs.push_back(max = layerDepth[i]);
	}
	ASSERT(min > 0.f);
	for(auto c : coefs)
		circs.push_back(1.f + roundf((max-c) / min));
}

void Parallax::Render(const Renderer& renderer, float centerWorldX, float angleRatio) const
{
	for (size_t i = 0; i < layers.size(); ++i)
		renderer.RenderParallaxLayer(
			layers[i], centerWorldX, angleRatio, 
			coefs[i], circs[i], 
			(flags & NOREPEAT(i)) == 0
		);
}