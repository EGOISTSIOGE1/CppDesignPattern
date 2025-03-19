#include "4.decorator.h"

#include <imgui.h>

using namespace _DecoratorPattern;

DecoratorPattern::DecoratorPattern(SDL_Renderer* renderer)
{
	player = new Player();

	prop_list.push_back(new Prop({ 100, 100 }, ResourcesManager::instance()->find_texture("coin"), 
		[&]() { player = new SpeedUpDecorator(player); }));
	prop_list.push_back(new Prop({ 620, 100 }, ResourcesManager::instance()->find_texture("favorites"),
		[&]() { player = new SizeGrownDecorator(player); }));
	prop_list.push_back(new Prop({ 360, 620 }, ResourcesManager::instance()->find_texture("likes"),
		[&]() { player = new AppearanceChangedDecorator(player); }));

	texture_target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 720, 720);
}

DecoratorPattern::~DecoratorPattern()
{
	delete player;

	for (Prop* prop : prop_list)
		delete prop;

	SDL_DestroyTexture(texture_target);
}

void DecoratorPattern::on_input(const SDL_Event* event)
{
	player->on_input(event);
}

void DecoratorPattern::on_update(float delta)
{
	player->on_update(delta);

	for (Prop* prop : prop_list)
		prop->check_collide(player);

	prop_list.erase(std::remove_if(
		prop_list.begin(), prop_list.end(),
		[](Prop* prop)
		{
			bool can_remove = prop->can_remove();
			if (can_remove) delete prop;
			return can_remove;
		}),
		prop_list.end());

	ImGui::Image(texture_target, ImGui::GetContentRegionAvail());

	ImGui::SetCursorPos({ 15, 15 });
	ImGui::TextColored({ 1.0f, 0.35f, 0.12f, 1.0f }, u8"• 使用 上/下/左/右 方向键控制飞机拾取道具获得不同属性");
}

void DecoratorPattern::on_render(SDL_Renderer* renderer)
{
	SDL_SetRenderTarget(renderer, texture_target);
	SDL_SetRenderDrawColor(renderer, 65, 65, 65, 255);
	SDL_RenderClear(renderer);

	for (const Prop* prop : prop_list)
		prop->on_render(renderer);

	player->on_render(renderer);

	SDL_SetRenderTarget(renderer, nullptr);
}
