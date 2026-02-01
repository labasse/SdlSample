#include "Renderer.h"

#define VISIBLE_AREA_COMPLETION 2.f

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
	view.x = lookAtWorldX * pixelsPerWorldUnit - view.w * LOOKAT_SCREEN_X;
	view.y = lookAtWorldY * pixelsPerWorldUnit - view.h * LOOKAT_SCREEN_Y;
}

void Renderer::EndRender()
{
	SDL_RenderPresent(back);
}

void Renderer::GetVisibleArea(SDL_Rect& worldRect) const
{
	worldRect.x = (int)(view.x / pixelsPerWorldUnit);
	worldRect.y = (int)(view.y / pixelsPerWorldUnit);
	worldRect.w = (int)(view.w / pixelsPerWorldUnit + VISIBLE_AREA_COMPLETION);
	worldRect.h = (int)(view.h / pixelsPerWorldUnit + VISIBLE_AREA_COMPLETION);
}

void Renderer::RenderAlignedTileRect(float col, float row) const
{
	SDL_FRect dstRect {	
		(float)(int)col, (float)(int)row, 
		pixelsPerWorldUnit, pixelsPerWorldUnit
	};
	SDL_SetRenderDrawColor(back, 0, 255, 0, 255);
	SDL_RenderRect(back, &dstRect);
}

void Renderer::RenderTile(SDL_Texture* texture, SDL_FRect src, float worldX, float worldY) const
{
	SDL_FRect dst{
		worldX * pixelsPerWorldUnit - view.x,
		worldY * pixelsPerWorldUnit - view.y,
		src.w, src.h
	};
	SDL_RenderTexture(back, texture, &src, &dst);
}

void Renderer::RenderTileScaledX(SDL_Texture* texture, SDL_FRect src, float dxFromLookAt, float worldY, float widthX) const
{
	SDL_FRect dst = {
		lookAtWorld.x * pixelsPerWorldUnit - view.x + dxFromLookAt,
		worldY * pixelsPerWorldUnit - view.y,
		widthX, src.h
	};
	SDL_RenderTexture(back, texture, &src, &dst);
}

void Renderer::RenderParallaxLayer(SDL_Texture* texture, float coef) const
{
	SDL_FRect rcd;

	rcd.w = view.w;
	rcd.h = view.h;
	rcd.x = -(float)((int)((view.x * LOOKAT_SCREEN_X + rcd.w) * coef) % (int)rcd.w);
	rcd.y = -view.y * coef;
	for (int i = 0; i < 2; ++i)
	{
		SDL_RenderTexture(back, texture, nullptr, &rcd);
		rcd.x += rcd.w;
	}
}
