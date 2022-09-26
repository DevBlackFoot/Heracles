#pragma once
#include "MonoBehaviour.h"

namespace Heracles
{
	namespace Altar
	{
		class AltarMonoBehaviour : public MonoBehaviour
		{

		public:
			AltarMonoBehaviour(std::weak_ptr<GameObject> obj);

			virtual ~AltarMonoBehaviour();

		public:
			virtual void OnTriggerEnter(std::shared_ptr<ColliderBase> other) override;
			virtual void OnTriggerStay(std::shared_ptr<ColliderBase> other) override;
			virtual void OnTriggerExit(std::shared_ptr<ColliderBase> other) override;
		};
	}
}

