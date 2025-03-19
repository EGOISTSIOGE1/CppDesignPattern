#ifndef _PROXY_H_
#define _PROXY_H_

#include "example.h"

#include <string>

class ProxyPattern : public Example
{
public:
	ProxyPattern(SDL_Renderer* renderer);
	~ProxyPattern();

	void on_update(float delta) override;
	void on_render(SDL_Renderer* renderer) override;

private:
	int font_size = 50;
	bool need_update = true;
	SDL_Rect rect_text = { 0 };
	SDL_Texture* texture_text = nullptr;
	SDL_Texture* texture_target = nullptr;
	std::string str_text = "Hello World !";
	float color_text[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

private:
	void update_texture_text(SDL_Renderer* renderer);

};

#endif // !_PROXY_H_
