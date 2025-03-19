#include "8.strategy.h"

#include <imgui.h>

using namespace _StrategyPattern;

StrategyPattern::StrategyPattern(SDL_Renderer* renderer)
{
	boar_list.push_back(new Boar(&player, { 100, 100 }));
	boar_list.push_back(new Boar(&player, { 620, 100 }));
	boar_list.push_back(new Boar(&player, { 360, 620 }));

	for (Boar* boar : boar_list)
		boar->set_strategy(&follow_strategy);

	texture_target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 720, 720);
}

StrategyPattern::~StrategyPattern()
{
	for (Boar* boar : boar_list)
		delete boar;

	SDL_DestroyTexture(texture_target);
}

void StrategyPattern::on_input(const SDL_Event* event)
{
	player.on_input(event);
}

void StrategyPattern::on_update(float delta)
{
	player.on_update(delta);
	for (Boar* boar : boar_list)
		boar->on_update(delta);

	if (ImGui::Button(is_using_follow_strategy ? u8"切换野猪为 [远离] 策略" 
		: u8"切换野猪为 [跟随] 策略", { ImGui::GetContentRegionAvail().x, 35 }))
	{
		is_using_follow_strategy = !is_using_follow_strategy;
		MoveStrategy* current_strategy = is_using_follow_strategy
			? (MoveStrategy*)&follow_strategy : (MoveStrategy*)&stay_away_strategy;
		for (Boar* boar : boar_list)
			boar->set_strategy(current_strategy);
	}

	ImGui::BeginChild("scene", ImGui::GetContentRegionAvail());
	ImGui::Image(texture_target, ImGui::GetContentRegionAvail());
	ImGui::SetCursorPos({ 10, 10 });
	ImGui::TextColored({ 1.0f, 0.35f, 0.12f, 1.0f }, u8"• 使用 上/下/左/右 方向键控制角色移动");
	ImGui::EndChild();
}

void StrategyPattern::on_render(SDL_Renderer* renderer)
{
	SDL_SetRenderTarget(renderer, texture_target);
	SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255);
	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, ResourcesManager::instance()->find_texture("grassland"), nullptr, nullptr);
	
	for (Boar* boar : boar_list)
		boar->on_render(renderer);
	player.on_render(renderer);

	SDL_SetRenderTarget(renderer, nullptr);
}
