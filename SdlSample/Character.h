#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include <SDL3/SDL.h>

#include "Controller.h"

// The main character in the game.
class Character {
public:
    // Constructs a Character with the given controller and animation texture.
    // @param controller The controller to handle character input.
	// @param anims The texture containing character animations.
    Character(const Controller& controller, SDL_Texture *anims);

	// Updates the character's state based on the elapsed time.
	// @param time The current SDL time in milliseconds.
    void Update(Uint64 time);

	// Renders the character using the given SDL renderer.
	// @param renderer The SDL renderer to use for rendering.
	void Render(SDL_Renderer* renderer);

	// Gets the current X position of the character.
    inline int GetX() const;

private:
    enum State {
        CHSTATE_IDLE,
        CHSTATE_WALKING,
        CHSTATE_JUMPING,
        CHSTATE_RUNNING
    } state;

    enum Dir {
        CHDIR_NORTH, 
        CHDIR_WEST, 
        CHDIR_SOUTH, 
        CHDIR_EAST
    } dir;

	inline int GetAnimFrame(int delay, int frame_count) const { return ((int)(updateTime - animStartTime) / delay) % frame_count; }
    int GetJumpFrame() const;

    int x, y;
    float speed;
    Uint64 animStartTime, updateTime;
	const Controller& ctrl;
	SDL_Texture* anims;
};

int Character::GetX() const {
    return x;
}

#endif // _CHARACTER_H_
