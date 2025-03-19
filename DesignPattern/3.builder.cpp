#include "3.builder.h"

#include <imgui.h>

using namespace _BuilderPattern;

static int type_weapon = 0, type_body = 0, type_hat = 0;

void CustomChickenBuilder::init_weapon()
{
	atlas_weapon.clear();

	switch (type_weapon)
	{
	case 0: atlas_weapon.load("weapon_fork_%d", 8); break;
	case 1: atlas_weapon.load("weapon_plate_%d", 8); break;
	case 2: atlas_weapon.load("weapon_spoon_%d", 8); break;
	}
}

void CustomChickenBuilder::init_body()
{
	atlas_body.clear();

	switch (type_body)
	{
	case 0: atlas_body.load("brown_chicken_%d", 8); break;
	case 1: atlas_body.load("red_chicken_%d", 8); break;
	case 2: atlas_body.load("white_chicken_%d", 8); break;
	}
}

void CustomChickenBuilder::init_hat()
{
	atlas_hat.clear();

	switch (type_hat)
	{
	case 0: atlas_hat.load("crown_%d", 8); break;
	case 1: atlas_hat.load("green_hat_%d", 8); break;
	case 2: atlas_hat.load("straw_hat_%d", 8); break;
	}
}

Chicken* CustomChickenBuilder::build()
{
	Chicken* chicken = new Chicken();

	chicken->set_weapon(&atlas_weapon);
	chicken->set_body(&atlas_body);
	chicken->set_hat(&atlas_hat);

	return chicken;
}

BuilderPattern::BuilderPattern(SDL_Renderer* renderer)
{
	delete chicken;

	texture_target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 332, 296);
}

BuilderPattern::~BuilderPattern()
{
	SDL_DestroyTexture(texture_target);
}

void BuilderPattern::on_update(float delta)
{
	if (chicken) 
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

	ImGui::BeginGroup();
	if (ImGui::Button(u8"生成伊丽莎白鸡", {ImGui::GetContentRegionAvail().x / 2, 35}))
	{
		elizabeth_chicken_builder.init_weapon();
		elizabeth_chicken_builder.init_body();
		elizabeth_chicken_builder.init_hat();
		delete chicken; chicken = elizabeth_chicken_builder.build();
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"生成绿帽吮指鸡", { ImGui::GetContentRegionAvail().x, 35 }))
	{
		green_hat_original_recipe_chicken_builder.init_weapon();
		green_hat_original_recipe_chicken_builder.init_body();
		green_hat_original_recipe_chicken_builder.init_hat();
		delete chicken; chicken = green_hat_original_recipe_chicken_builder.build();
	}
	if (ImGui::Button(u8"点击生成你自定义的鸡吧！", { ImGui::GetContentRegionAvail().x, 40 }))
	{
		custom_chicken_builder.init_weapon();
		custom_chicken_builder.init_body();
		custom_chicken_builder.init_hat();
		delete chicken; chicken = custom_chicken_builder.build();
	}
	ImGui::EndGroup();

	ImGui::BeginChild("chicken_result", ImGui::GetContentRegionAvail(), ImGuiChildFlags_Border);
	ImGui::Image(texture_target, ImGui::GetContentRegionAvail());
	ImGui::EndChild();
}

void BuilderPattern::on_render(SDL_Renderer* renderer)
{
	if (!chicken) return;
	
	SDL_SetRenderTarget(renderer, texture_target);
	SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255);
	SDL_RenderClear(renderer);

	chicken->on_render(renderer);

	SDL_SetRenderTarget(renderer, nullptr);
}