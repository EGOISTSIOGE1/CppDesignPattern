#include "11.interpreter.h"

#include <imgui_stdlib.h>
#include <SDL2_gfxPrimitives.h>

#include <string>

// Lua官方网站：https://lua.org/
// Lua官方资料合集：https://lua.org/docs.html
// Lua5.4参考手册：https://lua.org/manual/5.4/

static std::string str_buffer_print;
static SDL_Renderer* g_renderer = nullptr;

static int api_print(lua_State* lua_state)
{
	str_buffer_print += luaL_checkstring(lua_state, 1);
	str_buffer_print += "\n";

	return 0;
}

static int api_set_draw_color(lua_State* lua_state)
{
	SDL_SetRenderDrawBlendMode(g_renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(g_renderer, (Uint8)luaL_checkinteger(lua_state, 1), (Uint8)luaL_checkinteger(lua_state, 2), 
		(Uint8)luaL_checkinteger(lua_state, 3), (Uint8)luaL_checkinteger(lua_state, 4));

	return 0;
}

static int api_draw_circle(lua_State* lua_state)
{
	SDL_Color color_painter;
	SDL_GetRenderDrawColor(g_renderer, &(color_painter.r), &(color_painter.g), &(color_painter.b), &(color_painter.a));
	filledCircleRGBA(g_renderer, (Sint16)luaL_checkinteger(lua_state, 1), (Sint16)luaL_checkinteger(lua_state, 2), 
		(Sint16)luaL_checkinteger(lua_state, 3), color_painter.r, color_painter.g, color_painter.b, color_painter.a);

	return 0;
}

static int api_draw_rectangle(lua_State* lua_state)
{
	SDL_Rect rect =
	{
		(int)luaL_checkinteger(lua_state, 1),
		(int)luaL_checkinteger(lua_state, 2),
		(int)luaL_checkinteger(lua_state, 3),
		(int)luaL_checkinteger(lua_state, 4)
	};
	SDL_RenderFillRect(g_renderer, &rect);

	return 0;
}

InterpreterPattern::InterpreterPattern(SDL_Renderer* renderer)
{
	g_renderer = renderer;

	texture_target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 370, 370);

	ImGuiIO& ioImGui = ImGui::GetIO();
	font_code = ioImGui.Fonts->AddFontFromFileTTF(R"(resources\SarasaMonoSC-Regular.ttf)", 18.0f, nullptr, ioImGui.Fonts->GetGlyphRangesChineseSimplifiedCommon());

	TextEditor::LanguageDefinition lang_def = TextEditor::LanguageDefinition::Lua();
	{
		TextEditor::Identifier id_set_draw_color;
		id_set_draw_color.mDeclaration = u8"set_draw_color(r, g, b, a)：设置绘图颜色";
		lang_def.mIdentifiers.insert(std::make_pair(std::string("set_draw_color"), id_set_draw_color));
		TextEditor::Identifier id_draw_circle;
		id_draw_circle.mDeclaration = u8"draw_circle(x, y, r)：使用当前绘图颜色绘制填充圆";
		lang_def.mIdentifiers.insert(std::make_pair(std::string("draw_circle"), id_draw_circle));
		TextEditor::Identifier id_draw_rectangle;
		id_draw_rectangle.mDeclaration = u8"draw_rectangle(x, y, w, h)：使用当前绘图颜色绘制填充矩形";
		lang_def.mIdentifiers.insert(std::make_pair(std::string("draw_rectangle"), id_draw_rectangle));
	}
	code_editor.SetLanguageDefinition(lang_def);
	code_editor.SetShowWhitespaces(false);
	code_editor.SetText(u8R"(-- Lua 脚本环境中自定义的方法：
-- print(str)：向控制台输出字符串
-- set_draw_color(r, g, b, a)：设置绘图颜色
-- draw_circle(x, y, r)：使用当前绘图颜色绘制填充圆
-- draw_rectangle(x, y, w, h)：使用当前绘图颜色绘制填充矩形

-- 测试脚本的绘图功能
function test_draw()
	set_draw_color(228, 94, 50, 225)
	draw_circle(125, 125, 100)
	
	set_draw_color(0, 123, 67, 200)
	draw_rectangle(100, 145, 200, 100)

	set_draw_color(25, 68, 142, 155)
	draw_circle(300, 225, 50)

	set_draw_color(162, 32, 65, 95)
	draw_rectangle(280, 25, 55, 275)
end

-- 测试脚本的输出功能
function test_print()
	print(string.lower("HELLO WORLD"))
	print("current time: "..os.date("%Y-%m-%d %H:%M:%S", time or os.time()))
end

test_draw()
test_print()
	)");
}

InterpreterPattern::~InterpreterPattern()
{
	SDL_DestroyTexture(texture_target);
}

void InterpreterPattern::on_update(float delta)
{
	static const ImVec2 size_button = { ImGui::GetContentRegionAvail().x, 30 };
	if (ImGui::Button(u8"执 行 脚 本", size_button))
	{
		lua_State* lua_state = luaL_newstate();
		luaL_openlibs(lua_state);

		lua_pushcfunction(lua_state, api_print);
		lua_setglobal(lua_state, "print");
		lua_pushcfunction(lua_state, api_set_draw_color);
		lua_setglobal(lua_state, "set_draw_color");
		lua_pushcfunction(lua_state, api_draw_circle);
		lua_setglobal(lua_state, "draw_circle");
		lua_pushcfunction(lua_state, api_draw_rectangle);
		lua_setglobal(lua_state, "draw_rectangle");

		str_buffer_print = u8"【开始执行】\n==============\n";

		SDL_SetRenderTarget(g_renderer, texture_target);
		SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
		SDL_RenderClear(g_renderer);

		if (luaL_dostring(lua_state, code_editor.GetText().c_str()) != LUA_OK)
		{
			str_buffer_print += u8"\n【执行失败】\n==============\n";
			str_buffer_print += lua_tostring(lua_state, -1);
		}

		SDL_SetRenderTarget(g_renderer, nullptr);

		lua_close(lua_state);
	}

	{
		ImGui::PushFont(font_code);
		static const ImVec2 size_code_editor = { 350, ImGui::GetContentRegionAvail().y };
		code_editor.Render("code_editor", size_code_editor, true);
		ImGui::PopFont();
	}
	ImGui::SameLine();
	{
		ImGui::BeginGroup();
		{
			ImGui::TextUnformatted(u8"绘图内容：");
			static const ImVec2 size_paint_result = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().x };
			ImGui::BeginChild("paint_result", size_paint_result, ImGuiChildFlags_Border);
			ImGui::Image(texture_target, ImGui::GetContentRegionAvail());
			ImGui::EndChild();
		}
		ImGui::TextUnformatted(u8"控制台输出：");
		ImGui::InputTextMultiline("##print_result", &str_buffer_print, ImGui::GetContentRegionAvail(), ImGuiInputTextFlags_ReadOnly);
		ImGui::EndGroup();
	}

}
