#include "3.composite.h"

#include <imgui.h>
#include <imgui_stdlib.h>

using namespace _CompositePattern;

static bool combo_getter(void* data, int n, const char** out_str)
{
	ResourcesManager::ResIDList* id_list = (ResourcesManager::ResIDList*)data;
	*out_str = (*id_list)[n].c_str();

	return true;
}

void Node::on_inspect()
{
	ImGui::TextDisabled(u8"空节点");

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, 75);

	ImGui::TextUnformatted(u8"名称："); ImGui::NextColumn();
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
	ImGui::InputText(u8"##名称", &name);
	ImGui::NextColumn();

	ImGui::TextUnformatted(u8"位置："); ImGui::NextColumn();
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
	ImGui::DragFloat2(u8"##位置", &position.x, 0.1f, 0, 0, "%.2f");
	ImGui::NextColumn();

	ImGui::Columns(1);
}

void _CompositePattern::Node::on_update(float delta)
{
	if (get_parent())
		world_position = get_parent()->get_position() + position;

	for (Node* child : child_list)
		child->on_update(delta);
}

void _CompositePattern::Node::on_render(SDL_Renderer* renderer)
{
	for (Node* child : child_list)
		child->on_render(renderer);
}

_CompositePattern::TextureNode::~TextureNode()
{
	SDL_DestroyTexture(texture);
}

void TextureNode::on_inspect()
{
	Node::on_inspect();

	ImGui::Separator();

	ImGui::TextDisabled(u8"纹理节点");

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, 75);

	ImGui::TextUnformatted(u8"纹理："); ImGui::NextColumn(); 
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
	const ResourcesManager::ResIDList& resid_list = ResourcesManager::instance()->get_texture_resid_list();
	if (ImGui::Combo("##纹理", &idx_texture, &combo_getter, (void*)(&resid_list), (int)resid_list.size()))
	{
		texture = ResourcesManager::instance()->find_texture(resid_list[idx_texture]);
		update_size();
	}
	ImGui::NextColumn();

	ImGui::TextUnformatted(u8"尺寸："); ImGui::NextColumn();
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
	ImGui::DragFloat2(u8"##尺寸", &size.x, 1.0f, 0.0f, FLT_MAX, "%.2f", ImGuiSliderFlags_AlwaysClamp);
	ImGui::NextColumn();

	ImGui::TextUnformatted(u8"旋转："); ImGui::NextColumn();
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
	ImGui::DragFloat(u8"##旋转", &rotation, 0.1f, 0, 0, "%.2f deg");
	ImGui::NextColumn();

	ImGui::Columns(1);
}

void TextureNode::on_render(SDL_Renderer* renderer)
{
	SDL_FRect rect = { world_position.x, world_position.y, size.x, size.y };
	SDL_RenderCopyExF(renderer, texture, nullptr, &rect, rotation, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);

	Node::on_render(renderer);
}

void TextureNode::set_texture(SDL_Texture* texture)
{
	this->texture = texture;

	const ResourcesManager::ResIDList& resid_list = ResourcesManager::instance()->get_texture_resid_list();
	for (int i = 0; i < resid_list.size(); i++)
	{
		if (texture == ResourcesManager::instance()->find_texture(resid_list[i]))
		{
			idx_texture = i;
			break;
		}
	}

	update_size();
}

void TextureNode::update_size()
{
	int w, h;
	SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
	size = { (float)w, (float)h };
}

void TextNode::on_inspect()
{
	Node::on_inspect();

	ImGui::Separator();

	ImGui::TextDisabled(u8"文本节点");

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, 75);

	ImGui::TextUnformatted(u8"文本："); ImGui::NextColumn();
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
	if (ImGui::InputText(u8"##文本", &text))
		need_update = true;
	ImGui::NextColumn();

	ImGui::TextUnformatted(u8"颜色："); ImGui::NextColumn();
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
	if (ImGui::ColorEdit4(u8"##颜色", color_text, ImGuiColorEditFlags_AlphaPreview))
		need_update = true;
	ImGui::NextColumn();

	ImGui::TextUnformatted(u8"字号："); ImGui::NextColumn();
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
	if (ImGui::DragInt(u8"##字号", &font_size, 1.0f, 10, 500, "%d px", ImGuiSliderFlags_AlwaysClamp))
		need_update = true;
	ImGui::NextColumn();

	ImGui::TextUnformatted(u8"尺寸："); ImGui::NextColumn();
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
	ImGui::DragFloat2(u8"##尺寸", &size.x, 1.0f, 0.0f, FLT_MAX, "%.2f", ImGuiSliderFlags_AlwaysClamp);
	ImGui::NextColumn();

	ImGui::Columns(1);
}

void TextNode::on_render(SDL_Renderer* renderer)
{
	if (need_update)
	{
		update_texture_text(renderer);
		need_update = false;
	}

	SDL_FRect rect = { world_position.x, world_position.y, size.x, size.y };
	SDL_RenderCopyF(renderer, texture_text, nullptr, &rect);

	Node::on_render(renderer);
}

void _CompositePattern::TextNode::set_font_size(int font_size)
{
	this->font_size = font_size;
	need_update = true;
}

void TextNode::set_text(const std::string& text)
{
	this->text = text;
	need_update = true;
}

void TextNode::update_texture_text(SDL_Renderer* renderer)
{
	SDL_DestroyTexture(texture_text); texture_text = nullptr;

	SDL_Color color =
	{
		(Uint8)(color_text[0] * 255), (Uint8)(color_text[1] * 255),
		(Uint8)(color_text[2] * 255), (Uint8)(color_text[3] * 255)
	};
	SDL_Surface* surface = TTF_RenderUTF8_Blended(ResourcesManager::instance()
		->find_font("SarasaMonoSC-Regular")->get_font(font_size), text.c_str(), color);
	if (!surface) { size = { 0, 0 }; return; }

	texture_text = SDL_CreateTextureFromSurface(renderer, surface);
	size = { (float)surface->w, (float)surface->h };
	SDL_FreeSurface(surface);
}

void AudioNode::on_inspect()
{
	Node::on_inspect();

	ImGui::Separator();

	ImGui::TextDisabled(u8"音频节点");

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, 75);

	ImGui::TextUnformatted(u8"音频："); ImGui::NextColumn();
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
	const ResourcesManager::ResIDList& resid_list = ResourcesManager::instance()->get_audio_resid_list();
	ImGui::Combo("##音频", &idx_audio, &combo_getter, (void*)(&resid_list), (int)resid_list.size());
	ImGui::NextColumn();

	ImGui::TextUnformatted(u8"播放："); ImGui::NextColumn();
	static const ImVec2 size_btn = { ImGui::GetTextLineHeight(), ImGui::GetTextLineHeight() };
	if (ImGui::ImageButton(ResourcesManager::instance()->find_texture("icon-play"), size_btn))
		Mix_PlayChannel(-1, ResourcesManager::instance()->find_audio(ResourcesManager::instance()->get_audio_resid_list()[idx_audio]), 0);
	ImGui::NextColumn();

	ImGui::Columns(1);
}

void _CompositePattern::AudioNode::set_audio(Mix_Chunk* audio)
{
	const ResourcesManager::ResIDList& resid_list = ResourcesManager::instance()->get_audio_resid_list();
	for (int i = 0; i < resid_list.size(); i++)
	{
		if (audio == ResourcesManager::instance()->find_audio(resid_list[i]))
		{
			idx_audio = i;
			break;
		}
	}
}

CompositePattern::CompositePattern(SDL_Renderer* renderer)
{
	init_world_tree();

	texture_target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 470, 450);
}

CompositePattern::~CompositePattern()
{
	delete world_tree;
	SDL_DestroyTexture(texture_target);
}

void CompositePattern::on_update(float delta)
{
	world_tree->on_update(delta);

	{
		ImGui::BeginChild("tree_view", { 250, ImGui::GetContentRegionAvail().y }, 
			ImGuiChildFlags_Border, ImGuiWindowFlags_AlwaysHorizontalScrollbar);
		render_tree_view(world_tree);
		ImGui::EndChild();
	}
	ImGui::SameLine();
	{
		ImGui::BeginGroup();
		{
			ImGui::BeginChild("scene_view", { ImGui::GetContentRegionAvail().x, 455 }, ImGuiChildFlags_Border);
			ImGui::Image(texture_target, ImGui::GetContentRegionAvail());
			ImGui::EndChild();
		}
		{
			ImGui::BeginChild("inspector", ImGui::GetContentRegionAvail(), ImGuiChildFlags_Border);
			if (node_selected) node_selected->on_inspect();
			ImGui::EndChild();
		}
		ImGui::EndGroup();
	}

	if (need_show_popup)
	{
		ImGui::OpenPopup("context menu");
		need_show_popup = false;
	}

	if (ImGui::BeginPopup("context menu"))
	{
		if (ImGui::BeginMenu(u8"添加节点"))
		{
			if (render_menu_item(ResourcesManager::instance()->find_texture("icon-node"), u8"空节点"))
				node_selected->add_child(new Node());
			if (render_menu_item(ResourcesManager::instance()->find_texture("icon-texture"), u8"纹理节点"))
				node_selected->add_child(new TextureNode());
			if (render_menu_item(ResourcesManager::instance()->find_texture("icon-text"), u8"文本节点"))
				node_selected->add_child(new TextNode());
			if (render_menu_item(ResourcesManager::instance()->find_texture("icon-audio"), u8"音频节点"))
				node_selected->add_child(new AudioNode());
			ImGui::EndMenu();
		}

		ImGui::BeginDisabled(node_selected && !node_selected->get_parent());
		if (ImGui::MenuItem(u8"删除节点"))
		{
			if (node_selected && node_selected->get_parent())
			{
				node_selected->get_parent()->del_child(node_selected);
				delete node_selected; node_selected = nullptr;
			}
		}
		ImGui::EndDisabled();

		ImGui::EndPopup();
	}
}

void CompositePattern::on_render(SDL_Renderer* renderer)
{
	SDL_SetRenderTarget(renderer, texture_target);
	SDL_SetRenderDrawColor(renderer, 65, 65, 65, 255);
	SDL_RenderClear(renderer);

	world_tree->on_render(renderer);

	SDL_SetRenderTarget(renderer, nullptr);
}

void CompositePattern::init_world_tree()
{
	world_tree = new Node();
	world_tree->set_name(u8"世界树");
	{
		AudioNode* node_audio = new AudioNode();
		node_audio->set_name(u8"背景音乐");
		node_audio->set_audio(ResourcesManager::instance()->find_audio("bgm"));
		world_tree->add_child(node_audio);
	}
	{
		Node* node_brave = new Node();
		node_brave->set_name(u8"勇者");
		node_brave->set_position({ 75, 125 });
		{
			TextureNode* node_texture = new TextureNode();
			node_texture->set_name(u8"动画静帧");
			node_texture->set_texture(ResourcesManager::instance()->find_texture("brave"));
			node_brave->add_child(node_texture);

			TextNode* node_text = new TextNode();
			node_text->set_name(u8"你的名字");
			node_text->set_position({ -10, 155 });
			node_text->set_font_size(20);
			node_text->set_text(u8"- 芝士勇者 -");
			node_brave->add_child(node_text);

			AudioNode* node_audio = new AudioNode();
			node_audio->set_name(u8"私人笑声");
			node_audio->set_audio(ResourcesManager::instance()->find_audio("manbo"));
			node_brave->add_child(node_audio);
		}
		world_tree->add_child(node_brave);
	}
	{
		Node* node_ice_sword = new Node();
		node_ice_sword->set_name(u8"寒冰大剑");
		node_ice_sword->set_position({ 300, 120 });
		{
			TextureNode* node_texture = new TextureNode();
			node_texture->set_name(u8"冰剑");
			node_texture->set_texture(ResourcesManager::instance()->find_texture("BlueSword"));
			node_ice_sword->add_child(node_texture);

			TextNode* node_text = new TextNode();
			node_text->set_name(u8"剑名");
			node_text->set_position({ -40, 65 });
			node_text->set_font_size(15);
			node_text->set_text(u8"稀有 • 寒冰大剑");
			node_ice_sword->add_child(node_text);
		}
		world_tree->add_child(node_ice_sword);
	}
	{
		Node* node_ice_sword = new Node();
		node_ice_sword->set_name(u8"雷火剑");
		node_ice_sword->set_position({ 300, 220 });
		{
			TextureNode* node_texture = new TextureNode();
			node_texture->set_name(u8"火剑");
			node_texture->set_texture(ResourcesManager::instance()->find_texture("RedSword"));
			node_ice_sword->add_child(node_texture);

			TextNode* node_text = new TextNode();
			node_text->set_name(u8"剑名");
			node_text->set_position({ -30, 65 });
			node_text->set_font_size(15);
			node_text->set_text(u8"罕见 • 雷火剑");
			node_ice_sword->add_child(node_text);
		}
		world_tree->add_child(node_ice_sword);
	}
}

void CompositePattern::render_tree_view(Node* node)
{
	static const ImVec2 size_icon = { ImGui::GetTextLineHeight(), ImGui::GetTextLineHeight() };
	static const ImGuiTreeNodeFlags base_flag = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	bool is_open = false;
	bool has_child = !node->get_child_list().empty();

	if (!has_child)
	{
		ImGui::TreeNodeEx((void*)node->get_name().c_str(), base_flag
			| ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen |
			((node_selected == node) ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None), "");
	}
	else
	{
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		is_open = ImGui::TreeNodeEx((void*)node->get_name().c_str(), base_flag |
			((node_selected == node) ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None), "");
	}

	if (ImGui::IsItemClicked(ImGuiMouseButton_Left) || ImGui::IsItemClicked(ImGuiMouseButton_Right))
		node_selected = node;

	if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
		need_show_popup = true;

	ImGui::SameLine();
	ImGui::Image(node->get_icon(), size_icon);
	ImGui::SameLine();
	ImGui::TextUnformatted(node->get_name().c_str());

	if (has_child && is_open)
	{
		for (Node* child : node->get_child_list())
			render_tree_view(child);

		ImGui::TreePop();
	}
}

bool CompositePattern::render_menu_item(SDL_Texture* texture, const char* text)
{
	static const ImVec2 size_icon = { ImGui::GetTextLineHeight(), ImGui::GetTextLineHeight() };

	ImGui::Image(texture, size_icon);
	ImGui::SameLine();
	return ImGui::MenuItem(text);
}
