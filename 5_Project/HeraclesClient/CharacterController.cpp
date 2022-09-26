#include "pch.h"
#include "CharacterController.h"
#include "InputManager.h"

namespace Heracles
{
	const float tempRotateRate = 50.f / PI;
	float tempspeed = 1.f;


	CharacterController::CharacterController(std::weak_ptr<GameObject> obj)
		:ComponentBase(obj, ComponentType::MONOBEHAVIOR)
	{
	}

	CharacterController::~CharacterController()
	{
	}

	void CharacterController::Awake()
	{
		this->transform = this->GetGameObject()->GetComponent<Transform>();
		this->input = InputManager::GetInstance();
	}
	void CharacterController::Update(float tick)
	{
		Vector curr = this->transform->GetLook();
		Vector rot;

		tempspeed *= tick;

		if (input->GetInputState('W', KeyState::STAY))
		{
			//ȸ��
			if (Vector3Dot(Vector3Cross(Vector::UnitZ, curr), Vector::UnitY) > 0.f) //z���� look������ �����ʿ� ���� ��
			{
				rot = ConvertQuaternionToEuler(ConvertRotationAxisAngleToQuaternion(Vector::UnitY, tempRotateRate * tick));
				this->transform->SetRotation(rot);
			}
			else if (Vector3Dot(Vector3Cross(Vector::UnitZ, curr), Vector::UnitY) < 0.f)
			{
				rot = ConvertQuaternionToEuler(ConvertRotationAxisAngleToQuaternion(Vector::UnitY, -tempRotateRate * tick));
				this->transform->SetRotation(rot);
			}
			//�̵�
			this->transform->SetPosition(this->transform->GetWorldPosition() + this->transform->GetLook() * tempspeed, Space::WORLD);
		}

		else if (input->GetInputState('W', KeyState::STAY) && input->GetInputState('D', KeyState::STAY))
		{
			//ȸ��
			if (Vector3Dot(Vector3Cross(Vector3Normalize({ 1.f,0.f,1.f }), curr), Vector::UnitY) > 0.f)
			{
				rot = ConvertQuaternionToEuler(ConvertRotationAxisAngleToQuaternion(Vector::UnitY, tempRotateRate * tick));
				this->transform->SetRotation(rot);
			}
			else if (Vector3Dot(Vector3Cross(Vector3Normalize({ 1.f,0.f,1.f }), curr), Vector::UnitY) < 0.f)
			{
				rot = ConvertQuaternionToEuler(ConvertRotationAxisAngleToQuaternion(Vector::UnitY, -tempRotateRate * tick));
				this->transform->SetRotation(rot);
			}

			//�̵�
			this->transform->SetPosition(this->transform->GetWorldPosition() + this->transform->GetLook() * tempspeed, Space::WORLD);
		}
		else if (input->GetInputState('D', KeyState::STAY))
		{
			//ȸ��
			if (Vector3Dot(Vector3Cross(Vector::UnitX, curr), Vector::UnitY)>0.f)
			{
				rot = ConvertQuaternionToEuler(ConvertRotationAxisAngleToQuaternion(Vector::UnitY, -tempRotateRate * tick));
				this->transform->SetRotation(rot);
			}
			else if (Vector3Dot(Vector3Cross(Vector::UnitX, curr), Vector::UnitY) < 0.f)
			{
				rot = ConvertQuaternionToEuler(ConvertRotationAxisAngleToQuaternion(Vector::UnitY, -tempRotateRate * tick));

				this->transform->SetRotation(rot);
			}

			//�̵�
			this->transform->SetPosition(this->transform->GetWorldPosition() + this->transform->GetLook() * tempspeed, Space::WORLD);
		}
		else if (input->GetInputState('D', KeyState::STAY) && input->GetInputState('S', KeyState::STAY))
		{
			//ȸ��
			if (Vector3Dot(Vector3Cross(Vector3Normalize({ 1.f,0.f,-1.f }), curr), Vector::UnitY) > 0.f)
			{
				rot = ConvertQuaternionToEuler(ConvertRotationAxisAngleToQuaternion(Vector::UnitY, tempRotateRate * tick));
				this->transform->SetRotation(rot);
			}
			else if (Vector3Dot(Vector3Cross(Vector3Normalize({ 1.f,0.f,-1.f }), curr), Vector::UnitY) < 0.f)
			{
				rot = ConvertQuaternionToEuler(ConvertRotationAxisAngleToQuaternion(Vector::UnitY, -tempRotateRate * tick));
				this->transform->SetRotation(rot);
			}

			//�̵�
			this->transform->SetPosition(this->transform->GetWorldPosition() + this->transform->GetLook() * tempspeed, Space::WORLD);
		}
		else if (input->GetInputState('S', KeyState::STAY))
		{
			//ȸ��
			if (Vector3Dot(Vector3Cross(Vector3Normalize({ -1.f,0.f,0.f }), curr), Vector::UnitY) > 0.f)
			{
				rot = ConvertQuaternionToEuler(ConvertRotationAxisAngleToQuaternion(Vector::UnitY, tempRotateRate * tick));
				this->transform->SetRotation(rot);
			}
			else if (Vector3Dot(Vector3Cross(Vector3Normalize({ -1.f,0.f,0.f }), curr), Vector::UnitY) < 0.f)
			{
				rot = ConvertQuaternionToEuler(ConvertRotationAxisAngleToQuaternion(Vector::UnitY, -tempRotateRate * tick));
				this->transform->SetRotation(rot);
			}

			//�̵�
			this->transform->SetPosition(this->transform->GetWorldPosition() + this->transform->GetLook() * tempspeed, Space::WORLD);
		}
		else if (input->GetInputState('S', KeyState::STAY) && input->GetInputState('A', KeyState::STAY))
		{
			//ȸ��
			if (abs(curr.x / curr.z) > 1.f)
			{
				rot = ConvertQuaternionToEuler(ConvertRotationAxisAngleToQuaternion(Vector::UnitY, -tempRotateRate * tick));
				this->transform->SetRotation(rot);
			}
			else if (abs(curr.x / curr.z) < 1.f)
			{
				rot = ConvertQuaternionToEuler(ConvertRotationAxisAngleToQuaternion(Vector::UnitY, tempRotateRate * tick));
				this->transform->SetRotation(rot);
			}

			//�̵�
			this->transform->SetPosition(this->transform->GetWorldPosition() + this->transform->GetLook() * tempspeed, Space::WORLD);
		}
		else if (input->GetInputState('A', KeyState::STAY))
		{
			//ȸ��
			if (abs(curr.z) > 0.f)
			{
				rot = ConvertQuaternionToEuler(ConvertRotationAxisAngleToQuaternion(Vector::UnitY, tempRotateRate * tick));
				this->transform->SetRotation(rot);
			}
			else if (abs(curr.z) < 0.f)
			{
				rot = ConvertQuaternionToEuler(ConvertRotationAxisAngleToQuaternion(Vector::UnitY, -tempRotateRate * tick));
				this->transform->SetRotation(rot);
			}

			//�̵�
			this->transform->SetPosition(this->transform->GetWorldPosition() + this->transform->GetLook() * tempspeed, Space::WORLD);
		}
		else if (input->GetInputState('W', KeyState::STAY) && input->GetInputState('A', KeyState::STAY))
		{
			//ȸ��
			if (curr.z > 0.f)
			{
				rot = ConvertQuaternionToEuler(ConvertRotationAxisAngleToQuaternion(Vector::UnitY, tempRotateRate * tick));
				this->transform->SetRotation(rot);
			}
			else if (curr.x / curr.z < 1.f)
			{
				rot = ConvertQuaternionToEuler(ConvertRotationAxisAngleToQuaternion(Vector::UnitY, -tempRotateRate * tick));
				this->transform->SetRotation(rot);
			}
			//�̵�
			this->transform->SetPosition(this->transform->GetWorldPosition() + this->transform->GetLook() * tempspeed, Space::WORLD);
		}


	}

	void CharacterController::LateUpdate(float tick)
	{
	}

}

