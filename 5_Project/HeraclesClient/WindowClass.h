#pragma once
#include <windows.h>

class WindowClass
{
private:
	//GameEngine* gameEngine;
	//GameManager* gameManager;

private:
	LPCWSTR applicationName;
	HINSTANCE hinstance;
	HWND hwnd;

public:
	WindowClass();
	WindowClass(const WindowClass&);
	~WindowClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static WindowClass* ApplicationHandle = nullptr;
