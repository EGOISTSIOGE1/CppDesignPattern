#include "1.adapter.h"
#include "resources_manager.h"

#include <imgui.h>
#include <cJSON.h>
#include <pugixml.hpp>

#include <fstream>
#include <sstream>

using namespace _AdapterPattern;

bool JSONLoader::load(GameObjectList& dst, const std::string& path)
{
	std::ifstream file(path);
	if (!file.good())
		return false;

	std::stringstream str_stream;
	str_stream << file.rdbuf(); file.close();

	cJSON* json_root = cJSON_Parse(str_stream.str().c_str());
	if (!json_root) return false;

	cJSON* json_game_obj = nullptr;
	cJSON_ArrayForEach(json_game_obj, json_root)
	{
		cJSON* json_rect = cJSON_GetObjectItem(json_game_obj, "rect");
		cJSON* json_texture = cJSON_GetObjectItem(json_game_obj, "texture");
		cJSON* json_rotation = cJSON_GetObjectItem(json_game_obj, "rotation");
		SDL_Rect rect =
		{
			cJSON_GetObjectItem(json_rect, "x")->valueint, cJSON_GetObjectItem(json_rect, "y")->valueint,
			cJSON_GetObjectItem(json_rect, "w")->valueint, cJSON_GetObjectItem(json_rect, "h")->valueint,
		};
		dst.emplace_back(ResourcesManager::instance()->find_texture(json_texture->valuestring), rect, json_rotation->valuedouble);
	}

	cJSON_Delete(json_root);
	return true;
}

bool _AdapterPattern::XMLLoader::load(GameObjectList& dst, const std::string& path)
{
	pugi::xml_document doc;
	if (!doc.load_file(path.c_str())) return false;

	pugi::xml_node xml_root = doc.child("Scene");
	for (const pugi::xml_node& xml_game_obj : xml_root.children("GameObject"))
	{
		const pugi::xml_node& xml_rect = xml_game_obj.child("Rect");
		double rotation = xml_game_obj.attribute("rotation").as_double();
		SDL_Texture* texture = ResourcesManager::instance()->find_texture(xml_game_obj.attribute("texture").as_string());
		SDL_Rect rect =
		{
			xml_rect.attribute("x").as_int(), xml_rect.attribute("y").as_int(),
			xml_rect.attribute("w").as_int(), xml_rect.attribute("h").as_int(),
		};
		dst.emplace_back(texture, rect, rotation);
	}

	return true;
}

AdapterPattern::AdapterPattern(SDL_Renderer* renderer)
{
	texture_target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 768, 768);
}

AdapterPattern::~AdapterPattern()
{
	SDL_DestroyTexture(texture_target);
}

void AdapterPattern::on_update(float delta)
{
	if (ImGui::Button(u8"加载 \"data/json_scene.json\" 场景文件", { ImGui::GetContentRegionAvail().x / 2, 35 }))
	{
		reload_scene(&json_loader, "data/json_scene.json");
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"加载 \"data/xml_scene.xml\" 场景文件", { ImGui::GetContentRegionAvail().x, 35 }))
	{
		reload_scene(&xml_loader, "data/xml_scene.xml");
	}

	ImGui::BeginChild("scene", ImGui::GetContentRegionAvail());
	ImGui::Image(texture_target, ImGui::GetContentRegionAvail());
	ImGui::EndChild();
}

void AdapterPattern::on_render(SDL_Renderer* renderer)
{
	SDL_SetRenderTarget(renderer, texture_target);
	SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255);
	SDL_RenderClear(renderer);

	for (const GameObject& game_object : game_object_list)
	{
		SDL_RenderCopyEx(renderer, game_object.texture, nullptr, 
			&game_object.rect, game_object.rotation, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
	}

	SDL_SetRenderTarget(renderer, nullptr);
}

void AdapterPattern::reload_scene(_AdapterPattern::LoaderImpl* loader, const std::string& path)
{
	game_object_list.clear();

	if (!loader->load(game_object_list, path))
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"Reload Failed",
			u8"重新加载场景文件失败，请检查文件是否存在及文件内容是否符合标准", nullptr);
	}
}
