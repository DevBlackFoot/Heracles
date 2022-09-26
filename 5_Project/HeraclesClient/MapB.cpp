#include "pch.h"
#include "MapB.h"

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


	MapB::MapB(tstring sceneName) : SceneBase(sceneName)
	{
	}
	void MapB::BuildScene()
	{
		GameState::GetInstance()->mapCleared = false;
		GameState::GetInstance()->currMonsterNum = 0;
		GameState::GetInstance()->totalMonsterNumInMap = 7;


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

	void MapB::CreateWallCollider()
	{
		CreateWall(this, Vector(0.f, 0.f, 40.f), 80.f, 1.f);
		CreateWall(this, Vector(0.f, 0.f, -40.f), 80.f, 1.f);
		CreateWall(this, Vector(-40.f, 0.f, 0.f), 1.f, 80.f);
		CreateWall(this, Vector(40.f, 0.f, 0.f), 1.f, 80.f);
		CreateWall(this, Vector(0.f, 0.f, -30.f), 42.f, 18.f);
		CreateWall(this, Vector(-30.f, 0.f, 30.f), 20.f, 20.f);
		CreateWall(this, Vector(30.f, 0.f, 30.f), 20.f, 20.f);

	}
	void MapB::SetMonsterWave()
	{
		std::vector<std::pair<int, Vector>> first = { std::make_pair(1,Vector(-15.f, 0.f, -15.f)) ,std::make_pair(1,Vector(-15.f, 0.f, 15.f)),std::make_pair(1,Vector(-25.f, 0.f, 0.f)) };
		std::vector<std::pair<int, Vector>> second = { std::make_pair(2, Vector(10.f, 0.f, -10.f)) ,std::make_pair(2,Vector(10.f, 0.f, 10.f)),std::make_pair(1,Vector(-10.f, 0.f, -10.f)),std::make_pair(1,Vector(-10.f, 0.f, -10.f)) };
		std::vector<std::pair<int, Vector>> third = { std::make_pair(1,Vector(30.f, 0.f, -15.f)) ,std::make_pair(2,Vector(30.f, 0.f, 15.f)),std::make_pair(2,Vector(20.f, 0.f, 0.f)) };

		this->sceneController->GetComponent<SceneController>()->CreateMonsterSpawner(this, first);
		this->sceneController->GetComponent<SceneController>()->CreateMonsterSpawner(this, second);
		//this->sceneController->GetComponent<SceneController>()->CreateMonsterSpawner(this, third);
	}
	void MapB::CreateObjects()
	{
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
		ent1->GetGameObject()->GetComponent<Transform>()->SetPosition(ent1->GetGameObject()->GetComponent<Transform>()->GetWorldPosition() + Vector(-2.f, 0.f, 6.f), Space::WORLD);
		ent1->GetGameObject()->GetComponent<BoxCollider>()->SetLocalCenter(Vector(-5.f, 0.f, -8.f));
		ent1->SetNextScene(GameState::GetInstance()->GetNextMap1());
		ent1->SetEntranceType(false);

		auto ent2 = CreateEntranceWithIndex(this, this->ExitB, GameState::GetInstance()->rewardIdx2)->GetComponent<Entrance>();
		ent2->GetGameObject()->GetComponent<Transform>()->SetPosition(ent2->GetGameObject()->GetComponent<Transform>()->GetWorldPosition() + Vector(-12.f, 0.f, 0.f), Space::WORLD);
		ent2->GetGameObject()->GetComponent<BoxCollider>()->SetLocalCenter(Vector(8.f, 0.f, -5.f));
		ent2->SetNextScene(GameState::GetInstance()->GetNextMap2());
		ent2->SetEntranceType(true);


		auto rewardBox = CreateRewardBox(this, Vector(20.f, 0.f, 0.f), rewardIdx);
		rewardBox->GetComponent<RewardBoxMonoBehaviour>()->SetRewardObject(CreateRewardWithIndex(this, rewardIdx));
		this->sceneController->GetComponent<SceneController>()->SetRewardBox(rewardBox);

		//80x80 wall2 / 1hole
		std::shared_ptr<GameObject> map = CreateEmpty();

		map->AddComponent<MeshRenderer>()->Init("MapBFloor", "Resources/Model/Stage_80_80_2Wall_1Hole_Floor.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("MapBFloorFrame", "Resources/Model/Stage_80_80_2Wall_1Hole_FloorFrame.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("MapBBrazier", "Resources/Model/Stage_80_80_2Wall_1Hole_Brazier.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("MapBHeraStatue", "Resources/Model/Stage_80_80_2Wall_1Hole_HeraStatue.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("MapBPillar", "Resources/Model/Stage_80_80_2Wall_1Hole_Pillar.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("MapBPot", "Resources/Model/Stage_80_80_2Wall_1Hole_Pot.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("MapBTapestry", "Resources/Model/Stage_80_80_2Wall_1Hole_Tapestry.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("MapBWall", "Resources/Model/Stage_80_80_2Wall_1Hole_Wall.ase", RendererBase::ModelType::NONE_AO);
	}
	void MapB::CreateUI()
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

	void MapB::CreatePointLights()
	{
		float r = 0xff;
		float g = 0x63;
		float b = 0x49;

		float intensity = 1.0f;

		GraphicsEngineSpace::PointLight* pLight1 = CreatePointLight();
		pLight1->position = { 38.0594f, 4.693f, 7.06675f };
		pLight1->color = { r / 255.0f, g / 255.0f, b / 255.0f };
		pLight1->intensity = intensity;

		GraphicsEngineSpace::PointLight* pLight2 = CreatePointLight();
		pLight2->position = { 28.4408f, 4.693f, -37.0f };
		pLight2->color = { r / 255.0f, g / 255.0f, b / 255.0f };
		pLight2->intensity = intensity;

		GraphicsEngineSpace::PointLight* pLight3 = CreatePointLight();
		pLight3->position = { -6.84617f, 4.693f, 37.1082f };
		pLight3->color = { r / 255.0f, g / 255.0f, b / 255.0f };
		pLight3->intensity = intensity;

		GraphicsEngineSpace::PointLight* pLight4 = CreatePointLight();
		pLight4->position = { -14.8041f, 4.693f, -16.9457f };
		pLight4->color = { r / 255.0f, g / 255.0f, b / 255.0f };
		pLight4->intensity = intensity;

		GraphicsEngineSpace::PointLight* pLight5 = CreatePointLight();
		pLight5->position = { -35.031f, 4.693f, 16.9762f };
		pLight5->color = { r / 255.0f, g / 255.0f, b / 255.0f };
		pLight5->intensity = intensity;
	}
}