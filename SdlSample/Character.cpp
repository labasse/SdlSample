#include <stdio.h>
#include <algorithm>
#include "Settings.h"

#include "Character.h"

#define ANIM_WIDTH   128.f
#define ANIM_HEIGHT  128.f

#define WORLD_OFFSETX 1.f
#define WORLD_OFFSETY .62f

#define TILE_CENTER .5f

#define RUNNING_SPEED 0.011f
#define JUMPING_SPEED 0.009f

#define JUMP_DURATION 250

const struct 
{
	int GetFrame(Uint64 deltaTime) const 
	{
		int frame = (int)deltaTime / frameDelay;
		return loop ? frame % frameCount : std::min(frame, frameCount - 1);
	}

	int row, frameDelay, frameCount	; bool loop;
} Anims[] = {
	{	23	, 300		, 2			, true	},	// IDLE_LEFT
	{	25	, 300		, 2			, true	},	// IDLE_RIGHT
	{	 9	, 100		, 9			, true	},	// WALK_LEFT
	{	11	, 100		, 9			, true	},	// WALK_RIGHT
	{	39	, 70		, 8			, true	},	// RUN_LEFT
	{	41	, 70		, 8			, true	},	// RUN_RIGHT
	{	27	, 70		, 2			, false	},	// JUMP_LEFT
	{	29	, 70		, 2			, false	},	// JUMP_RIGHT
	{	31	, 100		, 2			, false	},	// FALL_LEFT
	{	33	, 100		, 2			, false }	// FALL_RIGHT
};

Character::Character(const Controller& controller, SDL_Texture *anims, const Level& level) : 
	colAligned(true),
	rowAligned(true),
	speed({ .0f, .0f }),
	state(State::IDLE_RIGHT),
	stateStartTime(0),
	ctrl(controller), 
	level(level),
	anims(anims),
	updateTime(SDL_GetTicks())
{
	size_t startCol, startRow;
	level.GetStart(startCol, startRow);
	world.x = Align((float)startCol);
	world.y = Align((float)startRow);
}

void Character::Update(Uint64 time)
{
	State newState = state;
	
	switch(state) {
	case State::IDLE_LEFT: 
	case State::IDLE_RIGHT:
		speed.x = speed.y = .0f;
		if (ctrl.IsLeftPressed()) {
			newState = State::RUN_LEFT; 
			speed.x = RUNNING_SPEED;
		}
		else if (ctrl.IsRightPressed()) {
			newState = State::RUN_RIGHT;
			speed.x = RUNNING_SPEED;
		}
		else if (ctrl.IsJumpPressed()) {
			speed.y = JUMPING_SPEED;
			newState = FindNewState(StateTransition::IDLE2JUMP);
		}
		break;
	case State::RUN_LEFT  : newState = UpdateRun(time, WEST, ctrl.IsLeftPressed() ); break;
	case State::RUN_RIGHT : newState = UpdateRun(time, EAST, ctrl.IsRightPressed()); break;
	case State::JUMP_LEFT : newState = UpdateJump(time, NORTHWEST); break;
	case State::JUMP_RIGHT: newState = UpdateJump(time, NORTHEAST); break;
	case State::FALL_LEFT : newState = UpdateFall(time, SOUTHWEST); break;
	case State::FALL_RIGHT: newState = UpdateFall(time, SOUTHEAST); break;
	}
	if (state != newState)
	{
		state = newState;
		stateStartTime = time;
	}
	updateTime = time;
}

inline static float DistToHalf(float val)
{
	return std::abs(val - (int)val - .5f);
}

inline static void FillCenterPassed(bool* centerPassed, float worldPos, float newPos)
{
	centerPassed && (*centerPassed = DistToHalf(worldPos) <= DistToHalf(newPos));
}

SDL_FPoint Character::UpdatePos(Uint64 time, const SDL_Point& dir, bool* xCenterPassed, bool* yCenterPassed) const
{
	auto delta = (float)(time - updateTime);
	SDL_FPoint newPos = {
		world.x + (float)dir.x * speed.x * delta,
		world.y + (float)dir.y * speed.y * delta
	};
	FillCenterPassed(xCenterPassed, world.x, newPos.x);
	FillCenterPassed(yCenterPassed, world.y, newPos.y);
	return newPos;
}

Character::State Character::UpdateJump(Uint64 time, const SDL_Point& vec)
{
	bool xCenterPassed;
	auto newPos = UpdatePos(time, vec, &xCenterPassed, nullptr);

	if (xCenterPassed && speed.x && GetNextTile(vec) == Level::TileType::SOLID)
	{
		world.x = Align(world.x);
		speed.x = 0;
		return FindNewState(StateTransition::JUMP2FALL);
	}
	world = newPos;
	return (time - stateStartTime < JUMP_DURATION)
		? state 
		: FindNewState(StateTransition::JUMP2FALL);
}

Character::State Character::UpdateRun(Uint64 time, const SDL_Point& vec, bool keepRunning)
{
	bool xCenterPassed;
	auto newPos = UpdatePos(time, vec, &xCenterPassed, nullptr);

	if( xCenterPassed )
	{ 
		if (GetBottomTile() == Level::TileType::EMPTY)
		{
			speed.y = JUMPING_SPEED;
			return FindNewState(StateTransition::RUN2FALL);
		}
		if(!keepRunning || GetNextTile(vec.x) == Level::TileType::SOLID)
		{ 
			world.x = Align(world.x);
			speed.x = 0.f;
			return FindNewState(StateTransition::RUN2IDLE);
		}
	}
	world = newPos;
	if (ctrl.IsJumpPressed())
	{
		speed.y = JUMPING_SPEED;
		return FindNewState(StateTransition::RUN2JUMP);
	}
	return state;
}

Character::State Character::UpdateFall(Uint64 time, const SDL_Point& vec)
{
	bool xCenterPassed, yCenterPassed;
	auto newPos = UpdatePos(time, vec, &xCenterPassed, &yCenterPassed);

	if (yCenterPassed && GetBottomTile() == Level::TileType::SOLID)
	{
		StateTransition stateTransition = StateTransition::FALL2RUN;

		world.y = Align(world.y);
		speed.y = 0.f;
		if (xCenterPassed)
		{
			world.x = Align(world.x);
			stateTransition = StateTransition::FALL2IDLE;
		}
		return FindNewState(stateTransition);
	}
	world = newPos;
	if (xCenterPassed && speed.x && GetNextTile(vec.x) == Level::TileType::SOLID)
	{
		world.x = Align(world.x);
		speed.x = 0.f;
	}
	return state;
}

void Character::Render(const Renderer &renderer)
{
	ASSERT(State::FIRST <= state && state < State::COUNT);
	const auto& anim = Anims[(int)state];
	SDL_FRect rcsAnim = {
		ANIM_WIDTH  * anim.GetFrame(updateTime - stateStartTime),
		ANIM_HEIGHT * anim.row,
		ANIM_WIDTH, ANIM_HEIGHT
	};
	renderer.Render(
		anims, rcsAnim, 
		GetWorldX() - WORLD_OFFSETX, 
		GetWorldY() - WORLD_OFFSETY
	);
#if DEBUG_CHARACTER_TILE
	renderer.RenderAlignedTileRect(Align(world.x), Align(world.y));
#endif
}
