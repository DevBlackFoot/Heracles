#pragma once
#include "ComponentBase.h"

namespace Heracles
{
	class TestController : public ComponentBase
	{
	private:
		std::shared_ptr<Transform> transform;
		std::shared_ptr<InputManager> input;
	public:
		TestController(std::weak_ptr<GameObject> obj);
		~TestController();

		virtual void Awake() override;
		virtual void Update(float tick) override;
		virtual void LateUpdate(float tick) override;
	};
}

