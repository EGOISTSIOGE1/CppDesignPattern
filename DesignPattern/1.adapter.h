#ifndef _ADAPTER_H_
#define _ADAPTER_H_

#include "example.h"

#include <string>
#include <vector>

namespace _AdapterPattern
{
	struct GameObject
	{
		double rotation = 0;
		SDL_Rect rect = { 0 };
		SDL_Texture* texture = nullptr;

		GameObject(SDL_Texture* _texture, const SDL_Rect& _rect, double _rotation)
			: texture(_texture), rect(_rect), rotation(_rotation) {}
	};

	using GameObjectList = std::vector<GameObject>;

	class LoaderImpl
	{
	public:
		virtual bool load(GameObjectList& dst, const std::string& path) = 0;

	};

	class JSONLoader : public LoaderImpl
	{
	public:
		bool load(GameObjectList& dst, const std::string& path) override;

	};

	class XMLLoader : public LoaderImpl
	{
	public:
		bool load(GameObjectList& dst, const std::string& path) override;

	};
}

class AdapterPattern : public Example
{
public:
	AdapterPattern(SDL_Renderer* renderer);
	~AdapterPattern();

	void on_update(float delta) override;
	void on_render(SDL_Renderer* renderer) override;

private:
	SDL_Texture* texture_target = nullptr;
	_AdapterPattern::XMLLoader xml_loader;
	_AdapterPattern::JSONLoader json_loader;
	_AdapterPattern::GameObjectList game_object_list;

private:
	void reload_scene(_AdapterPattern::LoaderImpl* loader, const std::string& path);

};

#endif // !_ADAPTER_H_
