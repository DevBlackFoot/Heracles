#include "pch.h"
#include "BeforeBossMap.h"
#include "GameObjectTemplate.h"
#include "PlayerCamera.h"

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

	BeforeBossMap::BeforeBossMap(tstring sceneName) :SceneBase(sceneName)
	{
	}
	void BeforeBossMap::BuildScene()
	{
		GameState::GetInstance()->mapCleared = true;
		GameState::GetInstance()->currMonsterNum = 0;

		SetSkyBox(TEXT("SkyBox"));

		SetIBL(TEXT("MSIBL"), TEXT("Resources/Texture/SunSubMixer_specularIBL.dds"), TEXT("Resources/Texture/SunSubMixer_diffuseIBL.dds"));

		std::shared_ptr<GameObject> player = CreatePlayer(this);

		if (GameState::GetInstance()->exitType)
		{
			player->GetComponent<Transform>()->SetPosition(this->SpawnA, Space::WORLD);
		}
		else
		{
			player->GetComponent<Transform>()->SetPosition(this->SpawnB, Space::WORLD);
		}
		auto camera = CreateCamera(this);
		camera->GetComponent<Player::PlayerCamera>()->SetTarget(player->GetComponent<Transform>());

		this->sceneController = CreateSceneController(this);
		this->sceneController->GetComponent<SceneController>();

		this->CreateWallCollider();
		this->SetMonsterWave();
		this->CreateObjects();
		this->CreateUI();
		this->CreatePointLights();
		SoundManager::GetInstance()->PlayBGM("BeforeBossMap");

	}
	void BeforeBossMap::CreateWallCollider()
	{
		CreateWall(this, Vector(0.f, 0.f, 20.f), 80.f, 1.f);
		CreateWall(this, Vector(0.f, 0.f, -20.f), 80.f, 1.f);
		CreateWall(this, Vector(-40.f, 0.f, 0.f), 1.f, 40.f);
		CreateWall(this, Vector(40.f, 0.f, 0.f), 1.f, 40.f);
		CreateWall(this, Vector(0.f, 0.f, 12.f), 30.f, 16.f);
		CreateWall(this, Vector(0.f, 0.f, -12.f), 30.f, 16.f);
	}
	void BeforeBossMap::SetMonsterWave()
	{
	}
	void BeforeBossMap::CreateObjects()
	{
		// 보스맵 전 통로
		auto ent1 = CreateEntranceWithIndex(this, this->ExitA,4)->GetComponent<Entrance>();
		ent1->GetGameObject()->GetComponent<Transform>()->SetRotation(Vector(0.f, 90.f, 0.f), Space::LOCAL);
		ent1->GetGameObject()->GetComponent<Transform>()->SetPosition(ent1->GetGameObject()->GetComponent<Transform>()->GetWorldPosition() + Vector(-0.5f, 0.f, 7.f), Space::WORLD);
		ent1->GetGameObject()->GetComponent<BoxCollider>()->SetLocalCenter(Vector(-5.f, 0.f, -8.f));
		ent1->SetNextScene(TEXT("BossMap"));
		ent1->SetEntranceType(false);

		std::shared_ptr<GameObject> map = CreateEmpty();

		map->AddComponent<MeshRenderer>()->Init("BeforeBossMapFloor", "Resources/Model/Stage_40_80_BeforeBoss_Floor.ase", RendererBase::ModelType::NONE_AO );
		map->AddComponent<MeshRenderer>()->Init("BeforeBossMapFloorFrame", "Resources/Model/Stage_40_80_BeforeBoss_FloorFrame.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("BeforeBossMapWall", "Resources/Model/Stage_40_80_BeforeBoss_Wall.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("BeforeBossMapBrazier", "Resources/Model/Stage_40_80_BeforeBoss_Brazier.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("BeforeBossMapHeraStatue", "Resources/Model/Stage_40_80_BeforeBoss_HeraStatue.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("BeforeBossMapPillar", "Resources/Model/Stage_40_80_BeforeBoss_Pillar.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("BeforeBossMapTapestry", "Resources/Model/Stage_40_80_BeforeBoss_Tapestry.ase", RendererBase::ModelType::NONE_AO);

	}
	void BeforeBossMap::CreateUI()
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

	void BeforeBossMap::CreatePointLights()
	{
		float r = 0xff;
		float g = 0x63;
		float b = 0x49;

		float intensity = 1.0f;

		GraphicsEngineSpace::PointLight* pLight1 = CreatePointLight();
		pLight1->position = { 37.0f, 4.693f, 13.0f };
		pLight1->color = { r / 255.0f, g / 255.0f, b / 255.0f };
		pLight1->intensity = intensity;

		GraphicsEngineSpace::PointLight* pLight2 = CreatePointLight();
		pLight2->position = { 37.0f, 4.693f, -13.0f };
		pLight2->color = { r / 255.0f, g / 255.0f, b / 255.0f };
		pLight2->intensity = intensity;

		GraphicsEngineSpace::PointLight* pLight3 = CreatePointLight();
		pLight3->position = { -32.7202f, 4.693f, 17.063f };
		pLight3->color = { r / 255.0f, g / 255.0f, b / 255.0f };
		pLight3->intensity = intensity;

		GraphicsEngineSpace::PointLight* pLight4 = CreatePointLight();
		pLight4->position = { -21.5993f, 4.693f, -16.8734f };
		pLight4->color = { r / 255.0f, g / 255.0f, b / 255.0f };
		pLight4->intensity = intensity;
	}
}