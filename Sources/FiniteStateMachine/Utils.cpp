#include "Utils.h"

#include <vector>
#include <queue>
#include <functional>

#include <QDebug>

//xwyw
QString FiniteStateMachines::Utils::CanProduceWordWithTwiceRepeatedW(const FiniteStateMachine& original_fsm, const QString& w)
{
	FiniteStateMachine fsm = RemoveEpsilonTransitions(original_fsm);

	std::vector<std::pair<size_t, size_t>> possible_start_positions_to_produce_w; // begin end

	#pragma region finding_possible_start_positions_to_produce_w
	{
		FiniteStateMachine tmp_fsm(fsm.GetStatesCount());
		for (size_t state = 0; state < fsm.GetStatesCount(); ++state)
		{
			tmp_fsm.MarkStateAsFinal(state);
			for (const auto& transition : fsm.GetTransitionsFrom(state))
				tmp_fsm.AddTransition(transition.GetFrom()->GetIndex(), 
				 transition.GetTo()->GetIndex(), 
					transition.GetChar());
		}

		for (size_t state = 0; state < fsm.GetStatesCount(); ++state)
		{
			for (const auto& transition : fsm.GetTransitionsFrom(state))
			{
				if (transition.GetChar() == w[0])
				{
					tmp_fsm.SetInitialState(state);

					for(const auto p_finish : Parse(tmp_fsm, w))
					{
						possible_start_positions_to_produce_w.emplace_back(state, p_finish->GetIndex());
					}
				}
			}
		}
	}
	#pragma endregion 


	std::vector<std::pair<QString, State const*>> first_halfs;

	#pragma region finding_first_preffix
	
	for(const auto& target : possible_start_positions_to_produce_w)
	{
		const auto cur = TryToGetToTheState(fsm, target.first);
		if(cur.first)
		{
			first_halfs.emplace_back(cur.second + w, fsm.GetState(target.second));
		}
	}

	#pragma endregion 


	#pragma region finding_last_segment_of_w

	FiniteStateMachine tmp_fsm(fsm.GetStatesCount());
	for (size_t state = 0; state < fsm.GetStatesCount(); ++state)
	{
		for (const auto& transition : fsm.GetTransitionsFrom(state))
			tmp_fsm.AddTransition(transition.GetFrom()->GetIndex(),
				transition.GetTo()->GetIndex(),
				transition.GetChar());
	}

	bool has_final_state = false;
	for(const auto start_end_pair : possible_start_positions_to_produce_w)
	{
		if(fsm.GetState(start_end_pair.second)->IsFinal())
		{
			has_final_state = true;
			tmp_fsm.MarkStateAsFinal(start_end_pair.first);
		}
	}

	if (!has_final_state)
		return "";

	for(const auto& first_half : first_halfs)
	{
		tmp_fsm.SetInitialState(first_half.second->GetIndex());

		auto cur = GetAnyPath(tmp_fsm);
		if(!cur.second)
			continue;

		const auto solution = first_half.first + cur.first + w;
		qDebug() << "Solution: " << solution;
		return solution;
	}

	#pragma endregion 

	return "";
}


std::set<const FiniteStateMachines::State*> FiniteStateMachines::Utils::Parse(const FiniteStateMachine& original_fsm, const QString& w)
{
	FiniteStateMachine fsm = RemoveEpsilonTransitions(original_fsm);

	std::queue<std::pair<size_t , size_t>> q;
	q.push({ fsm.GetInitialState() , 0 });

	std::set<const FiniteStateMachines::State*> solution;

	while(!q.empty())
	{
		const auto top	 = q.front();
		q.pop();
		const auto state = top.first;
		const auto dist  = top.second;

		if (dist == static_cast<size_t>(w.length()) && fsm.GetFinalStates().count(state) > 0)
		{
			solution.insert(fsm.GetState(state));
			continue;
		}


		if (dist >= w.length())
			continue;

		for(const auto& transition : fsm.GetTransitionsFrom(state))
		{
			if (transition.GetChar() != w.at(dist))
				continue;

			q.push({ fsm.GetStateIndex(transition.GetTo()) , dist + 1 });
		}
	}

	return solution;
}

std::pair<bool, QString> FiniteStateMachines::Utils::TryToGetToTheState(const FiniteStateMachine& original_fsm, size_t target_state)
{
	FiniteStateMachine fsm = RemoveEpsilonTransitions(original_fsm);

	std::vector<bool> used(fsm.GetStatesCount());
	std::function<QString(size_t, bool&)> impl = [&](size_t cur, bool& success) -> QString
	{
		if(cur == target_state)
		{
			success = true;
			return "";
		}

		used[cur] = true;

		for(const auto transition : fsm.GetTransitionsFrom(cur))
		{
			const auto ind = fsm.GetStateIndex(transition.GetTo());
			if(used[ind])
				continue;

			bool result = false;
			const auto str = impl(ind, result);
			if(result)
			{
				success = true;
				return QString(1, transition.GetChar()) + str;
			}
		}

		success = false;
		return "";
	};

	bool success = false;
	const auto str = impl(fsm.GetInitialState(), success);

	return { success , str };
}

std::pair<QString, const FiniteStateMachines::State*> FiniteStateMachines::Utils::GetAnyPath(
	const FiniteStateMachine& original_fsm)
{
	FiniteStateMachine fsm = RemoveEpsilonTransitions(original_fsm);

	std::vector<int> parent(fsm.GetStatesCount(), -1);
	std::vector<QChar> parent_char(fsm.GetStatesCount());
	parent[fsm.GetInitialState()] = -2;
	std::queue<size_t> q;
	q.push(fsm.GetInitialState());

	size_t reached_final_state = -1;

	while (!q.empty())
	{
		const size_t top = q.front();
		q.pop();

		if(fsm.GetState(top)->IsFinal())
		{
			reached_final_state = top;
			break;
		}

		for(const auto transition : fsm.GetTransitionsFrom(top))
		{
			if(parent[transition.GetTo()->GetIndex()] == -1)
			{
				parent[transition.GetTo()->GetIndex()] = top;
				parent_char[transition.GetTo()->GetIndex()] = transition.GetChar();
				q.push(transition.GetTo()->GetIndex());
			}
		}
	}

	if (static_cast<size_t>(-1) == reached_final_state)
		return { "" , nullptr };

	QString str;
	size_t cur = reached_final_state;
	while(-2 != parent[cur])
	{
		str += parent_char[cur];
		cur = parent[cur];
	}
	std::reverse(str.begin(), str.end());

	return { str , fsm.GetState(reached_final_state) };
}

FiniteStateMachines::FiniteStateMachine FiniteStateMachines::Utils::RemoveEpsilonTransitions(
	const FiniteStateMachine& fsm)
{
	if (!fsm.ContainsEpsilonTransitions())
		return fsm;

	const size_t n_states = fsm.GetStatesCount();

	std::vector<std::vector<bool>> eps_graph(n_states, std::vector<bool>(n_states));
	for(size_t i = 0 ; i < n_states; ++i)
	{
		for(const auto transition : fsm.GetTransitionsFrom(i))
		{
			if(transition.GetChar() == FSM_EPS)
			{
				eps_graph[i][transition.GetTo()->GetIndex()] = true;
			}
		}
	}

	for(size_t k = 0; k < n_states; ++k)
	{
		for(size_t i = 0; i < n_states; ++i)
		{
			for(size_t j = 0; j < n_states; ++j)
			{
				if (eps_graph[i][k] && eps_graph[k][j])
					eps_graph[i][j] = true;
			}
		}
	}

	FiniteStateMachine solution(n_states);
	solution.SetInitialState(fsm.GetInitialState());
	for(size_t i = 0 ; i < n_states; ++i)
	{
		for(const auto transition : fsm.GetTransitionsFrom(i))
		{
			if (transition.GetChar() != FSM_EPS)
			{
				solution.AddTransition(transition.GetFrom()->GetIndex(), transition.GetTo()->GetIndex(), transition.GetChar());
			}
		}
		if(fsm.GetState(i)->IsFinal())
		{
			solution.MarkStateAsFinal(i);
		}
	}

	for(size_t i = 0; i < n_states; ++i)
	{
		for(size_t j = 0; j < n_states; ++j)
		{
			if(eps_graph[i][j])
			{
				for(const auto transition : fsm.GetTransitionsFrom(j))
				{
					if (transition.GetChar() != FSM_EPS)
					{
						solution.AddTransition(i, transition.GetTo()->GetIndex(), transition.GetChar());
					}
				}
			}
		}
	}

	Q_ASSERT(!solution.ContainsEpsilonTransitions());
	return solution;
}
