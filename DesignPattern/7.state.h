#ifndef  _STATE_H_
#define _STATE_H_

#include "example.h"
#include "animation.h"

namespace _StatePattern
{
	class State
	{
	public:
		virtual ~State() = default;
		virtual void on_update(float delta) = 0;
		virtual void on_render(SDL_Renderer* renderer) = 0;

	};

	class AnimatedState : public State
	{
	public:
		AnimatedState(const char* atlas_temp, int num)
		{
			atlas.load(atlas_temp, num);
			animation.add_frame(&atlas);
			animation.set_interval(0.1f);
			animation.set_loop(true);
		}

		virtual void on_update(float delta)
		{
			animation.set_position(position);
			animation.on_update(delta);
		}

		virtual void on_render(SDL_Renderer* renderer)
		{
			animation.on_render(renderer);
		}

	protected:
		Atlas atlas;
		Vector2 position;
		Animation animation;

	};

	class AttackState : public AnimatedState
	{
	public:
		AttackState() : AnimatedState("Role_Attack%d", 6) { position = { 360, 374 }; }
		~AttackState() = default;

	};

	class IdleState : public AnimatedState
	{
	public:
		IdleState() : AnimatedState("Role_Idle%d", 6) { position = { 360, 374 }; }
		~IdleState() = default;

	};

	class JumpState : public AnimatedState
	{
	public:
		JumpState() : AnimatedState("Role_Jump%d", 10) { position = { 360, 374 }; }
		~JumpState() = default;

	};

	class RunState : public AnimatedState
	{
	public:
		RunState() : AnimatedState("Role_Run%d", 8) { position = { 360, 374 }; }
		~RunState() = default;

	};

	class Player
	{
	public:
		Player() = default;
		~Player() = default;

		void on_update(float delta)
		{
			if (!state) return;

			state->on_update(delta);
		}

		void on_render(SDL_Renderer* renderer)
		{
			if (!state) return;

			state->on_render(renderer);
		}

		void set_state(State* state)
		{
			this->state = state;
		}

	private:
		State* state = nullptr;

	};

}

class StatePattern : public Example
{
public:
	StatePattern(SDL_Renderer* renderer);
	~StatePattern();

	void on_update(float delta) override;
	void on_render(SDL_Renderer* renderer) override;

private:
	int idx_state = 0;
	_StatePattern::Player player;
	const char* state_name_list[4];
	_StatePattern::State* state_list[4];
	SDL_Texture* texture_target = nullptr;

};

#endif // ! _STATE_H_
