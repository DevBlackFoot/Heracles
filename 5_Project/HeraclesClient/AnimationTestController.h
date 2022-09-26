#pragma once

#include "ComponentBase.h"
#include "Animator.h"

namespace Heracles
{
	class GameEngineSpace::Animator;

	class AnimationTestController : public ComponentBase
	{
	private:
		std::shared_ptr<InputManager> input;
		std::shared_ptr<GameEngineSpace::Animator> animator;

	public:
		AnimationTestController(std::weak_ptr<GameObject> gameObj);
		virtual ~AnimationTestController() = default;

		virtual void Awake() override;
		virtual void LateUpdate(float tick) override;
	};

}
