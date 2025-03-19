#include "10.visitor.h"

using namespace _VisitorPattern;

static std::string str_output;

void InpsectVisitor::visit(const MagicBook& magic_book)
{
	str_output += u8"检视道具 [" + std::to_string(magic_book.get_id()) + u8"]：一本神奇的魔法书？\n";
}

void InpsectVisitor::visit(const OrdinaryPotion& ordinary_potion)
{
	str_output += u8"检视道具 [" + std::to_string(ordinary_potion.get_id()) + u8"]：只是一瓶普通的药水……\n";
}

void InpsectVisitor::visit(const Coins& coins)
{
	str_output += u8"检视道具 [" + std::to_string(coins.get_id()) + u8"]：亮晶晶的？钱币！\n";
}

void InpsectVisitor::visit(const Scrolls& scrolls)
{
	str_output += u8"检视道具 [" + std::to_string(scrolls.get_id()) + u8"]：普通人无法参透其中奥妙的卷轴。\n";
}

void InpsectVisitor::visit(const Stick& stick)
{
	str_output += u8"检视道具 [" + std::to_string(stick.get_id()) + u8"]：一根木棍，或许有用……\n";
}

void InpsectVisitor::visit(const Stone& stone)
{
	str_output += u8"检视道具 [" + std::to_string(stone.get_id()) + u8"]：你或许应该扔掉来节省背包空间的石头。\n";
}

void ApplyVisitor::visit(const MagicBook& magic_book)
{
	str_output += u8"使用道具 [" + std::to_string(magic_book.get_id()) + u8"]：你的体内被魔法元素充盈！\n";
}

void ApplyVisitor::visit(const OrdinaryPotion& ordinary_potion)
{
	str_output += u8"使用道具 [" + std::to_string(ordinary_potion.get_id()) + u8"]：什么事情也没有发生……\n";
}

void ApplyVisitor::visit(const Coins& coins)
{
	str_output += u8"使用道具 [" + std::to_string(coins.get_id()) + u8"]：你的财富值大大增加了！\n";
}

void ApplyVisitor::visit(const Scrolls& scrolls)
{
	str_output += u8"使用道具 [" + std::to_string(scrolls.get_id()) + u8"]：智慧等级不够无法进行学习！\n";
}

void ApplyVisitor::visit(const Stick& stick)
{
	str_output += u8"使用道具 [" + std::to_string(stick.get_id()) + u8"]：什么事情也没有发生……\n";
}

void ApplyVisitor::visit(const Stone& stone)
{
	str_output += u8"使用道具 [" + std::to_string(stone.get_id()) + u8"]：什么事情也没有发生……\n";
}

VisitorPattern::VisitorPattern() = default;

VisitorPattern::~VisitorPattern() = default;

void VisitorPattern::on_update(float delta)
{
	static const ImVec2 size_dummy = { 33, 0 };

	on_update_item_btn<MagicBook>(magic_book);				ImGui::SameLine(); ImGui::Dummy(size_dummy); ImGui::SameLine();
	on_update_item_btn<OrdinaryPotion>(ordinary_potion);	ImGui::SameLine(); ImGui::Dummy(size_dummy); ImGui::SameLine();
	on_update_item_btn<Coins>(coins);						ImGui::SameLine(); ImGui::Dummy(size_dummy); ImGui::SameLine();
	on_update_item_btn<Scrolls>(scrolls);					ImGui::SameLine(); ImGui::Dummy(size_dummy); ImGui::SameLine();
	on_update_item_btn<Stick>(stick);						ImGui::SameLine(); ImGui::Dummy(size_dummy); ImGui::SameLine();
	on_update_item_btn<Stone>(stone);

	ImGui::InputTextMultiline(u8"##输出内容", &str_output, ImGui::GetContentRegionAvail(), ImGuiInputTextFlags_ReadOnly);
}
