#include "6.observer.h"
#include "resources_manager.h"

#include <imgui.h>

using namespace _ObserverPattern;

static std::vector<Achievement> unlocked_achievement_list;

void Button1ClickedObserver::update(int state)
{
	switch (state)
	{
	case 1:
		unlocked_achievement_list.push_back(Achievement(ResourcesManager::instance()->find_texture("achievement_btn_1_1")));
		Mix_PlayChannel(-1, ResourcesManager::instance()->find_audio("unlock"), 0);
		break;
	case 10:
		unlocked_achievement_list.push_back(Achievement(ResourcesManager::instance()->find_texture("achievement_btn_1_10")));
		Mix_PlayChannel(-1, ResourcesManager::instance()->find_audio("unlock"), 0);
		break;
	default: break;
	}
}

void Button2ClickedObserver::update(int state)
{
	switch (state)
	{
	case 1:
		unlocked_achievement_list.push_back(Achievement(ResourcesManager::instance()->find_texture("achievement_btn_2_1")));
		Mix_PlayChannel(-1, ResourcesManager::instance()->find_audio("unlock"), 0);
		break;
	case 10:
		unlocked_achievement_list.push_back(Achievement(ResourcesManager::instance()->find_texture("achievement_btn_2_10")));
		Mix_PlayChannel(-1, ResourcesManager::instance()->find_audio("unlock"), 0);
		break;
	default: break;
	}
}

ObserverPattern::ObserverPattern()
{
	button_1_clicked_subject.attach(&btn_1_clicked_observer);
	button_2_clicked_subject.attach(&btn_2_clicked_observer);
}

ObserverPattern::~ObserverPattern() = default;

void ObserverPattern::on_update(float delta)
{
	static const ImVec2 size_btn = { ImGui::GetContentRegionAvail().x, 35 };
	if (ImGui::Button(u8"芝士按钮 [1] 点我解锁更多成就！", size_btn))
		button_1_clicked_subject.accumulate();
	if (ImGui::Button(u8"芝士按钮 [2] 点我解锁更多成就！", size_btn))
		button_2_clicked_subject.accumulate();
	
	ImGui::BeginChild("achievement", ImGui::GetContentRegionAvail(), ImGuiChildFlags_Border);
	for (const Achievement& achievement : unlocked_achievement_list)
		ImGui::Image(achievement.texture, { ImGui::GetContentRegionAvail().x, 135 });
	ImGui::Separator();
	ImGui::TextDisabled(u8"* 已解锁成就 %d/4", (int)unlocked_achievement_list.size());
	ImGui::EndChild();
}
