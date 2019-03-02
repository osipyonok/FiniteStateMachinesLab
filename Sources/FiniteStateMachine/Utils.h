#pragma once

#include "fsm_api.h"

#include "FiniteStateMachine.h"

#include <QString>
#include <set>


namespace FiniteStateMachines
{
	namespace Utils
	{
		FINITESTATEMACHINE_EXPORT QString CanProduceWordWithTwiceRepeatedW(const FiniteStateMachine& original_fsm, const QString& w);

		FINITESTATEMACHINE_EXPORT std::set<const FiniteStateMachines::State*> Parse(const FiniteStateMachine& original_fsm, const QString& w);

		FINITESTATEMACHINE_EXPORT std::pair<bool,QString> TryToGetToTheState(const FiniteStateMachine& original_fsm, size_t target_state);

		FINITESTATEMACHINE_EXPORT std::pair<QString, const State*> GetAnyPath(const FiniteStateMachine& original_fsm);

		FINITESTATEMACHINE_EXPORT FiniteStateMachine RemoveEpsilonTransitions(const FiniteStateMachine& fsm);
	}//Utils
}//FiniteStateMachine::Utils