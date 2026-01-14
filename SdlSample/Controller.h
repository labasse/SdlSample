#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <SDL3/SDL.h>

// The Controller class handles user input for character movement.
class Controller
{
public:
	// Constructs a Controller instance.
	Controller();

	// Virtual destructor for proper cleanup in derived classes.
	virtual ~Controller() = default;
	
	// Gets whether the left movement is required by the player.
	// @return true if the left direction is required; false otherwise.
	virtual bool IsLeftPressed() const = 0;

	// Gets whether the right movement is required by the player.
	// @return true if the right direction is required; false otherwise.
	virtual bool IsRightPressed() const = 0;

	// Gets whether the jump action is required by the player.
	// @return true if the jump action is required; false otherwise.
	virtual bool IsJumpPressed() const = 0;

	// Updates the controller state. This method can be overridden by derived classes.
	// @param frame_start The SDL start time of the current frame in milliseconds.
	// @remarks The base implementation does nothing.
	virtual void Update(Uint64 frame_start);

	// Gets whether any horizontal movement is required by the player.
	// @return true if either left or right direction is required; false otherwise.
	inline bool IsDirectionPressed() const { return IsLeftPressed() || IsRightPressed(); }
};

#endif // _CONTROLLER_H_
