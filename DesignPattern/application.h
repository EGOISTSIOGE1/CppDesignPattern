#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <SDL.h>

class Application
{
public:
	static Application* instance()
	{
		if (!application)
			application = new Application();

		return application;
	}

	int run(int argc, char** argv);

private:
	static Application* application;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	bool is_quit = false;

private:
	Application();
	~Application();

};

#endif // !_APPLICATION_H_
