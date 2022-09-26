#pragma once
#include "pch.h"
#include "GameEngineMk2.h"

namespace Heracles
{
	using namespace GameEngineSpace;


	class GameManager
	{
	public:
		GameManager();
		~GameManager();

		void Init(HINSTANCE hInstance);

		void Run();
		void Shutdown();

		void Test();


	private:
		std::shared_ptr<GameEngineMk2> gameEngine;
		std::shared_ptr<SceneManager> sceneManager;
		
	};

}

