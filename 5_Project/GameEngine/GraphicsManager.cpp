#include "pch.h"
#include "GraphicsManager.h"
#include "WindowManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "UIManager.h"

namespace GameEngineSpace
{
	// 스태틱변수 선언
	std::shared_ptr<GraphicsManager> GraphicsManager::instance = nullptr;

	bool GraphicsManager::Init(HWND hWnd, uint32 screenWidth, uint32 screenHeight)
	{
		// 여기서 Dll을 로드해줍니다.
#ifdef x64
#ifdef _DEBUG
		graphicsDLL = LoadLibrary(_T("DLL/GraphicsEngine_Debug_x64.dll"));
#else
		graphicsDLL = LoadLibrary(_T("DLL/GraphicsEngine_Release_x64.dll"));
#endif
#else
#ifdef _DEBUG
		graphicsDLL = LoadLibrary(_T("DLL/GraphicsEngine_Debug_x86.dll"));
#else
		graphicsDLL = LoadLibrary(_T("DLL/GraphicsEngine_Release_x86.dll"));
#endif
#endif

		if (graphicsDLL == nullptr)
		{
			// TO DO
			// DLL 로드 실패
			OutputDebugString(_T("Load graphics engine dll failed.\n"));
			return false;
		}

		OutputDebugString(_T("Load graphics engine dll success.\n"));

		using GraphicsEngineSpace::GraphicsEngineConstructor;

		GraphicsEngineConstructor graphicsConstructor = (GraphicsEngineConstructor)GetProcAddress(graphicsDLL, "CreateGraphicsEngine");
		renderer.reset(graphicsConstructor());

		// 초기 리사이즈를 제대로 하기위한 클라이언트 정보 얻어오기.
		RECT rect;
		GetClientRect(hWnd, &rect);
		uint32 _width = rect.right - rect.left;
		uint32 _height = rect.bottom - rect.top;

		/*if (renderer->CreateDirectXEngine(hWnd, screenWidth, screenHeight) != true)
		{
			OutputDebugString(_T("Create DirectX dll failed.\n"));
			return false;
		}*/

		if (renderer->CreateDirectXEngine(hWnd, _width, _height) != true)
		{
			OutputDebugString(_T("Create DirectX dll failed.\n"));
			return false;
		}

		// 리사이즈 펑션을 정합니다..
		// 윈도우 매니저 불러오기.
		WindowManager::GetInstance()->SetOnResizeFunc([&](uint32 width, uint32 height)
			{
				renderer->OnResize(width, height);
				UIManager::GetInstance()->OnResize(width, height);
			}
		);

		WindowManager::GetInstance()->SetOnMouseMoveFunc([&](float x, float y)
			{
				InputManager::GetInstance()->SetMousePos(x, y);
			}
		);

		// 팩토리 만들기

		Factory* factory = renderer->CreateFactory();



		// 라이트 정보
		dLight = factory->CreateDirectionalLight("DLight");
		float r = 0x4A;
		float g = 0x4D;
		float b = 0x96;

		dLight->color = { r / 255.0f, g / 255.0f, b / 255.0f };
		dLight->rotation.x += 45.0f;

		return true;
	}

	void GraphicsManager::Update()
	{
		/// 화면 갱신에 있어서 참조할 사항을 날려줍니다..

		dLight->rotation.y += 0.0001f;
	}

	void GraphicsManager::Release()
	{
		if (renderer != nullptr)
			renderer->Release();
		renderer.reset();

		if (graphicsDLL != nullptr)
			FreeLibrary(graphicsDLL);
	}

	std::shared_ptr<GraphicsManager> GraphicsManager::GetInstance()
	{
		if (instance == nullptr)
			instance = std::make_shared<GraphicsManager>();

		return instance;
	}
}