#ifndef _RESOURCES_MANAGER_H_
#define _RESOURCES_MANAGER_H_

#include "font_wrapper.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <string>
#include <vector>
#include <functional>
#include <filesystem>
#include <unordered_map>

class ResourcesManager
{
public:
	using ResIDList = std::vector<std::string>;

public:
	static ResourcesManager* instance()
	{
		if (!manager)
			manager = new ResourcesManager();

		return manager;
	}

	void load(SDL_Renderer* renderer);

	FontWrapper* find_font(const std::string& id) { return font_pool[id]; }
	Mix_Chunk* find_audio(const std::string& id) { return audio_pool[id]; }
	SDL_Texture* find_texture(const std::string& id) { return texture_pool[id]; }
	
	const ResIDList& get_font_resid_list() const { return font_resid_list; }
	const ResIDList& get_audio_resid_list() const { return audio_resid_list; }
	const ResIDList& get_texture_resid_list() const { return texture_resid_list; }

private:
	static ResourcesManager* manager;
	SDL_Renderer* renderer = nullptr;
	std::unordered_map<std::string, FontWrapper*> font_pool;
	std::unordered_map<std::string, Mix_Chunk*> audio_pool;
	std::unordered_map<std::string, SDL_Texture*> texture_pool;
	ResIDList font_resid_list, audio_resid_list, texture_resid_list;
	std::unordered_map<std::string, std::function<void(const std::filesystem::path& path)>> loader_pool;

private:
	ResourcesManager();
	~ResourcesManager();

};

#endif // !_RESOURCES_MANAGER_H_
