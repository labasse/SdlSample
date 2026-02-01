#include "Renderer.h"
#include "Settings.h"

#define VISIBLE_AREA_COMPLETION 2.f
#define PARALLAX_MAX_Y 10.f

Renderer::Renderer(SDL_Renderer* back, size_t viewWidth, size_t viewHeight, size_t pixelsPerWorldUnit) :
	back(back),
	lookAtWorld({ 0.f, 0.f }),
	width (static_cast<float>(viewWidth)), 
	height(static_cast<float>(viewHeight)),
	pixelsPerWorldUnit(static_cast<float>(pixelsPerWorldUnit))
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
	worldRect.x = static_cast<int>(lookAtWorld.x - width  * LOOKAT_SCREEN_X / pixelsPerWorldUnit);
	worldRect.y = static_cast<int>(lookAtWorld.y - height * LOOKAT_SCREEN_Y / pixelsPerWorldUnit);
	worldRect.w = static_cast<int>(width  / pixelsPerWorldUnit + VISIBLE_AREA_COMPLETION);
	worldRect.h = static_cast<int>(height / pixelsPerWorldUnit + VISIBLE_AREA_COMPLETION);
}

void Renderer::RenderAlignedTileRect(float col, float row) const
{
	SDL_FRect dstRect {	
		((float)(int)col - lookAtWorld.x) * pixelsPerWorldUnit + width  * LOOKAT_SCREEN_X, 
		((float)(int)row - lookAtWorld.y) * pixelsPerWorldUnit + height * LOOKAT_SCREEN_Y,
		pixelsPerWorldUnit, pixelsPerWorldUnit
	};
	SDL_SetRenderDrawColor(back, 0, 255, 0, 255);
	SDL_RenderRect(back, &dstRect);
}

void Renderer::RenderTile(SDL_Texture* texture, SDL_FRect src, float worldX, float worldY) const
{
	SDL_FRect dst{
		(worldX - lookAtWorld.x) * pixelsPerWorldUnit + width  * LOOKAT_SCREEN_X,
		(worldY - lookAtWorld.y) * pixelsPerWorldUnit + height * LOOKAT_SCREEN_Y,
		src.w, src.h
	};
	SDL_RenderTexture(back, texture, &src, &dst);
}

void Renderer::RenderTileScaledX(SDL_Texture* texture, SDL_FRect src, float dxFromLookAtMin, float dxFromLookAtMax, float worldY) const
{
	SDL_FRect dst = {
		width * LOOKAT_SCREEN_X + dxFromLookAtMin,
		(worldY - lookAtWorld.y) * pixelsPerWorldUnit + height * LOOKAT_SCREEN_Y,
		dxFromLookAtMax - dxFromLookAtMin, src.h
	};
	SDL_RenderTexture(back, texture, &src, &dst);
}

void Renderer::RenderParallaxLayer(SDL_Texture* texture, float centerWorldX, float angleRatio, float layerCoef, float layerCirc, bool repeatX) const
{
	ASSERT(centerWorldX >= 0);
	ASSERT(layerCoef >= 0);
	ASSERT(angleRatio > 0);
	size_t w = (size_t)width;
	float pixelsCoef = pixelsPerWorldUnit * layerCoef;
	size_t planarShift = static_cast<size_t>(centerWorldX * pixelsCoef);
	size_t rotateShift = static_cast<size_t>(angleRatio * layerCirc * width);
	size_t modulo = repeatX ? w : static_cast<size_t>(layerCirc * width);
	SDL_FRect rcd {
		- static_cast<float>((planarShift + rotateShift) % modulo),
		(PARALLAX_MAX_Y - lookAtWorld.y) * pixelsCoef,
		width, height
	};
	int repeat = repeatX || rotateShift >= (layerCirc-1) ? 2 : 1;

	for (int i = 0; i < repeat; ++i)
	{
		SDL_RenderTexture(back, texture, nullptr, &rcd);
		rcd.x += modulo;
	}
}
