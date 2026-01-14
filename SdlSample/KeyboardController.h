#ifndef _KEYBOARDCONTROLLER_H_
#define _KEYBOARDCONTROLLER_H_

#include "Controller.h"

class KeyboardController : public Controller
{
public:
	KeyboardController();

	virtual bool IsLeftPressed () const override;
	virtual bool IsRightPressed() const override;
	virtual bool IsJumpPressed () const override;
};

#endif // _KEYBOARDCONTROLLER_H_