#ifndef _EXAMPLE_MANAGER_H_
#define _EXAMPLE_MANAGER_H_

#include "example.h"

#include <string>
#include <vector>
#include <unordered_map>

class ExampleManager
{
public:
	static ExampleManager* instance()
	{
		if (!manager)
			manager = new ExampleManager();

		return manager;
	}

	void init(SDL_Renderer* renderer);

	void on_input(const SDL_Event* event);
	void on_update(float delta);
	void on_render();

private:
	struct MenuItem
	{
		std::string id;
		SDL_Texture* icon = nullptr;
		std::string title;

		MenuItem(const std::string& _id, SDL_Texture* _icon, const std::string& _title)
			: id(_id), icon(_icon), title(_title) { }
	};

	struct Subject
	{
		std::string title;
		std::vector<MenuItem> item_list;
	};

private:
	static ExampleManager* manager;
	SDL_Renderer* renderer = nullptr;

	std::string current_example_id;
	Example* current_example = nullptr;
	
	SDL_Texture* texture_target = nullptr;
	std::unordered_map<std::string, Example*> example_pool;
	Subject subject_creational, subject_structural, subject_behavioral;

private:
	ExampleManager();
	~ExampleManager();

	void on_update_blank_content();
	void switch_to(const std::string& id);
	void on_update_subject(const Subject& subject);
	void add_example(Subject& subject, const MenuItem& menu_item, Example* example);

};

#endif // !_EXAMPLE_MANAGER_H_
