#ifndef _ITERATOR_H_
#define _ITERATOR_H_

#include "example.h"
#include "resources_manager.h"

#include <random>
#include <string>
#include <vector>
#include <algorithm>

namespace _IteratorPattern
{
	class Item
	{
	public:
		enum class Type
		{
			Materials,
			Props,
			Money
		};

		enum class Grade
		{
			Ordinary,
			Rare,
			Legendary
		};

	public:
		Item() = default;
		virtual ~Item() = default;

		const std::string& get_name() const { return name; }
		Type get_type() const { return type; } 
		Grade get_grade() const { return grade; }
		SDL_Texture* get_texture() const { return texture; }

	protected:
		std::string name;
		Type type = Type::Materials;
		Grade grade = Grade::Ordinary;
		SDL_Texture* texture = nullptr;

	};

	class MagicBook : public Item
	{
	public:
		MagicBook()
		{
			name = u8"¸½Ä§Êé";
			type = Type::Props;
			grade = Grade::Legendary;
			texture = ResourcesManager::instance()->find_texture("book");
		}

		~MagicBook() = default;

	};

	class OrdinaryPotion : public Item
	{
	public:
		OrdinaryPotion()
		{
			name = u8"Ë®Æ¿";
			type = Type::Props;
			grade = Grade::Ordinary;
			texture = ResourcesManager::instance()->find_texture("bottle");
		}

		~OrdinaryPotion() = default;

	};

	class Coins : public Item
	{
	public:
		Coins()
		{
			name = u8"½ð±Ò";
			type = Type::Money;
			grade = Grade::Rare;
			texture = ResourcesManager::instance()->find_texture("coins");
		}

		~Coins() = default;

	};

	class Scrolls : public Item
	{
	public:
		Scrolls()
		{
			name = u8"ÑòÆ¤¾í";
			type = Type::Props;
			grade = Grade::Rare;
			texture = ResourcesManager::instance()->find_texture("scrolls");
		}

		~Scrolls() = default;

	};

	class Stick : public Item
	{
	public:
		Stick()
		{
			name = u8"Ä¾¹÷";
			type = Type::Materials;
			grade = Grade::Ordinary;
			texture = ResourcesManager::instance()->find_texture("stick");
		}

		~Stick() = default;

	};

	class Stone : public Item
	{
	public:
		Stone()
		{
			name = u8"Ê¯Í·";
			type = Type::Materials;
			grade = Grade::Ordinary;
			texture = ResourcesManager::instance()->find_texture("stone");
		}

		~Stone() = default;

	};

	using ItemList = std::vector<Item*>;

	class Iterator
	{
	public:
		Iterator(const ItemList& target_list)
		{
			cache_list = ItemList(target_list);
		}

		virtual ~Iterator() = default;

		virtual void reset() { idx = 0; }
		virtual int get_idx() const { return idx; }
		virtual Item* next() { return cache_list[idx++]; }
		virtual bool has_next() const { return idx < cache_list.size(); }

	protected:
		int idx = 0;
		ItemList cache_list;

	};

	class RandomIterator : public Iterator
	{
	public:
		RandomIterator(const ItemList& target_list) : Iterator(target_list)
		{
			std::shuffle(cache_list.begin(), cache_list.end(), std::mt19937(std::random_device()()));
		}

		~RandomIterator() = default;

	};

	class TypeSortIterator : public Iterator
	{
	public:
		TypeSortIterator(const ItemList& target_list) : Iterator(target_list)
		{
			std::sort(cache_list.begin(), cache_list.end(), [](const Item* item_1, const Item* item_2)
				{
					return item_1->get_type() < item_2->get_type();
				});
		}

		~TypeSortIterator() = default;

	};

	class GradeSortIterator : public Iterator
	{
	public:
		GradeSortIterator(const ItemList& target_list) : Iterator(target_list)
		{
			std::sort(cache_list.begin(), cache_list.end(), [](const Item* item_1, const Item* item_2)
				{
					return item_1->get_grade() < item_2->get_grade();
				});
		}

		~GradeSortIterator() = default;

	};
}

class IteratorPattern : public Example
{
public:
	IteratorPattern(SDL_Renderer* renderer);
	~IteratorPattern();

	void on_update(float delta) override;
	void on_render(SDL_Renderer* renderer) override;

private:
	SDL_Texture* texture_target = nullptr;
	_IteratorPattern::ItemList item_list;
	_IteratorPattern::Iterator* current_iterator = nullptr;

	_IteratorPattern::MagicBook sample_magic_book;
	_IteratorPattern::OrdinaryPotion sample_ordinary_potion;
	_IteratorPattern::Coins sample_coins;
	_IteratorPattern::Scrolls sample_scrolls;
	_IteratorPattern::Stick sample_stick;
	_IteratorPattern::Stone sample_stone;

private:
	void on_update_sample(_IteratorPattern::Item* item);

};

#endif // !_ITERATOR_H_
