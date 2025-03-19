#ifndef _DECORATOR_H_
#define _DECORATOR_H_

#include "vector2.h"
#include "example.h"
#include "resources_manager.h"

#include <vector>
#include <functional>

namespace _DecoratorPattern
{
	class Character
	{
	public:
		virtual ~Character() {};
		virtual void on_input(const SDL_Event* event) {}
		virtual void on_update(float delta) {}
		virtual void on_render(SDL_Renderer* renderer) {}

		virtual void set_speed(float speed) { this->speed = speed; }
		virtual void set_size(const Vector2& size) { this->size = size; }
		virtual void set_texture(SDL_Texture* texture) { this->texture = texture; }

		virtual const Vector2& get_size() const { return size; }
		virtual const Vector2& get_position() const { return position; }

	protected:
		Vector2 size;
		Vector2 position;
		float speed = 2.0f;
		SDL_Texture* texture = nullptr;

	};

	class Player : public Character
	{
	public:
		Player()
		{
			size = { 66, 66 };
			position = { 360, 360 };
			texture = ResourcesManager::instance()->find_texture("small_plane");
		}

		~Player() = default;

		void on_input(const SDL_Event* event) override
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

		void on_update(float delta) override
		{
			Vector2 direction = Vector2((float)(is_move_right - is_move_left), (float)(is_move_down - is_move_up)).normalize();
			position = position + direction * speed;
		}

		void on_render(SDL_Renderer* renderer) override
		{
			SDL_FRect rect =
			{
				position.x - size.x / 2,
				position.y - size.y / 2,
				get_size().x, get_size().y
			};
			SDL_RenderCopyF(renderer, texture, nullptr, &rect);
		}

	private:
		bool is_move_up = false, is_move_down = false;
		bool is_move_left = false, is_move_right = false;

	};

	class SpeedUpDecorator : public Character
	{
	public:
		SpeedUpDecorator(Character* character)
		{
			this->player = character;
		}

		~SpeedUpDecorator() = default;

		void on_input(const SDL_Event* event) override { player->on_input(event); }

		void on_update(float delta) override
		{
			player->set_speed(4.0f);

			player->on_update(delta);
		}

		void on_render(SDL_Renderer* renderer) override { player->on_render(renderer); }

		void set_speed(float speed) override { player->set_speed(speed); }
		void set_size(const Vector2& size) override { player->set_size(size); }
		void set_texture(SDL_Texture* texture) override { player->set_texture(texture); }

		const Vector2& get_size() const override { return player->get_size(); }
		const Vector2& get_position() const override { return player->get_position(); }

	private:
		Character* player = nullptr;

	};

	class SizeGrownDecorator : public Character
	{
	public:
		SizeGrownDecorator(Character* character)
		{
			this->player = character;
		}

		~SizeGrownDecorator() = default;

		void on_input(const SDL_Event* event) override { player->on_input(event); }

		void on_update(float delta) override
		{
			player->set_size({ 96, 96 });

			player->on_update(delta);
		}

		void on_render(SDL_Renderer* renderer) override { player->on_render(renderer); }

		void set_speed(float speed) override { player->set_speed(speed); }
		void set_size(const Vector2& size) override { player->set_size(size); }
		void set_texture(SDL_Texture* texture) override { player->set_texture(texture); }

		const Vector2& get_size() const override { return player->get_size(); }
		const Vector2& get_position() const override { return player->get_position(); }

	private:
		Character* player = nullptr;

	};

	class AppearanceChangedDecorator : public Character
	{
	public:
		AppearanceChangedDecorator(Character* character)
		{
			this->player = character;
		}

		~AppearanceChangedDecorator() = default;

		void on_input(const SDL_Event* event) override { player->on_input(event); }

		void on_update(float delta) override
		{
			player->set_texture(ResourcesManager::instance()->find_texture("large_plane"));

			player->on_update(delta);
		}

		void on_render(SDL_Renderer* renderer) override { player->on_render(renderer); }

		void set_speed(float speed) override { player->set_speed(speed); }
		void set_size(const Vector2& size) override { player->set_size(size); }
		void set_texture(SDL_Texture* texture) override { player->set_texture(texture); }

		const Vector2& get_size() const override { return player->get_size(); }
		const Vector2& get_position() const override { return player->get_position(); }

	private:
		Character* player = nullptr;

	};

	class Prop
	{
	public:
		using CollideCallback = std::function<void()>;

	public:
		Prop(const Vector2& _position, SDL_Texture* _texture, CollideCallback _callback)
			: position(_position), texture(_texture), callback(_callback) {}
		~Prop() = default;

		void on_render(SDL_Renderer* renderer) const
		{
			SDL_FRect rect = { position.x - 16, position.y - 16, 32, 32 };
			SDL_RenderCopyF(renderer, texture, nullptr, &rect);
		}

		void check_collide(const Character* character)
		{
			if (!is_valid) return;

			SDL_FPoint point = { position.x, position.y };
			SDL_FRect rect = 
			{ 
				character->get_position().x - character->get_size().x / 2, 
				character->get_position().y - character->get_size().y / 2, 
				character->get_size().x, character->get_size().y 
			};
			if (SDL_PointInFRect(&point, &rect))
			{
				callback();
				is_valid = false;
			}
		}

		bool can_remove() const
		{
			return !is_valid;
		}

	private:
		Vector2 position;
		bool is_valid = true;
		CollideCallback callback;
		SDL_Texture* texture = nullptr;

	};
}

class DecoratorPattern : public Example
{
public:
	DecoratorPattern(SDL_Renderer* renderer);
	~DecoratorPattern();

	void on_input(const SDL_Event* event) override;
	void on_update(float delta) override;
	void on_render(SDL_Renderer* renderer) override;

private:
	SDL_Texture* texture_target = nullptr;
	_DecoratorPattern::Character* player = nullptr;
	std::vector<_DecoratorPattern::Prop*> prop_list;

};

#endif // !_DECORATOR_H_
