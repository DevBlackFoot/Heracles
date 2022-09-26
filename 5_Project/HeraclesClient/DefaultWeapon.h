#pragma once
#include "WeaponBase.h"


namespace Heracles
{
	namespace Weapon
	{
		class DefaultSkill : public WeaponSkillBase
		{
		public:

		public:
			DefaultSkill();
			~DefaultSkill();
		};


		class DefaultWeapon : public WeaponBase
		{
		private:

		public:
			DefaultWeapon(std::weak_ptr<GameObject> obj);
			~DefaultWeapon();

			virtual void Awake() override;

			virtual void Activate() override;
			virtual void Deactivate() override;

		};
	}
}

