#include "example_manager.h"
#include "resources_manager.h"

#include "1.factory_method.h"
#include "2.abstract_factory.h"
#include "3.builder.h"
#include "4.prototype.h"
#include "5.singleton.h"

#include "1.adapter.h"
#include "2.bridge.h"
#include "3.composite.h"
#include "4.decorator.h"
#include "5.facade.h"
#include "6.flyweight.h"
#include "7.proxy.h"

#include "1.chain_of_responsibility.h"
#include "2.command.h"
#include "3.iterator.h"
#include "4.mediator.h"
#include "5.memento.h"
#include "6.observer.h"
#include "7.state.h"
#include "8.strategy.h"
#include "9.template_method.h"
#include "10.visitor.h"
#include "11.interpreter.h"

#include <imgui.h>

#include <Windows.h>

ExampleManager* ExampleManager::manager = nullptr;

void ExampleManager::init(SDL_Renderer* renderer)
{
	this->renderer = renderer;

	texture_target = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 720, 720);
	SDL_SetTextureBlendMode(texture_target, SDL_BLENDMODE_BLEND);

	subject_creational.title = u8"一、创建型模式（5）";
	SDL_Texture* texture_icon_creational = ResourcesManager::instance()->find_texture("icon-creational");
	add_example(subject_creational, MenuItem("factory_method", texture_icon_creational, u8"1. 工厂方法模式"), new FactoryMethodPattern(renderer));
	add_example(subject_creational, MenuItem("abstract_factory", texture_icon_creational, u8"2. 抽象工厂模式"), new AbstractFactoryPattern(renderer));
	add_example(subject_creational, MenuItem("builder", texture_icon_creational, u8"3. 建造者模式"), new BuilderPattern(renderer));
	add_example(subject_creational, MenuItem("prototype", texture_icon_creational, u8"4. 原型模式"), new PrototypePattern(renderer));
	add_example(subject_creational, MenuItem("singleton", texture_icon_creational, u8"5. 单例模式"), new SingletonPattern(renderer));

	subject_structural.title = u8"二、结构型模式（7）";
	SDL_Texture* texture_icon_structural = ResourcesManager::instance()->find_texture("icon-structural");
	add_example(subject_structural, MenuItem("adapter", texture_icon_structural, u8"1. 适配器模式"), new AdapterPattern(renderer));
	add_example(subject_structural, MenuItem("bridge", texture_icon_structural, u8"2. 桥接模式"), new BridgePattern());
	add_example(subject_structural, MenuItem("composite", texture_icon_structural, u8"3. 组合模式"), new CompositePattern(renderer));
	add_example(subject_structural, MenuItem("decorator", texture_icon_structural, u8"4. 装饰器模式"), new DecoratorPattern(renderer));
	add_example(subject_structural, MenuItem("facade", texture_icon_structural, u8"5. 外观模式"), new FacadePattern());
	add_example(subject_structural, MenuItem("flyweight", texture_icon_structural, u8"6. 享元模式"), new FlyweightPattern());
	add_example(subject_structural, MenuItem("proxy", texture_icon_structural, u8"7. 代理模式"), new ProxyPattern(renderer));

	subject_behavioral.title = u8"三、行为模式（11）";
	SDL_Texture* texture_icon_behavioral = ResourcesManager::instance()->find_texture("icon-behavioral");
	add_example(subject_behavioral, MenuItem("chain_of_responsibility", texture_icon_behavioral, u8"1. 责任链模式"), new ChainOfResponsibilityPattern());
	add_example(subject_behavioral, MenuItem("command", texture_icon_behavioral, u8"2. 命令模式"), new CommandPattern(renderer));
	add_example(subject_behavioral, MenuItem("iterator", texture_icon_behavioral, u8"3. 迭代器模式"), new IteratorPattern(renderer));
	add_example(subject_behavioral, MenuItem("mediator", texture_icon_behavioral, u8"4. 中介者模式"), new MediatorPattern());
	add_example(subject_behavioral, MenuItem("memento", texture_icon_behavioral, u8"5. 备忘录模式"), new MementoPattern(renderer));
	add_example(subject_behavioral, MenuItem("observer", texture_icon_behavioral, u8"6. 观察者模式"), new ObserverPattern());
	add_example(subject_behavioral, MenuItem("state", texture_icon_behavioral, u8"7. 状态模式"), new StatePattern(renderer));
	add_example(subject_behavioral, MenuItem("strategy", texture_icon_behavioral, u8"8. 策略模式"), new StrategyPattern(renderer));
	add_example(subject_behavioral, MenuItem("template_method", texture_icon_behavioral, u8"9. 模板方法模式"), new TemplateMethodPattern(renderer));
	add_example(subject_behavioral, MenuItem("visitor", texture_icon_behavioral, u8"10. 访问者模式"), new VisitorPattern());
	add_example(subject_behavioral, MenuItem("interpreter", texture_icon_behavioral, u8"11. 解释器模式"), new InterpreterPattern(renderer));
}

void ExampleManager::on_update_blank_content()
{
	ImGui::SetCursorPos({ 545, 665 });
	ImGui::TextDisabled(u8"@ Voidmatrix"); ImGui::SameLine();
	if (ImGui::ImageButton(ResourcesManager::instance()->find_texture("icon-bilibili"), { 14, 14 }))
		ShellExecute(NULL, TEXT("open"), TEXT("https://space.bilibili.com/25864506"), NULL, NULL, SW_SHOWNORMAL);
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::TextDisabled(u8"点击打开我的 Bilibili 主页 ~");
		ImGui::EndTooltip();
	}
}

void ExampleManager::switch_to(const std::string& id)
{
	if (current_example)
		current_example->on_exit();

	current_example_id = id;
	current_example = example_pool[id];

	if (current_example)
		current_example->on_enter();
}

void ExampleManager::on_input(const SDL_Event* event)
{
	if (!current_example) return;

	current_example->on_input(event);
}

void ExampleManager::on_update(float delta)
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);

	static const ImGuiWindowFlags flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoSavedSettings;

	ImGui::Begin("main_window", nullptr, flags);

	static const ImVec2 size_menu = { 180, ImGui::GetContentRegionAvail().y };
	{
		ImGui::BeginChild("menu", size_menu, ImGuiChildFlags_Border);
		on_update_subject(subject_creational);
		on_update_subject(subject_structural);
		on_update_subject(subject_behavioral);
		ImGui::EndChild();
	}

	ImGui::SameLine();

	static const ImVec2 size_stage = ImGui::GetContentRegionAvail();
	{
		ImGui::BeginChild("stage", size_stage, ImGuiChildFlags_Border);
		if (current_example)
			current_example->on_update(delta);
		else
			on_update_blank_content();
		ImGui::EndChild();
	}

	ImGui::End();
}

void ExampleManager::on_render()
{
	if (!current_example) return;

	current_example->on_render(renderer);
}

ExampleManager::ExampleManager() = default;

ExampleManager::~ExampleManager()
{
	if (current_example)
		current_example->on_exit();

	for (auto& pair : example_pool)
		delete pair.second;
}

void ExampleManager::on_update_subject(const Subject& subject)
{
	if (ImGui::CollapsingHeader(subject.title.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		for (const MenuItem& menu_item : subject.item_list)
		{
			static const ImVec2 size_icon = 
			{ 
				ImGui::GetTextLineHeightWithSpacing(), 
				ImGui::GetTextLineHeightWithSpacing() 
			};

			ImGui::Image(menu_item.icon, size_icon); ImGui::SameLine();
			if (ImGui::Selectable(menu_item.title.c_str(), current_example_id == menu_item.id))
				switch_to(menu_item.id);
		}
	}
}

void ExampleManager::add_example(Subject& subject, const MenuItem& menu_item, Example* example)
{
	subject.item_list.emplace_back(menu_item);
	example_pool[menu_item.id] = example;
}