#include "pch.h"
#include "SceneController.h"

#include "InputManager.h"
#include "SceneManager.h"

#include "PlayerStateMachine.h"
#include "GameState.h"

#include "GameObjectTemplate.h"
#include "SceneManager.h"



namespace Heracles
{
	using namespace InGameObject;

	SceneController::SceneController(std::weak_ptr<GameObject> obj)
		:ComponentBase(obj,ComponentType::MONOBEHAVIOR)
	{
	}

	void SceneController::Start()
	{
		if (rewardBox != nullptr)
		{
			rewardBox->SetGameObjectEnable(false);
		}
	}

	void SceneController::PreUpdate(float tick)
	{
		GameState::GetInstance()->totalPlayTime += tick;
	}

	void SceneController::Update(float tick)
	{
		if (GameState::GetInstance()->totalMonsterNumInMap == 0)
		{
			GameState::GetInstance()->mapCleared = true;
		}
		if (GameState::GetInstance()->mapCleared)
		{
			if (rewardBox != nullptr)
			{
				rewardBox->SetGameObjectEnable(true);
			}
		}
	}

	void SceneController::LateUpdate(float tick)
	{
		//다음 웨이브 세팅
		if (GameState::GetInstance()->currMonsterNum == 0 && GameState::GetInstance()->mapCleared == false)
		{
			this->SetNextWave();
		}
	}

	void SceneController::CreateMonsterSpawner(SceneBase* scene, std::vector<std::pair<int, Vector>>& v)
	{
		std::shared_ptr<MonsterSpawner> ms = std::make_shared<MonsterSpawner>();
		for (auto& mon : v)
		{
			switch (mon.first)
			{
				case 1:
				{
					auto monster = CreateMonster(scene, mon.second);
					monster->SetGameObjectEnable(false);
					ms->SetMonsterToList(monster);
					break;
				}
				case 2:
				{
					auto monster = CreateBigMonster(scene, mon.second);
					monster->SetGameObjectEnable(false);
					ms->SetMonsterToList(monster);
					break;
				}
				case 3:
				{
					break;
				}
			
			}
		}
		this->spawner.push(ms);
	}

	void SceneController::SetNextWave()
	{
		if (this->spawner.size() == 0)
		{
			return;
		}
		auto ms = this->spawner.front();
		ms->SpawnMonster();
		this->spawner.pop();
	}

	void MonsterSpawner::SetMonsterToList(std::shared_ptr<GameObject> monster)
	{
		this->monsterList.emplace_back(monster);
	}

	void MonsterSpawner::SpawnMonster()
	{
		for (auto monster : this->monsterList)
		{
			GameState::GetInstance()->currMonsterNum += 1;
			monster->SetGameObjectEnable(true);
		}
		return;
	}

	void SceneController::SelectNextMap()
	{
		GameState::GetInstance()->clearedMapNum += 1;

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dist(0, GameState::GetInstance()->mapMonster.size()-1);

		int clearNum = GameState::GetInstance()->clearedMapNum;

		if (GameState::GetInstance()->nextMap == TEXT("BeforeBossMap"))
		{
			GameState::GetInstance()->nextMap = TEXT("BossMap");
			return;
		}

		if (clearNum < 3)
		{
			int temp = dist(gen);
			GameState::GetInstance()->nextMap = GameState::GetInstance()->mapMonster[temp];
			GameState::GetInstance()->rewardIdx1 = temp % 3;
			temp = dist(gen);
			GameState::GetInstance()->nextMap2 = GameState::GetInstance()->mapMonster[temp];
			GameState::GetInstance()->rewardIdx2 = temp % 3;
		}
		else if (clearNum == 3)
		{
			GameState::GetInstance()->nextMap = TEXT("UpgradeMap");
			GameState::GetInstance()->nextMap2 = TEXT("UpgradeMap");
		}
		else if (clearNum > 3 && clearNum < 6)
		{
			int temp = dist(gen);
			GameState::GetInstance()->nextMap = GameState::GetInstance()->mapMonster[temp];
			GameState::GetInstance()->rewardIdx1 = temp % 3;
			temp = dist(gen);
			GameState::GetInstance()->nextMap2 = GameState::GetInstance()->mapMonster[temp];
			GameState::GetInstance()->rewardIdx2 = temp % 3;
		}
		else if(clearNum == 6)
		{
			GameState::GetInstance()->nextMap = TEXT("BeforeBossMap");
			GameState::GetInstance()->nextMap2 = TEXT("BeforeBossMap");
		}
		else
		{
			GameState::GetInstance()->nextMap = TEXT("BeforeBossMap");
			GameState::GetInstance()->nextMap2 = TEXT("BeforeBossMap");
		}
	}

}