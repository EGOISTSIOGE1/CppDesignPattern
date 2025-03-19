#ifndef _VISITOR_H_
#define _VISITOR_H_

#include "example.h"
#include "resources_manager.h"

#include <imgui.h>
#include <imgui_stdlib.h>

namespace _VisitorPattern
{
	class MagicBook;
	class OrdinaryPotion;
	class Coins;
	class Scrolls;
	class Stick;
	class Stone;

	class Visitor
	{
	public:
		virtual void visit(const MagicBook& magic_book) = 0;
		virtual void visit(const OrdinaryPotion& ordinary_potion) = 0;
		virtual void visit(const Coins& coins) = 0;
		virtual void visit(const Scrolls& scrolls) = 0;
		virtual void visit(const Stick& stick) = 0;
		virtual void visit(const Stone& stone) = 0;

	};

	class Item
	{
	public:
		Item(int _id, SDL_Texture* _texture) 
			: id(_id), texture(_texture) {}
		virtual ~Item() = default;

		virtual void accept(Visitor& visitor) = 0;

		int get_id() const { return id; }
		SDL_Texture* get_texture() const { return texture; }

	private:
		int id;
		SDL_Texture* texture;

	};

	class MagicBook : public Item
	{
	public:
		MagicBook() : Item(1, ResourcesManager::instance()->find_texture("book")) {}
		~MagicBook() = default;

		void accept(Visitor& visitor) override { visitor.visit(*this); }

	};

	class OrdinaryPotion : public Item
	{
	public:
		OrdinaryPotion() : Item(2, ResourcesManager::instance()->find_texture("bottle")) {}
		~OrdinaryPotion() = default;

		void accept(Visitor& visitor) override { visitor.visit(*this); }

	};

	class Coins : public Item
	{
	public:
		Coins() : Item(3, ResourcesManager::instance()->find_texture("coins")) {}
		~Coins() = default;

		void accept(Visitor& visitor) override { visitor.visit(*this); }

	};

	class Scrolls : public Item
	{
	public:
		Scrolls() : Item(4, ResourcesManager::instance()->find_texture("scrolls")) {}
		~Scrolls() = default;

		void accept(Visitor& visitor) override { visitor.visit(*this); }

	};

	class Stick : public Item
	{
	public:
		Stick() : Item(5, ResourcesManager::instance()->find_texture("stick")) {}
		~Stick() = default;

		void accept(Visitor& visitor) override { visitor.visit(*this); }

	};

	class Stone : public Item
	{
	public:
		Stone() : Item(6, ResourcesManager::instance()->find_texture("stone")) {}
		~Stone() = default;

		void accept(Visitor& visitor) override { visitor.visit(*this); }

	};

	class InpsectVisitor : public Visitor
	{
	public:
		void visit(const MagicBook& magic_book);
		void visit(const OrdinaryPotion& ordinary_potion);
		void visit(const Coins& coins);
		void visit(const Scrolls& scrolls);
		void visit(const Stick& stick);
		void visit(const Stone& stone);

	};

	class ApplyVisitor : public Visitor
	{
	public:
		void visit(const MagicBook& magic_book);
		void visit(const OrdinaryPotion& ordinary_potion);
		void visit(const Coins& coins);
		void visit(const Scrolls& scrolls);
		void visit(const Stick& stick);
		void visit(const Stone& stone);

	};
}

class VisitorPattern : public Example
{
public:
	VisitorPattern();
	~VisitorPattern();

	void on_update(float delta) override;

private:
	_VisitorPattern::MagicBook magic_book;
	_VisitorPattern::OrdinaryPotion ordinary_potion;
	_VisitorPattern::Stick stick;	_VisitorPattern::Stone stone;
	_VisitorPattern::Coins coins;	_VisitorPattern::Scrolls scrolls;
	
	_VisitorPattern::ApplyVisitor apply_visitor;
	_VisitorPattern::InpsectVisitor inspect_visitor;

private:
	template <typename T>
	void on_update_item_btn(T& item)
	{
		ImGui::ImageButton(item.get_texture(), { 64, 64 });
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
			item.accept(inspect_visitor);
		if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
			item.accept(apply_visitor);
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::TextDisabled(u8"鼠标左键：检视道具");
			ImGui::TextDisabled(u8"鼠标右键：使用道具");
			ImGui::EndTooltip();
		}
	}

};

#endif // !_VISITOR_H_
