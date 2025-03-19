#ifndef _OBSERVER_H_
#define _OBSERVER_H_

#include "example.h"

#include <vector>

namespace _ObserverPattern
{
	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void update(int state) = 0;
		
	};

	class Subject
	{
	public:
		virtual ~Subject() = default;
		virtual void attach(Observer* observer)
		{
			observer_list.push_back(observer);
		}

		virtual void notify()
		{
			for (Observer* observer : observer_list)
				observer->update(state);
		}
		
	protected:
		int state = 0;
		std::vector<Observer*> observer_list;

	};

	class ButtonClicked : public Subject
	{
	public:
		void accumulate()
		{
			state++;
			notify();
		}

	};

	class Button1ClickedObserver : public Observer
	{
	public:
		void update(int state) override;

	};

	class Button2ClickedObserver : public Observer
	{
	public:
		void update(int state) override;

	};

	struct Achievement
	{
		SDL_Texture* texture = nullptr;

		Achievement(SDL_Texture* _texture) : texture(_texture) {}
		~Achievement() = default;
	};
}

class ObserverPattern : public Example
{
public:
	ObserverPattern();
	~ObserverPattern();

	void on_update(float delta) override;

private:
	_ObserverPattern::Button1ClickedObserver btn_1_clicked_observer;
	_ObserverPattern::Button2ClickedObserver btn_2_clicked_observer;
	_ObserverPattern::ButtonClicked button_1_clicked_subject, button_2_clicked_subject;

};

#endif // !_OBSERVER_H_
