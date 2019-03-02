#pragma once

#include "fsm_api.h"

#include "Transition.h"
#include "State.h"

#include <vector>
#include <map>
#include <set>
#include <memory>
#include <QChar>

#define FSM_EPS QChar(0x3b5)

namespace FiniteStateMachines
{
	class State;

	class FINITESTATEMACHINE_EXPORT FiniteStateMachine
	{
	public:
		FiniteStateMachine(size_t num_states);

		void AddTransition(size_t from, size_t to, QChar ch);

		void SetInitialState(size_t initial_state);
		void MarkStateAsFinal(size_t state);
		void MarkStateAsRegular(size_t state);

		std::vector<Transition> GetTransitionsFrom(size_t state) const;
		size_t GetStatesCount() const;
		const State* GetState(size_t state) const;
		std::set<size_t> GetFinalStates() const;
		size_t GetInitialState() const;
		size_t GetStateIndex(const State* state) const;
		bool ContainsEpsilonTransitions() const;

	private:
		size_t m_num_states;
		size_t m_initial_state;
		std::set<size_t> m_final_states;
		std::map<size_t/*index*/, std::shared_ptr<State>> m_states;
		std::map<size_t/*from*/, std::vector<Transition>> m_transitions;
		bool m_contains_eps_transitions;
	};
}//FiniteStateMachines
