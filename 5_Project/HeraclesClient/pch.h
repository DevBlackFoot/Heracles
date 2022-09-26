#pragma once

#include <vector>
#include <memory>
#include <random>
#include <chrono>
// ���� ������Ʈ�� ������Ʈ
#include "../GameEngine/GameObject.h"
#include "../GameEngine/Camera.h"
#include "../GameEngine/BasicCameraController.h"
#include "../GameEngine/Transform.h"
#include "../GameEngine/MeshRenderer.h"

// �Ŵ���
#include "../GameEngine/WindowManager.h"
#include "../GameEngine/SceneManager.h"
#include "../GameEngine/SceneBase.h"
#include "../GameEngine/InputManager.h"
#include "../GameEngine/Timer.h"

// ���� ������ ���ϰ� ���� ����..
using namespace GameEngineSpace;

// ���̺귯�� �߰�.
// 32�� ��
#ifdef x64
#ifdef _DEBUG
	#pragma comment(lib, "../x64/Debug/GameEngine.lib")
#else
	#pragma comment(lib, "../x64/Release/GameEngine.lib")
#endif
// 64�� ��.
#else
#ifdef _DEBUG
	#pragma comment(lib, "../Debug/GameEngine.lib")
#else
	#pragma comment(lib, "../Release/GameEngine.lib")
#endif
#endif

