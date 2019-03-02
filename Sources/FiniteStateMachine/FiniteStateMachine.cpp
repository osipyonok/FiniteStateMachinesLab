#include "FiniteStateMachine.h"

namespace FiniteStateMachines
{
	FiniteStateMachine::FiniteStateMachine(size_t num_states)
		: m_num_states{ num_states }
		, m_contains_eps_transitions { false }
	{
		Q_ASSERT(num_states > 0);

		for (size_t i = 0; i < m_num_states; ++i)
		{
			m_states[i] = std::make_shared<State>(i);
		}
	}

	void FiniteStateMachine::AddTransition(size_t from, size_t to, QChar ch)
	{
		Q_ASSERT(from >= 0 && from < m_num_states);
		Q_ASSERT(to >= 0 && to < m_num_states);

		m_transitions[from].emplace_back(m_states[from].get(), m_states[to].get(), ch);

		if (FSM_EPS == ch)
			m_contains_eps_transitions = true;
	}

	void FiniteStateMachine::SetInitialState(size_t initial_state)
	{
		Q_ASSERT(initial_state >= 0 && initial_state < m_num_states);

		m_initial_state = initial_state;
	}

	void FiniteStateMachine::MarkStateAsFinal(size_t state)
	{
		Q_ASSERT(state >= 0 && state < m_num_states);

		m_states[state]->SetFinal(true);
		m_final_states.insert(state);
	}

	void FiniteStateMachine::MarkStateAsRegular(size_t state)
	{
		Q_ASSERT(state >= 0 && state < m_num_states);

		m_states[state]->SetFinal(false);
		m_final_states.erase(state);
	}

	std::vector<Transition> FiniteStateMachine::GetTransitionsFrom(size_t state) const
	{
		Q_ASSERT(state >= 0 && state < m_num_states);

		const auto it = m_transitions.find(state);
		return m_transitions.end() != it ? it->second : std::vector<Transition>();
	}

	size_t FiniteStateMachine::GetStatesCount() const
	{
		return m_num_states;
	}

	const State* FiniteStateMachine::GetState(size_t state) const
	{
		Q_ASSERT(state >= 0 && state < m_num_states);
		return m_states.at(state).get();
	}

	std::set<size_t> FiniteStateMachine::GetFinalStates() const
	{
		return m_final_states;
	}

	size_t FiniteStateMachine::GetInitialState() const
	{
		return m_initial_state;
	}

	size_t FiniteStateMachine::GetStateIndex(const State* state) const
	{
		Q_ASSERT(state);

		const auto it = std::find_if(m_states.begin(), m_states.end(), [state](const auto& item)
		{
			return item.second.get() == state;
		});

		if (it != m_states.end())
			return it->first;

		return std::numeric_limits<size_t>::max();
	}

	bool FiniteStateMachine::ContainsEpsilonTransitions() const
	{
		return m_contains_eps_transitions;
	}
}//FiniteStateMachines
