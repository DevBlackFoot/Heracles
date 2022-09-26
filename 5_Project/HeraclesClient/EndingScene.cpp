#include "pch.h"
#include "EndingScene.h"

#include "UIManager.h"
#include "GameState.h"
#include "Timer.h"

Heracles::EndingScene::EndingScene(tstring sceneName)
	: SceneBase(sceneName)
{
}

Heracles::EndingScene::~EndingScene()
{
}

void Heracles::EndingScene::BuildScene()
{
	auto endCanvas = UIManager::GetInstance()->GetCanvas("EndingCanvas");

	endCanvas->SetEnable(true);

	auto text = endCanvas->GetTextUI("EndText");

	// 총 시간 -> 타이머에서 받아온다.
	float totalTime =  Timer::GetInstance()->TotalTime();


	int totalMin = static_cast<int>(totalTime) / 60;
	int totalSec = static_cast<int>(totalTime) % 60;

	std::string timeStr;
	// 자릿수가 하나면 앞에 0 추가
	if(totalMin / 10 == 0)
		timeStr += "0" + std::to_string(totalMin);
	else
		timeStr += std::to_string(totalMin);

	timeStr += " : ";

	if (totalSec / 10 == 0)
		timeStr += "0" + std::to_string(totalSec);
	else
		timeStr += std::to_string(totalSec);

	text->SetText(timeStr);

	endCanvas->SetEnable(true);
}
