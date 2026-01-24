#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#ifdef _DEBUG
#include <cassert>

#define ASSERT(condition)	(assert(condition))
#else
#define ASSERT(condition)
#endif

#endif // _SETTINGS_H_
