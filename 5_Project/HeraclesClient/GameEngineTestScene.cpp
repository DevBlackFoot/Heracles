#include "pch.h"
#include "Animator.h"
#include "SkinnedMeshRenderer.h"
#include "SpriteAnimator.h"
#include "GameEngineTestScene.h"

#include "UIManager.h"
#include "GameEngineManager.h"
#include "GameState.h"
#include "PlayerCamera.h"

#include "GameObjectTemplate.h"
#include "JsonObjectTemplate.h"

#include "InGameUI.h"
#include "SettingUI.h"

#include "PlayerController.h"

Heracles::GameEngineTestScene::GameEngineTestScene(tstring sceneName)
	: SceneBase(sceneName)
{
}

Heracles::GameEngineTestScene::~GameEngineTestScene()
{
}

void Heracles::GameEngineTestScene::BuildScene()
{
	using namespace InGameObject;

	// 맵빌더 추가
	//BuildMap(TEXT("Contents/DummyMap.json"), TEXT("Contents/MapObjects.json"));
	//JsonObject::BuildJsonMap(this->shared_from_this(), TEXT("Contents/DummyMap2.json"), TEXT("Contents/MapObjects.json"));

	// 실례지만 스카이 박스 추가하겠습니다
	SetSkyBox(TEXT("SkyBox"));
	SetIBL(TEXT("MSIBL"), TEXT("Resources/Texture/SunSubMixer_specularIBL.dds"), TEXT("Resources/Texture/SunSubMixer_diffuseIBL.dds"));


	//UIManager::GetInstance()->CreateUIText("TestUI", "TEST", Vector{ 1.0f, 1.0f, 1.0f }, Vector{ 40.f, 10.f, 1.0f }, 0.f, { 1.0f, 1.0f });

	//UIManager::GetInstance()->CreateUIButton("TestButton", "None", "Resources/Texture/bricks.dds", "Hover", "Resources/Texture/000000002405.dds", "Down", "Resources/Texture/000000002406_b.dds", 10, 10, 200, 100, 1.0f);

	// 애니메이션 구경.
	/*std::shared_ptr<GameObject> door = CreateEmpty();
	door->AddComponent<SkinnedMeshRenderer>()->Init("Obstacle", "Resources/Model/ThornyObstacle_ActivateAnimation.ase");
	door->AddComponent<Animator>()->AddAnimation("Resources/Model/ThornyObstacle_ActivateAnimation.ase", "Active");
	door->GetComponent<Animator>()->AddAnimation("Resources/Model/ThornyObstacle_EnteringAnimation.ase", "Enter");
	door->GetComponent<Animator>()->SetAnimation("Enter");*/

	/*std::shared_ptr<GameObject> door = CreateEmpty();
	door->AddComponent<SkinnedMeshRenderer>()->Init("Obstacle", "Resources/Model/Door_CloseAnimation.ase");
	door->AddComponent<Animator>()->AddAnimation("Resources/Model/Door_OpenAnimation.ase", "Open");
	door->GetComponent<Animator>()->AddAnimation("Resources/Model/Door_CloseAnimation.ase", "Close");*/
	//door->GetComponent<Animator>()->SetAnimation("Close");

	std::shared_ptr<GameObject> player = CreatePlayer(this);
	//player->GetComponent<Player::PlayerController>()->SetIsEnable(false);
	std::shared_ptr<GameObject> boss = CreateBoss(this);
	/*auto camera = CreateCamera(this);
	camera->GetComponent<Player::PlayerCamera>()->SetTarget(player->GetComponent<Transform>());
	*/

	//auto altar = CreateAltar(this);

	auto camera = CreateEmpty();
	camera->GetComponent<Transform>()->SetPosition({ 0.f, 1.0f, -10.0f });
	SetMainCamera(camera->AddComponent<Camera>());
	camera->AddComponent<BasicCameraController>();

	std::shared_ptr<GameObject> map = CreateEmpty();

	map->AddComponent<MeshRenderer>()->Init("MapIFloor", "Resources/Model/Stage_80_128_2Wall_3Hole_Floor.ase", RendererBase::ModelType::NONE_AO);
	map->AddComponent<MeshRenderer>()->Init("MapIFloorFrame", "Resources/Model/Stage_80_128_2Wall_3Hole_FloorFrame.ase", RendererBase::ModelType::NONE_AO);
	map->AddComponent<MeshRenderer>()->Init("MapIBrazier", "Resources/Model/Stage_80_128_2Wall_3Hole_Brazier.ase", RendererBase::ModelType::NONE_AO);
	map->AddComponent<MeshRenderer>()->Init("MapIHeraStatue", "Resources/Model/Stage_80_128_2Wall_3Hole_HeraStatue.ase", RendererBase::ModelType::NONE_AO);
	map->AddComponent<MeshRenderer>()->Init("MapIPillar", "Resources/Model/Stage_80_128_2Wall_3Hole_Pillar.ase", RendererBase::ModelType::NONE_AO);
	map->AddComponent<MeshRenderer>()->Init("MapIPot", "Resources/Model/Stage_80_128_2Wall_3Hole_Pot.ase", RendererBase::ModelType::NONE_AO);
	map->AddComponent<MeshRenderer>()->Init("MapITapestry", "Resources/Model/Stage_80_128_2Wall_3Hole_Tapestry.ase", RendererBase::ModelType::NONE_AO);
	map->AddComponent<MeshRenderer>()->Init("MapIWall", "Resources/Model/Stage_80_128_2Wall_3Hole_Wall.ase", RendererBase::ModelType::NONE_AO);

	//map->GetComponent<Transform>()->SetRotation({0.f, 180.f, 0.f});

	//map->AddComponent<MeshRenderer>()->Init("MapFloor", "Resources/Model/Stage_80_80_4Hole_Floor.ase");
	//map->AddComponent<MeshRenderer>()->Init("MapFloorFrame", "Resources/Model/Stage_80_80_4Hole_FloorFrame.ase");
	//map->AddComponent<MeshRenderer>()->Init("MapWall", "Resources/Model/Stage_80_80_4Hole_Wall.ase");

	//std::shared_ptr<GameObject> MeshObjTest = CreateEmpty();
	//MeshObjTest->AddComponent<MeshRenderer>()->Init("TestObj", "Resources/Model/Stage_40_80_2Hole_ver.ase");
	//MeshObjTest->GetComponent<Transform>()->SetPosition({0.f, 20.f, 0.f});

	// 게임 매니저 깡통을 하나 만들어서 관리를 해보자..!
	std::shared_ptr<GameObject> gameManager = CreateEmpty();
	gameManager->AddComponent<GameEngineManager>();

	// Ingame UI Test
	std::shared_ptr<GameObject> ui = CreateEmpty();

	ui->SetName(_T("TestName"));

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
	player->SetChild(ui);
	camera->SetChild(ui);
	ui->AddComponent<SpriteAnimator>()->Init("Effect1");
	ui->GetComponent<SpriteAnimator>()->SetSpriteTexture("Effect1", "Resources/Effect/ef_03_SpriteSheet.png", 2048, 1024, 4, 2, 0.05f);
	ui->GetComponent<SpriteAnimator>()->SetPosition({ 0.0f, 10.0f, 0.0f });
	ui->GetComponent<SpriteAnimator>()->SetWidth(10.0f);
	ui->GetComponent<SpriteAnimator>()->SetHeight(10.0f);
	ui->GetComponent<SpriteAnimator>()->SetLoop(false);

	// Effect Test
	//std::shared_ptr<GameObject> effect1 = CreateEmpty();
	//player->SetChild(effect1);
	//effect1->AddComponent<SpriteAnimator>()->Init("Effect1");
	//effect1->GetComponent<SpriteAnimator>()->SetSpriteTexture("Effect1", "Resources/Effect/ef_03_SpriteSheet.png", 2048, 1024, 4, 2, 0.05f);
	//effect1->GetComponent<SpriteAnimator>()->SetPosition({ 0.0f, 10.0f, 0.0f });
	//effect1->GetComponent<SpriteAnimator>()->SetWidth(10.0f);
	//effect1->GetComponent<SpriteAnimator>()->SetHeight(10.0f);
	//effect1->GetComponent<SpriteAnimator>()->SetLoop(false);

	std::shared_ptr<GameObject> effect2 = CreateEmpty();
	effect2->AddComponent<SpriteAnimator>()->Init("Effect2");
	effect2->GetComponent<SpriteAnimator>()->SetSpriteTexture("Effect2", "Resources/Effect/ef_16_green_SpriteSheet.png", 2048, 2048, 4, 4, 0.05f);
	effect2->GetComponent<SpriteAnimator>()->SetPosition({ 10.0f, 10.0f, 0.0f });
	effect2->GetComponent<SpriteAnimator>()->SetWidth(10.0f);
	effect2->GetComponent<SpriteAnimator>()->SetHeight(10.0f);
	effect2->GetComponent<SpriteAnimator>()->SetLoop(true);

	std::shared_ptr<GameObject> effect3 = CreateEmpty();
	effect3->AddComponent<SpriteAnimator>()->Init("Effect3");
	effect3->GetComponent<SpriteAnimator>()->SetSpriteTexture("Effect3", "Resources/Effect/expl_02_04_SpriteSheet.png", 4096, 2048, 8, 4, 0.05f);
	effect3->GetComponent<SpriteAnimator>()->SetPosition({ 20.0f, 10.0f, 0.0f });
	effect3->GetComponent<SpriteAnimator>()->SetWidth(10.0f);
	effect3->GetComponent<SpriteAnimator>()->SetHeight(10.0f);
	effect3->GetComponent<SpriteAnimator>()->SetLoop(true);

	std::shared_ptr<GameObject> effect4 = CreateEmpty();
	effect4->AddComponent<SpriteAnimator>()->Init("Effect4");
	effect4->GetComponent<SpriteAnimator>()->SetSpriteTexture("Effect4", "Resources/Effect/Flame_2_SpriteSheet.png", 2048, 2048, 4, 4, 0.05f);
	effect4->GetComponent<SpriteAnimator>()->SetPosition({ 30.0f, 10.0f, 0.0f });
	effect4->GetComponent<SpriteAnimator>()->SetWidth(10.0f);
	effect4->GetComponent<SpriteAnimator>()->SetHeight(10.0f);
	effect4->GetComponent<SpriteAnimator>()->SetLoop(true);

	std::shared_ptr<GameObject> effect5 = CreateEmpty();
	effect5->AddComponent<SpriteAnimator>()->Init("Effect5");
	effect5->GetComponent<SpriteAnimator>()->SetSpriteTexture("Effect5", "Resources/Effect/ground_explosion_purple_SpriteSheet.png", 1024, 1024, 4, 4, 0.05f);
	effect5->GetComponent<SpriteAnimator>()->SetPosition({ 40.0f, 10.0f, 0.0f });
	effect5->GetComponent<SpriteAnimator>()->SetWidth(10.0f);
	effect5->GetComponent<SpriteAnimator>()->SetHeight(10.0f);
	effect5->GetComponent<SpriteAnimator>()->SetLoop(true);

	std::shared_ptr<GameObject> effect6 = CreateEmpty();
	effect6->AddComponent<SpriteAnimator>()->Init("Effect6");
	effect6->GetComponent<SpriteAnimator>()->SetSpriteTexture("Effect6", "Resources/Effect/ready_attack_SpriteSheet.png", 2048, 1024, 8, 4, 0.05f);
	effect6->GetComponent<SpriteAnimator>()->SetPosition({ 50.0f, 10.0f, 0.0f });
	effect6->GetComponent<SpriteAnimator>()->SetWidth(10.0f);
	effect6->GetComponent<SpriteAnimator>()->SetHeight(10.0f);
	effect6->GetComponent<SpriteAnimator>()->SetLoop(true);

	std::shared_ptr<GameObject> effect7 = CreateEmpty();
	effect7->AddComponent<SpriteAnimator>()->Init("Effect7");
	effect7->GetComponent<SpriteAnimator>()->SetSpriteTexture("Effect7", "Resources/Effect/Smoke.png", 2048, 2048, 4, 4, 0.05f);
	effect7->GetComponent<SpriteAnimator>()->SetPosition({ 60.0f, 10.0f, 0.0f });
	effect7->GetComponent<SpriteAnimator>()->SetWidth(10.0f);
	effect7->GetComponent<SpriteAnimator>()->SetHeight(10.0f);
	effect7->GetComponent<SpriteAnimator>()->SetLoop(true);

	// Point Light Test
	float r = 0xff;
	float g = 0x63;
	float b = 0x49;

	GraphicsEngineSpace::PointLight* pointLight1 = CreatePointLight();
	pointLight1->color = { r / 255.0f, g / 255.0f, b / 255.0f };
	pointLight1->position.y += 5.0f;
	pointLight1->position.z += -6.0f;
	pointLight1->intensity = 2.0f;
	
	GraphicsEngineSpace::PointLight* pointLight2 = CreatePointLight();
	pointLight2->color = { r / 255.0f, g / 255.0f, b / 255.0f };
	pointLight2->position.x += 20.0f;
	pointLight2->position.y += 5.0f;
	pointLight2->position.z += -3.0f;
	pointLight2->intensity = 1.0f;
	
	GraphicsEngineSpace::PointLight* pointLight3 = CreatePointLight();
	pointLight3->color = { r / 255.0f, g / 255.0f, b / 255.0f };
	pointLight3->position.x -= 20.0f;
	pointLight3->position.y += 5.0f;
	pointLight3->position.z += -9.0f;
	pointLight3->intensity = 3.0f;

	CreateMoney(this, { 5.0f, 10.0f, 0.0f });

}
