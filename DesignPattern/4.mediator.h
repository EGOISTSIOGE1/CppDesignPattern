#ifndef _MEDIATOR_H_
#define _MEDIATOR_H_

#include "example.h"
#include "animation.h"

namespace _MediatorPattern
{
	class Colleague;

	class Mediator
	{
	public:
		virtual ~Mediator() = default;
		virtual void send(const std::string& message, Colleague* colleague) = 0;

	};

	class Colleague
	{
	public:
		Colleague(Mediator* _mediator) : mediator(_mediator) {}
		virtual ~Colleague() = default;

		virtual void on_update(float delta) = 0;
		virtual void receive(const std::string& message) = 0;

	protected:
		Mediator* mediator;

	};

	class ChatRoom : public Mediator
	{
	private:
		std::vector<Colleague*> colleagues;

	public:
		void add_colleague(Colleague* colleague)
		{
			colleagues.push_back(colleague);
		}

		void send(const std::string& message, Colleague* sender) override
		{
			for (Colleague* colleague : colleagues)
			{
				if (colleague != sender)
					colleague->receive(message);
			}
		}
	};

	class User : public Colleague
	{
	public:
		User(Mediator* mediator, const char* atlas_path)
			: Colleague(mediator)
		{
			static int next_id = 0;
			id = next_id; next_id++;

			atlas.load(atlas_path, 6);
			animation.set_loop(true);
			animation.set_interval(0.1f);
			animation.add_frame(&atlas);
		}

		~User() = default;

		void on_update(float delta) override;

		void receive(const std::string& message) override
		{
			str_chat_buffer.append("[RECEIVE]: " + message + "\n");
		}

	private:
		int id = 0;
		Atlas atlas;
		Animation animation;
		std::string str_chat_buffer;
		std::string str_input_buffer;

	private:
		void send(const std::string& message)
		{
			mediator->send(message, this);
		}

	};
}

class MediatorPattern : public Example
{
public:
	MediatorPattern();
	~MediatorPattern();

	void on_update(float delta) override;

private:
	_MediatorPattern::ChatRoom chat_room;
	_MediatorPattern::Colleague* user_1 = nullptr;
	_MediatorPattern::Colleague* user_2 = nullptr;
	_MediatorPattern::Colleague* user_3 = nullptr;
	_MediatorPattern::Colleague* user_4 = nullptr;

};

#endif // !_MEDIATOR_H_
