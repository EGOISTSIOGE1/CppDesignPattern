#include "4.prototype.h"

#include <imgui.h>
#include <SDL_mixer.h>

using namespace _PrototypePattern;

static int type_weapon = 0, type_body = 0, type_hat = 0;

PrototypePattern::PrototypePattern(SDL_Renderer* renderer)
{
	texture_target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 665, 590);

	atlas_weapon_fork.load("weapon_fork_%d", 8);
	atlas_weapon_plate.load("weapon_plate_%d", 8);
	atlas_weapon_spoon.load("weapon_spoon_%d", 8);

	atlas_body_brown.load("brown_chicken_%d", 8);
	atlas_body_red.load("red_chicken_%d", 8);
	atlas_body_white.load("white_chicken_%d", 8);

	atlas_hat_crown.load("crown_%d", 8);
	atlas_hat_green.load("green_hat_%d", 8);
	atlas_hat_straw.load("straw_hat_%d", 8);
}

PrototypePattern::~PrototypePattern()
{
	SDL_DestroyTexture(texture_target);
}

void PrototypePattern::on_enter()
{
	srand((unsigned int)time(NULL));
	prototype_chicken = new Chicken();
}

void PrototypePattern::on_exit()
{
	for (Chicken* chicken : chicken_list)
		delete chicken;
	chicken_list.clear();

	delete prototype_chicken; prototype_chicken = nullptr;
}

void PrototypePattern::on_update(float delta)
{
	for (Chicken* chicken : chicken_list)
		chicken->on_update(delta);

	ImGui::BeginGroup();
	ImGui::TextUnformatted(u8"武器：");				ImGui::SameLine();
	ImGui::RadioButton(u8"叉子", &type_weapon, 0);	ImGui::SameLine();
	ImGui::RadioButton(u8"盘子", &type_weapon, 1);	ImGui::SameLine();
	ImGui::RadioButton(u8"汤匙", &type_weapon, 2);

	ImGui::TextUnformatted(u8"身体：");				ImGui::SameLine();
	ImGui::RadioButton(u8"棕色", &type_body, 0);		ImGui::SameLine();
	ImGui::RadioButton(u8"红色", &type_body, 1);		ImGui::SameLine();
	ImGui::RadioButton(u8"白色", &type_body, 2);	

	ImGui::TextUnformatted(u8"帽子：");				ImGui::SameLine();
	ImGui::RadioButton(u8"皇冠", &type_hat, 0);		ImGui::SameLine();
	ImGui::RadioButton(u8"绿帽", &type_hat, 1);		ImGui::SameLine();
	ImGui::RadioButton(u8"草帽", &type_hat, 2);
	ImGui::EndGroup();

	ImGui::SameLine();

	if (ImGui::Button(u8"将当前属性的鸡生成到世界中", { ImGui::GetContentRegionAvail().x, 80 }))
	{
		switch (type_weapon)
		{
		case 0: prototype_chicken->set_weapon(&atlas_weapon_fork); break;
		case 1: prototype_chicken->set_weapon(&atlas_weapon_plate); break;
		case 2: prototype_chicken->set_weapon(&atlas_weapon_fork); break;
		}
		switch (type_body)
		{
		case 0: prototype_chicken->set_body(&atlas_body_brown); break;
		case 1: prototype_chicken->set_body(&atlas_body_red); break;
		case 2: prototype_chicken->set_body(&atlas_body_white); break;
		}
		switch (type_hat)
		{
		case 0: prototype_chicken->set_hat(&atlas_hat_crown); break;
		case 1: prototype_chicken->set_hat(&atlas_hat_green); break;
		case 2: prototype_chicken->set_hat(&atlas_hat_straw); break;
		}
		chicken_list.push_back(prototype_chicken->clone());
		Mix_PlayChannel(-1, ResourcesManager::instance()->find_audio("click"), 0);
	}

	ImGui::BeginChild("chicken_result", ImGui::GetContentRegionAvail(), ImGuiChildFlags_Border);
	ImGui::Image(texture_target, ImGui::GetContentRegionAvail());
	ImGui::EndChild();
}

void PrototypePattern::on_render(SDL_Renderer* renderer)
{
	SDL_SetRenderTarget(renderer, texture_target);
	SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255);
	SDL_RenderClear(renderer);

	for (Chicken* chicken : chicken_list)
		chicken->on_render(renderer);

	SDL_SetRenderTarget(renderer, nullptr);
}
