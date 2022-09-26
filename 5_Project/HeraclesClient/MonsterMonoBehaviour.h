#pragma once
#include "MonoBehaviour.h"

namespace Heracles
{
	namespace Monster
	{

		class MonsterMonoBehaviour : public MonoBehaviour
		{
		private:
			bool detectPlayer = false;
			GraphicsEngineSpace::Canvas* debugCanvas;

		public:
			MonsterMonoBehaviour(std::weak_ptr<GameObject> obj);

			virtual void Awake() override;

			virtual void PreUpdate(float tick) override;

			virtual void LateUpdate(float tick) override;

			virtual void OnTriggerEnter(std::shared_ptr<ColliderBase> other) override;
			virtual void OnTriggerExit(std::shared_ptr<ColliderBase> other) override;
			virtual void OnTriggerStay(std::shared_ptr<ColliderBase> other) override;
		};
	}
}

