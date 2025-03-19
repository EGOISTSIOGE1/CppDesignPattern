#ifndef _COMPOSITE_H_
#define _COMPOSITE_H_

#include "vector2.h"
#include "example.h"
#include "resources_manager.h"

#include <string>
#include <vector>

namespace _CompositePattern
{
	class Node
	{
	public:
		using NodeList = std::vector<Node*>;

	public:
		Node()
		{
			name = "Node";
			texture_icon = ResourcesManager::instance()->find_texture("icon-node");
		}

		virtual ~Node()
		{
			for (Node* child : child_list)
				delete child;
		}

		virtual void on_inspect();
		virtual void on_update(float delta);
		virtual void on_render(SDL_Renderer* renderer);

		Node* get_parent() const
		{
			return parent;
		}

		void set_name(const std::string& name)
		{
			this->name = name;
		}

		const std::string& get_name() const
		{
			return name;
		}

		void set_position(const Vector2& position)
		{
			this->position = position;
		}

		const Vector2& get_position() const
		{
			return world_position;
		}

		SDL_Texture* get_icon() const
		{
			return texture_icon;
		}

		void add_child(Node* child)
		{
			child->parent = this;
			child_list.push_back(child);
		}

		void del_child(Node* child)
		{
			child_list.erase(std::remove(child_list.begin(),
				child_list.end(), child), child_list.end());
		}

		NodeList& get_child_list()
		{
			return child_list;
		}

	protected:
		std::string name;
		Vector2 position;
		Vector2 world_position;

		NodeList child_list;
		Node* parent = nullptr;

		SDL_Texture* texture_icon = nullptr;

	};

	class TextureNode : public Node
	{
	public:
		TextureNode() 
		{ 
			name = "TextureNode";
			texture_icon = ResourcesManager::instance()->find_texture("icon-texture");
		}
		~TextureNode();

		void on_inspect() override;
		void on_render(SDL_Renderer* renderer) override;

		void set_texture(SDL_Texture* texture);

	private:
		Vector2 size;
		float rotation = 0;
		int idx_texture = 0;
		SDL_Texture* texture = nullptr;

	private:
		void update_size();

	};

	class TextNode : public Node
	{
	public:
		TextNode() 
		{ 
			name = "TextNode"; 
			texture_icon = ResourcesManager::instance()->find_texture("icon-text");
		}
		~TextNode() = default;

		void on_inspect() override;
		void on_render(SDL_Renderer* renderer) override;

		void set_font_size(int font_size);
		void set_text(const std::string& text);

	private:
		Vector2 size;
		int font_size = 35;
		bool need_update = true;
		std::string text = u8"文本节点";
		SDL_Texture* texture_text = nullptr;
		float color_text[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	private:
		void update_texture_text(SDL_Renderer* renderer);

	};

	class AudioNode : public Node
	{
	public:
		AudioNode()
		{ 
			name = "AudioNode";
			texture_icon = ResourcesManager::instance()->find_texture("icon-audio");
		}
		~AudioNode() = default;

		void on_inspect() override;

		void set_audio(Mix_Chunk* audio);

	private:
		int idx_audio = 0;
		std::string id_audio;

	};
}

class CompositePattern : public Example
{
public:
	CompositePattern(SDL_Renderer* renderer);
	~CompositePattern();

	void on_update(float delta) override;
	void on_render(SDL_Renderer* renderer) override;

private:
	bool need_show_popup = false;
	SDL_Texture* texture_target = nullptr;
	_CompositePattern::Node* world_tree = nullptr;
	_CompositePattern::Node* node_selected = nullptr;

private:
	void init_world_tree();
	void render_tree_view(_CompositePattern::Node* node);
	bool render_menu_item(SDL_Texture* texture, const char* text);

};

#endif // !_COMPOSITE_H_
