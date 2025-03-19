#ifndef _BRIDGE_H_
#define _BRIDGE_H_

#include "example.h"

#include <string>

namespace _BridgePattern
{
    class GraphicsImpl 
    {
    public:
        virtual void init_window(int width, int height) = 0;
        virtual void* load_image(const std::string& path) = 0;
        virtual void show_image(void* image, int x, int y) = 0;
        virtual bool need_quit() = 0;
        virtual void quit_window() = 0;

    };

    class RaylibImpl : public GraphicsImpl 
    {
    public:
        void init_window(int width, int height) override;
        void* load_image(const std::string& path) override;
        void show_image(void* image, int x, int y) override;
        bool need_quit() override;
        void quit_window() override;

    };

    class EasyXImpl : public GraphicsImpl
    {
    public:
        void init_window(int width, int height) override;
        void* load_image(const std::string& path) override;
        void show_image(void* image, int x, int y) override;
        bool need_quit() override;
        void quit_window() override;

    };

    class Window 
    {
    public:
        Window(GraphicsImpl* _impl) : impl(_impl) {}
        ~Window() = default;

        void create(int w, int h) 
        {
            impl->init_window(w, h);
        }

        void load_image(const std::string& path)
        {
            image = impl->load_image(path);
        }

        void run()
        {
            while (!impl->need_quit())
            {
                impl->show_image(image, 0, 0);
                SDL_Delay(10);
            }

            impl->quit_window();
        }

    private:
        void* image = nullptr;
        GraphicsImpl* impl = nullptr;

    };
}

class BridgePattern : public Example
{
public:
	BridgePattern();
	~BridgePattern();

    void on_update(float delta) override;

};

#endif // !_BRIDGE_H_
