#pragma once

#include <vector>
#include <memory>
#include <random>
#include <chrono>
// 게임 오브젝트와 컴포넌트
#include "../GameEngine/GameObject.h"
#include "../GameEngine/Camera.h"
#include "../GameEngine/BasicCameraController.h"
#include "../GameEngine/Transform.h"
#include "../GameEngine/MeshRenderer.h"

// 매니저
#include "../GameEngine/WindowManager.h"
#include "../GameEngine/SceneManager.h"
#include "../GameEngine/SceneBase.h"
#include "../GameEngine/InputManager.h"
#include "../GameEngine/Timer.h"

// 게임 엔진을 편하게 쓰기 위한..
using namespace GameEngineSpace;

// 라이브러리 추가.
// 32일 때
#ifdef x64
#ifdef _DEBUG
	#pragma comment(lib, "../x64/Debug/GameEngine.lib")
#else
	#pragma comment(lib, "../x64/Release/GameEngine.lib")
#endif
// 64일 때.
#else
#ifdef _DEBUG
	#pragma comment(lib, "../Debug/GameEngine.lib")
#else
	#pragma comment(lib, "../Release/GameEngine.lib")
#endif
#endif

