#include "pch.h"
#include "WindowClass.h"


static const int SCREEN_WIDTH = 1920;
static const int SCREEN_HEIGHT = 1080;


WindowClass::WindowClass()
{
}

WindowClass::WindowClass(const WindowClass&)
{
}

WindowClass::~WindowClass()
{
}

bool WindowClass::Initialize()
{
	int screenWidth, screenHeight;

	screenWidth = 0;
	screenHeight = 0;

	InitializeWindows(screenWidth, screenHeight);

	//this->gameEngine = new GameEngine;
	//this->gameEngine->Initialize(m_hwnd);

	//this->gameManager = new GameManager(gameEngine);
	//this->gameManager->Initialize();

	return true;
}

void WindowClass::Shutdown()
{
	ShutdownWindows();
	return;
}

void WindowClass::Run()
{
	MSG msg;

	ZeroMemory(&msg, sizeof(MSG));

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			break;
		}
		else
		{
			if (!Frame())
			{
				break;
			}
		}
	}
	return;
}

LRESULT WindowClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam)
{
	switch (umsg)
	{

	case WM_KEYDOWN:
	{
		return 0;
	}

	case WM_KEYUP:
	{
		return 0;
	}

	default:
	{
		return DefWindowProc(hwnd, umsg, wParam, lParam);
	}
	}
}

bool WindowClass::Frame()
{
	//client loop
	//gameEngine->Frame();
	return true;
}

void WindowClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	ApplicationHandle = this;

	hinstance = GetModuleHandle(NULL);
	applicationName = L"GraphicsEngine";

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (false)
	{
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
		posX = posY = 0;
	}
	else
	{
		screenWidth = SCREEN_WIDTH;
		screenHeight = SCREEN_HEIGHT;

		// 이거 사용 시 화면 해상도에 따라 변동폭 존재..
		//posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		//posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	hwnd = CreateWindowEx(WS_EX_APPWINDOW, applicationName, applicationName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, SCREEN_WIDTH, SCREEN_HEIGHT, nullptr, nullptr, hinstance, nullptr);

	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	return;
}

void WindowClass::ShutdownWindows()
{
	if (false)
	{
		ChangeDisplaySettings(nullptr, 0);
	}

	DestroyWindow(hwnd); 
	hwnd = nullptr;

	UnregisterClass(applicationName, hinstance);
	hinstance = nullptr;

	ApplicationHandle = nullptr;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	default:
	{
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}
