#include "pch.h"
#include "FirstScene.h"

#include "PlayerCamera.h"
#include "GameObjectTemplate.h"
#include "UIManager.h"
#include "GameState.h"

namespace Heracles
{
	using namespace InGameObject;

	FirstScene::FirstScene(tstring sceneName) : SceneBase(sceneName)
	{
	}
	void FirstScene::BuildScene()
	{
		SetSkyBox(TEXT("SkyBox"));
		SetIBL(TEXT("MSIBL"), TEXT("Resources/Texture/SunSubMixer_specularIBL.dds"), TEXT("Resources/Texture/SunSubMixer_diffuseIBL.dds"));

		std::shared_ptr<GameObject> player = CreatePlayer(this);
		player->GetComponent<Transform>()->SetPosition(Vector(50.f,0.f,0.f), Space::WORLD);

		auto camera = CreateCamera(this);
		camera->GetComponent<Player::PlayerCamera>()->SetTarget(player->GetComponent<Transform>());

		this->CreateWallCollider();
		this->SpawnMonster();
		this->CreateObjects();
	}

	void FirstScene::CreateWallCollider()
	{
		CreateWall(this, Vector(0.f, 0.f, 40.f), 120.f, 1.f);
		CreateWall(this, Vector(0.f, 0.f, -40.f), 120.f, 1.f);
		CreateWall(this, Vector(60.f, 0.f, 0.f), 1.f, 80.f);
		CreateWall(this, Vector(-60.f, 0.f, 0.f), 1.f, 80.f);

		CreateWall(this, Vector(40.f, 0.f, -30.f), 40.f, 20.f);
		CreateWall(this, Vector(40.f, 0.f, -30.f), 40.f, 20.f);
		CreateWall(this, Vector(-40.f, 0.f, 0.f), 40.f, 20.f);
		CreateWall(this, Vector(-40.f, 0.f, 0.f), 40.f, 20.f);
		
		//CreateWall(this, Vector(0.f, 0.f, 0.f), 20.f, 20.f);
	}

	void FirstScene::SpawnMonster()
	{
		CreateMonster(this, Vector(0.f, 0.f, -20.f));
		CreateMonster(this, Vector(20.f, 0.f, 0.f));
		CreateMonster(this, Vector(-20.f, 0.f, 0.f));
		CreateMonster(this, Vector(-20.f, 0.f, -20.f));
	}

	void FirstScene::CreateObjects()
	{
		//CreateReward(this, Vector(40.f, 0.f, 0.f));
		std::shared_ptr<GameObject> map = CreateEmpty();
		map->AddComponent<MeshRenderer>()->Init("MapFloor", "Resources/Model/Stage_40_80_2Hole_ver1.3.ase", RendererBase::ModelType::NONE_AO);
		map->GetComponent<Transform>()->SetRotation({ 0.f, 180.f, 0.f });
		map->GetComponent<Transform>()->SetPosition(Vector(0.f, -5.f, 0.f));
		CreateSceneController(this);
	}
}