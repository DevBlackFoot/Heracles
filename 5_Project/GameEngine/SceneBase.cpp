#include "pch.h"
#include "SceneBase.h"

#include "GameObject.h"
#include "RendererBase.h"
#include "GraphicsManager.h"

#include "Timer.h"
#include "UIManager.h"

#include "ColliderBase.h"

#include "Camera.h"
#include "InputManager.h"



namespace GameEngineSpace
{
#ifdef _DEBUG
	bool SceneBase::debugRender = true;
#else
	bool SceneBase::debugRender = false;
#endif

	SceneBase::SceneBase(tstring sceneName) :
		sceneName(sceneName)
		, mainCamera(nullptr)
		, gameObjectInScene(std::vector<std::shared_ptr<GameObject>>())
		, renderObjInScene(std::vector<std::shared_ptr<RendererBase>>())
		, colliderObjInScene(std::vector<std::shared_ptr<ColliderBase>>())
		, skyBox(nullptr)
		, ibl(nullptr)
	{

	}

	SceneBase::~SceneBase()
	{
	}

	void SceneBase::Awake()
	{
		// ���� ���۽�..
		for (auto& gameObj : gameObjectInScene)
		{
			gameObj->Awake();
		}
	}

	void SceneBase::Start()
	{
		for (auto& gameObj : gameObjectInScene)
		{
			gameObj->Start();
		}
	}

	void SceneBase::PreUpdate(float tick)
	{
		for (auto& gameObj : gameObjectInScene)
		{
			gameObj->PreUpdate(tick);
		}
	}

	void SceneBase::Update(float tick)
	{
		for (auto& gameObj : gameObjectInScene)
		{
			gameObj->Update(tick);
		}
	}

	void SceneBase::LateUpdate(float tick)
	{
		for (auto& gameObj : gameObjectInScene)
		{
			gameObj->LateUpdate(tick);
		}
	}

	void SceneBase::Render(float tick)
	{
		// �׷��Ƚ� ������ ��� ����
		GraphicsManager::GetInstance()->GetRenderer()->BeginRender();

		std::shared_ptr<GraphicsEngine> graphicsEngine = GraphicsManager::GetInstance()->GetRenderer();

		if (skyBox != nullptr)
		{
			// SkyBox!
			graphicsEngine->GraphicsDebugBeginEvent("SkyBox");
			Matrix viewMatrix = mainCamera->GetViewMatrix();
			viewMatrix.r[3] = { 0.f, 0.f, 0.f, 1.0f };
			const Matrix& projMatrix = mainCamera->GetProjMatrix();
			Matrix viewProj = MatrixTranspose(viewMatrix * projMatrix);
			skyBox->Render(graphicsEngine.get(), viewProj);
			graphicsEngine->GraphicsDebugEndEvent();
		}

		if (ibl != nullptr)
		{
			// IBL Set
			ibl->SetUpIBL(4, 5, 6, ShaderType::PIXEL);
		}


		// �� ������Ʈ���� ����.
		for (auto& renderObj : renderObjInScene)
		{
			// TODO : �ٿ�� ���� ����� �ش� �������� �������� �ø��� ���ݴϴ�.

			if (renderObj->GetIsEnable())
			{
				renderObj->PrepareRender(tick);
				renderObj->Render(GraphicsManager::GetInstance()->GetRenderer());
			}
		}

		for (auto* iter : pointLights)
		{
			iter->SetUpPointLight();
		}

		// �׷��Ƚ� ���������� ����
		GraphicsManager::GetInstance()->GetRenderer()->Render();

		GraphicsManager::GetInstance()->GetRenderer()->PostProcess();

		// UI Render
		UIManager::GetInstance()->Render();

		if (InputManager::GetInstance()->GetInputState(VK_F2, KeyState::DOWN))
			debugRender = !debugRender;

		if (debugRender == true)
		{

			// �ݶ��̴� ����.
			for (auto& colObj : colliderObjInScene)
			{
				if (colObj->GetIsEnable())
				{
					colObj->ColliderRender();
				}
			}

			// Debug ��� �ӽ÷� FPS ����
			static float frameTime = 0.0f;
			static int fps = 0;
			static int frameCount = 0;

			float deltaTime = Timer::GetInstance()->DeltaTime();
			frameTime += deltaTime;
			frameCount++;

			if (frameTime >= 1.0f)
			{
				fps = frameCount;
				frameCount = 0;
				frameTime = 0.0f;
			}

			GraphicsManager::GetInstance()->GetRenderer()->DebugRender(fps, deltaTime, debugRender);
		}

		GraphicsManager::GetInstance()->GetRenderer()->EndRender();
	}

	void SceneBase::OnEnable()
	{
		for (auto& gameObj : gameObjectInScene)
		{
			gameObj->OnEnable();
		}
	}

	void SceneBase::SetSkyBox(const tstring& skyBoxName)
	{
		using namespace GraphicsEngineSpace;

		// ���丮�� ���ҽ� �Ŵ����� �޾ƿɽô�.
		Factory* factory = GraphicsManager::GetInstance()->GetRenderer()->CreateFactory();
		ResourceManager* resourceManager = GraphicsManager::GetInstance()->GetRenderer()->GetResourceManager();

		std::string strName;
		strName.assign(skyBoxName.begin(), skyBoxName.end());

		// ��ī�� �ڽ��� ������ݴϴ�.
		skyBox = resourceManager->GetSkyBox(strName);

		if (skyBox == nullptr)
			skyBox = factory->CreateSkyBox(strName);
	}

	void SceneBase::SetIBL(const tstring& iblName, const tstring& radiancePath, const tstring& irradiancePath)
	{
		// ��ī�̹ڽ��� ������ �����մϴ�.
		if (skyBox == nullptr)
			return;

		using namespace GraphicsEngineSpace;

		// ���丮�� ���ҽ� �Ŵ����� �޾ƿɽô�.
		Factory* factory = GraphicsManager::GetInstance()->GetRenderer()->CreateFactory();
		ResourceManager* resourceManager = GraphicsManager::GetInstance()->GetRenderer()->GetResourceManager();

		std::string strName;
		strName.assign(iblName.begin(), iblName.end());

		std::string radianceStr;
		radianceStr.assign(radiancePath.begin(), radiancePath.end());

		std::string irradianceStr;
		irradianceStr.assign(irradiancePath.begin(), irradiancePath.end());

		// �ϴ� ���� ������ ����� �ݴϴ�.
		ibl = resourceManager->GetIBLTexture(strName + "_radiance");

		if (ibl == nullptr)
			ibl = factory->CreateIBLTexture(strName, radianceStr, irradianceStr);

		// ��ī�̹ڽ��� ibl Texture�� �������ݴϴ�.
		skyBox->SetTexture(resourceManager->GetTexture(strName + "_radiance"));
	}

	GraphicsEngineSpace::PointLight* SceneBase::CreatePointLight()
	{
		Factory* factory = GraphicsManager::GetInstance()->GetRenderer()->CreateFactory();
		ResourceManager* resourceManager = GraphicsManager::GetInstance()->GetRenderer()->GetResourceManager();

		GraphicsEngineSpace::PointLight* newPointLight = factory->CreatePointLight("PointLight");

		if (newPointLight == nullptr)
			return nullptr;

		pointLights.push_back(newPointLight);

		return newPointLight;
	}

	void SceneBase::ClearScene()
	{
		// ���������� �����ش�.
		// ������� �ϳ��� ���� �� ���������� ������
		for (auto& gameObj : gameObjectInScene)
		{
			gameObj->Release();
		}

		gameObjectInScene.clear();
		renderObjInScene.clear();
		colliderObjInScene.clear();
		pointLights.clear();

		// UI �����ֱ�
		UIManager::GetInstance()->ClearUI();
	}

	void SceneBase::AddGameObject(std::shared_ptr<GameObject> newGameObj)
	{
		gameObjectInScene.push_back(newGameObj);

		// �ڽĵ��� ������ �ڽĵ鵵 �־��ش�.
		const std::vector<std::shared_ptr<GameObject>> children = newGameObj->GetChildren();

		for (auto child : children)
		{
			this->AddGameObject(child);
		}
	}

	std::shared_ptr<GameObject> SceneBase::CreateEmpty()
	{
		// ����� ������Ʈ�� �����.. Ʈ�������� ���� �� �־��ش�.
		std::shared_ptr<GameObject> newGameObj = std::make_shared<GameObject>();

		newGameObj->AddComponent<Transform>();

		AddGameObject(newGameObj);

		// ��ȯ.
		return newGameObj;
	}

	void SceneBase::AddRenderer(std::shared_ptr<RendererBase> renderObj)
	{
		renderObjInScene.push_back(renderObj);
	}

	void SceneBase::AddCollider(std::shared_ptr<ColliderBase> colliderObj)
	{
		colliderObjInScene.push_back(colliderObj);
	}

}
