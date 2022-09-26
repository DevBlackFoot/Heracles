#include "pch.h"
#include "TestScene.h"

#include "GameObjectTemplate.h"
#include "UIManager.h"



namespace Heracles
{
	using namespace InGameObject;

	TestScene::TestScene(tstring sceneName)
		: SceneBase(sceneName)
	{
	}

	TestScene::~TestScene()
	{
	}

	void TestScene::BuildScene()
	{
		// 실례지만 맵하나 뽑겠습니다..
		//BuildMap(TEXT("Contents/DummyMap.json"), TEXT("Contents/MapObjects.json"));

		//캐릭터 만들기
		CreatePlayer(this);
		CreateCamera(this);
#ifdef _DEBUG
		UIManager::GetInstance()->CreateUIText("TestScene", "TestScene", Vector(0.5f, 0.5f, 1.f), Vector(600.f, 20.f, 0.f), 0.f, Vector(2.f, 2.f, 2.f));
		UIManager::GetInstance()->SetTextUIIsEnable("TestScene", true);

		UIManager::GetInstance()->CreateUIText("Money", "Money : ", Vector(1.f, 1.f, 0.f), Vector(100.f, 200.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f));
		UIManager::GetInstance()->CreateUIText("Armor", "Armor : ", Vector(1.f, 1.f, 0.f), Vector(100.f, 220.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f));
		UIManager::GetInstance()->CreateUIText("PlayNum", " : ", Vector(1.f, 1.f, 0.f), Vector(100.f, 240.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f));
#endif

	}



}