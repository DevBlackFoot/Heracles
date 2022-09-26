#include "pch.h"
#include "DemoScene.h"

#include "PlayerCamera.h"
#include "GameObjectTemplate.h"
#include "UIManager.h"
#include "GameState.h"

#include "TestBossController.h"
#include "SkinnedMeshRenderer.h"
#include "Animator.h"
#include "Entrance.h"

namespace Heracles
{
	using namespace InGameObject;
	Heracles::DemoScene::DemoScene(tstring sceneName) : SceneBase(sceneName)
	{
	}

	Heracles::DemoScene::~DemoScene()
	{
	}

	void Heracles::DemoScene::BuildScene()
	{		
		std::shared_ptr<GameObject> player = CreatePlayer(this);
		auto camera = CreateCamera(this);
		camera->GetComponent<Player::PlayerCamera>()->SetTarget(player->GetComponent<Transform>());

		//auto t  = CreateEntrance(this, TEXT("TEmp"), Vector(10.f, 0.f, 10.f));
		//t->GetComponent<entrance>()->SetEntranceModel(1);
		//t->AddComponent<Boss::TestBossController>();

		std::shared_ptr<GameObject> entrance = this->CreateEmpty();

		entrance->GetComponent<Transform>()->SetPosition( Vector(5.f, 0.f, 5.f), Space::WORLD);
		entrance->GetComponent<Transform>()->SetScale(Vector(1.2f, 1.2f, 1.2f));

		entrance->SetName(TEXT("Te"));
		entrance->SetTag(TEXT("TE"));

		entrance->AddComponent<SkinnedMeshRenderer>()->Init("Door1", "Resources/Model/Door_OpenAnimation.ase", RendererBase::ModelType::NONE_AO);
		entrance->AddComponent<Animator>();

		entrance->AddComponent<Boss::TestBossController>();


	}
}

