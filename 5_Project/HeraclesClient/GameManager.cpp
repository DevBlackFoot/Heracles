#include "pch.h"
#include "GameManager.h"
#include "GameEngineTestScene.h"
#include "InputManager.h"
#include "Actor.h"

#include "GameState.h"

#include "DemoScene.h"
#include "TestScene.h"
#include "MainMenuScene.h"
#include "StartScene.h"
#include "FirstScene.h"
#include "StartCutScene.h"
#include "MapA.h"
#include "MapB.h"
#include "MapC.h"
#include "MapD.h"
#include "MapE.h"
#include "MapF.h"
#include "MapG.h"
#include "MapH.h"
#include "MapI.h"
#include "BeforeBossMap.h"
#include "BossMap.h"
#include "StartCurseMap.h"
#include "UpgradeMap.h"
#include "EndingScene.h"

#include "UITestScene.h"

Heracles::GameManager::GameManager()
{

}

Heracles::GameManager::~GameManager()
{
}

void Heracles::GameManager::Init(HINSTANCE hInstance)
{
	this->gameEngine = GameEngineSpace::GameEngineMk2::GetEngineInstnace();
	this->gameEngine->Init(TEXT("Heracles"), hInstance, 1920, 1080);

	this->sceneManager = GameEngineSpace::SceneManager::GetInstance();
	Heracles::GameState::GetInstance()->Init();
}

void Heracles::GameManager::Run()
{
	this->gameEngine->Tick();
}

void Heracles::GameManager::Shutdown()
{
	this->gameEngine->Release();
}

void Heracles::GameManager::Test()
{
	std::shared_ptr<Heracles::MainMenuScene> mainmenu = std::make_shared<Heracles::MainMenuScene>(TEXT("MainMenu"));
	this->sceneManager->AddGameScene(mainmenu);

	std::shared_ptr<StartScene> startScene = std::make_shared<StartScene>(TEXT("StartScene"));
	this->sceneManager->AddGameScene(startScene);

	std::shared_ptr<StartCutScene> startCutScene = std::make_shared<StartCutScene>(TEXT("StartCutScene"));
	this->sceneManager->AddGameScene(startCutScene);

	std::shared_ptr<MapA> mapA = std::make_shared<MapA>(TEXT("MapA"));
	this->sceneManager->AddGameScene(mapA);
	
	std::shared_ptr<MapB> mapB = std::make_shared<MapB>(TEXT("MapB"));
	this->sceneManager->AddGameScene(mapB);

	std::shared_ptr<MapC> mapC = std::make_shared<MapC>(TEXT("MapC"));
	this->sceneManager->AddGameScene(mapC);

	std::shared_ptr<MapE> mapE = std::make_shared<MapE>(TEXT("MapE"));
	this->sceneManager->AddGameScene(mapE);

	std::shared_ptr<MapF> mapF = std::make_shared<MapF>(TEXT("MapF"));
	this->sceneManager->AddGameScene(mapF);

	std::shared_ptr<MapI> mapI = std::make_shared<MapI>(TEXT("MapI"));
	this->sceneManager->AddGameScene(mapI);

	std::shared_ptr<BeforeBossMap> beforeboss = std::make_shared<BeforeBossMap>(TEXT("BeforeBossMap"));
	this->sceneManager->AddGameScene(beforeboss);

	std::shared_ptr<BossMap> bossmap = std::make_shared<BossMap>(TEXT("BossMap"));
	this->sceneManager->AddGameScene(bossmap);

	std::shared_ptr<StartCurseMap> startCurse = std::make_shared<StartCurseMap>(TEXT("StartCurseMap"));
	this->sceneManager->AddGameScene(startCurse);

	std::shared_ptr<UpgradeMap> upgrade = std::make_shared<UpgradeMap>(TEXT("UpgradeMap"));
	this->sceneManager->AddGameScene(upgrade);

	std::shared_ptr<Heracles::TestScene> test = std::make_shared<Heracles::TestScene>(TEXT("Test"));
	this->sceneManager->AddGameScene(test);

	std::shared_ptr<GameEngineTestScene> testScene = std::make_shared<GameEngineTestScene>(TEXT("TestScene"));
	this->sceneManager->AddGameScene(testScene);

	std::shared_ptr<UITestScene> uitestScene = std::make_shared<Heracles::UITestScene>(TEXT("UITestScene"));
	this->sceneManager->AddGameScene(uitestScene);

	std::shared_ptr<EndingScene> endScene = std::make_shared<EndingScene>(TEXT("EndingScene"));
	this->sceneManager->AddGameScene(endScene);

	std::shared_ptr<DemoScene> demo = std::make_shared<DemoScene>(TEXT("Demo"));
	this->sceneManager->AddGameScene(demo);
	//this->sceneManager->LoadScene(TEXT("MapI"));
	//this->sceneManager->LoadScene(TEXT("UITestScene"));
	//this->sceneManager->LoadScene(TEXT("BeforeBossMap"));
	this->sceneManager->LoadScene(TEXT("MainMenu"));

	//this->sceneManager->LoadScene(TEXT("EndingScene"));
}