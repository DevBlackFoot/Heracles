#include "pch.h"
#include "MapE.h"

#include "GameObjectTemplate.h"
#include "PlayerCamera.h"
#include "RewardBoxMonoBehaviour.h"

#include "GameState.h"
#include "SceneController.h"
#include "Entrance.h"
#include "IngameUI.h"
#include "SoundManager.h"

#include "UIManager.h"
#include "BoxCollider.h"
#include "SettingUI.h"

namespace Heracles
{
	using namespace InGameObject;

	MapE::MapE(tstring sceneName) : SceneBase(sceneName)
	{
	}
	void MapE::BuildScene()
	{
		GameState::GetInstance()->mapCleared = false;
		GameState::GetInstance()->totalMonsterNumInMap = 6;
		GameState::GetInstance()->currMonsterNum = 0;


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

	void MapE::CreateWallCollider()
	{
		CreateWall(this, Vector(0.f, 0.f, 48.f), 100.f, 1.f);
		CreateWall(this, Vector(0.f, 0.f, -48.f), 100.f, 1.f);
		CreateWall(this, Vector(-30.f, 0.f, 0.f), 1.f, 100.f);
		CreateWall(this, Vector(48.f, 0.f, 0.f), 1.f, 100.f);

		CreateWall(this, Vector(-7.f, 0.f, -32.f), 46.f, 32.f);
		CreateWall(this, Vector(23.f, 0.f, 10.f), 46.f, 20.f);
		CreateWall(this, Vector(23.f, 0.f, 45.f), 46.f, 10.f);

	}
	void MapE::SetMonsterWave()
	{
		std::vector<std::pair<int, Vector>> first = { std::make_pair(1,Vector(40.f, 0.f, -15.f)) ,std::make_pair(1,Vector(15.f, 0.f, -15.f)) };
		std::vector<std::pair<int, Vector>> second = { std::make_pair(2,Vector(-5.f, 0.f, -15.f)) ,std::make_pair(2,Vector(15.f, 0.f, -15.f)),std::make_pair(2,Vector(-25.f, 0.f, -15.f)),std::make_pair(2,Vector(-10.f, 0.f, -20.f)) };
		std::vector<std::pair<int, Vector>> third = { std::make_pair(1,Vector(-10.f, 0.f, 30.f)) ,std::make_pair(2,Vector(-30.f, 0.f,10.f)),std::make_pair(2,Vector(-20.f, 0.f, 10.f)) };

		this->sceneController->GetComponent<SceneController>()->CreateMonsterSpawner(this, first);
		this->sceneController->GetComponent<SceneController>()->CreateMonsterSpawner(this, second);
		//this->sceneController->GetComponent<SceneController>()->CreateMonsterSpawner(this, third);
	}
	void MapE::CreateObjects()
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
		ent1->GetGameObject()->GetComponent<Transform>()->SetPosition(ent1->GetGameObject()->GetComponent<Transform>()->GetWorldPosition() + Vector(-1.f, 0.f, 1.5f), Space::WORLD);
		ent1->GetGameObject()->GetComponent<BoxCollider>()->SetLocalCenter(Vector(-5.f, 0.f, -8.f));
		ent1->SetNextScene(GameState::GetInstance()->GetNextMap1());
		ent1->SetEntranceType(false);


		auto ent2 = CreateEntranceWithIndex(this, this->ExitB, GameState::GetInstance()->rewardIdx2)->GetComponent<Entrance>();
		ent2->GetGameObject()->GetComponent<Transform>()->SetPosition(ent2->GetGameObject()->GetComponent<Transform>()->GetWorldPosition() + Vector(1.f, 0.f, -1.f), Space::WORLD);
		ent2->GetGameObject()->GetComponent<BoxCollider>()->SetLocalCenter(Vector(8.f, 0.f, -5.f));
		ent2->SetNextScene(GameState::GetInstance()->GetNextMap2());
		ent2->SetEntranceType(true);


		auto rewardBox = CreateRewardBox(this, Vector(-20.f, 0.f, 25.f), rewardIdx);
		rewardBox->GetComponent<RewardBoxMonoBehaviour>()->SetRewardObject(CreateRewardWithIndex(this, rewardIdx));
		this->sceneController->GetComponent<SceneController>()->SetRewardBox(rewardBox);

		//100x100 wall3
		std::shared_ptr<GameObject> map = CreateEmpty();

		map->AddComponent<MeshRenderer>()->Init("MapEFloor", "Resources/Model/Stage_96_96_3Wall_Floor.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("MapEFloorFrame", "Resources/Model/Stage_96_96_3Wall_FloorFrame.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("MapEBrazier", "Resources/Model/Stage_96_96_3Wall_Brazier.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("MapEPillar", "Resources/Model/Stage_96_96_3Wall_Pillar.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("MapEPot", "Resources/Model/Stage_96_96_3Wall_Pot.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("MapETapestry", "Resources/Model/Stage_96_96_3Wall_Tapestry.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("MapEWall", "Resources/Model/Stage_96_96_3Wall_Wall.ase", RendererBase::ModelType::NONE_AO);

		map->GetComponent<Transform>()->SetPosition(Vector(0.f,-1.f,0.f),Space::WORLD);

		//map->GetComponent<Transform>()->SetRotation({0.f, 180.f, 0.f});
	}
	void MapE::CreateUI()
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

	void MapE::CreatePointLights()
	{
		float r = 0xff;
		float g = 0x63;
		float b = 0x49;

		float intensity = 1.0f;

		GraphicsEngineSpace::PointLight* pLight1 = CreatePointLight();
		pLight1->position = { 45.5035f, 4.693f, -35.8519f };
		pLight1->color = { r / 255.0f, g / 255.0f, b / 255.0f };
		pLight1->intensity = intensity;

		GraphicsEngineSpace::PointLight* pLight2 = CreatePointLight();
		pLight2->position = { -3.06664f, 4.693f, 2.75134f };
		pLight2->color = { r / 255.0f, g / 255.0f, b / 255.0f };
		pLight2->intensity = intensity;

		GraphicsEngineSpace::PointLight* pLight3 = CreatePointLight();
		pLight3->position = { -29.4307f, 4.693f, -13.5599f };
		pLight3->color = { r / 255.0f, g / 255.0f, b / 255.0f };
		pLight3->intensity = intensity;

		GraphicsEngineSpace::PointLight* pLight4 = CreatePointLight();
		pLight4->position = { -29.421f, 4.693f, 44.9734f };
		pLight4->color = { r / 255.0f, g / 255.0f, b / 255.0f };
		pLight4->intensity = intensity;
	}
}