#include "1.factory_method.h"

#include <imgui.h>

using namespace _FactoryMethodPattern;

FactoryMethodPattern::FactoryMethodPattern(SDL_Renderer* renderer)
{
	SwordFactory sword_factory;
	SlimeFactory slime_factory;

	game_obj_list.push_back(sword_factory.create(Attribute::Ice,	{ 200, 100 }));
	game_obj_list.push_back(sword_factory.create(Attribute::Grass,	{ 200, 300 }));
	game_obj_list.push_back(sword_factory.create(Attribute::Fire,	{ 200, 500 }));

	game_obj_list.push_back(slime_factory.create(Attribute::Ice,	{ 425, 140 }));
	game_obj_list.push_back(slime_factory.create(Attribute::Grass,	{ 425, 340 }));
	game_obj_list.push_back(slime_factory.create(Attribute::Fire,	{ 425, 540 }));

	texture_target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 720, 720);
}

FactoryMethodPattern::~FactoryMethodPattern()
{
	for (GameObject* game_obj : game_obj_list)
		delete game_obj;

	SDL_DestroyTexture(texture_target);
}

void FactoryMethodPattern::on_update(float delta)
{
	for (GameObject* game_obj : game_obj_list)
		game_obj->on_update(delta);

	ImGui::Image(texture_target, ImGui::GetContentRegionAvail());
}

void FactoryMethodPattern::on_render(SDL_Renderer* renderer)
{
	SDL_SetRenderTarget(renderer, texture_target);
	SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255);
	SDL_RenderClear(renderer);

	for (GameObject* game_obj : game_obj_list)
		game_obj->on_render(renderer);

	SDL_SetRenderTarget(renderer, nullptr);
}
