#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include <SDL3/SDL.h>

#include "Renderer.h"
#include "Controller.h"
#include "Level.h"

// The main character in the game.
class Character {
public:
    // Constructs a Character with the given controller and animation texture.
    // @param controller The controller to handle character input.
	// @param anims The texture containing character animations.
    Character(const Controller& controller, SDL_Texture *anims, const Level& level);

	// Updates the character's state based on the elapsed time.
	// @param time The current SDL time in milliseconds.
    void Update(Uint64 time);

	// Renders the character using the given SDL renderer.
	// @param renderer The SDL renderer to use for rendering.
	void Render(const Renderer& renderer);

	// Gets the current X position of the character in grid world coordinates.
    inline float GetWorldX() const { return world.x; }

    // Gets the current X position of the character in grid world coordinates.
	inline float GetWorldY() const { return world.y; }

private:
    enum class State {
        FIRST,
        IDLE_LEFT = FIRST, 
        IDLE_RIGHT,
		WALK_LEFT, 
        WALK_RIGHT,
        RUN_LEFT, 
        RUN_RIGHT,
        JUMP_LEFT, 
        JUMP_RIGHT,
        FALL_LEFT, 
        FALL_RIGHT,
		// Add new states above this line
		COUNT
    } state;

    enum class StateTransition {
		IDLE2JUMP = (int)State::JUMP_LEFT - (int)State::IDLE_LEFT,
		RUN2JUMP  = (int)State::JUMP_LEFT - (int)State::RUN_LEFT,
		RUN2FALL  = (int)State::FALL_LEFT - (int)State::RUN_LEFT,
        RUN2IDLE  = (int)State::IDLE_LEFT - (int)State::RUN_LEFT,
        JUMP2FALL = (int)State::FALL_LEFT - (int)State::JUMP_LEFT,
        FALL2IDLE = (int)State::IDLE_LEFT - (int)State::FALL_LEFT,
		FALL2RUN  = (int)State::RUN_LEFT  - (int)State::FALL_LEFT 
    };
	
    inline State FindNewState(StateTransition transition) const {
        return (State)((int)state + (int)transition);
	}

    const SDL_Point NORTH = {  0, -1 };
    const SDL_Point WEST  = { -1,  0 };
    const SDL_Point SOUTH = {  0,  1 };
    const SDL_Point EAST  = {  1,  0 };
    const SDL_Point NORTHWEST = { -1,  1 };
    const SDL_Point NORTHEAST = {  1,  1 };
    const SDL_Point SOUTHWEST = { -1, -1 };
    const SDL_Point SOUTHEAST = {  1, -1 };

    inline static float Align(float val)  { return (float)((int)val + 0.5f); }
    
    inline Level::TileType GetNextTile(int dx, int dy = 0) const {
        return level.GetTile(dx + (int)world.x, dy + (int)world.y);
    }
    inline Level::TileType GetNextTile  (const SDL_Point& dir) const { return GetNextTile(dir.x, dir.y); }
    inline Level::TileType GetBottomTile()                     const { return GetNextTile(0, -1);}

    SDL_FPoint UpdatePos(Uint64 time, const SDL_Point& dir, bool* xCenterPassed, bool* yCenterPassed) const;
    
	State UpdateRun(Uint64 time, const SDL_Point& dir, bool keepRunning);
    State UpdateJump(Uint64 time, const SDL_Point& dir);
    State UpdateFall(Uint64 time, const SDL_Point& dir);
    
	bool colAligned, rowAligned;
    SDL_FPoint world;
    SDL_FPoint speed;
    Uint64 stateStartTime, updateTime;
	const Controller& ctrl;
	const Level& level;
	SDL_Texture* anims;
};

#endif // _CHARACTER_H_
