#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "example.h"
#include "animation.h"

namespace _CommandPattern
{
	struct Map
	{
		enum class GridType
		{
			Goal,
			Accessible,
			Inaccessible
		};

		std::vector<std::vector<GridType>> grids;
	};

	class Command
	{
	public:
		virtual ~Command() {}
		virtual void execute() = 0;

	};

	class Player
	{
	public:
		Player(Map* _map) : map(_map)
		{
			reset();
		
			replay_timer.set_wait_time(0.25f);
			replay_timer.set_one_shot(false);
			replay_timer.set_on_timeout([&]()
				{
					if (is_replaying && idx_cmd < cmd_list.size())
					{
						cmd_list[idx_cmd]->execute();
						idx_cmd++;
					}
				});

			atlas.load("Warrior_Idle_%d", 6);
			animation.set_loop(true);
			animation.set_interval(0.1f);
			animation.add_frame(&atlas);
		}
		~Player()
		{
			for (Command* cmd : cmd_list)
				delete cmd;
		}

		void on_input(const SDL_Event* event)
		{
			if (is_replaying) return;

			switch (event->type)
			{
			case SDL_KEYDOWN:
				switch (event->key.keysym.sym)
				{
				case SDLK_UP:		on_move({ 0, -1 });		break;
				case SDLK_DOWN:		on_move({ 0, 1 });		break;
				case SDLK_LEFT:		on_move({ -1, 0 });		break;
				case SDLK_RIGHT:	on_move({ 1, 0 });		break;
				default: break;
				}
				break;
			default: break;
			}
		}

		void on_update(float delta)
		{
			Vector2 position =
			{
				idx_map_grid.x * 48.0f + 24.0f,
				idx_map_grid.y * 48.0f + 24.0f - 30.0f 
			};
			animation.set_position(position);
			animation.on_update(delta);

			replay_timer.on_update(delta);
		}

		void on_render(SDL_Renderer* renderer)
		{
			animation.on_render(renderer);
		}

		void on_move(const SDL_Point& dir);

		void reset();
		void replay();

		bool can_replay() const { return is_achieved_goal; }

	private:
		Map* map = nullptr;
		Timer replay_timer;
		bool is_replaying = false;
		bool is_achieved_goal = false;

		int idx_cmd = 0;
		std::vector<Command*> cmd_list;

		Atlas atlas;
		Animation animation;
		SDL_Point idx_map_grid = { 0 };

	};

	class MoveCommand : public Command
	{
	public:
		MoveCommand(Player* _player, const SDL_Point& dir) 
			: player(_player), direction(dir) {}
		~MoveCommand() = default;

		void execute() override
		{
			player->on_move(direction);
		}

	private:
		Player* player = nullptr;
		SDL_Point direction = { 0 };

	};
}

class CommandPattern : public Example
{
public:
	CommandPattern(SDL_Renderer* renderer);
	~CommandPattern();

	void on_input(const SDL_Event* event) override;
	void on_update(float delta) override;
	void on_render(SDL_Renderer* renderer) override;

private:
	_CommandPattern::Map map;
	SDL_Texture* texture_target = nullptr;
	_CommandPattern::Player* player = nullptr;

};

#endif // !_COMMAND_H_
