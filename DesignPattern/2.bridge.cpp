#include "2.bridge.h"

#include <imgui.h>

using namespace _BridgePattern;

BridgePattern::BridgePattern() = default;

BridgePattern::~BridgePattern() = default;

void BridgePattern::on_update(float delta)
{
	if (ImGui::Button(u8"创建 EasyX 图形后端的窗口", {ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y / 2}))
	{
		EasyXImpl easyx_impl;
		Window window(&easyx_impl);

		window.create(1280, 720);
		window.load_image("resources/splash.png");
		window.run();
	}

	if (ImGui::Button(u8"创建 Raylib 图形后端的窗口", ImGui::GetContentRegionAvail()))
	{
		RaylibImpl raylib_impl;
		Window window(&raylib_impl);

		window.create(1280, 720);
		window.load_image("resources/splash.png");
		window.run();
	}
}
