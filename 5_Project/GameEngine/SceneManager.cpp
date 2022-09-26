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
			// ���� Scene�� ������ �����ش�..
			currentScene->ClearScene();
		}

		currentScene = scenes.at(reservedLoadSceneName);

		reservedLoadScene = false;

		// �� �ʱ�ȭ.
		currentScene->BuildScene();
		currentScene->Awake();
		currentScene->Start();
	}

	std::shared_ptr<SceneManager> SceneManager::GetInstance()
	{
		// �ν��Ͻ��� ������ ������ְ� ������ �ν��Ͻ� ����
		if(instance == nullptr)
		{
			instance = std::make_shared<SceneManager>();
		}

		return instance;
	}

	void SceneManager::Init()
	{
		// ���Ŀ� ������ ������ ���⼭ ������ Init���ش�.
		physicsManager = PhysicsManager::GetInstance();

		physicsManager->Init();
	}

	void SceneManager::Release()
	{
		currentScene.reset();

		physicsManager->Release();

		// �� ��ü ������
		for(auto& scene : scenes)
		{
			scene.second->ClearScene();
		}
	}

	void SceneManager::Update()
	{
		// ���� ���� �ִٸ�..
		if(currentScene != nullptr)
		{
			float deltaTime = Timer::GetInstance()->DeltaTime();

			// ��Ÿ Ÿ�� �׽�Ʈ�� �ڵ�
				// ƽ�� �������� -> �Լ��� ���� ����� ���� ����Ѵ�.
			if(isChangedTick == true)
			{
				deltaTime *= deltaMultiple;
				// ���� üũ�� ���ش�.
				if(deltaTime < 0.0f)
					deltaTime = 0.0f;
			}

			InputManager::GetInstance()->PadUpdate(deltaTime);

			// �� ������Ʈ �� �浹 ������Ʈ..
			physicsManager->Update(deltaTime, currentScene->colliderObjInScene);

			// ������� ������Ʈ ���ش�.
			currentScene->PreUpdate(deltaTime);
			currentScene->Update(deltaTime);
			currentScene->LateUpdate(deltaTime);

			// ���� ���� => ������ �̵�
			currentScene->Render(deltaTime);

			if(InputManager::GetInstance()->GetInputState(VK_F1, KeyState::DOWN))
			{
				isDebugString = !isDebugString;
				UIManager::GetInstance()->GetCanvas("DebugCanvas")->SetEnable(isDebugString);
			}

			// �׸��� OnEnable..
			currentScene->OnEnable();
		}

		// �� ������ �����Ǿ� ������.. �ٲ��ش�.
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
		// ���� Map�� �����ϴ°�.
		assert(scenes.find(_sceneName) != scenes.end());

		// ���� �����Ѵ�.
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
