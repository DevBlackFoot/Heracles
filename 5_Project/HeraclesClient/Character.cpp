#include "pch.h"
#include "Character.h"

namespace Heracles
{
	Character::Character()
	{
	}
	Character::~Character()
	{
	}
	void Character::Initialize(tstring name, tstring tag, uint32 layer, SceneBase* currScene)
	{
		this->gameObject = currScene->CreateEmpty();
		
		// 지금은 컴포넌트 추가마다 모델을 로드하는데 미리 로드해둔것을 리소스 매니저로부터 받아오는 구조 필요
		//std::shared_ptr<LegacyRenderer> renderer = this->gameObject->AddComponent<LegacyRenderer>();
		//this->gameObject->GetComponent<Transform>()->SetPosition(Vector(0.f, 0.f, 0.f), Space::WORLD);
		//renderer->Init("Genji", "Resources/Model/genji_max.ase");
		
		std::shared_ptr<Camera> mainCamera =currScene->GetMainCamera();

		// 부모와의 상대포지션 설정하는 함수 필요

		if (!mainCamera)
		{
			return;
		}

		this->gameObject->SetChild(mainCamera->GetGameObject());

		this->gameObject->SetName(name);
		this->gameObject->SetTag(tag);
		this->gameObject->SetLayer(layer);
	}

	void Character::SetToScene(SceneBase* scene)
	{
		// 씬에 오브젝트 추가하는 함수 필요
	}

	void Character::Move(float scale)
	{
		
		Vector pos = this->gameObject->GetComponent<Transform>()->GetWorldPosition();

		Vector dist = this->gameObject->GetComponent<Transform>()->GetWorldRotation() * scale;
		
		this->gameObject->GetComponent<Transform>()->SetPosition(pos + dist);
	}

	void Character::Attack()
	{
		//자식 오브젝트(검)->콜라이더 이동, 이펙트 렌더;
	}

	void Character::OnAttacked(float Damage)
	{
		this->HP -= (1.f - this->armor) * Damage;
		this->isInvinsable = true;
		// 피격 애니메이션(있으면)
	}

	void Character::Die()
	{
	}

	void Character::Resurrect()
	{
	}
}