#include "Renderer.h"

#define VISIBLE_AREA_COMPLETION 2.f

Renderer::Renderer(SDL_Renderer* back, int viewWidth, int viewHeight, int pixelsPerWorldUnit) : 
	back(back),
	lookAtWorld({ 0.f, 0.f }),
	view({ 0.0f, 0.0f, (float)viewWidth, (float)viewHeight }),
	pixelsPerWorldUnit((float)pixelsPerWorldUnit)
{
    
}

void Renderer::BeginRender(float lookAtWorldX, float lookAtWorldY)
{
	lookAtWorld.x = lookAtWorldX;
	lookAtWorld.y = lookAtWorldY;
	view.x = lookAtWorldX * GetPixelsPerWorldUnit() - view.w * LOOKAT_SCREEN_X;
	view.y = lookAtWorldY * GetPixelsPerWorldUnit() - view.h * LOOKAT_SCREEN_Y;
}

void Renderer::EndRender()
{
	SDL_RenderPresent(back);
}

void Renderer::GetVisibleArea(SDL_Rect& worldRect) const
{
	worldRect.x = (int)(view.x / GetPixelsPerWorldUnit());
	worldRect.y = (int)(view.y / GetPixelsPerWorldUnit());
	worldRect.w = (int)(view.w / GetPixelsPerWorldUnit() + VISIBLE_AREA_COMPLETION);
	worldRect.h = (int)(view.h / GetPixelsPerWorldUnit() + VISIBLE_AREA_COMPLETION);
}

void Renderer::RenderAlignedTileRect(float col, float row) const
{
	SDL_FRect dstRect = GetTileRect(
		(float)(int)col, (float)(int)row, 
		pixelsPerWorldUnit, pixelsPerWorldUnit,
		nullptr
	);
	SDL_SetRenderDrawColor(back, 0, 255, 0, 255);
	SDL_RenderRect(back, &dstRect);
}

void Renderer::RenderTile(SDL_Texture* texture, const SDL_FRect& srcRect, float worldX, float worldY, const SDL_FPoint* offset) const
{
	SDL_FRect dstRect = GetTileRect(worldX, worldY, srcRect.w, srcRect.h, offset);

	SDL_RenderTexture(back, texture, &srcRect, &dstRect);
}

SDL_FRect Renderer::GetTileRect(float worldX, float worldY, float pixelWidth, float pixelHeight, const SDL_FPoint* offset) const
{
	if (offset != nullptr)
	{
		worldX += offset->x;
		worldY += offset->y;
	}
	return SDL_FRect{
		worldX * GetPixelsPerWorldUnit() - GetView().x,
		worldY * GetPixelsPerWorldUnit() - GetView().y,
		pixelWidth, pixelHeight
	};
}

void Renderer::RenderParallaxLayer(SDL_Texture* texture, float coef) const
{
	SDL_FRect rcd;

	rcd.w = GetView().w;
	rcd.h = GetView().h;
	rcd.x = -(float)((int)((GetView().x * LOOKAT_SCREEN_X + rcd.w) * coef) % (int)rcd.w);
	rcd.y = -GetView().y * coef;
	for (int i = 0; i < 2; ++i)
	{
		SDL_RenderTexture(GetRenderer(), texture, nullptr, &rcd);
		rcd.x += rcd.w;
	}
}
