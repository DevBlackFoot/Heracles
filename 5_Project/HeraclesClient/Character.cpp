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
		
		// ������ ������Ʈ �߰����� ���� �ε��ϴµ� �̸� �ε��صа��� ���ҽ� �Ŵ����κ��� �޾ƿ��� ���� �ʿ�
		//std::shared_ptr<LegacyRenderer> renderer = this->gameObject->AddComponent<LegacyRenderer>();
		//this->gameObject->GetComponent<Transform>()->SetPosition(Vector(0.f, 0.f, 0.f), Space::WORLD);
		//renderer->Init("Genji", "Resources/Model/genji_max.ase");
		
		std::shared_ptr<Camera> mainCamera =currScene->GetMainCamera();

		// �θ���� ��������� �����ϴ� �Լ� �ʿ�

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
		// ���� ������Ʈ �߰��ϴ� �Լ� �ʿ�
	}

	void Character::Move(float scale)
	{
		
		Vector pos = this->gameObject->GetComponent<Transform>()->GetWorldPosition();

		Vector dist = this->gameObject->GetComponent<Transform>()->GetWorldRotation() * scale;
		
		this->gameObject->GetComponent<Transform>()->SetPosition(pos + dist);
	}

	void Character::Attack()
	{
		//�ڽ� ������Ʈ(��)->�ݶ��̴� �̵�, ����Ʈ ����;
	}

	void Character::OnAttacked(float Damage)
	{
		this->HP -= (1.f - this->armor) * Damage;
		this->isInvinsable = true;
		// �ǰ� �ִϸ��̼�(������)
	}

	void Character::Die()
	{
	}

	void Character::Resurrect()
	{
	}
}