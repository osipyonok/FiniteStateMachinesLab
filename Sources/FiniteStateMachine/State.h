#pragma once

#include "fsm_api.h"


namespace FiniteStateMachines
{
	class FINITESTATEMACHINE_EXPORT State
	{
	public:
		explicit State(size_t index, bool is_final = false)
			: m_index{ index }
			, m_is_final{ is_final }
		{}

		size_t GetIndex() const { return  m_index; }
		bool   IsFinal() const { return m_is_final; }

		void SetFinal(bool is_final) { m_is_final = is_final; }

	private:
		const size_t m_index;
		bool m_is_final;
	};
}