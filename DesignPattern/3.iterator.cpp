#include "3.iterator.h"

#include <imgui.h>

using namespace _IteratorPattern;

IteratorPattern::IteratorPattern(SDL_Renderer* renderer)
{
	for (int i = 0; i < 2; i++)
		item_list.push_back(new MagicBook());
	for (int i = 0; i < 4; i++)
		item_list.push_back(new OrdinaryPotion());
	for (int i = 0; i < 5; i++)
		item_list.push_back(new Coins());
	for (int i = 0; i < 2; i++)
		item_list.push_back(new Scrolls());
	for (int i = 0; i < 6; i++)
		item_list.push_back(new Stick());
	for (int i = 0; i < 8; i++)
		item_list.push_back(new Stone());

	current_iterator = new RandomIterator(item_list);

	texture_target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 360, 190);
}

IteratorPattern::~IteratorPattern()
{
	for (Item* item : item_list)
		delete item;

	SDL_DestroyTexture(texture_target);
}

void IteratorPattern::on_update(float delta)
{
	ImGui::Dummy({ 0, 15 });

	on_update_sample(&sample_magic_book);		ImGui::SameLine();
	on_update_sample(&sample_ordinary_potion);	ImGui::SameLine();
	on_update_sample(&sample_coins);
	on_update_sample(&sample_scrolls);			ImGui::SameLine();
	on_update_sample(&sample_stick);			ImGui::SameLine();
	on_update_sample(&sample_stone);

	ImGui::Dummy({ 0, 50 });

	if (ImGui::Button(u8"随机排序", { ImGui::GetContentRegionAvail().x / 3.0f, 35 }))
	{
		delete current_iterator;
		current_iterator = new RandomIterator(item_list);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"按类型排序", { ImGui::GetContentRegionAvail().x / 2.0f, 35 }))
	{
		delete current_iterator;
		current_iterator = new TypeSortIterator(item_list);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"按品质排序", { ImGui::GetContentRegionAvail().x, 35 }))
	{
		delete current_iterator;
		current_iterator = new GradeSortIterator(item_list);
	}

	ImGui::Dummy({ 0, 50 });

	ImGui::BeginChild("backpack", ImGui::GetContentRegionAvail());
	ImGui::Image(texture_target, ImGui::GetContentRegionAvail());
	ImGui::EndChild();
}

void IteratorPattern::on_render(SDL_Renderer* renderer)
{
	SDL_SetRenderTarget(renderer, texture_target);
	SDL_SetRenderDrawColor(renderer, 65, 65, 65, 255);
	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, ResourcesManager::instance()->find_texture("backpack"), nullptr, nullptr);

	current_iterator->reset();
	while (current_iterator->has_next())
	{
		int idx = current_iterator->get_idx();
		Item* item = current_iterator->next();

		SDL_Rect rect = 
		{ 
			38 + (idx % 8) * 40 - 32 / 2, 
			34 + (idx / 8) * 40 - 32 / 2,
			32, 32 
		};
		SDL_RenderCopy(renderer, item->get_texture(), nullptr, &rect);
	}

	SDL_SetRenderTarget(renderer, nullptr);
}

void IteratorPattern::on_update_sample(Item* item)
{
	static const char* type_name[3] = { u8"材料", u8"道具", u8"货币" };
	static const char* grade_name[3] = { u8"平凡", u8"稀有", u8"传说" };

	ImGui::Image(item->get_texture(), { 64, 64 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 0.75f });
	ImGui::SameLine();
	{
		ImGui::BeginGroup();
		ImGui::Text(u8"< %s >", item->get_name().c_str());
		ImGui::TextUnformatted(u8"类型："); ImGui::SameLine();
		ImGui::TextDisabled(type_name[(int)item->get_type()]);
		ImGui::TextUnformatted(u8"品质："); ImGui::SameLine();
		ImGui::TextDisabled(grade_name[(int)item->get_grade()]);
		ImGui::EndGroup();
	}
	ImGui::SameLine();
	ImGui::Dummy({ 65, 0 });
}
