#include "pch.h"
#include "BasicCameraController.h"

#include "Timer.h"
#include "InputManager.h"

#include "GameObject.h"
#include "Camera.h"
#include "Transform.h"
#include "UIManager.h"

namespace GameEngineSpace
{

	BasicCameraController::BasicCameraController(std::weak_ptr<GameObject> gameObj)
		: ComponentBase(gameObj, ComponentType::MONOBEHAVIOR), moveSpeed(10.0f)
	{
		transform = GetGameObject()->GetComponent<Transform>();
	}

	BasicCameraController::~BasicCameraController()
	{
	}

	void BasicCameraController::Release()
	{
		camera.reset();
		__super::Release();
	}

	void BasicCameraController::Awake()
	{
		// ī�޶� ���
		camera = GetGameObject()->GetComponent<Camera>();
		/*UIManager::GetInstance()->CreateUIText("CameraUpGE", "CameraLook : ", Vector{ 1.0f, 1.0f, 1.0f }, Vector{ 100.f, 200.f, 1.0f }, 0.f, Vector{ 1.f, 1.f, 1.f });
		UIManager::GetInstance()->CreateUIText("CameraLookGE", "CameraLook : ", Vector{1.0f, 1.0f, 1.0f}, Vector{100.f, 250.f, 1.0f}, 0.f, Vector{1.f, 1.f, 1.f});
		UIManager::GetInstance()->CreateUIText("CameraRightGE", "CameraLook : ", Vector{ 1.0f, 1.0f, 1.0f }, Vector{ 100.f, 300.f, 1.0f }, 0.f, Vector{ 1.f, 1.f, 1.f });*/

	}

	void BasicCameraController::PreUpdate(float tick)
	{
		/*Vector nowUp = transform->GetUp();
		std::string str = "CameraUp : ";
		str += "x : " + std::to_string(nowUp.x) + " y : " + std::to_string(nowUp.y) + " z : " + std::to_string(nowUp.z);
		UIManager::GetInstance()->SetTextUIText("CameraUpGE", str);

		// ������ �������� ���ô�..
		Vector nowLook = transform->GetLook();
		str = "CameraLook : ";
		str += "x : " + std::to_string(nowLook.x) + " y : " + std::to_string(nowLook.y) + " z : " + std::to_string(nowLook.z);
		UIManager::GetInstance()->SetTextUIText("CameraLookGE", str);

		Vector nowRight = transform->GetRight();
		str = "CameraRight : ";
		str += "x : " + std::to_string(nowRight.x) + " y : " + std::to_string(nowRight.y) + " z : " + std::to_string(nowRight.z);
		UIManager::GetInstance()->SetTextUIText("CameraRightGE", str);*/

	}

	void BasicCameraController::LateUpdate(float tick)
	{
		Vector rot = transform->GetWorldRotation();

		float delta = tick;

		camera->UpdateViewMatrix();

		if (InputManager::GetInstance()->GetInputState('W', KeyState::STAY)) // ������
		{
			// ���۷��� Ÿ�� ���길 �����ؼ� ���͸� ������ �̾�����Ѵ�..!
			Vector look = transform->GetLook();
			transform->SetPosition(transform->GetWorldPosition() + (look * moveSpeed * delta));
		}

		if (InputManager::GetInstance()->GetInputState('S', KeyState::STAY)) // s
		{
			Vector look = transform->GetLook();	
			transform->SetPosition(transform->GetWorldPosition() - (look * moveSpeed * delta));
		}

		if (InputManager::GetInstance()->GetInputState('A', KeyState::STAY)) // a
		{
			Vector right = transform->GetRight();	
			transform->SetPosition(transform->GetWorldPosition() - (right * moveSpeed * delta));
		}

		if (InputManager::GetInstance()->GetInputState('D', KeyState::STAY)) // d
		{
			Vector right = transform->GetRight();	
			transform->SetPosition(transform->GetWorldPosition() + (right * moveSpeed * delta));
		}

		if (InputManager::GetInstance()->GetInputState('Q', KeyState::STAY)) // q
		{
			Vector up = transform->GetUp();
			transform->SetPosition(transform->GetWorldPosition() - (up * moveSpeed * delta));
		}

		if (InputManager::GetInstance()->GetInputState('E', KeyState::STAY)) // e
		{
			Vector up = transform->GetUp();
			transform->SetPosition(transform->GetWorldPosition() + (up * moveSpeed * delta));
		}

		Vector currentMousePos = InputManager::GetInstance()->GetMousePos();

		if(InputManager::GetInstance()->GetInputState(VK_RBUTTON, KeyState::STAY))
		{
			Vector distPos = currentMousePos - prevMousePos;
			Vector deltaMouseMove = 0.1f * distPos;

			// World Right Up �������� ȸ��/
			Vector worldXY = {deltaMouseMove.y, deltaMouseMove.x, 0.f};

			// ���� ������ ȸ��.
			transform->SetRotation(rot + worldXY, Space::WORLD);
		}

		prevMousePos = currentMousePos;
	}
}
