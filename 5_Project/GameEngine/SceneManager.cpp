#include "pch.h"
#include "SceneManager.h"

#include "SceneBase.h"
#include "Camera.h"
#include "PhysicsManager.h"
#include "Timer.h"

#include "InputManager.h"
#include "UIManager.h"

namespace GameEngineSpace
{
	std::shared_ptr<SceneManager> SceneManager::instance = nullptr;

	void SceneManager::ChangeScene()
	{
		if(currentScene != nullptr)
		{
			// 기존 Scene의 내용을 지워준다..
			currentScene->ClearScene();
		}

		currentScene = scenes.at(reservedLoadSceneName);

		reservedLoadScene = false;

		// 씬 초기화.
		currentScene->BuildScene();
		currentScene->Awake();
		currentScene->Start();
	}

	std::shared_ptr<SceneManager> SceneManager::GetInstance()
	{
		// 인스턴스가 없으면 만들어주고 있으면 인스턴스 리턴
		if(instance == nullptr)
		{
			instance = std::make_shared<SceneManager>();
		}

		return instance;
	}

	void SceneManager::Init()
	{
		// 이후에 물리를 넣으면 여기서 물리를 Init해준다.
		physicsManager = PhysicsManager::GetInstance();

		physicsManager->Init();
	}

	void SceneManager::Release()
	{
		currentScene.reset();

		physicsManager->Release();

		// 씬 전체 릴리즈
		for(auto& scene : scenes)
		{
			scene.second->ClearScene();
		}
	}

	void SceneManager::Update()
	{
		// 현재 씬이 있다면..
		if(currentScene != nullptr)
		{
			float deltaTime = Timer::GetInstance()->DeltaTime();

			// 델타 타임 테스트용 코드
				// 틱이 변했으면 -> 함수에 의해 변경된 값을 사용한다.
			if(isChangedTick == true)
			{
				deltaTime *= deltaMultiple;
				// 음수 체크를 해준다.
				if(deltaTime < 0.0f)
					deltaTime = 0.0f;
			}

			InputManager::GetInstance()->PadUpdate(deltaTime);

			// 씬 업데이트 전 충돌 업데이트..
			physicsManager->Update(deltaTime, currentScene->colliderObjInScene);

			// 순서대로 업데이트 해준다.
			currentScene->PreUpdate(deltaTime);
			currentScene->Update(deltaTime);
			currentScene->LateUpdate(deltaTime);

			// 이후 렌더 => 데이터 이동
			currentScene->Render(deltaTime);

			if(InputManager::GetInstance()->GetInputState(VK_F1, KeyState::DOWN))
			{
				isDebugString = !isDebugString;
				UIManager::GetInstance()->GetCanvas("DebugCanvas")->SetEnable(isDebugString);
			}

			// 그리고 OnEnable..
			currentScene->OnEnable();
		}

		// 씬 변경이 예정되어 있으면.. 바꿔준다.
		if(reservedLoadScene)
		{
			ChangeScene();
		}
	}

	void SceneManager::AddGameScene(std::shared_ptr<SceneBase> _scene)
	{
		if(_scene != nullptr)
		{
			scenes.insert({_scene->GetSceneName(), _scene});
		}
	}

	void SceneManager::LoadScene(tstring _sceneName)
	{
		// 씬이 Map에 존재하는가.
		assert(scenes.find(_sceneName) != scenes.end());

		// 씬을 예약한다.
		reservedLoadScene = true;
		reservedLoadSceneName = _sceneName;
	}

	void SceneManager::UpdateMainCameraAspectRatio(uint32 _screenWidth, uint32 _screenHeight)
	{
		if(currentScene != nullptr)
		{
			std::shared_ptr<Camera> _mainCamera = currentScene->GetMainCamera();

			if(_mainCamera != nullptr)
			{
				currentScene->GetMainCamera()->SetAspectRatio(static_cast<float>(_screenWidth) / _screenHeight);
			}
		}
	}

	void SceneManager::ChangeTick(float multiple)
	{
		isChangedTick = true;

		deltaMultiple = multiple;
	}
}
