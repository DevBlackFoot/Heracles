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
		// 순서 약간 중요..
		// 모든 매니저를 Init해줍니다.
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

		// 모든 매니저의 Init이 끝난뒤.. 윈도우를 띄워줍니다.
		WindowManager::GetInstance()->StartWindow();

		// UITest
		UIManager::GetInstance()->CreateCanvasAll();
	}

	INT GameEngineMk2::Tick()
	{
		// 매 틱 돌때마다..
			// 윈도우 매니저 내부의 update를 돌립니다
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
