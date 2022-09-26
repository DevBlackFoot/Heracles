#include "pch.h"
#include "GameEngineMk2.h"

#include "InputManager.h"
#include "Timer.h"
#include "WindowManager.h"
#include "GraphicsManager.h"
#include "SceneManager.h"
#include "UIManager.h"
#include "JsonDoc.h"

namespace GameEngineSpace
{
	std::shared_ptr<GameEngineMk2> GameEngineMk2::gameEngineInstance = nullptr;

	void GameEngineMk2::Init(tstring gameTitle, HINSTANCE hInst, int screenWidth,
	                         int screenHeight)
	{
		// ���� �ణ �߿�..
		// ��� �Ŵ����� Init���ݴϴ�.
		WindowManager::GetInstance()->InitWindow(gameTitle, hInst, screenWidth, screenHeight);
		Timer::GetInstance()->Init();
		InputManager::GetInstance();
		GraphicsManager::GetInstance()->Init(WindowManager::GetInstance()->GetHWND(), screenWidth, screenHeight);
		SceneManager::GetInstance()->Init();
		UIManager::GetInstance()->Init();

		//std::string st = "Contents/gamerule.json";
		//std::string key = "InitChapterID";

		//JsonDoc* jsondoc = new JsonDoc();
		//JsonDoc* jsondocChar = new JsonDoc();
		//bool failsucess = jsondoc->LoadJsonFromFile(st);
		//st = "Contents/CharacterTable.json";
		//failsucess = jsondocChar->LoadJsonFromFile(st);

		//int a = jsondoc->GetValue<int>(key);	

		//key = "FaceOffset";
		//auto x = jsondocChar->GetValue2DArray<int>(key, 101, true);

		// ��� �Ŵ����� Init�� ������.. �����츦 ����ݴϴ�.
		WindowManager::GetInstance()->StartWindow();

		// UITest
		UIManager::GetInstance()->CreateCanvasAll();
	}

	INT GameEngineMk2::Tick()
	{
		// �� ƽ ��������..
			// ������ �Ŵ��� ������ update�� �����ϴ�
		return WindowManager::GetInstance()->Update();
	}

	void GameEngineMk2::Release()
	{
		Timer::GetInstance()->Release();
		SceneManager::GetInstance()->Release();
		GraphicsManager::GetInstance()->Release();
		WindowManager::GetInstance()->Release();
	}
}
