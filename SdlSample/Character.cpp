#include <stdio.h>
#include "Settings.h"

#include "Character.h"

#define ANIM_WIDTH   128.f
#define ANIM_HEIGHT  128.f

#define JUMPFALL_DURATION 400

const struct 
{
	int GetFrame(Uint64 deltaTime) const 
	{
		int frame = (int)deltaTime / frameDelay;
		return loop ? frame % frameCount : std::min(frame, frameCount - 1);
	}

	int row, frameDelay, frameCount	; bool loop;
} Anims[] = {
	{	22	, 300		, 2			, true	},	// IDLE
	{	8	, 100		, 9			, true	},	// WALK
	{	38	, 70		, 8			, true	},	// RUN
	{	26	, 70		, 2			, false	},	// JUMP
	{	30	, 100		, 2			, false	}	// FALL
};

Character::Character(const Controller& controller, SDL_Texture *anims) : 
	state(State::IDLE), 
	dir(Dir::SOUTH), 
	x(1000), y(0), speed(0), 
	stateStartTime(0),
	ctrl(controller), anims(anims),
	updateTime(SDL_GetTicks())
{
}

void Character::Update(Uint64 time)
{
	State newState = state;	

	switch(state) {
		case State::IDLE:
			if (ctrl.IsDirectionPressed())
			{
				newState = State::RUNNING;
				speed = ctrl.IsLeftPressed() ? -.2f : .2f;
			}
			break;
		case State::RUNNING:
		case State::WALKING:
			if (ctrl.IsJumpPressed())
			{
				newState = State::JUMPING;
			}
			else if (DirectionwiseTest(!ctrl.IsLeftPressed(), !ctrl.IsRightPressed()))
			{
				newState = State::IDLE;
			}
			x += (int)(speed * (float)(time - updateTime));
			break;
		case State::JUMPING: 
			newState = UpdateJumpFall(time, State::FALLING, Dir::NORTH);
			break;
		case State::FALLING: {
			newState = UpdateJumpFall(time, State::IDLE   , Dir::SOUTH);
			break;
		}
	}
	if (state != newState)
	{
		state = newState;
		stateStartTime = time;
	}
	updateTime = time;
}

Character::State Character::UpdateJumpFall(Uint64 time, Character::State nextState, Character::Dir yDir)
{
	int delta = (int)(speed * (float)(time - updateTime));

	x += delta;
	y += DirectionwiseTest(yDir== Dir::SOUTH, yDir==Dir::NORTH) ? -delta : delta;
	return (time - stateStartTime) > JUMPFALL_DURATION ? nextState : state;
}

void Character::Render(SDL_Renderer* renderer)
{
	ASSERT(State::IDLE <= state && state < State::COUNT);
	const auto& anim = Anims[(int)state];
	static const SDL_FRect rcdAnim = {
		(SCREEN_WIDTH  - ANIM_WIDTH ) / 2, 
		(SCREEN_HEIGHT - ANIM_HEIGHT) / 2, 
		ANIM_WIDTH, 
		ANIM_HEIGHT 
	};
	SDL_FRect rcsAnim = rcdAnim;

	rcsAnim.x = ANIM_WIDTH  * anim.GetFrame(updateTime - stateStartTime);
	rcsAnim.y = ANIM_HEIGHT * (anim.row + (int)(speed < 0 ? Dir::WEST : Dir::EAST));

	SDL_RenderTexture(renderer, anims, &rcsAnim, &rcdAnim);
}
