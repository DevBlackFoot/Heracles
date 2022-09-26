//#include "pch.h"
//#include "WindowClass.h"
//
//#include "../GameEngine/GameEngineMk2.h"
//
//#include "TestScene.h"
#include "pch.h"
#include "GameManager.h"


int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	Heracles::GameManager gameManager;
	gameManager.Init(hInstance);

	gameManager.Test();

	gameManager.Run();

	gameManager.Shutdown();
	return 0;
}

