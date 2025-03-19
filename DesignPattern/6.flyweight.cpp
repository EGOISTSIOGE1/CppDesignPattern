#include "6.flyweight.h"

#include <imgui.h>

void FlyweightPattern::on_update(float delta)
{
	static const ImVec2 size_text = ImGui::CalcTextSize(str_text.c_str());
	static const ImVec2 size_content = ImGui::GetContentRegionAvail();
	static const ImVec2 position_cursor = { (size_content.x - size_text.x) / 2, (size_content.y - size_text.y) / 2 };

	ImGui::SetCursorPos(position_cursor);
	ImGui::TextDisabled(str_text.c_str());
}