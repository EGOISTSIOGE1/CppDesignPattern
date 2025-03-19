#include "2.command.h"

#include <imgui.h>

using namespace _CommandPattern;

void Player::on_move(const SDL_Point& dir)
{
	SDL_Point new_idx = { idx_map_grid.x + dir.x, idx_map_grid.y + dir.y };
	if (new_idx.x < 0 || new_idx.x >= 12 || new_idx.y < 0 || new_idx.y >= 12
		|| map->grids[new_idx.y][new_idx.x] == Map::GridType::Inaccessible)
		return;

	idx_map_grid = new_idx;

	if (!is_replaying)
		cmd_list.push_back(new MoveCommand(this, dir));
	
	if (map->grids[new_idx.y][new_idx.x] == Map::GridType::Goal)
	{
		is_achieved_goal = true;
		Mix_PlayChannel(-1, ResourcesManager::instance()->find_audio("achieve"), 0);
	}
}

void _CommandPattern::Player::reset()
{
	idx_cmd = 0;
	idx_map_grid = { 2, 2 };
}

void Player::replay()
{
	reset();

	is_replaying = true;
}

CommandPattern::CommandPattern(SDL_Renderer* renderer)
{
	Map::GridType goal = Map::GridType::Goal;
	Map::GridType accs = Map::GridType::Accessible;
	Map::GridType inac = Map::GridType::Inaccessible;

	map.grids =
	{
		{ inac, inac, inac, inac, inac, inac, inac, inac, inac, inac, inac, inac },
		{ inac, accs, accs, accs, accs, inac, inac, inac, inac, inac, inac, inac },
		{ inac, accs, accs, accs, inac, inac, inac, inac, inac, inac, inac, inac },
		{ inac, inac, inac, accs, accs, inac, inac, inac, inac, inac, inac, inac },
		{ inac, accs, inac, accs, accs, accs, accs, accs, accs, accs, accs, inac },
		{ inac, accs, inac, inac, accs, accs, accs, accs, inac, inac, inac, inac },
		{ inac, accs, accs, inac, inac, inac, accs, accs, accs, accs, goal, inac },
		{ inac, accs, accs, accs, accs, inac, accs, accs, accs, accs, accs, inac },
		{ inac, accs, accs, accs, accs, inac, inac, inac, accs, accs, accs, inac },
		{ inac, accs, accs, accs, accs, accs, accs, inac, accs, accs, accs, inac },
		{ inac, accs, accs, accs, accs, accs, accs, inac, accs, accs, accs, inac },
		{ inac, inac, inac, inac, inac, inac, inac, inac, inac, inac, inac, inac },
	};

	player = new Player(&map);

	texture_target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 576, 576);
}

CommandPattern::~CommandPattern()
{
	delete player;

	SDL_DestroyTexture(texture_target);
}

void CommandPattern::on_input(const SDL_Event* event)
{
	player->on_input(event);
}

void CommandPattern::on_update(float delta)
{
	player->on_update(delta);

	ImGui::BeginDisabled(!player->can_replay());
	if (ImGui::Button(u8"回放玩家操作", { ImGui::GetContentRegionAvail().x, 35 }))
		player->replay();
	ImGui::EndDisabled();

	ImGui::BeginChild("scene", ImGui::GetContentRegionAvail());
	ImGui::Image(texture_target, ImGui::GetContentRegionAvail());
	ImGui::SetCursorPos({ 15, 15 });
	ImGui::TextColored({ 1.0f, 0.35f, 0.12f, 1.0f }, u8"• 使用 上/下/左/右 方向键控制角色到达金币处后点击按钮回放玩家操作");
	ImGui::EndChild();
}

void CommandPattern::on_render(SDL_Renderer* renderer)
{
	SDL_SetRenderTarget(renderer, texture_target);
	SDL_SetRenderDrawColor(renderer, 65, 65, 65, 255);
	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, ResourcesManager::instance()->find_texture("maze"), nullptr, nullptr);

	player->on_render(renderer);

	SDL_SetRenderTarget(renderer, nullptr);
}
