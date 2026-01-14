#include <SDL3/SDL.h>

#include "KeyboardController.h"

KeyboardController::KeyboardController()
{
}

bool KeyboardController::IsLeftPressed() const
{
	auto states = SDL_GetKeyboardState(nullptr);

	return states[SDL_SCANCODE_LEFT];
}

bool KeyboardController::IsRightPressed() const
{
	return *(SDL_GetKeyboardState(nullptr) + SDL_SCANCODE_RIGHT);
}

bool KeyboardController::IsJumpPressed() const
{
	return *(SDL_GetKeyboardState(nullptr) + SDL_SCANCODE_UP);
}