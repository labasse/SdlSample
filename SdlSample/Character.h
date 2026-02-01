#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include <SDL3/SDL.h>

#include "Renderer.h"
#include "Controller.h"
#include "Level.h"
#include "TileSheet.h"

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
        IDLE_LADDER = FIRST,
        IDLE_LEFT,
        IDLE_RIGHT,
        RUN_LEFT, 
        RUN_RIGHT,
        JUMP_LEFT, 
        JUMP_RIGHT,
        FALL_LEFT, 
        FALL_RIGHT,
        CLIMB_UP,
		CLIMB_DOWN,
		// Add new states above this line
		COUNT
    } state;

    enum class StateTransition {
		IDLE2JUMP = (int)State::JUMP_LEFT - (int)State::IDLE_LEFT,
		RUN2JUMP  = (int)State::JUMP_LEFT - (int)State::RUN_LEFT,
		RUN2FALL  = (int)State::FALL_LEFT - (int)State::RUN_LEFT,
        JUMP2FALL = (int)State::FALL_LEFT - (int)State::JUMP_LEFT,
		FALL2RUN  = (int)State::RUN_LEFT  - (int)State::FALL_LEFT 
    };
	
    inline State FindNewState(StateTransition transition) const {
        return (State)((int)state + (int)transition);
	}
    State FindIdleState() const;
   
    inline bool IsTile(int flags, int dx, int dy = 0) const {
        return level.IsTileAround(world, dx, dy, flags);
    }
    inline bool IsTile(const SDL_Point& dir, int flags) const { return IsTile(flags, dir.x, dir.y); }
    inline bool IsBottomTile(int flags) const { return IsTile(Level::SOUTH, flags); }

    SDL_FPoint UpdatePos(Uint64 time, const SDL_Point& dir, bool* xCenterPassed, bool* yCenterPassed) const;
    
	State UpdateRun  (Uint64 time, const SDL_Point& dir, bool keepRunning);
    State UpdateJump (Uint64 time, const SDL_Point& dir, bool grip);
    State UpdateFall (Uint64 time, const SDL_Point& dir);
    State UpdateClimb(Uint64 time, const SDL_Point& dir, const SDL_Point& nextLadder, bool keepClimbing);

	bool colAligned, rowAligned;
    SDL_FPoint world;
    SDL_FPoint speed;
    Uint64 stateStartTime, updateTime;
	const Controller& ctrl;
	const Level& level;
	TileSheet anims;
};

#endif // _CHARACTER_H_
