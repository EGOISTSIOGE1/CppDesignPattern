#include "7.proxy.h"
#include "resources_manager.h"

#include <imgui.h>
#include <imgui_stdlib.h>

ProxyPattern::ProxyPattern(SDL_Renderer* renderer)
{
	texture_target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 664, 548);
}

ProxyPattern::~ProxyPattern()
{
	SDL_DestroyTexture(texture_target);
}

void ProxyPattern::on_update(float delta)
{
	ImGui::TextUnformatted(u8"文本："); ImGui::SameLine();
	if (ImGui::InputText(u8"##文本", &str_text))
		need_update = true;
	ImGui::TextUnformatted(u8"颜色："); ImGui::SameLine();
	if (ImGui::ColorEdit4(u8"##颜色", color_text, ImGuiColorEditFlags_AlphaPreview))
		need_update = true;
	ImGui::TextUnformatted(u8"字号："); ImGui::SameLine();
	if (ImGui::DragInt(u8"##字号", &font_size, 1.0f, 10, 500, "%d px", ImGuiSliderFlags_AlwaysClamp))
		need_update = true;
	
	ImGui::Dummy({ 0, 5 });

	ImGui::TextDisabled(u8"* 此外，另见 framework/font_wrapper.h 中的代码实现。");

	ImGui::Dummy({ 0, 5 });

	ImGui::BeginChild("render_result", ImGui::GetContentRegionAvail(), ImGuiChildFlags_Border);
	ImGui::Image(texture_target, ImGui::GetContentRegionAvail());
	ImGui::EndChild();
}

void ProxyPattern::on_render(SDL_Renderer* renderer)
{
	if (need_update)
	{
		update_texture_text(renderer);
		need_update = false;
	}

	SDL_SetRenderTarget(renderer, texture_target);
	SDL_SetRenderDrawColor(renderer, 51, 76, 76, 255);
	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, texture_text, nullptr, &rect_text);

	SDL_SetRenderTarget(renderer, nullptr);
}

void ProxyPattern::update_texture_text(SDL_Renderer* renderer)
{
	SDL_DestroyTexture(texture_text); texture_text = nullptr;

	SDL_Color color = 
	{ 
		(Uint8)(color_text[0] * 255), (Uint8)(color_text[1] * 255), 
		(Uint8)(color_text[2] * 255), (Uint8)(color_text[3] * 255) 
	};
	SDL_Surface* surface = TTF_RenderUTF8_Blended(ResourcesManager::instance()
		->find_font("SarasaMonoSC-Regular")->get_font(font_size), str_text.c_str(), color);
	if (!surface) return;

	texture_text = SDL_CreateTextureFromSurface(renderer, surface);
	rect_text.w = surface->w, rect_text.h = surface->h;
	SDL_FreeSurface(surface);
}
