#pragma once
#include "MonoBehaviour.h"

namespace Heracles
{
	namespace Altar
	{

		class WeaponAltarMonoBehaviour : public MonoBehaviour
		{
		public:
			WeaponAltarMonoBehaviour(std::weak_ptr<GameObject> obj);

			virtual ~WeaponAltarMonoBehaviour();

		public:
			void OnTriggerEnter(std::shared_ptr<ColliderBase> other) override;
			void OnTriggerStay(std::shared_ptr<ColliderBase> other) override;
			void OnTriggerExit(std::shared_ptr<ColliderBase> other) override;
		};

	}
}


