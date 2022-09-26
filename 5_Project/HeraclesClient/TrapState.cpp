#include "pch.h"
#include "TrapState.h"

namespace Heracles
{
	namespace Trap
	{
		TrapState::TrapState(std::weak_ptr<GameObject> obj)
			:ComponentBase(obj,ComponentType::MONOBEHAVIOR)
		{
		}
		void TrapState::Update(float tick)
		{
		}
		void TrapState::LateUpdate(float tick)
		{
		}
	}
}