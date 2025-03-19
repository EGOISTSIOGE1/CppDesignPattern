#ifndef _CHAIN_OF_RESPONSIBILITY_H_
#define _CHAIN_OF_RESPONSIBILITY_H_

#include "example.h"

#include <imgui.h>

namespace _ChainOfResponsibilityPattern
{
	class ClickHandler
	{
	public:
		virtual ~ClickHandler() = default;

		virtual void handle() = 0;
		virtual void on_update() = 0;

		void set_next(ClickHandler* next)
		{
			this->next = next;
		}

	protected:
		ClickHandler* next = nullptr;

	};

	class ChildWindow : public ClickHandler
	{
	public:
		ChildWindow(int _id, const ImVec4& _color, const ImVec2& _size, const ImVec2& _offset)
			: id(_id), color(_color), size(_size), offset(_offset) { }
		~ChildWindow() = default;

		void handle() override;
		void on_update() override;

	private:
		int id;
		ImVec2 size;
		ImVec4 color;
		ImVec2 offset;

	};
}

class ChainOfResponsibilityPattern : public Example
{
public:
	ChainOfResponsibilityPattern();
	~ChainOfResponsibilityPattern();

	void on_update(float delta) override;

private:
	_ChainOfResponsibilityPattern::ChildWindow window_1, window_2, window_3;

};

#endif // !_CHAIN_OF_RESPONSIBILITY_H_
