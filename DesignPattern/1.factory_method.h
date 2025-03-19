#ifndef _FACTORY_METHOD_H_
#define _FACTORY_METHOD_H_

#include "example.h"
#include "animation.h"

namespace _FactoryMethodPattern
{
	enum class Attribute { Ice, Grass, Fire };

	class GameObject
	{
	public:
		GameObject(const Vector2& _position) : position(_position) {}

		virtual void on_update(float delta) = 0;
		virtual void on_render(SDL_Renderer* renderer) = 0;

	protected:
		Vector2 position;

	};

	class Sword : public GameObject
	{
	public:
		Sword(const Vector2& _position) : GameObject(_position) {}

		void on_update(float delta) override {}
		void on_render(SDL_Renderer* renderer) override
		{
			SDL_FRect rect = { position.x, position.y, 64, 64 };
			SDL_RenderCopyF(renderer, texture, nullptr, &rect);
		}

	protected:
		SDL_Texture* texture = nullptr;

	};

	class Slime : public GameObject
	{
	public:
		Slime(const Vector2& _position) : GameObject(_position) {}

		void on_update(float delta) override
		{
			animation.on_update(delta);
			animation.set_position(position);
		}

		void on_render(SDL_Renderer* renderer) override
		{
			animation.on_render(renderer);
		}

	protected:
		Atlas atlas;
		Animation animation;

	};

	class IceSword : public Sword
	{
	public:
		IceSword(const Vector2& _position) : Sword(_position)
		{
			texture = ResourcesManager::instance()->find_texture("BlueSword");
		}

	};

	class GrassSword : public Sword
	{
	public:
		GrassSword(const Vector2& _position) : Sword(_position)
		{
			texture = ResourcesManager::instance()->find_texture("GreenSword");
		}

	};

	class FireSword : public Sword
	{
	public:
		FireSword(const Vector2& _position) : Sword(_position)
		{
			texture = ResourcesManager::instance()->find_texture("RedSword");
		}

	};

	class IceSlime : public Slime
	{
	public:
		IceSlime(const Vector2& _position) : Slime(_position)
		{
			atlas.load("MegaSlimeBlue_0%d", 6);
			animation.add_frame(&atlas);
			animation.set_loop(true);
			animation.set_interval(0.1f);
		}

	};

	class GrassSlime : public Slime
	{
	public:
		GrassSlime(const Vector2& _position) : Slime(_position)
		{
			atlas.load("MegaSlimeGreen_0%d", 6);
			animation.add_frame(&atlas);
			animation.set_loop(true);
			animation.set_interval(0.1f);
		}

	};

	class FireSlime : public Slime
	{
	public:
		FireSlime(const Vector2& _position) : Slime(_position)
		{
			atlas.load("MegaSlimeRed_0%d", 6);
			animation.add_frame(&atlas);
			animation.set_loop(true);
			animation.set_interval(0.1f);
		}

	};

	class SwordFactory
	{
	public:
		Sword* create(Attribute attribute, const Vector2& position)
		{
			switch (attribute)
			{
			case _FactoryMethodPattern::Attribute::Ice:
				return new IceSword(position);
				break;
			case _FactoryMethodPattern::Attribute::Grass:
				return new GrassSword(position);
				break;
			case _FactoryMethodPattern::Attribute::Fire:
				return new FireSword(position);
				break;
			default:
				return nullptr;
				break;
			}
		}
	};

	class SlimeFactory
	{
	public:
		Slime* create(Attribute attribute, const Vector2& position)
		{
			switch (attribute)
			{
			case _FactoryMethodPattern::Attribute::Ice:
				return new IceSlime(position);
				break;
			case _FactoryMethodPattern::Attribute::Grass:
				return new GrassSlime(position);
				break;
			case _FactoryMethodPattern::Attribute::Fire:
				return new FireSlime(position);
				break;
			default:
				return nullptr;
				break;
			}
		}
	};
}

class FactoryMethodPattern : public Example
{
public:
	FactoryMethodPattern(SDL_Renderer* renderer);
	~FactoryMethodPattern();

	void on_update(float delta) override;
	void on_render(SDL_Renderer* renderer) override;

private:
	SDL_Texture* texture_target = nullptr;
	std::vector<_FactoryMethodPattern::GameObject*> game_obj_list;

};

#endif // !_FACTORY_METHOD_H_
