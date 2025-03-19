#ifndef _STRATEGY_H_
#define _STRATEGY_H_

#include "example.h"
#include "animation.h"

namespace _StrategyPattern
{
	class MoveStrategy
	{
	public:
		virtual ~MoveStrategy() = default;
		virtual bool need_stop(const Vector2& pos_target, const Vector2& pos_self) = 0;
		virtual Vector2 cal_direction(const Vector2& pos_target, const Vector2& pos_self) = 0;

	};

	class FollowStrategy : public MoveStrategy
	{
	public:
		bool need_stop(const Vector2& pos_target, const Vector2& pos_self) override
		{
			return (pos_target - pos_self).length() <= 150.0f;
		}

		Vector2 cal_direction(const Vector2& pos_target, const Vector2& pos_self) override
		{
			return (pos_target - pos_self).normalize();
		}

	};

	class StayAwayStrategy : public MoveStrategy
	{
	public:
		bool need_stop(const Vector2& pos_target, const Vector2& pos_self) override
		{
			return (pos_target - pos_self).length() >= 250.0f;
		}

		Vector2 cal_direction(const Vector2& pos_target, const Vector2& pos_self) override
		{
			return (pos_self - pos_target).normalize();
		}

	};

	class Player
	{
	public:
		Player()
		{
			atlas.load("player_right_%d", 5);

			animation.add_frame(&atlas);
			animation.set_loop(true);
			animation.set_interval(0.1f);

			position = { 360, 360 };
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
			if (std::abs(direction.x) > 0.0001f) is_facing_right = direction.x > 0;
			position = position + direction * speed;

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

		const Vector2& get_position() const
		{
			return position;
		}

	private:
		Atlas atlas;
		Vector2 position;
		Animation animation;
		bool is_facing_right = true;
		bool is_move_up = false, is_move_down = false;
		bool is_move_left = false, is_move_right = false;

	};

	class Boar
	{
	public:
		Boar(Player* player, Vector2 position)
		{
			this->player = player;
			this->position = position;

			atlas_idle.load("Boar-Idle%d", 4);
			animation_idle.add_frame(&atlas_idle);
			animation_idle.set_loop(true);
			animation_idle.set_interval(0.1f);

			atlas_run.load("Boar-Run%d", 6);
			animation_run.add_frame(&atlas_run);
			animation_run.set_loop(true);
			animation_run.set_interval(0.1f);

			current_animation = &animation_idle;
		}

		~Boar() = default;

		void on_update(float delta)
		{
			if (move_strategy->need_stop(player->get_position(), position))
				current_animation = &animation_idle;
			else
			{
				static const float speed = 1.0f;
				const Vector2 move_dir = move_strategy->cal_direction(player->get_position(), position);
				if (std::abs(move_dir.x) > 0.0001f) is_facing_right = move_dir.x > 0;
				position = position + move_dir * speed;
				current_animation = &animation_run;
			}

			current_animation->on_update(delta);
			current_animation->set_position(position);
			current_animation->set_flip(is_facing_right ? SDL_RendererFlip::SDL_FLIP_HORIZONTAL : SDL_RendererFlip::SDL_FLIP_NONE);
		}

		void on_render(SDL_Renderer* renderer)
		{
			SDL_FRect rect = { position.x - 16, position.y + 20, 32, 20 };
			SDL_RenderCopyF(renderer, ResourcesManager::instance()->find_texture("shadow_player"), nullptr, &rect);

			current_animation->on_render(renderer);
		}

		void set_strategy(MoveStrategy* move_strategy)
		{
			this->move_strategy = move_strategy;
		}

	private:
		Vector2 position;
		Player* player = nullptr;
		MoveStrategy* move_strategy = nullptr;

		bool is_facing_right = true;
		Atlas atlas_idle, atlas_run;
		Animation animation_idle, animation_run;
		Animation* current_animation = nullptr;

	};

}

class StrategyPattern : public Example
{
public:
	StrategyPattern(SDL_Renderer* renderer);
	~StrategyPattern();

	void on_input(const SDL_Event* event) override;
	void on_update(float delta) override;
	void on_render(SDL_Renderer* renderer) override;

private:
	_StrategyPattern::Player player;
	SDL_Texture* texture_target = nullptr;
	std::vector<_StrategyPattern::Boar*> boar_list;

	bool is_using_follow_strategy = true;
	_StrategyPattern::FollowStrategy follow_strategy;
	_StrategyPattern::StayAwayStrategy stay_away_strategy;

};

#endif // !_STRATEGY_H_
