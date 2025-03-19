#include "2.bridge.h"

#include <codecvt>
#include <graphics.h>

using namespace _BridgePattern;

void EasyXImpl::init_window(int width, int height)
{
    initgraph(width, height, EW_NOCLOSE);
    SetWindowText(GetHWnd(), _T("Window Created By [EasyX] £¨°´ Esc ÍË³ö´°¿Ú£©"));
}

void* EasyXImpl::load_image(const std::string& path)
{
    static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;

    IMAGE* img = new IMAGE();
    loadimage(img, convert.from_bytes(path).c_str());

    return img;
}

void EasyXImpl::show_image(void* image, int x, int y)
{
    IMAGE* img = static_cast<IMAGE*>(image);

    putimage(x, y, img);
}

bool EasyXImpl::need_quit()
{
    ExMessage msg;
    while (peekmessage(&msg))
    {
        if (msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE)
            return true;
    }

    return false;
}

void EasyXImpl::quit_window()
{
    closegraph();
}