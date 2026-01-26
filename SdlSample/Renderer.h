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

	void RenderTile(SDL_Texture* texture, const SDL_FRect& srcRect, float worldX, float worldY, const SDL_FPoint* offset = nullptr) const;
	void RenderAlignedTileRect(float col, float row) const;
	void RenderParallaxLayer(SDL_Texture* texture, float coef) const;
	void RenderRotativePlatform(SDL_Texture* texture,
		const SDL_FRect& srcFace, 
		const SDL_FRect& srcSide, 
		const SDL_FRect& srcBack, 
		float worldX, float worldY
	) const;
	
protected:
	SDL_FRect GetTileRect(float worldX, float worldY, float pixelWidth, float pixelHeight, const SDL_FPoint* offset) const;
	
	inline SDL_Renderer* GetRenderer() const { return back; }
	
	inline const SDL_FRect&  GetView		() const { return view; }
	inline const SDL_FPoint& GetLookAtWorld	() const { return lookAtWorld; }
	
	inline float GetPixelsPerWorldUnit() const { return pixelsPerWorldUnit; }
private:
	SDL_Renderer* back;
	SDL_FPoint lookAtWorld;
	SDL_FRect view;
	float pixelsPerWorldUnit;
	int circumference;
	float tileAngle;
	float radius;
};

#endif // _CAMERA_H_