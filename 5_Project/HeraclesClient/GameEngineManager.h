#pragma once

#include "ComponentBase.h"

namespace Heracles
{

	class GameEngineManager : public ComponentBase
	{
	private:
		std::shared_ptr<InputManager> input;

	public:
		GameEngineManager(std::weak_ptr<GameObject> gameObj);
		virtual  ~GameEngineManager() = default;

		virtual void Awake() override;
		virtual void LateUpdate(float tick) override;
	};

}
