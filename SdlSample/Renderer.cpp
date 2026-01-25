#include "Renderer.h"

Renderer::Renderer(SDL_Renderer* back, int viewWidth, int viewHeight, int pixelsPerWorldUnit) : 
	back(back),
	view({ 0.0f, 0.0f, (float)viewWidth, (float)viewHeight }),
	pixelsPerWorldUnit((float)pixelsPerWorldUnit)
{
    
}

void Renderer::BeginRender(float lookAtX, float lookAtY)
{
	view.x = lookAtX * pixelsPerWorldUnit - view.w / 2.f;
	view.y = lookAtY * pixelsPerWorldUnit + view.h / 2.f;
}

void Renderer::EndRender()
{
	SDL_RenderPresent(back);
}
void Renderer::RenderAlignedTileRect(float col, float row) const
{
	SDL_FRect dstRect = {
		(float)(int)col * pixelsPerWorldUnit - view.x,
		view.y - ((float)(int)row + 1.f) * pixelsPerWorldUnit,
		pixelsPerWorldUnit,
		pixelsPerWorldUnit
	};
	SDL_SetRenderDrawColor(back, 0, 255, 0, 255);
	SDL_RenderRect(back, &dstRect);
}

void Renderer::Render(SDL_Texture* texture, const SDL_FRect& srcRect, float worldX, float worldY) const
{
	SDL_FRect dstRect = {
		worldX * pixelsPerWorldUnit - view.x,
		view.y - worldY * pixelsPerWorldUnit - srcRect.h,
		srcRect.w,
		srcRect.h
	};
	SDL_RenderTexture(back, texture, &srcRect, &dstRect);
}

void Renderer::RenderLayer(SDL_Texture* texture, float coef) const
{
    SDL_FRect rcdParallax = view;

    rcdParallax.x = -(float)((int)((view.x + view.w) * coef) % (int)(view.w));
    rcdParallax.y = (view.y - view.h / 2) * coef;
    for (int i = 0; i < 2; ++i)
    {
        SDL_RenderTexture(back, texture, nullptr, &rcdParallax);
        rcdParallax.x += view.w;
    }
}
