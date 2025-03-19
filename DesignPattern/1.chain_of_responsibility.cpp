#include "1.chain_of_responsibility.h"

#include <imgui_stdlib.h>

using namespace _ChainOfResponsibilityPattern;

static std::string str_output;
static bool is_bubbling = true;

ChainOfResponsibilityPattern::ChainOfResponsibilityPattern()
	: window_1(1, { 0.65f, 0.2f, 0.2f, 1.0f }, { 679, 575 }, { 0, 0 }),
		window_2(2, { 0.2f, 0.65f, 0.2f, 1.0f }, { 425, 350 }, { 50, 50 }),
		window_3(3, { 0.2f, 0.2f, 0.65f, 1.0f }, { 200, 175 }, { 100, 100 })
{
	window_3.set_next(&window_2);
	window_2.set_next(&window_1);
}

ChainOfResponsibilityPattern::~ChainOfResponsibilityPattern() = default;

void ChildWindow::handle()
{
	str_output += u8"点击消息传递到 [" + std::to_string(id) + u8"] 号窗口！\n";

	if (next && is_bubbling)
		next->handle();
}

void ChildWindow::on_update()
{
	ImGui::SetCursorPos(offset);

	ImGui::PushStyleColor(ImGuiCol_ChildBg, color);
	ImGui::BeginChild(id, size, ImGuiChildFlags_Border);
	ImGui::Text(u8"窗口 [%d]", id);
	if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		str_output.clear(); 
		handle();
	}
	ImGui::EndChild();
	ImGui::PopStyleColor();
}

void ChainOfResponsibilityPattern::on_update(float delta)
{
	ImGui::TextUnformatted(u8"允许鼠标点击消息冒泡："); ImGui::SameLine();
	ImGui::Checkbox(u8"##允许鼠标点击消息冒泡", &is_bubbling);

	ImGui::BeginChild("region", { ImGui::GetContentRegionAvail().x, 575 });

	window_1.on_update();
	window_2.on_update();
	window_3.on_update();

	ImGui::EndChild();

	ImGui::InputTextMultiline(u8"##输出内容", &str_output, ImGui::GetContentRegionAvail(), ImGuiInputTextFlags_ReadOnly);
}
