#pragma once
#include "MonoBehaviour.h"

namespace Heracles
{
	class RewardBoxMonoBehaviour : public MonoBehaviour
	{
	private:
		int index;
		std::string type;
		bool animationPlaying = false;
		std::shared_ptr<GameObject> reward;
		bool triggered = false;
	public:
		RewardBoxMonoBehaviour(std::weak_ptr<GameObject> obj);

		virtual void Update(float tick) override;

		virtual void OnTriggerEnter(std::shared_ptr<ColliderBase> other) override;
		virtual void OnTriggerStay(std::shared_ptr<ColliderBase> other) override;
		virtual void OnTriggerExit(std::shared_ptr<ColliderBase> other) override;

		inline void SetIndex(int idx) { this->index = idx; }
		inline void SetType(const std::string& type) { this->type = type; }
		inline void SetRewardObject(std::shared_ptr<GameObject> obj) { this->reward = obj; }
	};

}
