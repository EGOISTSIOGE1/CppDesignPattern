#ifndef _ABSTRACT_FACTORY_H_
#define _ABSTRACT_FACTORY_H_

#include "example.h"
#include "animation.h"

namespace _AbstractFactoryPattern
{
	enum class Size { Small, Medium, Large };
	enum class Attribute { Ice, Grass, Fire };

	class SizeDef
	{
	public:
		virtual Size get_size() = 0;

	};

	class AttributeDef
	{
	public:
		virtual Attribute get_attribute() = 0;

	};

	class SmallSizeDef : public SizeDef
	{
	public:
		Size get_size() override
		{
			return Size::Small;
		}

	};

	class MediumSizeDef : public SizeDef
	{
	public:
		Size get_size() override
		{
			return Size::Medium;
		}

	};

	class LargeSizeDef : public SizeDef
	{
	public:
		Size get_size() override
		{
			return Size::Large;
		}

	};

	class IceAttributeDef : public AttributeDef
	{
	public:
		Attribute get_attribute() override
		{
			return Attribute::Ice;
		}

	};

	class GrassAttributeDef : public AttributeDef
	{
	public:
		Attribute get_attribute() override
		{
			return Attribute::Grass;
		}

	};

	class FireAttributeDef : public AttributeDef
	{
	public:
		Attribute get_attribute() override
		{
			return Attribute::Fire;
		}

	};

	class SlimeDefFactory
	{
	public:
		virtual SizeDef* get_size_def() = 0;
		virtual AttributeDef* get_attribute_def() = 0;

	};

	class SmallIceSlimeDefFactory : public SlimeDefFactory
	{
	public:
		SizeDef* get_size_def() override
		{
			return &small_size_def;
		}

		AttributeDef* get_attribute_def() override
		{
			return &ice_attribute_def;
		}

	private:
		SmallSizeDef small_size_def;
		IceAttributeDef ice_attribute_def;

	};

	class MediumIceSlimeDefFactory : public SlimeDefFactory
	{
	public:
		SizeDef* get_size_def() override
		{
			return &medium_size_def;
		}

		AttributeDef* get_attribute_def() override
		{
			return &ice_attribute_def;
		}

	private:
		MediumSizeDef medium_size_def;
		IceAttributeDef ice_attribute_def;

	};

	class LargeIceSlimeDefFactory : public SlimeDefFactory
	{
	public:
		SizeDef* get_size_def() override
		{
			return &large_size_def;
		}

		AttributeDef* get_attribute_def() override
		{
			return &ice_attribute_def;
		}

	private:
		LargeSizeDef large_size_def;
		IceAttributeDef ice_attribute_def;

	};

	class SmallGrassSlimeDefFactory : public SlimeDefFactory
	{
	public:
		SizeDef* get_size_def() override
		{
			return &small_size_def;
		}

		AttributeDef* get_attribute_def() override
		{
			return &grass_attribute_def;
		}

	private:
		SmallSizeDef small_size_def;
		GrassAttributeDef grass_attribute_def;

	};

	class MediumGrassSlimeDefFactory : public SlimeDefFactory
	{
	public:
		SizeDef* get_size_def() override
		{
			return &medium_size_def;
		}

		AttributeDef* get_attribute_def() override
		{
			return &grass_attribute_def;
		}

	private:
		MediumSizeDef medium_size_def;
		GrassAttributeDef grass_attribute_def;

	};

	class LargeGrassSlimeDefFactory : public SlimeDefFactory
	{
	public:
		SizeDef* get_size_def() override
		{
			return &large_size_def;
		}

		AttributeDef* get_attribute_def() override
		{
			return &grass_attribute_def;
		}

	private:
		LargeSizeDef large_size_def;
		GrassAttributeDef grass_attribute_def;

	};

	class SmallFireSlimeDefFactory : public SlimeDefFactory
	{
	public:
		SizeDef* get_size_def() override
		{
			return &small_size_def;
		}

		AttributeDef* get_attribute_def() override
		{
			return &fire_attribute_def;
		}

	private:
		SmallSizeDef small_size_def;
		FireAttributeDef fire_attribute_def;

	};

	class MediumFireSlimeDefFactory : public SlimeDefFactory
	{
	public:
		SizeDef* get_size_def() override
		{
			return &medium_size_def;
		}

		AttributeDef* get_attribute_def() override
		{
			return &fire_attribute_def;
		}

	private:
		MediumSizeDef medium_size_def;
		FireAttributeDef fire_attribute_def;

	};

	class LargeFireSlimeDefFactory : public SlimeDefFactory
	{
	public:
		SizeDef* get_size_def() override
		{
			return &large_size_def;
		}

		AttributeDef* get_attribute_def() override
		{
			return &fire_attribute_def;
		}

	private:
		LargeSizeDef large_size_def;
		FireAttributeDef fire_attribute_def;

	};

	class Slime
	{
	public:
		Slime(SlimeDefFactory* factory, const Vector2& position);
		~Slime();

		void on_update(float delta)
		{
			animation.on_update(delta);
			animation.set_position(position);
		}

		void on_render(SDL_Renderer* renderer)
		{
			animation.on_render(renderer);
		}

	private:
		Atlas atlas;
		Vector2 position;
		Animation animation;

	};
}

class AbstractFactoryPattern : public Example
{
public:
	AbstractFactoryPattern(SDL_Renderer* renderer);
	~AbstractFactoryPattern();

	void on_update(float delta) override;
	void on_render(SDL_Renderer* renderer) override;

private:
	SDL_Texture* texture_target = nullptr;
	std::vector<_AbstractFactoryPattern::Slime*> slime_list;

};

#endif // !_ABSTRACT_FACTORY_H_
