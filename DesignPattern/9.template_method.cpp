#include "9.template_method.h"

#include <imgui.h>

#include <random>

using namespace _TemplateMethodPattern;

void CustomizedMapGen::on_inspect_config()
{
	ImGui::SetNextWindowSize({ 320, 285 });
	if (!ImGui::BeginPopup("config_window")) return;

	{
		ImGui::TextDisabled(u8"地形");
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 110);

		ImGui::TextUnformatted(u8"深坑数量："); ImGui::NextColumn();
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		ImGui::DragInt(u8"##深坑数量", &num_hole, 1.0f, 0, 10, "%d", ImGuiSliderFlags_AlwaysClamp);
		ImGui::NextColumn();

		ImGui::TextUnformatted(u8"地刺数量："); ImGui::NextColumn();
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		ImGui::DragInt(u8"##地刺数量", &num_spikes, 1.0f, 0, 10, "%d", ImGuiSliderFlags_AlwaysClamp);
		ImGui::NextColumn();

		ImGui::Columns(1);
	}
	ImGui::Separator();
	{
		ImGui::TextDisabled(u8"生物");
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 110);

		ImGui::TextUnformatted(u8"生成精灵："); ImGui::NextColumn();
		ImGui::Checkbox(u8"##生成精灵", &has_npc);
		ImGui::NextColumn();

		ImGui::TextUnformatted(u8"蜥蜴数量："); ImGui::NextColumn();
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		ImGui::DragInt(u8"##蜥蜴数量", &num_lizard, 1.0f, 0, 10, "%d", ImGuiSliderFlags_AlwaysClamp);
		ImGui::NextColumn();

		ImGui::TextUnformatted(u8"哥布林数量："); ImGui::NextColumn();
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		ImGui::DragInt(u8"##哥布林数量", &num_goblin, 1.0f, 0, 10, "%d", ImGuiSliderFlags_AlwaysClamp);
		ImGui::NextColumn();

		ImGui::Columns(1);
	}
	ImGui::Separator();
	{
		ImGui::TextDisabled(u8"其他");
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 110);

		ImGui::TextUnformatted(u8"生成出口："); ImGui::NextColumn();
		ImGui::Checkbox(u8"##生成出口", &has_exit);
		ImGui::NextColumn();

		ImGui::TextUnformatted(u8"生成奖励宝箱："); ImGui::NextColumn();
		ImGui::Checkbox(u8"##生成奖励宝箱", &has_reward);	
		ImGui::NextColumn();

		ImGui::Columns(1);
	}

	ImGui::EndPopup();
}

void CustomizedMapGen::generate_npc(Map* map)
{
	if (has_npc)
	{
		const std::vector<SDL_Point>& empty_grid_list = map->get_empty_grids();
		map->add_item(new Elf(), empty_grid_list[rand() % empty_grid_list.size()]);
	}
}

void CustomizedMapGen::generate_exit(Map* map)
{
	if (has_exit)
	{
		const std::vector<SDL_Point>& empty_grid_list = map->get_empty_grids();
		map->add_item(new Stairs(), empty_grid_list[rand() % empty_grid_list.size()]);
	}
}

void CustomizedMapGen::generate_trap(Map* map)
{
	{
		std::vector<SDL_Point> target_grid_list(num_hole);
		const std::vector<SDL_Point>& empty_grid_list = map->get_empty_grids();
		std::sample(empty_grid_list.begin(), empty_grid_list.end(),
			target_grid_list.begin(), num_hole, std::mt19937(std::random_device()()));
		for (const SDL_Point& grid : target_grid_list)
			map->add_item(new Hole(), grid);
	}
	{
		std::vector<SDL_Point> target_grid_list(num_spikes);
		const std::vector<SDL_Point>& empty_grid_list = map->get_empty_grids();
		std::sample(empty_grid_list.begin(), empty_grid_list.end(),
			target_grid_list.begin(), num_spikes, std::mt19937(std::random_device()()));
		for (const SDL_Point& grid : target_grid_list)
			map->add_item(new Spikes(), grid);
	}
}

void CustomizedMapGen::generate_enemy(Map* map)
{
	{
		std::vector<SDL_Point> target_grid_list(num_goblin);
		const std::vector<SDL_Point>& empty_grid_list = map->get_empty_grids();
		std::sample(empty_grid_list.begin(), empty_grid_list.end(),
			target_grid_list.begin(), num_goblin, std::mt19937(std::random_device()()));
		for (const SDL_Point& grid : target_grid_list)
			map->add_item(new Goblin(), grid);
	}
	{
		std::vector<SDL_Point> target_grid_list(num_lizard);
		const std::vector<SDL_Point>& empty_grid_list = map->get_empty_grids();
		std::sample(empty_grid_list.begin(), empty_grid_list.end(),
			target_grid_list.begin(), num_lizard, std::mt19937(std::random_device()()));
		for (const SDL_Point& grid : target_grid_list)
			map->add_item(new Lizard(), grid);
	}
}

void CustomizedMapGen::generate_reward(Map* map)
{
	if (has_reward)
	{
		const std::vector<SDL_Point>& empty_grid_list = map->get_empty_grids();
		map->add_item(new Chest(), empty_grid_list[rand() % empty_grid_list.size()]);
	}
}

TemplateMethodPattern::TemplateMethodPattern(SDL_Renderer* renderer)
{
	map = map_generator.generate();

	texture_target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 720, 720);
}

TemplateMethodPattern::~TemplateMethodPattern()
{
	delete map;

	SDL_DestroyTexture(texture_target);
}

void TemplateMethodPattern::on_update(float delta)
{
	map->on_update(delta);

	if (ImGui::ImageButton(ResourcesManager::instance()->find_texture("icon-config"), { 30, 30 }))
		ImGui::OpenPopup("config_window");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::TextDisabled(u8"点击打开配置面板");
		ImGui::EndTooltip();
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"重新生成关卡地图", { ImGui::GetContentRegionAvail().x, 35 }))
	{
		delete map;
		map = map_generator.generate();
	}

	ImGui::BeginChild("map", ImGui::GetContentRegionAvail());
	ImGui::Image(texture_target, ImGui::GetContentRegionAvail());
	ImGui::EndChild();

	map_generator.on_inspect_config();
}

void TemplateMethodPattern::on_render(SDL_Renderer* renderer)
{
	SDL_SetRenderTarget(renderer, texture_target);
	SDL_SetRenderDrawColor(renderer, 65, 65, 65, 255);
	SDL_RenderClear(renderer);

	map->on_render(renderer);

	SDL_SetRenderTarget(renderer, nullptr);
}
