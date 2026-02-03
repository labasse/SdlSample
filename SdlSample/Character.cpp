#include <stdio.h>
#include <algorithm>
#include "Settings.h"

#include "Character.h"

#define ANIM_SIZE	128
#define ANIM_COLUMNS 13

#define WORLD_OFFSETX 1.f
#define WORLD_OFFSETY 1.38f

#define TILE_CENTER .5f

#define RUNNING_SPEED		0.011f
#define CLIMBING_SPEED		0.005f
#define JUMPING_SPEEDY		0.009f
#define JUMPING_SPEEDX_GRIP 0.004f

#define UP     0
#define LEFT   1
#define RIGHT  3

#define ROW_WALK	8
#define ROW_CLIMB	21
#define ROW_IDLE	22
#define ROW_JUMP	26
#define ROW_FALL	30
#define ROW_RUN		38

#define DELAY_IDLE		300
#define DELAY_RUNJUMP    70
#define DELAY_FALLCLIMB 100

#define JUMP_DURATION 250

TileSheet::Anim Anims[] = {				// frameCount
	{	ROW_IDLE + UP	, DELAY_IDLE		, 2	},			// IDLE_LADDER
	{	ROW_IDLE + LEFT	, DELAY_IDLE		, 2	},			// IDLE_LEFT
	{	ROW_IDLE + RIGHT, DELAY_IDLE		, 2	},			// IDLE_RIGHT
	{	ROW_RUN  + LEFT	, DELAY_RUNJUMP		, 8	},			// RUN_LEFT
	{	ROW_RUN  + RIGHT, DELAY_RUNJUMP		, 8	},			// RUN_RIGHT
	{	ROW_JUMP + LEFT	, DELAY_RUNJUMP		, 2	, false	},	// JUMP_LEFT
	{	ROW_JUMP + RIGHT, DELAY_RUNJUMP		, 2	, false	},	// JUMP_RIGHT
	{	ROW_FALL + LEFT	, DELAY_FALLCLIMB	, 2	, false	},	// FALL_LEFT
	{	ROW_FALL + RIGHT, DELAY_FALLCLIMB	, 2	, false },	// FALL_RIGHT
	{ 	ROW_CLIMB		, DELAY_FALLCLIMB	, 6	},			// CLIMB_UP
	{   ROW_CLIMB		, DELAY_FALLCLIMB	, 6	}			// CLIMB_DOWN
};

Character::Character(const Controller& controller, SDL_Texture *anims, const Level& level) : 
	colAligned(true),
	rowAligned(true),
	speed({ .0f, .0f }),
	state(State::IDLE_RIGHT),
	stateStartTime(0),
	ctrl(controller), 
	level(level),
	anims(anims, ANIM_SIZE, ANIM_COLUMNS),
	updateTime(SDL_GetTicks())
{
	size_t startCol, startRow;
	level.GetStart(startCol, startRow);
	world.x = Renderer::Align(startCol);
	world.y = Renderer::Align(startRow);
}

void Character::Update(Uint64 time)
{
	State newState = state;
	
	switch(state) {
	case State::IDLE_LEFT: 
	case State::IDLE_RIGHT:
	case State::IDLE_LADDER:
		speed.x = speed.y = .0f;
		if (ctrl.IsLeftPressed()) {
			newState = State::RUN_LEFT; 
			speed.x = RUNNING_SPEED;
		}
		else if (ctrl.IsRightPressed()) {
			newState = State::RUN_RIGHT;
			speed.x = RUNNING_SPEED;
		}
		else if (IsTile(Level::HERE, TILEFLAG_CLIMBABLE) && ctrl.IsJumpPressed())
		{
			speed.y = CLIMBING_SPEED;
			newState = State::CLIMB_UP;
		}
		else if (IsBottomTile(TILEFLAG_CLIMBABLE) && ctrl.IsGoDownPressed()) {
			speed.y = CLIMBING_SPEED;
			newState = State::CLIMB_DOWN;
		}
		else if (ctrl.IsJumpPressed()) {
			speed.y = JUMPING_SPEEDY;
			newState = FindNewState(StateTransition::IDLE2JUMP);
		}
		break;
	case State::RUN_LEFT  : newState = UpdateRun(time, Level::WEST, ctrl.IsLeftPressed ()); break;
	case State::RUN_RIGHT : newState = UpdateRun(time, Level::EAST, ctrl.IsRightPressed()); break;
	case State::JUMP_LEFT : newState = UpdateJump(time, Level::NORTHWEST, ctrl.IsLeftPressed ()); break;
	case State::JUMP_RIGHT: newState = UpdateJump(time, Level::NORTHEAST, ctrl.IsRightPressed()); break;
	case State::FALL_LEFT : newState = UpdateFall(time, Level::SOUTHWEST); break;
	case State::FALL_RIGHT: newState = UpdateFall(time, Level::SOUTHEAST); break;
	case State::CLIMB_UP  : newState = UpdateClimb(time, Level::NORTH, Level::HERE, ctrl.IsJumpPressed()); break;
	case State::CLIMB_DOWN: newState = UpdateClimb(time, Level::SOUTH, Level::SOUTH, ctrl.IsGoDownPressed()); break;
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
	if(centerPassed != nullptr)
		*centerPassed = DistToHalf(worldPos) <= DistToHalf(newPos);
}

Character::State Character::FindIdleState() const
{
	if (IsBottomTile(TILEFLAG_CLIMBABLE) || IsTile(Level::HERE, TILEFLAG_CLIMBABLE)) {
		return State::IDLE_LADDER;
	}
	return ((int)state)%2 == 1 ? State::IDLE_LEFT : State::IDLE_RIGHT;
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
	level.Normalize(newPos.x, newPos.y);
	return newPos;
}

Character::State Character::UpdateJump(Uint64 time, const SDL_Point& vec, bool grip)
{
	bool xCenterPassed, yCenterPassed;
	auto newPos = UpdatePos(time, vec, &xCenterPassed, &yCenterPassed);

	if (yCenterPassed && IsTile(TILEFLAG_SOLID, 0, vec.y))
	{
		world.y = Renderer::Align(world.y);
		return FindNewState(StateTransition::JUMP2FALL);
	}
	else if (xCenterPassed && IsTile(Level::HERE, TILEFLAG_CLIMBABLE))
	{
		world.x = Renderer::Align(world.x);
		speed.x = 0.f;
		return State::IDLE_LADDER;
	}
	else if (xCenterPassed && IsTile(TILEFLAG_SOLID, vec.x))
	{
		world.x = Renderer::Align(world.x);
		speed.x = 0;
	}
	else if (!speed.x && grip && !IsTile(TILEFLAG_SOLID, vec.x))
	{
		world.x = Renderer::Align(world.x);
		speed.x = JUMPING_SPEEDX_GRIP;
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

	if( ctrl.IsJumpPressed() )
	{
		speed.y = JUMPING_SPEEDY;
		return FindNewState(StateTransition::RUN2JUMP);
	}
	if( xCenterPassed )
	{ 
		if (!IsBottomTile(TILEFLAG_CAN_PASSON))
		{
			speed.y = JUMPING_SPEEDY;
			return FindNewState(StateTransition::RUN2FALL);
		}
		if(!keepRunning || IsTile(TILEFLAG_CANT_PASSTHRU, vec.x))
		{ 
			world.x = Renderer::Align(world.x);
			speed.x = 0.f;
			return FindIdleState();
		}
	}
	level.Normalize(newPos.x, newPos.y);
	world = newPos;
	return state;
}

Character::State Character::UpdateFall(Uint64 time, const SDL_Point& vec)
{
	bool xCenterPassed, yCenterPassed;
	auto newPos = UpdatePos(time, vec, &xCenterPassed, &yCenterPassed);

	if (yCenterPassed && IsBottomTile(TILEFLAG_CAN_PASSON))
	{
		world.y = Renderer::Align(world.y);
		speed.x = RUNNING_SPEED;
		speed.y = 0.f;
		if (xCenterPassed)
		{
			world.x = Renderer::Align(world.x);
			return FindIdleState();
		}
		else
			return FindNewState(StateTransition::FALL2RUN);
	}
	level.Normalize(newPos.x, newPos.y);
	world = newPos;
	if (xCenterPassed && speed.x && IsTile(TILEFLAG_SOLID, vec.x))
	{
		world.x = Renderer::Align(world.x);
		speed.x = 0.f;
	}
	return state;
}

Character::State Character::UpdateClimb(Uint64 time, const SDL_Point& dir, const SDL_Point& nextLadder, bool keepClimbing)
{
	bool yCenterPassed;
	auto newPos = UpdatePos(time, dir, nullptr, &yCenterPassed);

	if (yCenterPassed && (!IsTile(nextLadder, TILEFLAG_CLIMBABLE) || !keepClimbing))
	{
		world.y = Renderer::Align(world.y);
		return State::IDLE_LADDER;
	}
	world = newPos;
	return state;
}

void Character::Render(const Renderer &renderer)
{
	ASSERT(State::FIRST <= state && state < State::COUNT);
	auto srcAnim = anims.FromAnim(Anims[(int)state], updateTime - stateStartTime);

	renderer.RenderTile(
		anims.GetTexture(), srcAnim, 
		GetWorldX() - WORLD_OFFSETX, 
		GetWorldY() - WORLD_OFFSETY
	);
#if DEBUG_CHARACTER_TILE
	renderer.RenderAlignedTileRect(world.x, world.y);
#endif
}
