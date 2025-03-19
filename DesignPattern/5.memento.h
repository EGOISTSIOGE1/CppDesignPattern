#ifndef _MEMENTO_H_
#define _MEMENTO_H_

#include "example.h"
#include "animation.h"

#include <cJSON.h>

namespace _MementoPattern
{
	class Serializable
	{
	public:
		virtual ~Serializable() = default;

		virtual cJSON* dump() = 0;
		virtual void load(const cJSON* json) = 0;

	};

	class Player : public Serializable
	{
	public:
		Player()
		{
			atlas.load("player_right_%d", 5);

			animation.add_frame(&atlas);
			animation.set_loop(true);
			animation.set_interval(0.1f);

			position = { 100, 200 };
		}

		~Player() = default;

		void on_input(const SDL_Event* event)
		{
			switch (event->type)
			{
			case SDL_KEYDOWN:
				switch (event->key.keysym.sym)
				{
				case SDLK_UP:		is_move_up = true;		break;
				case SDLK_DOWN:		is_move_down = true;	break;
				case SDLK_LEFT:		is_move_left = true;	break;
				case SDLK_RIGHT:	is_move_right = true;	break;
				default: break;
				}
				break;
			case SDL_KEYUP:
				switch (event->key.keysym.sym)
				{
				case SDLK_UP:		is_move_up = false;		break;
				case SDLK_DOWN:		is_move_down = false;	break;
				case SDLK_LEFT:		is_move_left = false;	break;
				case SDLK_RIGHT:	is_move_right = false;	break;
				default: break;
				}
				break;
			default: break;
			}
		}

		void on_update(float delta)
		{
			static const float speed = 2.0f;
			Vector2 direction = Vector2((float)(is_move_right - is_move_left), (float)(is_move_down - is_move_up)).normalize();
			if (std::abs(direction.x) > 0.0001f)
				is_facing_right = direction.x > 0;
			position = position + direction * speed;

			if (position.y < 120) position.y = 120;
			if (position.y > 384) position.y = 384;
			if (position.x < 0) position.x = 0;
			if (position.x > 384) position.x = 384;

			animation.on_update(delta);
			animation.set_position(position);
			animation.set_flip(is_facing_right ? SDL_RendererFlip::SDL_FLIP_NONE : SDL_RendererFlip::SDL_FLIP_HORIZONTAL);
		}

		void on_render(SDL_Renderer* renderer)
		{
			SDL_FRect rect = { position.x - 16, position.y + 30, 32, 20 };
			SDL_RenderCopyF(renderer, ResourcesManager::instance()->find_texture("shadow_player"), nullptr, &rect);

			animation.on_render(renderer);
		}

		cJSON* dump() override
		{
			cJSON* json_root = cJSON_CreateObject();

			cJSON* json_position = cJSON_AddObjectToObject(json_root, "position");
			cJSON_AddNumberToObject(json_position, "x", position.x);
			cJSON_AddNumberToObject(json_position, "y", position.y);

			cJSON_AddBoolToObject(json_root, "facing_right", is_facing_right);

			return json_root;
		}

		void load(const cJSON* json) override
		{
			cJSON* json_position = cJSON_GetObjectItem(json, "position");
			position.x = (float)cJSON_GetObjectItem(json_position, "x")->valuedouble;
			position.y = (float)cJSON_GetObjectItem(json_position, "y")->valuedouble;

			is_facing_right = cJSON_GetObjectItem(json, "facing_right")->valueint;
		}

	private:
		Atlas atlas;
		Vector2 position;
		Animation animation;
		bool is_facing_right = true;
		bool is_move_up = false, is_move_down = false;
		bool is_move_left = false, is_move_right = false;

	};

	class Switch : public Serializable
	{
	public:
		Switch() = default;
		~Switch() = default;

		void on_input(const SDL_Event* event)
		{
			switch (event->type)
			{
			case SDL_KEYDOWN:
				switch (event->key.keysym.sym)
				{
				case SDLK_SPACE: 
					is_opened = !is_opened; 
					Mix_PlayChannel(-1, ResourcesManager::instance()->find_audio("switch"), 0);
					break;
				default: break;
				}
				break;
			default: break;
			}
		}

		void on_render(SDL_Renderer* renderer)
		{
			static const SDL_FRect rect = { 215, 250, 122, 88 };
			SDL_RenderCopyF(renderer, ResourcesManager::instance()
				->find_texture(is_opened ? "bar_opened" : "bar_closed"), nullptr, &rect);
		}

		cJSON* dump() override
		{
			cJSON* json_root = cJSON_CreateObject();
			cJSON_AddBoolToObject(json_root, "opened", is_opened);
			return json_root;
		}

		void load(const cJSON* json) override
		{
			cJSON* json_position = cJSON_GetObjectItem(json, "position");
			is_opened = cJSON_GetObjectItem(json, "opened")->valueint;
		}

	private:
		bool is_opened = false;

	};
}

class MementoPattern : public Example
{
public:
	MementoPattern(SDL_Renderer* renderer);
	~MementoPattern();

	void on_input(const SDL_Event* event) override;
	void on_update(float delta) override;
	void on_render(SDL_Renderer* renderer) override;

private:
	_MementoPattern::Player player;
	_MementoPattern::Switch _switch;
	SDL_Texture* texture_target = nullptr;

private:
	void load_scene();
	void dump_scene();

};

#endif // !_MEMENTO_H_
