#ifndef _PROTOTYPE_H_
#define _PROTOTYPE_H_

#include "example.h"
#include "animation.h"

namespace _PrototypePattern
{
	class Prototype
	{
	public:
		virtual Prototype* clone() const = 0;

	};

	class Chicken : public Prototype
	{
	public:
		Chicken()
		{
			animation_weapon.set_loop(true);
			animation_body.set_loop(true);
			animation_hat.set_loop(true);

			animation_weapon.set_interval(0.1f);
			animation_body.set_interval(0.1f);
			animation_hat.set_interval(0.1f);

			Vector2 position;
			position.x = (float)(rand() % 665);
			position.y = (float)(rand() % 590);
			animation_weapon.set_position(position);
			animation_body.set_position(position);
			animation_hat.set_position(position);
		}

		~Chicken() = default;

		void set_weapon(Atlas* atlas)
		{
			atlas_weapon = atlas;
			animation_weapon.add_frame(atlas);
		}

		void set_body(Atlas* atlas)
		{
			atlas_body = atlas;
			animation_body.add_frame(atlas);
		}

		void set_hat(Atlas* atlas)
		{
			atlas_hat = atlas;
			animation_hat.add_frame(atlas);
		}

		void on_update(float delta)
		{
			animation_weapon.on_update(delta);
			animation_body.on_update(delta);
			animation_hat.on_update(delta);
		}

		void on_render(SDL_Renderer* renderer)
		{
			animation_weapon.on_render(renderer);
			animation_body.on_render(renderer);
			animation_hat.on_render(renderer);
		}

		Chicken* clone() const override
		{
			Chicken* chicken = new Chicken();

			chicken->set_weapon(atlas_weapon);
			chicken->set_body(atlas_body);
			chicken->set_hat(atlas_hat);

			return chicken;
		}

	private:
		Atlas* atlas_weapon = nullptr;
		Atlas* atlas_body = nullptr;
		Atlas* atlas_hat = nullptr;
		Animation animation_weapon;
		Animation animation_body;
		Animation animation_hat;

	};
}

class PrototypePattern : public Example
{
public:
	PrototypePattern(SDL_Renderer* renderer);
	~PrototypePattern();

	void on_enter() override;
	void on_exit() override;

	void on_update(float delta) override;
	void on_render(SDL_Renderer* renderer) override;

private:
	SDL_Texture* texture_target = nullptr;
	std::vector<_PrototypePattern::Chicken*> chicken_list;
	_PrototypePattern::Chicken* prototype_chicken = nullptr;
	Atlas atlas_weapon_fork, atlas_weapon_plate, atlas_weapon_spoon;
	Atlas atlas_body_brown, atlas_body_red, atlas_body_white;
	Atlas atlas_hat_crown, atlas_hat_green, atlas_hat_straw;

};

#endif // !_PROTOTYPE_H_
