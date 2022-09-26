#include "pch.h"
#include "WeaponAltarMonoBehaviour.h"
#include "WeaponAltarState.h"
#include "SpriteAnimator.h"

namespace Heracles
{
	namespace Altar
	{
		WeaponAltarMonoBehaviour::WeaponAltarMonoBehaviour(std::weak_ptr<GameObject> obj)
			: MonoBehaviour(obj)
		{
		}

		WeaponAltarMonoBehaviour::~WeaponAltarMonoBehaviour()
		{
		}

		void WeaponAltarMonoBehaviour::OnTriggerEnter(std::shared_ptr<ColliderBase> other)
		{
			this->GetGameObject()->GetComponent<WeaponAltarState>()->SetTrigger(true);
			this->GetGameObject()->GetComponent<SpriteAnimator>()->SetIsEnable(true);

		}

		void WeaponAltarMonoBehaviour::OnTriggerStay(std::shared_ptr<ColliderBase> other)
		{
		}

		void WeaponAltarMonoBehaviour::OnTriggerExit(std::shared_ptr<ColliderBase> other)
		{
			this->GetGameObject()->GetComponent<WeaponAltarState>()->SetTrigger(false);
			this->GetGameObject()->GetComponent<SpriteAnimator>()->SetIsEnable(false);

		}
	}
}


