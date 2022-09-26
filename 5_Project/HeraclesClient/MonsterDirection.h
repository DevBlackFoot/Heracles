#pragma once
#include "MonoBehaviour.h"


namespace Heracles
{
	namespace Monster
	{
		class MonsterDirection : public MonoBehaviour
		{
		private:

		public:
			virtual void OnTriggerEnter(std::shared_ptr<ColliderBase> other) override;
			virtual void OnTriggerStay(std::shared_ptr<ColliderBase> other) override;
			virtual void OnTriggerExit(std::shared_ptr<ColliderBase> other) override;
		};

	}
}

