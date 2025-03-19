#include "5.singleton.h"

#include <imgui.h>

using namespace _SingletonPattern;

SceneManager* SceneManager::manager = nullptr;

void Level_1::on_reach()
{
	SceneManager::instance()->switch_to("level_2");
}

void Level_2::on_reach()
{
	SceneManager::instance()->switch_to("level_3");
}

void Level_3::on_reach()
{
	SceneManager::instance()->switch_to("level_1");
}

SingletonPattern::SingletonPattern(SDL_Renderer* renderer)
{
	SceneManager::instance()->add("level_1", new Level_1(&player));
	SceneManager::instance()->add("level_2", new Level_2(&player));
	SceneManager::instance()->add("level_3", new Level_3(&player));
	SceneManager::instance()->switch_to("level_1");

	texture_target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 720, 720);
}

SingletonPattern::~SingletonPattern()
{
	SDL_DestroyTexture(texture_target);
}

void SingletonPattern::on_input(const SDL_Event* event)
{
	SceneManager::instance()->on_input(event);
}

void SingletonPattern::on_update(float delta)
{
	SceneManager::instance()->on_update(delta);

	ImGui::Image(texture_target, ImGui::GetContentRegionAvail());

	ImGui::SetCursorPos({ 15, 15 });
	ImGui::TextColored({ 1.0f, 0.35f, 0.12f, 1.0f }, u8"• 使用 上/下/左/右 方向键控制直升机达到停机坪完成关卡");
}

void SingletonPattern::on_render(SDL_Renderer* renderer)
{
	SDL_SetRenderTarget(renderer, texture_target);
	SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255);
	SDL_RenderClear(renderer);

	SceneManager::instance()->on_render(renderer);

	SDL_SetRenderTarget(renderer, nullptr);
}
