#pragma once

#include "ComponentBase.h"

namespace Heracles
{
	using namespace GameEngineSpace;
	


	class CharacterController : public ComponentBase
	{
	private:
		std::shared_ptr<Transform> transform;
		std::shared_ptr<InputManager> input;

	public:
		CharacterController(std::weak_ptr<GameObject> obj);
		~CharacterController();

		virtual void Awake() override;
		virtual void Update(float tick) override;
		virtual void LateUpdate(float tick) override;
	};
}


