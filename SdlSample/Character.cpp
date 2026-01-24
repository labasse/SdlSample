#include <stdio.h>
#include "Settings.h"

#include "Character.h"

#define ANIM_WIDTH   128.f
#define ANIM_HEIGHT  128.f

#define ANIM_FRAMES_IDLE 2
#define ANIM_FRAMES_WALK 9
#define ANIM_FRAMES_JUMP 2
#define ANIM_FRAMES_FALL 2
#define ANIM_FRAMES_RUN  8

#define ANIM_DELAY_IDLE 300
#define ANIM_DELAY_WALK 100
#define ANIM_DELAY_RUN  70
#define ANIM_DELAY_JUMP 70
#define ANIM_DELAY_FALL	100

#define DURATION_JUMP 400
#define DURATION_FALL 400

enum AnimRow {
	SPELLCAST = 0,
	THRUST = 4,
	WALK = 8,
	SLASH = 12,
	SHOOT = 16,
	HURT = 20,
	CLIMB = 21,
	IDLE = 22,
	JUMP = 26,
	FALL = 30,
	EMOTE = 34,
	RUN = 38,
	WATERING = 42
};


Character::Character(const Controller& controller, SDL_Texture *anims) : 
	state(CHSTATE_IDLE), 
	dir(CHDIR_SOUTH), 
	x(1000), y(0), speed(0), 
	animStartTime(0),
	ctrl(controller), anims(anims),
	updateTime(SDL_GetTicks())
{
}

void Character::Update(Uint64 time)
{
	State newState = state;	

	switch(state) {
		case CHSTATE_IDLE:
			if (ctrl.IsDirectionPressed())
			{
				newState = CHSTATE_RUNNING;
				speed = ctrl.IsLeftPressed() ? -.2f : .2f;
			}
			break;
		case CHSTATE_RUNNING:
		case CHSTATE_WALKING:
			if (ctrl.IsJumpPressed())
			{
				newState = CHSTATE_JUMPING;
			}
			else if (speed > 0 && !ctrl.IsRightPressed() || speed<0 && !ctrl.IsLeftPressed())
			{
				newState = CHSTATE_IDLE;
			}
			x += (int)(speed * (float)(time - updateTime));
			break;
		case CHSTATE_JUMPING: {
			if ((time - animStartTime) > DURATION_JUMP)
			{
				newState = CHSTATE_FALLING;
			}
			int delta = (int)(speed * (float)(time - updateTime));

			x += delta;
			y -= std::abs(delta);
			break;
		}
		case CHSTATE_FALLING: {
			if ((time - animStartTime) > DURATION_FALL)
			{
				newState = CHSTATE_IDLE;
			}
			int delta = (int)(speed * (float)(time - updateTime));

			x += delta;
			y += std::abs(delta);
			break;
		}
	}
	if (state != newState)
	{
		state = newState;
		animStartTime = time;
	}
	updateTime = time;
}

void Character::Render(SDL_Renderer* renderer)
{
	SDL_FRect rcdAnim = { 
		(SCREEN_WIDTH  - ANIM_WIDTH ) / 2, 
		(SCREEN_HEIGHT - ANIM_HEIGHT) / 2, 
		ANIM_WIDTH, 
		ANIM_HEIGHT 
	};
	SDL_FRect rcsAnim = rcdAnim;
	int row, frame;

	switch (state)
	{
	case CHSTATE_JUMPING: 
		row   = AnimRow::JUMP;
		frame = GetAnimFrame(ANIM_DELAY_JUMP, ANIM_FRAMES_JUMP, false);
		break;
	case CHSTATE_FALLING:
		row = AnimRow::FALL;
		frame = GetAnimFrame(ANIM_DELAY_FALL, ANIM_FRAMES_FALL, false);
		break;
	case CHSTATE_WALKING:
		row = AnimRow::WALK;
		frame = GetAnimFrame(ANIM_DELAY_WALK, ANIM_FRAMES_WALK, true);
		break;
	case CHSTATE_RUNNING:
		row   = AnimRow::RUN;
		frame = GetAnimFrame(ANIM_DELAY_RUN, ANIM_FRAMES_RUN, true);
		break;
	default:
		row   = AnimRow::IDLE;
		frame = GetAnimFrame(ANIM_DELAY_IDLE, ANIM_FRAMES_IDLE, true);
		break;
	}
	rcsAnim.x = ANIM_WIDTH  * frame;
	rcsAnim.y = ANIM_HEIGHT * (row + (speed < 0 ? CHDIR_WEST : CHDIR_EAST));

	SDL_RenderTexture(renderer, anims, &rcsAnim, &rcdAnim);
}

int Character::GetAnimFrame(int delay, int frame_count, bool loop) const {
	int frame = (int)(updateTime - animStartTime) / delay;
	return loop ? frame % frame_count : std::min(frame, frame_count-1);
}
