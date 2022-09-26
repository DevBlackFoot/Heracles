#include "pch.h"

#include "PlayerController.h"
#include "PlayerStateMachine.h"

#include "RayCollider.h"
#include "InputManager.h"
#include "GameState.h"
#include "UIManager.h"
#include "Transform.h"

#include "Animator.h"
namespace Heracles
{
	namespace Player
	{
		static float elsilon = 0.01f;
		static float rotateMultiplyer = 200.f;
		static float moveSpeedMultiplyer = 2.f;

		static bool CompareVector(Vector v1, Vector v2)
		{
			if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

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

		PlayerController::PlayerController(std::weak_ptr<GameObject> obj) : ComponentBase(obj, ComponentType::MONOBEHAVIOR)			
		{				
		}
		PlayerController::~PlayerController()
		{
		}

		void PlayerController::Awake()
		{
			this->playerTransform = this->GetGameObject()->GetComponent<Transform>();
			this->input = InputManager::GetInstance();

			/*auto cameraTransform = SceneManager::GetInstance()->GetCurrentScene()->GetMainCamera()->GetGameObject()->GetComponent<Transform>();
			auto cameraLook = cameraTransform->GetLook();
			auto cameraRight = cameraTransform->GetRight();

			dirStop = Vector::Zero;
			dirUp = cameraLook;
			dirLeft = -cameraRight;
			dirRight = cameraRight;
			dirDown = -cameraLook;*/

		}
		void PlayerController::Start()
		{
		}
		void PlayerController::PreUpdate(float tick)
		{
			Canvas* debugCanvas = UIManager::GetInstance()->GetCanvas("DebugCanvas");

			this->moveSpeed = this->GetGameObject()->GetComponent<Player::PlayerStateMachine>()->moveSpeed * tick * moveSpeedMultiplyer;
			this->rotateDegree = this->GetGameObject()->GetComponent<Player::PlayerStateMachine>()->rotateSpeed * tick * rotateMultiplyer;
			if (this->GetGameObject()->GetComponent<RayCollider>()->GetHit().hitGameObject != nullptr)
			{
				UIManager::GetInstance()->SetTextUIIsEnable("Ray", true, debugCanvas);
			}
			else
			{
				UIManager::GetInstance()->SetTextUIIsEnable("Ray", false, debugCanvas);
			}

		}
		void PlayerController::Update(float tick)
		{
			if (GameState::GetInstance()->interacting)
			{
				return;
			}

			Player::EventBase* e = this->GetGameObject()->GetComponent<Player::PlayerStateMachine>()->GetEventData();

			if (this->input->GetInputState(VK_END, KeyState::DOWN))
			{
				GameState::GetInstance()->mapCleared = true;
			}
			if (this->input->GetInputState(VK_TAB, KeyState::DOWN))
			{
				this->GetGameObject()->GetComponent<PlayerStateMachine>()->memoryPieceNum += 1;
			}
			if (this->input->GetInputState(VK_SHIFT, KeyState::STAY) && this->input->GetInputState('V', KeyState::DOWN))
			{
				float target = this->GetGameObject()->GetComponent<PlayerStateMachine>()->currHp + 10.f;
				if (target >= this->GetGameObject()->GetComponent<PlayerStateMachine>()->maxHp)
				{

				}
				else
				{
					this->GetGameObject()->GetComponent<PlayerStateMachine>()->currHp += 10.f;
				}
			}



			//if (this->input->GetInputState('R', KeyState::STAY))
			//{
			//	auto transform = this->GetGameObject()->GetComponent<Transform>();
			//	transform->LookAt(transform->GetWorldPosition() + Vector(-1.f,0.f,-1.f));
			//	transform->SetPosition(transform->GetWorldPosition() + VecotrMultFloat(Vector(-1.f, 0.f, -1.f), this->moveSpeed * 4.f), Space::WORLD);
			//}

			if (this->input->GetInputState(' ', KeyState::DOWN) || input->GetGamePad()->IsPadCommand(XPadInput::ButtonA, XPadCommand::Push))
			{
				if (this->GetGameObject()->GetComponent<Player::PlayerStateMachine>()->dodgeTimer < 0.001f)
				{
					e->externalEventType = Player::ExternalEvent::DODGE;
					return;
				}
			}

			//if (this->input->GetInputState('Q', KeyState::DOWN))
			//{
			//	if (this->GetGameObject()->GetComponent<Player::PlayerStateMachine>()->skillTimer < 0.001f)
			//	{
			//		e->externalEventType = Player::ExternalEvent::SKILL;
			//		return;
			//	}
			//}

			if (this->input->GetInputState(VK_LBUTTON, KeyState::DOWN) || input->GetGamePad()->IsPadCommand(XPadInput::ButtonB, XPadCommand::Push))
			{
				if (this->GetGameObject()->GetComponent<Player::PlayerStateMachine>()->attackTimer < 0.001f)
				{
					e->externalEventType = Player::ExternalEvent::ATTACK;
					return;
				}
			}

			if (this->GetGameObject()->GetComponent<PlayerStateMachine>()->canMove)
			{
				this->MoveCheck(e);
			}
		}

		void PlayerController::LateUpdate(float tick)
		{
#ifdef _DEBUG
			if (InputManager::GetInstance()->GetInputState('1', KeyState::DOWN))
			{
				GameState::GetInstance()->GetPlayer()->GetComponent<PlayerStateMachine>()->SavePlayerDataToGameState();
				SceneManager::GetInstance()->LoadScene(TEXT("StartScene"));
			}
			else if (InputManager::GetInstance()->GetInputState('2', KeyState::DOWN))
			{
				GameState::GetInstance()->GetPlayer()->GetComponent<PlayerStateMachine>()->SavePlayerDataToGameState();
				SceneManager::GetInstance()->LoadScene(TEXT("MapA"));
			}
			else if (InputManager::GetInstance()->GetInputState('3', KeyState::DOWN))
			{
				GameState::GetInstance()->GetPlayer()->GetComponent<PlayerStateMachine>()->SavePlayerDataToGameState();
				SceneManager::GetInstance()->LoadScene(TEXT("MapB"));
			}
			else if (InputManager::GetInstance()->GetInputState('4', KeyState::DOWN))
			{
				GameState::GetInstance()->GetPlayer()->GetComponent<PlayerStateMachine>()->SavePlayerDataToGameState();
				SceneManager::GetInstance()->LoadScene(TEXT("MapC"));
			}
			else if (InputManager::GetInstance()->GetInputState('5', KeyState::DOWN))
			{
				GameState::GetInstance()->GetPlayer()->GetComponent<PlayerStateMachine>()->SavePlayerDataToGameState();
				SceneManager::GetInstance()->LoadScene(TEXT("MapE"));
			}
			else if (InputManager::GetInstance()->GetInputState('6', KeyState::DOWN))
			{
				GameState::GetInstance()->GetPlayer()->GetComponent<PlayerStateMachine>()->SavePlayerDataToGameState();
				SceneManager::GetInstance()->LoadScene(TEXT("MapF"));
			}
			else if (InputManager::GetInstance()->GetInputState('7', KeyState::DOWN))
			{
				GameState::GetInstance()->GetPlayer()->GetComponent<PlayerStateMachine>()->SavePlayerDataToGameState();
				SceneManager::GetInstance()->LoadScene(TEXT("MapI"));
			}
			else if (InputManager::GetInstance()->GetInputState('8', KeyState::DOWN))
			{
				GameState::GetInstance()->GetPlayer()->GetComponent<PlayerStateMachine>()->SavePlayerDataToGameState();
				SceneManager::GetInstance()->LoadScene(TEXT("BossMap"));
			}
#endif 
			//Debug
			{
				Canvas* debugCanvas = UIManager::GetInstance()->GetCanvas("DebugCanvas");

			
				std::string text = "LookAt : ";
				text.append(std::to_string(this->playerTransform->GetLook().x));
				text.append(", ");
				text.append(std::to_string(this->playerTransform->GetLook().y));
				text.append(", ");
				text.append(std::to_string(this->playerTransform->GetLook().z));
				UIManager::GetInstance()->SetTextUIText("LookAt", text, debugCanvas);
			}
		}

		void PlayerController::MoveCheck(EventBase* e)
		{
			Vector inputAxis = this->CheckInputAxis();
			Vector look = this->GetGameObject()->GetComponent<Transform>()->GetLook();


			if (!CompareVector(inputAxis, Vector::Zero))
			{
				e->externalEventType = ExternalEvent::MOVE;
				if (Vector3Dot(inputAxis, look) >= 1.f - elsilon)
				{
					MoveLookDir(1.f, inputAxis);
				}
				else
				{
					Rotate(look, inputAxis);
				}
			}
		}

		Vector PlayerController::CheckInputAxis()
		{
			// 패드 방향 상태
			bool padUp = input->GetGamePad()->IsPadCommand(XPadInput::Up, XPadCommand::KeepPress);
			bool padDown = input->GetGamePad()->IsPadCommand(XPadInput::Down, XPadCommand::KeepPress);
			bool padLeft = input->GetGamePad()->IsPadCommand(XPadInput::Left, XPadCommand::KeepPress);
			bool padRight = input->GetGamePad()->IsPadCommand(XPadInput::Right, XPadCommand::KeepPress);

			// 패드 실제 입력방향 벡터
			Vector stickDirection = input->GetGamePad()->GetStickDirection();

			// 카메라 정보에서 Look과 Right를 들고온다..
			

			Vector padAxis = Vector::Zero;
			if (padUp || padDown || padLeft || padRight)
				padAxis = CalculateRotationVector(padUp, padDown, padLeft, padRight);

			// 키보드 방향키 입력 상태
			bool keyboardUpStay = input->GetInputState('W', KeyState::STAY);
			bool keyboardDownStay = input->GetInputState('S', KeyState::STAY);
			bool keyboardLeftStay = input->GetInputState('A', KeyState::STAY);
			bool keyboardRightStay = input->GetInputState('D', KeyState::STAY);
			
			// 키보드와 패드 상태에 따른 축변화를 계산한다.
			Vector keyboardAxis = CalculateRotationVector(keyboardUpStay, keyboardDownStay, keyboardLeftStay, keyboardRightStay);						

			return keyboardAxis + padAxis == Vector::Zero ? Vector::Zero : Vector3Normalize(keyboardAxis + padAxis);
			
		}

		void PlayerController::MoveLookDir(float m, Vector inputAxis)
		{
			std::shared_ptr<Transform> transform = this->GetGameObject()->GetComponent<Transform>();
			auto ray = this->GetGameObject()->GetComponent<RayCollider>();
			if (ray->GetHit().hitGameObject != nullptr)
			{
				Vector target = ray->GetHit().hitGameObject->GetComponent<Transform>()->GetWorldPosition();
				Vector player = this->GetGameObject()->GetComponent<Transform>()->GetWorldPosition();

				if (ray->GetHit().hitDist < 2.f && ray->GetHit().hitGameObject->GetTag() == TEXT("Obstacle"))
				{
					return;
				}
			}

			transform->LookAt(transform->GetWorldPosition() + inputAxis);			
			transform->SetPosition(transform->GetWorldPosition() + VecotrMultFloat(inputAxis, this->moveSpeed * m), Space::WORLD);

		}

		void PlayerController::Rotate(Vector forward, Vector inputAxis)
		{

			if (Vector3Dot(Vector::UnitY, Vector3Cross(inputAxis, forward)) > 0.f) // 오른쪽회전 필요
			{
				this->GetGameObject()->GetComponent<Transform>()->AddRotation({ 0.f,-this->rotateDegree,0.f });
			}
			else
			{
				this->GetGameObject()->GetComponent<Transform>()->AddRotation({ 0.f,this->rotateDegree,0.f });
			}
			//this->MoveLookDir(0.1f, inputAxis);
		}

		Vector PlayerController::CalculateRotationVector(bool up, bool down, bool left, bool right)
		{
			Vector result = Vector::Zero;
			if (up == true)
				result -= dirUp;
			if (down == true)
				result -= dirDown;
			if (left == true)
				result -= dirLeft;
			if (right == true)
				result -= dirRight;

			return result == Vector::Zero ? Vector::Zero : Vector3Normalize(result);
		}
	}
}