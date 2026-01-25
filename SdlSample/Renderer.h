#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <SDL3/SDL.h>

class Renderer
{
public:
	Renderer(
		SDL_Renderer* back, 
		int viewWidth, int viewHeight, 
		int pixelsPerWorldUnit);

	void BeginRender(float lookAtWorldX, float lookAtWorldY);
	void EndRender();

	void Render(SDL_Texture* texture, const SDL_FRect& srcRect, float worldX, float worldY) const;
	void RenderAlignedTileRect(float col, float row) const;
	void RenderLayer(SDL_Texture* texture, float coef) const;

private:
	SDL_Renderer* back;
	SDL_FRect view;
	float pixelsPerWorldUnit;
};

#endif // _CAMERA_H_