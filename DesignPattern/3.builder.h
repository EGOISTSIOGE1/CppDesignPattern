#ifndef _BUILDER_H_
#define _BUILDER_H_

#include "example.h"
#include "animation.h"

namespace _BuilderPattern
{
	class Chicken
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

			Vector2 position = { 166, 148 };
			animation_weapon.set_position(position);
			animation_body.set_position(position);
			animation_hat.set_position(position);
		}

		~Chicken() = default;

		void set_weapon(Atlas* atlas)
		{
			animation_weapon.add_frame(atlas);
		}

		void set_body(Atlas* atlas)
		{
			animation_body.add_frame(atlas);
		}

		void set_hat(Atlas* atlas)
		{
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

	private:
		Animation animation_weapon;
		Animation animation_body;
		Animation animation_hat;

	};

	class Builder
	{
	public:
		virtual void init_weapon() = 0;
		virtual void init_body() = 0;
		virtual void init_hat() = 0;
		virtual Chicken* build() = 0;

	protected:
		Atlas atlas_weapon;
		Atlas atlas_body;
		Atlas atlas_hat;

	};

	class ElizabethChickenBuilder : public Builder
	{
	public:
		void init_weapon() override
		{
			atlas_weapon.clear();
			atlas_weapon.load("weapon_fork_%d", 8);
		}

		void init_body() override
		{
			atlas_body.clear();
			atlas_body.load("white_chicken_%d", 8);
		}

		void init_hat() override
		{
			atlas_hat.clear();
			atlas_hat.load("crown_%d", 8);
		}

		Chicken* build() override
		{
			Chicken* chicken = new Chicken();

			chicken->set_weapon(&atlas_weapon);
			chicken->set_body(&atlas_body);
			chicken->set_hat(&atlas_hat);

			return chicken;
		}

	};

	class GreenHatOriginalRecipeChickenBuilder : public Builder
	{
	public:
		void init_weapon() override
		{
			atlas_weapon.clear();
			atlas_weapon.load("weapon_plate_%d", 8);
		}

		void init_body() override
		{
			atlas_body.clear();
			atlas_body.load("brown_chicken_%d", 8);
		}

		void init_hat() override
		{
			atlas_hat.clear();
			atlas_hat.load("green_hat_%d", 8);
		}

		Chicken* build() override
		{
			Chicken* chicken = new Chicken();

			chicken->set_weapon(&atlas_weapon);
			chicken->set_body(&atlas_body);
			chicken->set_hat(&atlas_hat);

			return chicken;
		}

	};

	class CustomChickenBuilder : public Builder
	{
	public:
		void init_weapon() override;
		void init_body() override;
		void init_hat() override;
		Chicken* build() override;

	};
}

class BuilderPattern : public Example
{
public:
	BuilderPattern(SDL_Renderer* renderer);
	~BuilderPattern();

	void on_update(float delta) override;
	void on_render(SDL_Renderer* renderer) override;

private:
	SDL_Texture* texture_target = nullptr;
	_BuilderPattern::Chicken* chicken = nullptr;
	_BuilderPattern::CustomChickenBuilder custom_chicken_builder;
	_BuilderPattern::ElizabethChickenBuilder elizabeth_chicken_builder;
	_BuilderPattern::GreenHatOriginalRecipeChickenBuilder green_hat_original_recipe_chicken_builder;

};

#endif // !_BUILDER_H_
