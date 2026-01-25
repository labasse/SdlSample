#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_

#include <SDL3/SDL.h>
#include <vector>

class ResourceManager
{
public:
	ResourceManager();
	ResourceManager(const ResourceManager& other);
	~ResourceManager();

	bool LoadTextures(SDL_Renderer* renderer, const char *filenames[]);
	
	inline SDL_Texture* GetTexture(int index) const { return textures[index]; }
private:
	std::vector<SDL_Texture*> textures;
};

#endif // _RESOURCE_MANAGER_H_