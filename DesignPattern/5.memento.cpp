#include "5.memento.h"

#include <imgui.h>

#include <fstream>
#include <sstream>

using namespace _MementoPattern;

MementoPattern::MementoPattern(SDL_Renderer* renderer)
{
	texture_target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 384, 384);
}

MementoPattern::~MementoPattern()
{
	SDL_DestroyTexture(texture_target);
}

void MementoPattern::on_input(const SDL_Event* event)
{
	player.on_input(event);
	_switch.on_input(event);
}

void MementoPattern::on_update(float delta)
{
	player.on_update(delta);

	if (ImGui::Button(u8"加载 \"data/save.json\" 存档文件到场景", { ImGui::GetContentRegionAvail().x / 2, 35 }))
		load_scene();
	ImGui::SameLine();
	if (ImGui::Button(u8"保存场景到 \"data/save.json\" 存档文件", { ImGui::GetContentRegionAvail().x, 35 }))
		dump_scene();

	ImGui::BeginChild("scene", ImGui::GetContentRegionAvail());
	ImGui::Image(texture_target, ImGui::GetContentRegionAvail());
	ImGui::SetCursorPos({ 10, 10 });
	ImGui::TextColored({ 1.0f, 0.35f, 0.12f, 1.0f }, u8"• 使用 上/下/左/右 方向键控制角色移动，使用 空格键 切换拉杆开关状态");
	ImGui::EndChild();
}

void MementoPattern::on_render(SDL_Renderer* renderer)
{
	SDL_SetRenderTarget(renderer, texture_target);
	SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255);
	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, ResourcesManager::instance()->find_texture("tidy_room"), nullptr, nullptr);
	_switch.on_render(renderer); player.on_render(renderer);

	SDL_SetRenderTarget(renderer, nullptr);
}

void MementoPattern::load_scene()
{
	std::ifstream file("data/save.json");
	if (!file.good()) return;

	std::stringstream str_stream;
	str_stream << file.rdbuf(); file.close();

	cJSON* json_root = cJSON_Parse(str_stream.str().c_str());
	if (!json_root) return;

	player.load(cJSON_GetObjectItem(json_root, "player"));
	_switch.load(cJSON_GetObjectItem(json_root, "switch"));

	cJSON_Delete(json_root);
}

void MementoPattern::dump_scene()
{
	std::ofstream file("data/save.json");
	if (!file.good()) return;

	cJSON* json_root = cJSON_CreateObject();

	cJSON_AddItemToObject(json_root, "player", player.dump());
	cJSON_AddItemToObject(json_root, "switch", _switch.dump());

	char* str_json = cJSON_Print(json_root);
	file << str_json; file.flush(); file.close();

	free(str_json);
	cJSON_Delete(json_root);
}
