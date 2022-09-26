#include "pch.h"
#include "StartScene.h"

#include "GameObjectTemplate.h"
#include "PlayerCamera.h"
#include "RewardBoxMonoBehaviour.h"

#include "GameState.h"
#include "SceneController.h"
#include "Entrance.h"
#include "IngameUI.h"
#include "SoundManager.h"
#include "BoxCollider.h"

#include "UIManager.h"
#include "SettingUI.h"

namespace Heracles
{
	using namespace InGameObject;

	StartScene::StartScene(tstring sceneName) : SceneBase(sceneName)
	{
	}
	void StartScene::BuildScene()
	{
		GameState::GetInstance()->totalMonsterNumInMap = 6;
		GameState::GetInstance()->currMonsterNum = 0;
		GameState::GetInstance()->mapCleared = false;


		SetSkyBox(TEXT("SkyBox"));
		SetIBL(TEXT("MSIBL"), TEXT("Resources/Texture/SunSubMixer_specularIBL.dds"), TEXT("Resources/Texture/SunSubMixer_diffuseIBL.dds"));

		std::shared_ptr<GameObject> player = CreatePlayer(this);
		if (GameState::GetInstance()->exitType)
		{
			player->GetComponent<Transform>()->SetPosition(this->SpawnA, Space::WORLD);
			this->rewardIdx = GameState::GetInstance()->rewardIdx2;
		}
		else
		{
			player->GetComponent<Transform>()->SetPosition(this->SpawnB, Space::WORLD);
			this->rewardIdx = GameState::GetInstance()->rewardIdx1;
		}

		auto camera = CreateCamera(this);
		camera->GetComponent<Player::PlayerCamera>()->SetTarget(player->GetComponent<Transform>());

		this->sceneController = CreateSceneController(this);
		this->sceneController->GetComponent<SceneController>()->SelectNextMap();

		this->CreateWallCollider();
		this->SetMonsterWave();
		this->CreateObjects();
		this->CreateUI();
		this->CreatePointLights();

		SoundManager::GetInstance()->PlayBGM("MonsterMap");
	}
	void StartScene::CreateWallCollider()
	{
		CreateWall(this, Vector(0.f, 0.f, 20.f), 80.f, 1.f);
		CreateWall(this, Vector(0.f, 0.f, -20.f), 80.f, 1.f);
		CreateWall(this, Vector(-40.f, 0.f, 0.f), 1.f, 40.f);
		CreateWall(this, Vector(40.f, 0.f, 0.f), 1.f, 40.f);
		CreateWall(this, Vector(20.f, 0.f, 0.f), 8.f, 16.f);
		CreateWall(this, Vector(-20.f, 0.f, 0.f), 8.f, 16.f);
	}
	void StartScene::SetMonsterWave()
	{
		std::vector<std::pair<int,Vector>> first = { std::make_pair(1,Vector(-30.f, 0.f, -0.f)) ,std::make_pair(1,Vector(-30.f, 0.f, 0.f)) };
		std::vector<std::pair<int, Vector>> second = { std::make_pair(1,Vector(0.f, 0.f, -8.f)) ,std::make_pair(2,Vector(0.f, 0.f, 8.f)) };
		std::vector<std::pair<int, Vector>> third = { std::make_pair(2,Vector(30.f, 0.f, 8.f)) ,std::make_pair(2,Vector(30.f, 0.f, -8.f)) };

		this->sceneController->GetComponent<SceneController>()->CreateMonsterSpawner(this,first);
		this->sceneController->GetComponent<SceneController>()->CreateMonsterSpawner(this,second);
		this->sceneController->GetComponent<SceneController>()->CreateMonsterSpawner(this,third);
	};

	void StartScene::CreateObjects()
	{
		std::shared_ptr<GameObject> map = CreateEmpty();
		map->AddComponent<MeshRenderer>()->Init("StartMapFloor", "Resources/Model/Stage_40_80_2Hole_Floor.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("StartMapBrazier", "Resources/Model/Stage_40_80_2Hole_Brazier.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("StartMapFloorFrame", "Resources/Model/Stage_40_80_2Hole_FloorFrame.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("StartMapPillar", "Resources/Model/Stage_40_80_2Hole_Pillar.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("StartMapPot", "Resources/Model/Stage_40_80_2Hole_Pot.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("StartMapTapestry", "Resources/Model/Stage_40_80_2Hole_Tapestry.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("StartMapWall", "Resources/Model/Stage_40_80_2Hole_Wall.ase", RendererBase::ModelType::NONE_AO);

		std::shared_ptr<Entrance> ent1 = nullptr;
		if (GameState::GetInstance()->clearedMapNum == 2)
		{
			ent1 = CreateEntranceWithIndex(this, this->ExitA, 3)->GetComponent<Entrance>();
		}
		else
		{
			ent1 = CreateEntranceWithIndex(this, this->ExitA, GameState::GetInstance()->rewardIdx1)->GetComponent<Entrance>();
		}
		ent1->GetGameObject()->GetComponent<Transform>()->SetRotation(Vector(0.f, 90.f, 0.f), Space::LOCAL);
		ent1->GetGameObject()->GetComponent<Transform>()->SetPosition(ent1->GetGameObject()->GetComponent<Transform>()->GetWorldPosition()+Vector(0.f,0.f,3.f),Space::WORLD);
		ent1->GetGameObject()->GetComponent<BoxCollider>()->SetLocalCenter(Vector(-5.f, 0.f, -8.f));

		ent1->SetNextScene(GameState::GetInstance()->GetNextMap1());
		ent1->SetEntranceType(false);

		auto ent2 = CreateEntranceWithIndex(this, this->ExitB, GameState::GetInstance()->rewardIdx2)->GetComponent<Entrance>();		
		ent2->GetGameObject()->GetComponent<Transform>()->SetPosition(ent1->GetGameObject()->GetComponent<Transform>()->GetWorldPosition() + Vector(-22.f, 0.f, 15.f), Space::WORLD);
		ent2->GetGameObject()->GetComponent<BoxCollider>()->SetLocalCenter(Vector(8.f, 0.f, -5.f));
		ent2->SetNextScene(GameState::GetInstance()->GetNextMap2());
		ent2->SetEntranceType(true);


		auto rewardBox = CreateRewardBox(this, Vector(0.f, 0.f, 0.f), rewardIdx);
		rewardBox->GetComponent<RewardBoxMonoBehaviour>()->SetRewardObject(CreateRewardWithIndex(this, rewardIdx));
		this->sceneController->GetComponent<SceneController>()->SetRewardBox(rewardBox);

		//40x80
	}
	void StartScene::CreateUI()
	{
		std::shared_ptr<GameObject> ui = CreateEmpty();
		ui->AddComponent<UI::InGameUI>()->SetMaxHP(100.0f);
		ui->GetComponent<UI::InGameUI>()->SetHP(100.0f);

		ui->GetComponent<UI::InGameUI>()->SetCurse("Pain");
		ui->GetComponent<UI::InGameUI>()->SetCurse("Blind");
		ui->GetComponent<UI::InGameUI>()->SetCurse("Madness");

		ui->GetComponent<UI::InGameUI>()->SetEquipment("MasterSword");
		ui->GetComponent<UI::InGameUI>()->SetSwitching("AtenaSword");
		ui->GetComponent<UI::InGameUI>()->Swap();
		ui->GetComponent<UI::InGameUI>()->SetMemories(0);

		ui->AddComponent<UI::SettingUI>()->SetBackButtonEnable(true);
	}

	void StartScene::CreatePointLights()
	{
		float r = 0xff;
		float g = 0x63;
		float b = 0x49;

		float intensity = 1.0f;

		GraphicsEngineSpace::PointLight* pLight1 = CreatePointLight();
		pLight1->position = { 36.9254f, 4.693f, -17.6641f };
		pLight1->color = { r / 255.0f, g / 255.0f, b / 255.0f };
		pLight1->intensity = intensity;

		GraphicsEngineSpace::PointLight* pLight2 = CreatePointLight();
		pLight2->position = { 3.09286f, 5.0f, -17.6286f };
		pLight2->color = { r / 255.0f, g / 255.0f, b / 255.0f };
		pLight2->intensity = intensity;

		GraphicsEngineSpace::PointLight* pLight3 = CreatePointLight();
		pLight3->position = { -34.5522f, 5.0f, 17.112f };
		pLight3->color = { r / 255.0f, g / 255.0f, b / 255.0f };
		pLight3->intensity = intensity;
	}
}