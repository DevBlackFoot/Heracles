#include "pch.h"
#include "GameEngineManager.h"

#include "InputManager.h"
#include "UIManager.h"
#include "SceneManager.h"

namespace Heracles
{
	GameEngineManager::GameEngineManager(std::weak_ptr<GameObject> gameObj)
		: ComponentBase(gameObj)
	{
	}

	void GameEngineManager::Awake()
	{
		input = InputManager::GetInstance();
	}

	void GameEngineManager::LateUpdate(float tick)
	{
		if(UIManager::GetInstance()->GetButtonState("TestButton") == ButtonState::UP)
		{
			SceneManager::GetInstance()->LoadScene(TEXT("DemoScene"));
		}

		if(input->GetInputState('0', KeyState::DOWN))
			SceneManager::GetInstance()->ChangeTick(0.5f);

		if(input->GetInputState('9', KeyState::DOWN))
			SceneManager::GetInstance()->SetTickOrigin();
	}
}
