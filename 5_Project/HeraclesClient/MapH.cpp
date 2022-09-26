#include "pch.h"
#include "MapH.h"

#include "GameObjectTemplate.h"
#include "PlayerCamera.h"
#include "RewardBoxMonoBehaviour.h"

#include "GameState.h"
#include "SceneController.h"
#include "Entrance.h"
#include "IngameUI.h"
#include "SoundManager.h"

namespace Heracles
{
	using namespace InGameObject;

	MapH::MapH(tstring sceneName) : SceneBase(sceneName)
	{
	}

	void MapH::BuildScene()
	{
		GameState::GetInstance()->totalMonsterNumInMap = 11;
		GameState::GetInstance()->mapCleared = false;
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
		SoundManager::GetInstance()->PlayBGM("MonsterMap");

	}

	void MapH::CreateWallCollider()
	{
		CreateWall(this, Vector(0.f, 0.f, 40.f), 120.f, 1.f);
		CreateWall(this, Vector(0.f, 0.f, -40.f), 120.f, 1.f);
		CreateWall(this, Vector(60.f, 0.f, 0.f), 1.f, 80.f);
		CreateWall(this, Vector(-60.f, 0.f, 0.f), 1.f, 80.f);

		CreateWall(this, Vector(0.f, 0.f, -10.f), 20.f, 10.f);
		CreateWall(this, Vector(0.f, 0.f, 0.f), 60.f, 10.f);
		CreateWall(this, Vector(0.f, 0.f, 22.5f), 80.f, 35.f);
	}

	void MapH::SetMonsterWave()
	{
		std::vector<std::pair<int, Vector>> first = { std::make_pair(1,Vector(-50.f, 0.f, 20.f)) ,std::make_pair(1,Vector(-50.f, 0.f, 0.f)),std::make_pair(1,Vector(-50.f, 0.f, -20.f)) };
		std::vector<std::pair<int, Vector>> second = { std::make_pair(1,Vector(-15.f, 0.f, -10.f)) ,std::make_pair(1,Vector(-15.f, 0.f, -30.f)),std::make_pair(1,Vector(15.f, 0.f, -10.f)),std::make_pair(1,Vector(15.f, 0.f, -30.f)) };
		std::vector<std::pair<int, Vector>> third = { std::make_pair(1,Vector(20.f, 0.f, -30.f)) ,std::make_pair(1,Vector(40.f, 0.f,-30.f)),std::make_pair(1,Vector(50.f, 0.f, -20.f)),std::make_pair(1,Vector(50.f, 0.f, 0.f)) };

		this->sceneController->GetComponent<SceneController>()->CreateMonsterSpawner(this, first);
		this->sceneController->GetComponent<SceneController>()->CreateMonsterSpawner(this, second);
		this->sceneController->GetComponent<SceneController>()->CreateMonsterSpawner(this, third);
	
	}

	void MapH::CreateObjects()
	{
		auto ent1 = CreateEntrance(this, TEXT("Entrance"), this->ExitA)->GetComponent<Entrance>();
		ent1->SetNextScene(GameState::GetInstance()->GetNextMap1());
		ent1->SetEntranceType(false);


		auto ent2 = CreateEntrance(this, TEXT("Entrance"), this->ExitB)->GetComponent<Entrance>();
		ent2->GetGameObject()->GetComponent<Transform>()->SetRotation(Vector(0.f, 90.f, 0.f), Space::LOCAL);
		ent2->SetNextScene(GameState::GetInstance()->GetNextMap2());
		ent2->SetEntranceType(true);


		auto rewardBox = CreateRewardBox(this, Vector(20.f, 0.f, 0.f), rewardIdx);
		rewardBox->GetComponent<RewardBoxMonoBehaviour>()->SetRewardObject(CreateRewardWithIndex(this, rewardIdx));
		this->sceneController->GetComponent<SceneController>()->SetRewardBox(rewardBox);

		//80x120 hole4 ¾È¾¸


	}

	void MapH::CreateUI()
	{
		std::shared_ptr<GameObject> inGameUI = CreateEmpty();
		inGameUI->AddComponent<UI::InGameUI>()->SetMaxHP(100.0f);
		inGameUI->GetComponent<UI::InGameUI>()->SetHP(100.0f);

		inGameUI->GetComponent<UI::InGameUI>()->SetCurse("Pain");
		inGameUI->GetComponent<UI::InGameUI>()->SetCurse("Blind");
		inGameUI->GetComponent<UI::InGameUI>()->SetCurse("Madness");

		inGameUI->GetComponent<UI::InGameUI>()->SetEquipment("MasterSword");
		inGameUI->GetComponent<UI::InGameUI>()->SetSwitching("AtenaSword");
		inGameUI->GetComponent<UI::InGameUI>()->Swap();
		inGameUI->GetComponent<UI::InGameUI>()->SetMemories(0);
	}

}