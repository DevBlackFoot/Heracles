#pragma once
#include "MonoBehaviour.h"

namespace Heracles
{
	class Money : public MonoBehaviour
	{
	private:
		std::shared_ptr<Transform> transform;

	public:
		Vector initialPos;
	public:
		Money(std::weak_ptr<GameObject> obj);

		virtual void Start() override;

		virtual void Update(float tick) override;

		virtual void OnTriggerEnter(std::shared_ptr<ColliderBase> other) override;
		virtual void OnTriggerStay(std::shared_ptr<ColliderBase> other) override;
		virtual void OnTriggerExit(std::shared_ptr<ColliderBase> other) override;
	};
}

