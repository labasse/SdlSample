#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <SDL3/SDL.h>

#define LOOKAT_SCREEN_X 0.5f
#define LOOKAT_SCREEN_Y 0.7f

class Renderer
{
public:
	Renderer(
		SDL_Renderer* back, 
		int viewWidth, int viewHeight,
		int pixelsPerWorldUnit);

	void BeginRender(float lookAtWorldX, float lookAtWorldY);
	void EndRender();

	void GetVisibleArea(SDL_Rect& worldRect) const;

	void RenderTile		  (SDL_Texture* texture, SDL_FRect src, float worldX, float worldY) const;
	void RenderTileScaledX(SDL_Texture* texture, SDL_FRect src, float dxFromLookAt, float worldY, float widthX) const;

	void RenderAlignedTileRect(float col, float row) const;
	void RenderParallaxLayer(SDL_Texture* texture, float centerWorldX, float angleRatio, float layerCoef, float layerCirc, bool repeatable) const;
	
	inline const SDL_FPoint& GetLookAtWorld() const { return lookAtWorld; }

	inline static float Align(size_t val) { return static_cast<float>(val) + 0.5f; }
	inline static float Align(float  val) { return Align(static_cast<size_t>(val)); }
private:
	SDL_Renderer* back;
	SDL_FPoint lookAtWorld;
	SDL_FRect view;
	float pixelsPerWorldUnit;
};

#endif // _CAMERA_H_