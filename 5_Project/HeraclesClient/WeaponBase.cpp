#include "pch.h"
#include "WeaponBase.h"

namespace Heracles
{
	namespace Weapon 
	{
		WeaponBase::WeaponBase(std::weak_ptr<GameObject> obj)
			:ComponentBase(obj,ComponentType::MONOBEHAVIOR)
		{
		}
		WeaponBase::~WeaponBase()
		{
		}



		WeaponSkillBase::WeaponSkillBase()
		{
		}
	}
}