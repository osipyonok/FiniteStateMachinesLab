#pragma once

#include "fsm_api.h"

#include "State.h"

#include <QChar>

namespace FiniteStateMachines
{
	class FINITESTATEMACHINE_EXPORT Transition
	{
	public:
		Transition(State *from, State *to, QChar ch)
			: mp_from{ from }
			, mp_to{ to }
			, m_char{ ch }
		{}

		State* GetFrom() const { return mp_from; }
		State* GetTo() const { return mp_to; }
		QChar  GetChar() const { return m_char; }

	private:
		State* mp_from;
		State* mp_to;
		QChar  m_char;
	};
}