#include "pch.h"
#include "BossMap.h"

#include "GameObjectTemplate.h"
#include "PlayerCamera.h"

#include "PlayerStateMachine.h"
#include "GameState.h"
#include "SceneController.h"
#include "Entrance.h"
#include "IngameUI.h"
#include "SoundManager.h"
#include "GraphicsManager.h"

#include "UIManager.h"
#include "SettingUI.h"

namespace Heracles
{
	using namespace InGameObject;

	BossMap::BossMap(tstring sceneName) :SceneBase(sceneName)
	{
	}

	void BossMap::BuildScene()
	{
		GameState::GetInstance()->mapCleared = false;
		GameState::GetInstance()->currMonsterNum = 0;

		SetSkyBox(TEXT("SkyBox"));

		SetIBL(TEXT("MSIBL"), TEXT("Resources/Texture/SunSubMixer_specularIBL.dds"), TEXT("Resources/Texture/SunSubMixer_diffuseIBL.dds"));

		std::shared_ptr<GameObject> player = CreatePlayer(this);

		player->GetComponent<Player::PlayerStateMachine>()->invincableTime = 0.1f;

		if (GameState::GetInstance()->exitType)
		{
			player->GetComponent<GameEngineSpace::Transform>()->SetPosition(this->SpawnA, Space::WORLD);
		}
		else
		{
			player->GetComponent<GameEngineSpace::Transform>()->SetPosition(this->SpawnB, Space::WORLD);
		}

		auto camera = CreateCamera(this);
		camera->GetComponent<Player::PlayerCamera>()->SetTarget(player->GetComponent<GameEngineSpace::Transform>());

		this->sceneController = CreateSceneController(this);
		this->sceneController->GetComponent<SceneController>()->SelectNextMap();

		this->CreateWallCollider();
		this->SetMonsterWave();
		this->CreateObjects();
		this->CreateMap();
		this->CreateUI();
		this->CreatePointLights();
		SoundManager::GetInstance()->PlayBGM("BossBattle");

		GraphicsManager::GetInstance()->GetDirectionalLight()->intensity = 1.5f;
	}

	void BossMap::CreateWallCollider()
	{
		CreateWall(this, Vector(-56.f, 0.f, 21.f), 80.f, 1.f);
		CreateWall(this, Vector(-56.f, 0.f, -21.f), 80.f, 1.f);
		CreateWall(this, Vector(-85.f, 0.f, 0.f), 1.f, 40.f);
		CreateWall(this, Vector(-4.f, 0.f, 33.f), 40.f, 1.f)->GetComponent<GameEngineSpace::Transform>()->SetRotation(Vector(0.f, -45.f, 0.f), Space::LOCAL);
		CreateWall(this, Vector(-4.f, 0.f, -35.f), 40.f, 1.f)->GetComponent<GameEngineSpace::Transform>()->SetRotation(Vector(0.f, 45.f, 0.f), Space::LOCAL);
		CreateWall(this, Vector(32.f, 0.f, 48.f),40.f, 1.f);
		CreateWall(this, Vector(32.f, 0.f, -50.f), 50.f, 1.f);
		CreateWall(this, Vector(66.f, 0.f, 34.f), 40.f, 1.f)->GetComponent<GameEngineSpace::Transform>()->SetRotation(Vector(0.f, 45.f, 0.f), Space::LOCAL);
		CreateWall(this, Vector(66.f, 0.f, -34.f), 40.f, 1.f)->GetComponent<GameEngineSpace::Transform>()->SetRotation(Vector(0.f, -45.f, 0.f), Space::LOCAL);
		CreateWall(this, Vector(80.f, 0.f, 0.f), 1.f, 40.f);
	}
	void BossMap::CreateUI()
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
		ui->GetComponent<UI::InGameUI>()->SetBossHPBarEnable(true);

		ui->AddComponent<UI::SettingUI>()->SetBackButtonEnable(true);
	}

	void BossMap::CreatePointLights()
	{
		float r = 0xff;
		float g = 0x63;
		float b = 0x49;

		float intensity = 1.0f;

		GraphicsEngineSpace::PointLight* pLight1 = CreatePointLight();
		pLight1->position = { 53.95f, 4.693f, 41.702f };
		pLight1->color = { r / 255.0f, g / 255.0f, b / 255.0f };
		pLight1->intensity = intensity;

		GraphicsEngineSpace::PointLight* pLight2 = CreatePointLight();
		pLight2->position = { 73.0189f, 4.693f, -22.7418f };
		pLight2->color = { r / 255.0f, g / 255.0f, b / 255.0f };
		pLight2->intensity = intensity;

		GraphicsEngineSpace::PointLight* pLight3 = CreatePointLight();
		pLight3->position = { 13.0791f, 4.693f, -45.2782f };
		pLight3->color = { r / 255.0f, g / 255.0f, b / 255.0f };
		pLight3->intensity = intensity;

		GraphicsEngineSpace::PointLight* pLight4 = CreatePointLight();
		pLight4->position = { -23.0f, 4.693f, 17.0f };
		pLight4->color = { r / 255.0f, g / 255.0f, b / 255.0f };
		pLight4->intensity = intensity;

		GraphicsEngineSpace::PointLight* pLight5 = CreatePointLight();
		pLight5->position = { -23.0f, 4.693f, -17.0f };
		pLight5->color = { r / 255.0f, g / 255.0f, b / 255.0f };
		pLight5->intensity = intensity;

		GraphicsEngineSpace::PointLight* pLight6 = CreatePointLight();
		pLight6->position = { -49.0f, 4.693f, 17.0f };
		pLight6->color = { r / 255.0f, g / 255.0f, b / 255.0f };
		pLight6->intensity = intensity;

		GraphicsEngineSpace::PointLight* pLight7 = CreatePointLight();
		pLight7->position = { -49.0f, 4.693f, -17.0f };
		pLight7->color = { r / 255.0f, g / 255.0f, b / 255.0f };
		pLight7->intensity = intensity;

		GraphicsEngineSpace::PointLight* pLight8 = CreatePointLight();
		pLight8->position = { -80.0f, 4.693f, 17.0f };
		pLight8->color = { r / 255.0f, g / 255.0f, b / 255.0f };
		pLight8->intensity = intensity;

		GraphicsEngineSpace::PointLight* pLight9 = CreatePointLight();
		pLight9->position = { -80.0f, 4.693f, -17.0f };
		pLight9->color = { r / 255.0f, g / 255.0f, b / 255.0f };
		pLight9->intensity = intensity;
	}

	void BossMap::SetMonsterWave()
	{

	}
	void BossMap::CreateObjects()
	{
		//º¸½º ¸Ê

		CreateBoss(this)->GetComponent<GameEngineSpace::Transform>()->SetPosition(Vector(65.f, 0.f, 0.f), Space::WORLD);

		std::shared_ptr<GameObject> map = CreateEmpty();

		map->AddComponent<MeshRenderer>()->Init("BossMapFloor", "Resources/Model/BossStage_Floor.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("BossMapBrazier", "Resources/Model/BossStage_Brazier.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("BossMapFloorFrame", "Resources/Model/BossStage_FloorFrame.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("BossMapPillar", "Resources/Model/BossStage_Pillar.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("BossMapPot", "Resources/Model/BossStage_Pot.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("BossMapTapestry", "Resources/Model/BossStage_Tapestry.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("BossMapWall", "Resources/Model/BossStage_Wall.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("BossMapMemoryGem", "Resources/Model/BossStage_MemoryGem.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("BossMapWeaponCoin", "Resources/Model/BossStage_WeaponCoin.ase", RendererBase::ModelType::NONE_AO);

	}

	void BossMap::CreateMap()
	{
		
	}
}
