#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "Settings.h"
#include "KeyboardController.h"
#include "Character.h"

#define FRAME_DURATION 16
#define MAX_IMAGES      8
#define PARALLAX_LAYERS 6

const char* ImageFiles[MAX_IMAGES] = {
    "assets/bg0.png",
    "assets/bg1.png",
    "assets/bg2.png",
    "assets/bg3.png",
    "assets/bg4.png",
    "assets/bg5.png",
    "assets/anims.png",
	"assets/objs.png"
};

enum ImageIndices {
	IMG_PARALLAX = 0,
	IMG_ANIMS    = 6
};

void ThrowSdl(bool is_ok);
void ThrowSdl(void* ptr);

Uint64 Throttle(Uint64 frame_start, Uint64 frame_duration);

bool Update(Uint64 frame_start);
void ParallaxRender(SDL_Renderer* renderer, SDL_Texture* textures[], int povX, int povY);

int main(int argc, char *argv[])
{
    SDL_Window   * wnd = nullptr;
    SDL_Renderer * bck = nullptr;
	SDL_Texture* textures[MAX_IMAGES] = { nullptr };
    
    try
    {
        ThrowSdl(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS));

        ThrowSdl(wnd = SDL_CreateWindow  ("Sample", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_INPUT_FOCUS));
        ThrowSdl(bck = SDL_CreateRenderer(wnd, nullptr));
        for (int i = 0; i < MAX_IMAGES; ++i)
        {
			SDL_Surface *surface;

            ThrowSdl(surface = SDL_LoadSurface(ImageFiles[i]));
            ThrowSdl(textures[i] = SDL_CreateTextureFromSurface(bck, surface));
			SDL_DestroySurface(surface);
        }
        KeyboardController controller;
        Character character(controller, textures[IMG_ANIMS]);

        SDL_SetWindowPosition(wnd, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

        for (auto frame_start = SDL_GetTicks(); Update(frame_start); frame_start = Throttle(frame_start, FRAME_DURATION))
        {
			controller.Update(frame_start);
			character .Update(frame_start);

			ParallaxRender(bck, textures, character.GetX(), character.GetY());
			character.Render(bck);
            SDL_RenderPresent(bck);
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
	}
    for (auto texture : textures)
    {
        SDL_DestroyTexture(texture);
    }
    SDL_DestroyRenderer(bck);
    SDL_DestroyWindow  (wnd);
    SDL_Quit();
	return 0;
}

void ThrowSdl(bool is_ok)
{
    if (!is_ok)
        throw std::runtime_error(SDL_GetError());
}

void ThrowSdl(void* ptr)
{
    ThrowSdl(ptr != nullptr);
}

Uint64 Throttle(Uint64 frame_start, Uint64 frame_duration)
{
    Sint64 wait = frame_duration + frame_start - SDL_GetTicks();

    if (wait > 0L)
        SDL_Delay((Uint32)wait);
    return SDL_GetTicks();
}

bool Update(Uint64 frame_start)
{
    SDL_Event event;

    SDL_PollEvent(&event);
	return event.type != SDL_EVENT_QUIT;
}

void ParallaxRender(SDL_Renderer* renderer, SDL_Texture* textures[], int povX, int povY)
{
    SDL_FRect rcdParallax = { 0.0f, 0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT };
    int iLayers;

    for (iLayers = 0; iLayers < 2; ++iLayers)
        SDL_RenderTexture(renderer, textures[IMG_PARALLAX + iLayers], nullptr, &rcdParallax);

    for (; iLayers < PARALLAX_LAYERS; ++iLayers)
    {
        rcdParallax.x = -(float)((povX / (1 << (PARALLAX_LAYERS - iLayers - 1))) % SCREEN_WIDTH);
        rcdParallax.y = -(float)(povY / (1 << (PARALLAX_LAYERS - iLayers - 1)));
        for (int j = 0; j < 2; ++j)
        {
            SDL_RenderTexture(renderer, textures[IMG_PARALLAX + iLayers], nullptr, &rcdParallax);
            rcdParallax.x += (float)SCREEN_WIDTH;
        }
    }
}