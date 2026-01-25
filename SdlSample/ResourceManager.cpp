#include <stdexcept>
#include "ResourceManager.h"

ResourceManager::ResourceManager()
{ }

ResourceManager::ResourceManager(const ResourceManager& other)
{
	throw std::runtime_error("ResourceManager copy constructor is not supported.");
}

ResourceManager::~ResourceManager()
{
	for (auto texture : textures) {
		SDL_DestroyTexture(texture);
	}
}

bool ResourceManager::LoadTextures(SDL_Renderer* renderer, const char* filenames[])
{
	for (const char** p = filenames; *p != nullptr; ++p) {
		SDL_Surface* surface = SDL_LoadSurface(*p);
		if (surface == nullptr) {
			return false;
		}
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_DestroySurface(surface);
		if (texture == nullptr) {
			return false;
		}
		textures.push_back(texture);
	}
	return true;
}
