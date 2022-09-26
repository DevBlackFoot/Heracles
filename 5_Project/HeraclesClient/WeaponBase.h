#pragma once


namespace Heracles
{
	namespace Weapon
	{
		class WeaponSkillBase
		{
		public:
			float skillCoolTime;
			int skillDamage;
			int id;
			
		public:
			WeaponSkillBase();


		};


		class WeaponBase : public ComponentBase
		{
		protected:

			std::shared_ptr<WeaponSkillBase> weaponSkill;

			int weaponId;
			std::string weaponName;
			int weaponMaxLevel;
			float changeCoolTime;
			Vector size;

			Vector relativePosition;

		public:
			int weaponLevel;
			int attackDamage;
			
			bool upgraded = false;

		public:
			WeaponBase(std::weak_ptr<GameObject> obj);
			~WeaponBase();

			virtual void Activate() = 0;
			virtual void Deactivate() = 0;


		};
	}
}

