#include "Renderer.h"
#include "Settings.h"

#define VISIBLE_AREA_COMPLETION 2.f
#define PARALLAX_MAX_Y 10.f

Renderer::Renderer(SDL_Renderer* back, int viewWidth, int viewHeight, int pixelsPerWorldUnit) : 
	back(back),
	lookAtWorld({ 0.f, 0.f }),
	view({ 0.0f, 0.0f, (float)viewWidth, (float)viewHeight }),
	pixelsPerWorldUnit((float)pixelsPerWorldUnit)
{ }

void Renderer::BeginRender(float lookAtWorldX, float lookAtWorldY)
{
	lookAtWorld.x = lookAtWorldX;
	lookAtWorld.y = lookAtWorldY;
}

void Renderer::EndRender()
{
	SDL_RenderPresent(back);
}

void Renderer::GetVisibleArea(SDL_Rect& worldRect) const
{
	worldRect.x = (int)(lookAtWorld.x - view.w * LOOKAT_SCREEN_X / pixelsPerWorldUnit);
	worldRect.y = (int)(lookAtWorld.y - view.h * LOOKAT_SCREEN_Y / pixelsPerWorldUnit);
	worldRect.w = (int)(view.w / pixelsPerWorldUnit + VISIBLE_AREA_COMPLETION);
	worldRect.h = (int)(view.h / pixelsPerWorldUnit + VISIBLE_AREA_COMPLETION);
}

void Renderer::RenderAlignedTileRect(float col, float row) const
{
	SDL_FRect dstRect {	
		((float)(int)col - lookAtWorld.x) * pixelsPerWorldUnit + view.w * LOOKAT_SCREEN_X, 
		((float)(int)row - lookAtWorld.y) * pixelsPerWorldUnit + view.h * LOOKAT_SCREEN_Y,
		pixelsPerWorldUnit, pixelsPerWorldUnit
	};
	SDL_SetRenderDrawColor(back, 0, 255, 0, 255);
	SDL_RenderRect(back, &dstRect);
}

void Renderer::RenderTile(SDL_Texture* texture, SDL_FRect src, float worldX, float worldY) const
{
	SDL_FRect dst{
		(worldX - lookAtWorld.x) * pixelsPerWorldUnit + view.w * LOOKAT_SCREEN_X,
		(worldY - lookAtWorld.y) * pixelsPerWorldUnit + view.h * LOOKAT_SCREEN_Y,
		src.w, src.h
	};
	SDL_RenderTexture(back, texture, &src, &dst);
}

void Renderer::RenderTileScaledX(SDL_Texture* texture, SDL_FRect src, float dxFromLookAt, float worldY, float widthX) const
{
	SDL_FRect dst = {
		view.w * LOOKAT_SCREEN_X + dxFromLookAt,
		(worldY - lookAtWorld.y) * pixelsPerWorldUnit + view.h * LOOKAT_SCREEN_Y,
		widthX, src.h
	};
	SDL_RenderTexture(back, texture, &src, &dst);
}

void Renderer::RenderParallaxLayer(SDL_Texture* texture, float centerWorldX, float angleRatio, float layerCoef, float layerCirc, bool repeatX) const
{
	ASSERT(centerWorldX >= 0);
	ASSERT(layerCoef >= 0);
	ASSERT(angleRatio > 0);
	size_t w = (size_t)view.w;
	float pixelsCoef = pixelsPerWorldUnit * layerCoef;
	size_t planarShift = static_cast<size_t>(centerWorldX * pixelsCoef);
	size_t rotateShift = static_cast<size_t>(angleRatio * layerCirc * view.w);
	size_t modulo = repeatX ? w : static_cast<size_t>(layerCirc * view.w);
	SDL_FRect rcd {
		- static_cast<float>((planarShift + rotateShift) % modulo),
		(PARALLAX_MAX_Y - lookAtWorld.y) * pixelsCoef,
		view.w, view.h
	};
	int repeat = repeatX || rotateShift >= (layerCirc-1) ? 2 : 1;

	for (int i = 0; i < repeat; ++i)
	{
		SDL_RenderTexture(back, texture, nullptr, &rcd);
		rcd.x += modulo;
	}
}
