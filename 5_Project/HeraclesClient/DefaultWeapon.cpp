#include "pch.h"
#include "DefaultWeapon.h"

namespace Heracles
{
	namespace Weapon
	{
		DefaultSkill::DefaultSkill()
		{
			this->skillCoolTime = 5.f;
			this->skillDamage = 1;
			this->id = 0;
		}
		DefaultSkill::~DefaultSkill()
		{
		}

		DefaultWeapon::DefaultWeapon(std::weak_ptr<GameObject> obj)
			:WeaponBase(obj)
		{
			this->weaponSkill = std::make_shared<DefaultSkill>();
			this->weaponId = 0;
			this->weaponName = "Default";
			this->weaponMaxLevel = 2;
			this->changeCoolTime = 10.f;
			this->size = Vector(1.f, 5.f, 1.f);

			this->weaponLevel = 1;
			this->attackDamage = 1;
			this->upgraded = false;
			
		}
		DefaultWeapon::~DefaultWeapon()
		{
		}
		void DefaultWeapon::Awake()
		{
		}
		void DefaultWeapon::Activate()
		{
		}
		void DefaultWeapon::Deactivate()
		{
		}
	}
}

