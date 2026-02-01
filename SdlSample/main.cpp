#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "Character.h"
#include "KeyboardController.h"
#include "Parallax.h"
#include "PlanarLevel.h"
#include "Renderer.h"
#include "RotativeLevel.h"
#include "ResourceManager.h"
#include "Settings.h"

#define FRAME_DURATION 16
#define PARALLAX_LAYERS 6

const char* ImageFiles[] = {
    "assets/bg0.png",
    "assets/bg1.png",
    "assets/bg2.png",
    "assets/bg3.png",
    "assets/bg4.png",
    "assets/bg5.png",
    "assets/anims.png",
	"assets/level.png",
    // Add new image files above this line
    nullptr
};

enum ImageIndices {
	IMG_PARALLAX= 0,
	IMG_ANIMS   = 6,
	IMG_LEVEL   = 7
};

const float ParallaxCoefs[PARALLAX_LAYERS] = { 0.f, 0.f, 0.15f, 0.25f, 0.4f, 0.6f };

void ThrowSdl(bool is_ok);
void ThrowSdl(void* ptr);

Uint64 Throttle(Uint64 frame_start, Uint64 frame_duration);

bool Update(Uint64 frame_start);

int main(int argc, char *argv[])
{
    SDL_Window   * wnd = nullptr;
    SDL_Renderer * bck = nullptr;
    try
    {
        ResourceManager assets;
        
        ThrowSdl(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS));
        ThrowSdl(wnd = SDL_CreateWindow  ("Sample", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_INPUT_FOCUS));
        ThrowSdl(bck = SDL_CreateRenderer(wnd, nullptr));
		ThrowSdl(assets.LoadTextures(bck, ImageFiles));
        SDL_SetWindowPosition(wnd, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

        KeyboardController controller;
		Renderer renderer(bck, SCREEN_WIDTH, SCREEN_HEIGHT, LEVEL_TILE_SIZE);
        Parallax parallax(assets, IMG_PARALLAX, ParallaxCoefs, PARALLAX_LAYERS, NOREPEAT(1));
        //PlanarLevel level(assets.GetTexture(IMG_LEVEL), parallax);
        //level.Load(PlanarLevel::Level0);
        RotativeLevel level(assets.GetTexture(IMG_LEVEL), parallax);
        level.Load(RotativeLevel::Level1);

        Character character(controller, assets.GetTexture(IMG_ANIMS), level);
		
        for (auto frame_start = SDL_GetTicks(); Update(frame_start); frame_start = Throttle(frame_start, FRAME_DURATION))
        {
			controller.Update(frame_start);
			character .Update(frame_start);

			renderer.BeginRender(character.GetWorldX(), character.GetWorldY());
			{
			    level    .Render(renderer);
			    character.Render(renderer);
            }
            renderer.EndRender();
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
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