#include <stdio.h>
#include "Settings.h"

#include "Character.h"

#define ANIM_WIDTH   64.f
#define ANIM_HEIGHT  64.f

#define ANIM_FRAMES_IDLE 2
#define ANIM_FRAMES_WALK 9
#define ANIM_FRAMES_JUMP 5

#define ANIM_DELAY_IDLE 300
#define ANIM_DELAY_WALK 100
#define ANIM_DELAY_JUMPA 100
#define ANIM_DELAY_JUMPB 300
#define ANIM_DELAY_JUMP (ANIM_DELAY_JUMPA*2 + ANIM_DELAY_JUMPB*3)


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
	SIT = 30,
	EMOTE = 34,
	RUN = 36,
	WATERING = 40
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
				newState = CHSTATE_WALKING;
				speed = ctrl.IsLeftPressed() ? -.1f : .1f;
			}
			break;
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
		case CHSTATE_JUMPING:
			if ((time - animStartTime) > ANIM_DELAY_JUMP)
			{
				newState = CHSTATE_IDLE;
			}
			x += (int)(speed * (int)(time - updateTime));
			break;
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
		frame = GetJumpFrame();
		break;
	case CHSTATE_WALKING:
		row   = AnimRow::WALK;
		frame = GetAnimFrame(ANIM_DELAY_WALK, ANIM_FRAMES_WALK);
		break;
	default:
		row   = AnimRow::IDLE;
		frame = GetAnimFrame(ANIM_DELAY_IDLE, ANIM_FRAMES_IDLE);
		break;
	}
	rcsAnim.x = ANIM_WIDTH  * frame;
	rcsAnim.y = ANIM_HEIGHT * (row + (speed < 0 ? CHDIR_WEST : CHDIR_EAST));

	SDL_RenderTexture(renderer, anims, &rcsAnim, &rcdAnim);
}

int Character::GetJumpFrame() const
{
	int elapsed = (int)(updateTime - animStartTime);

	if (elapsed < ANIM_DELAY_JUMPA * 2)
	{
		return elapsed / ANIM_DELAY_JUMPA;
	}
	if (elapsed >= ANIM_DELAY_JUMP)
	{
		return ANIM_FRAMES_JUMP - 1;
	}
	return 2 + (elapsed - ANIM_DELAY_JUMPA * 2) / ANIM_DELAY_JUMPB;
}
