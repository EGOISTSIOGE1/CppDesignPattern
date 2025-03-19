#include "2.abstract_factory.h"

#include <imgui.h>

using namespace _AbstractFactoryPattern;

static std::vector<std::vector<const char*>> atlas_path_template_map =
{
	{ "SlimeBlue_0%d", "SlimeGreen_0%d", "SlimeRed_0%d" },
	{ "MegaSlimeBlue_0%d", "MegaSlimeGreen_0%d", "MegaSlimeRed_0%d" },
	{ "KingSlimeBlue_0%d", "KingSlimeGreen_0%d", "KingSlimeRed_0%d" }
};

Slime::Slime(SlimeDefFactory* factory, const Vector2& position)
{
	this->position = position;

	int idx_size = (int)factory->get_size_def()->get_size();
	int idx_attrbute = (int)factory->get_attribute_def()->get_attribute();

	atlas.load(atlas_path_template_map[idx_size][idx_attrbute], 6);

	animation.add_frame(&atlas);
	animation.set_loop(true);
	animation.set_interval(0.1f);
}

Slime::~Slime() = default;

AbstractFactoryPattern::AbstractFactoryPattern(SDL_Renderer* renderer)
{
	SmallIceSlimeDefFactory small_ice_slime_def_factory;
	MediumIceSlimeDefFactory medium_ice_slime_def_factory;
	LargeIceSlimeDefFactory large_ice_slime_def_factory;
	SmallGrassSlimeDefFactory small_grass_slime_def_factory;
	MediumGrassSlimeDefFactory medium_grass_slime_def_factory;
	LargeGrassSlimeDefFactory large_grass_slime_def_factory;
	SmallFireSlimeDefFactory small_fire_slime_def_factory;
	MediumFireSlimeDefFactory medium_fire_slime_def_factory;
	LargeFireSlimeDefFactory large_fire_slime_def_factory;

	slime_list.push_back(new Slime(&small_ice_slime_def_factory,	{ 200, 110 }));
	slime_list.push_back(new Slime(&small_grass_slime_def_factory,	{ 200, 310 }));
	slime_list.push_back(new Slime(&small_fire_slime_def_factory,	{ 200, 510 }));
	slime_list.push_back(new Slime(&medium_ice_slime_def_factory,	{ 350, 110 }));
	slime_list.push_back(new Slime(&medium_grass_slime_def_factory,	{ 350, 310 }));
	slime_list.push_back(new Slime(&medium_fire_slime_def_factory,	{ 350, 510 }));	
	slime_list.push_back(new Slime(&large_ice_slime_def_factory,	{ 500, 100 }));
	slime_list.push_back(new Slime(&large_grass_slime_def_factory,	{ 500, 300 }));
	slime_list.push_back(new Slime(&large_fire_slime_def_factory,	{ 500, 500 }));

	texture_target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 720, 720);
}

AbstractFactoryPattern::~AbstractFactoryPattern()
{
	SDL_DestroyTexture(texture_target);

	for (Slime* slime : slime_list)
		delete slime;
}

void AbstractFactoryPattern::on_update(float delta)
{
	for (Slime* slime : slime_list)
		slime->on_update(delta);

	ImGui::Image(texture_target, ImGui::GetContentRegionAvail());
}

void AbstractFactoryPattern::on_render(SDL_Renderer* renderer)
{
	SDL_SetRenderTarget(renderer, texture_target);
	SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255);
	SDL_RenderClear(renderer);

	for (Slime* slime : slime_list)
		slime->on_render(renderer);

	SDL_SetRenderTarget(renderer, nullptr);
}