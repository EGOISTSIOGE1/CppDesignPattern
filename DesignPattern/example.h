#ifndef _EXAMPLE_H_
#define _EXAMPLE_H_

#include <SDL.h>

class Example
{
public:
	Example() {}
	virtual ~Example() = default;

	virtual void on_enter() {}
	virtual void on_exit() {}

	virtual void on_input(const SDL_Event* event) {}
	virtual void on_update(float delta) {}
	virtual void on_render(SDL_Renderer* renderer) {}

};

#endif // !_EXAMPLE_H_
