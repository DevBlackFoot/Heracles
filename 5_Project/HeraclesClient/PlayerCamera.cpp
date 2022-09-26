#include "pch.h"
#include "PlayerCamera.h"
#include "GameState.h"

namespace Heracles
{
	namespace Player
	{
		PlayerCamera::PlayerCamera(std::weak_ptr<GameObject> obj)
			:ComponentBase(obj,ComponentType::CAMERA)
		{
		}
		void PlayerCamera::Start()
		{
			if (GameState::GetInstance()->curseSight)
			{
				//radialBlur
			}
			this->target = GameState::GetInstance()->GetPlayer()->GetComponent<Transform>();
			this->transform = this->GetGameObject()->GetComponent<Transform>();
			
		}
		void PlayerCamera::PreUpdate(float tick)
		{
			this->transform->SetPosition(target->GetWorldPosition() + Vector(this->x, this->y, this->z));
			
			if (this->cameraLookat)
			{
				this->transform->LookAt(target->GetWorldPosition());
				this->cameraLookat = false;
			}
		}
		void PlayerCamera::LateUpdate(float tick)
		{

			
		}
		void PlayerCamera::SetCameraPos(float x,float y, float z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}
	}
}