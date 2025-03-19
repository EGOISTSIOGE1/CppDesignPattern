#include "2.bridge.h"

#include <raylib.h>

using namespace _BridgePattern;

void RaylibImpl::init_window(int width, int height)
{
    InitWindow(width, height, "Window Created By [Raylib]");
}

void* RaylibImpl::load_image(const std::string& path)
{
    Texture2D* texture = new Texture2D(LoadTexture(path.c_str()));

    return texture;
}

void RaylibImpl::show_image(void* image, int x, int y)
{
    BeginDrawing();
    DrawTexture(*(Texture2D*)image, x, y, { 255, 255, 255, 255 });
    EndDrawing();
}

bool RaylibImpl::need_quit()
{
    return WindowShouldClose();
}

void RaylibImpl::quit_window()
{
    CloseWindow();
}