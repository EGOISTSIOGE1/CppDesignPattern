#define SDL_MAIN_HANDLED

#include "application.h"

#pragma comment(linker, "/subsystem:windows /entry:mainCRTStartup")

int main(int argc, char** argv)
{
	return Application::instance()->run(argc, argv);
}