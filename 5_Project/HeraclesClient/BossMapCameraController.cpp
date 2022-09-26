#include "pch.h"
#include "BossMapCameraController.h"

#include "GameState.h"
#include "SoundManager.h"
#include "SceneManager.h"
#include "BossTree.h"

#include "PlayerCamera.h"
#include "Animator.h"
#include "PlayerStateMachine.h"

namespace Heracles
{
	namespace Boss
	{
		static Vector VecotrMultFloat(Vector v, float f)
		{
			return Vector
			{
				v.x * f,
				v.y * f,
				v.z * f,
				v.w * f
			};
		}

		BossMapCameraController::BossMapCameraController(std::weak_ptr<GameObject> obj) : ComponentBase(obj,ComponentType::MONOBEHAVIOR)
		{
		}
		void BossMapCameraController::Awake()
		{

		}

		void BossMapCameraController::Start()
		{
			this->boss = GameState::GetInstance()->GetBoss();
			this->player = GameState::GetInstance()->GetPlayer();
			this->target = player->GetComponent<Transform>();

			this->transform = this->GetGameObject()->GetComponent<Transform>();
			this->transform->SetPosition(this->initialPos, Space::WORLD);
			SoundManager::GetInstance()->PlayEffect("BossStartRoar");

			GameState::GetInstance()->interacting = true;
		}

		void BossMapCameraController::PreUpdate(float tick)
		{

		}

		void BossMapCameraController::Update(float tick)
		{
			if (this->timer <= 3.f)
			{
				this->GetGameObject()->GetComponent<Transform>()->SetPosition(this->initialPos + VecotrMultFloat(Vector(1.f, 0.f, 0.f), 105.f/3.f * this->timer));
				this->timer += tick;
			}
			else if (this->timer < 5.f)
			{
				this->GetGameObject()->GetComponent<Transform>()->SetRotation(VecotrMultFloat(Vector(0.f, 1.f, 0.f) , 180.f * (this->timer - 30.f)),Space::LOCAL);
				this->timer += tick;
			}
			else if(this->timer <= 7.f)
			{
				this->GetGameObject()->GetComponent<Transform>()->SetPosition(this->initialPos + VecotrMultFloat(Vector(1.f, 0.f, 0.f), 105.f/3.f * (70.f - this->timer)));
				this->timer += tick;
			}
			else
			{
				GameState::GetInstance()->interacting = false;
				boss->GetComponent<BossTree>()->onStart = false;
			}
			
		}

		void BossMapCameraController::LateUpdate(float tick)
		{
		}
	}
}