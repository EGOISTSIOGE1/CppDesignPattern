#ifndef _TEMPLATE_METHOD_H_
#define _TEMPLATE_METHOD_H_

#include "example.h"
#include "animation.h"

namespace _TemplateMethodPattern
{
	class Item
	{
	public:
		virtual ~Item() = default;

		virtual void on_update(float delta) {}
		virtual void on_render(SDL_Renderer* renderer) = 0;

		void set_position(const Vector2& position)
		{
			this->position = position;
		}

	protected:
		Vector2 position;

	};

	class StaticItem : public Item
	{
	public:
		StaticItem(const std::string& texture_id)
		{
			texture = ResourcesManager::instance()->find_texture(texture_id);
		}

		~StaticItem() = default;

		void on_render(SDL_Renderer* renderer) override
		{
			SDL_FRect rect = { position.x - 20, position.y - 20, 40, 40 };
			SDL_RenderCopyF(renderer, texture, nullptr, &rect);
		}

	private:
		SDL_Texture* texture = nullptr;

	};

	class DynamicItem : public Item
	{
	public:
		DynamicItem(const std::string& atlas_temp, int num)
		{
			atlas.load(atlas_temp.c_str(), num);
			animation.set_loop(true);
			animation.set_interval(0.4f / num);
			animation.add_frame(&atlas);
		}

		~DynamicItem() = default;

		void on_update(float delta) override
		{
			animation.on_update(delta);
			animation.set_position(position);
		}

		void on_render(SDL_Renderer* renderer) override
		{
			animation.on_render(renderer);
		}

	private:
		Atlas atlas;
		Animation animation;

	};

	class Chest : public StaticItem
	{
	public:
		Chest() : StaticItem("chest_full_open_anim_f1") {}
		~Chest() = default;

	};

	class Stairs : public StaticItem
	{
	public:
		Stairs() : StaticItem("floor_stairs") {}
		~Stairs() = default;

	};

	class Hole : public StaticItem
	{
	public:
		Hole() : StaticItem("hole") {}
		~Hole() = default;

	};

	class Elf : public DynamicItem
	{
	public:
		Elf() : DynamicItem("elf_f_idle_anim_f%d", 4) {}
		~Elf() = default;

	};

	class Goblin : public DynamicItem
	{
	public:
		Goblin() : DynamicItem("goblin_idle_anim_f%d", 4) {}
		~Goblin() = default;

	};

	class Lizard : public DynamicItem
	{
	public:
		Lizard() : DynamicItem("lizard_f_idle_anim_f%d", 4) {}
		~Lizard() = default;

	};

	class Spikes : public DynamicItem
	{
	public:
		Spikes() : DynamicItem("floor_spikes_anim_f%d", 2) {}
		~Spikes() = default;

	};

	class Map
	{
	public:
		Map()
		{
			grids = std::vector<std::vector<Item*>>(15, std::vector<Item*>(16, nullptr));
		}

		~Map()
		{
			for (const std::vector<Item*>& line : grids)
			{
				for (Item* item : line)
					delete item;
			}
		}

		void add_item(Item* item, const SDL_Point& idx)
		{
			delete grids[idx.y][idx.x];
			grids[idx.y][idx.x] = item;

			const Vector2 position = 
			{ 
				(idx.x + 1) * 40.f + 20,
				(idx.y + 2) * 40.f + 20 
			};
			item->set_position(position);
		}

		const std::vector<SDL_Point>& get_empty_grids()
		{
			empty_grid_idx_list.clear();

			for (int y = 0; y < 15; y++)
				for (int x = 0; x < 16; x++)
					if (!grids[y][x]) empty_grid_idx_list.push_back({ x, y });

			return empty_grid_idx_list;
		}

		void on_update(float delta)
		{
			for (int y = 0; y < 15; y++)
				for (int x = 0; x < 16; x++)
					if (grids[y][x]) grids[y][x]->on_update(delta);
		}

		void on_render(SDL_Renderer* renderer)
		{
			if (need_update_background)
			{
				update_texture_background(renderer);
				need_update_background = false;
			}

			SDL_RenderCopy(renderer, texture_backgorund, nullptr, nullptr);
			for (int y = 14; y >= 0; y--)
			{
				for (int x = 0; x < 16; x++)
					if (grids[y][x]) grids[y][x]->on_render(renderer);
			}
			SDL_RenderCopy(renderer, ResourcesManager::instance()->find_texture("dungeon_background"), nullptr, nullptr);
		}

	private:
		bool need_update_background = true;
		SDL_Texture* texture_backgorund = nullptr;

		std::vector<std::vector<Item*>> grids;
		std::vector<SDL_Point> empty_grid_idx_list;

	private:
		void update_texture_background(SDL_Renderer* renderer)
		{
			SDL_DestroyTexture(texture_backgorund);
			texture_backgorund = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 720, 720);

			SDL_SetRenderTarget(renderer, texture_backgorund);
			for (int y = 0; y < 15; y++)
			{
				for (int x = 0; x < 16; x++)
				{
					SDL_Texture* texture_floor = nullptr;
					if (rand() % 100 < 10)
						texture_floor = ResourcesManager::instance()->find_texture((rand() % 2) ? "floor_2" : "floor_3");
					else
						texture_floor = ResourcesManager::instance()->find_texture("floor_1");

					SDL_Rect rect = { (x + 1) * 40, (y + 2) * 40, 40, 40 };
					SDL_RenderCopy(renderer, texture_floor, nullptr, &rect);
				}
			}
			SDL_SetRenderTarget(renderer, nullptr);
		}

	};

	class MapGenTemplate
	{
	public:
		virtual ~MapGenTemplate() = default;

		Map* generate()
		{
			Map* map = new Map();

			generate_exit(map);
			generate_trap(map);
			generate_reward(map);

			generate_npc(map);
			generate_enemy(map);
			
			return map;
		}

	protected:
		virtual void generate_npc(Map* map) = 0;
		virtual void generate_exit(Map* map) = 0;
		virtual void generate_trap(Map* map) = 0;
		virtual void generate_enemy(Map* map) = 0;
		virtual void generate_reward(Map* map) = 0;

	};

	class CustomizedMapGen : public MapGenTemplate
	{
	public:
		CustomizedMapGen() = default;
		~CustomizedMapGen() = default;

		void on_inspect_config();

	protected:
		void generate_npc(Map* map) override;
		void generate_exit(Map* map) override;
		void generate_trap(Map* map) override;
		void generate_enemy(Map* map) override;
		void generate_reward(Map* map) override;

	private:
		int num_hole = 2;
		int num_spikes = 4;
		int num_goblin = 6;
		int num_lizard = 3;
		bool has_npc = true;
		bool has_exit = true;
		bool has_reward = true;

	};

}

class TemplateMethodPattern : public Example
{
public:
	TemplateMethodPattern(SDL_Renderer* renderer);
	~TemplateMethodPattern();

	void on_update(float delta) override;
	void on_render(SDL_Renderer* renderer) override;

private:
	SDL_Texture* texture_target = nullptr;
	_TemplateMethodPattern::Map* map = nullptr;
	_TemplateMethodPattern::CustomizedMapGen map_generator;

};

#endif // !_TEMPLATE_METHOD_H_
