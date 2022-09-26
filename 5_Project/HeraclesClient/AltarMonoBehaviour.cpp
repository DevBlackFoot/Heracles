#include "pch.h"
#include "AltarMonoBehaviour.h"
#include "AltarState.h"
#include "SpriteAnimator.h"

namespace Heracles
{
	namespace Altar
	{
		AltarMonoBehaviour::AltarMonoBehaviour(std::weak_ptr<GameObject> obj)
			:MonoBehaviour(obj)
		{
		}

		AltarMonoBehaviour::~AltarMonoBehaviour()
		{
		}

		void AltarMonoBehaviour::OnTriggerEnter(std::shared_ptr<ColliderBase> other)
		{
			this->GetGameObject()->GetComponent<AltarState>()->SetTrigger(true);
			this->GetGameObject()->GetComponent<SpriteAnimator>()->SetIsEnable(true);
		}
		void AltarMonoBehaviour::OnTriggerStay(std::shared_ptr<ColliderBase> other)
		{
			//this->GetGameObject()->GetComponent<AltarState>()->SetTrigger(true);
		}
		void AltarMonoBehaviour::OnTriggerExit(std::shared_ptr<ColliderBase> other)
		{
			this->GetGameObject()->GetComponent<AltarState>()->SetTrigger(false);
			this->GetGameObject()->GetComponent<SpriteAnimator>()->SetIsEnable(false);
		}
	}
}