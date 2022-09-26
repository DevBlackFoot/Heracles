#include "pch.h"
#include "BehaviourTree.h"

#include "Timer.h"
#include <random>

namespace Heracles
{
	bool Selector::Run()
	{
		this->running = true;

		for (auto node : this->children)
		{
			if (node->Run())
			{
				return true;
			}
		}

		this->running = false;

		return false;
	}

	bool Sequencer::Run()
	{
		this->running = true;
		for (auto node : this->children)
		{
			if (!node->Run())
			{
				return false;
			}
		}
		this->running = false;
		return true;
	}

	bool Condition::Run()
	{
		if (this->nextNode == nullptr)
		{
			return true;
		}
		if (this->nextNode->running)
		{
			return true;
		}
		else
		{
			return false;
		}
	}


	RandomSelector::RandomSelector(int distRange, int weight)
		:distRange(distRange),weight(weight)
	{
	}

	std::shared_ptr<Node> RandomSelector::SelectAction()
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> distribution(1, this->distRange);
		for (auto node : children)
		{
			if (distribution(gen) % this->weight != 0)
			{
				this->selectedAction = node;
				return node;
			}
		}
		this->selectedAction = this->children.front();
		return this->children.front();
	}

	bool RandomSelector::Run()
	{
		if (this->running)
		{
			if (this->selectedAction->Run())
			{
				return true;
			}
			else
			{
				this->running = false;
				return false;
			}
		}
		else
		{
			this->running = true;
			this->SelectAction();
			return true;
		}
	}

	bool Action::Run()
	{
		if (this->timer > 0.f)
		{
			this->timer -= Timer::GetInstance()->DeltaTime();
			this->progressTime += Timer::GetInstance()->DeltaTime();
			return true; // 진행중
		}
		else
		{
			this->timer = 0.f;
			this->progressTime = 0.f;
			this->running = false;
			return false; // 끝
		}

	}


	bool Succeeder::Run()
	{
		if (this->GetChild() == nullptr)
		{
			return true;
		}
		else
		{
			this->GetChild()->Run();
			return true;
		}
	}

	bool Failer::Run()
	{
		if (this->GetChild() == nullptr)
		{
			return false;
		}
		else
		{
			this->GetChild()->Run();
			return false;
		}
	}

	bool Inverter::Run()
	{
		if (this->GetChild() == nullptr)
		{
			return true;
		}
		else
		{
			return !this->GetChild()->Run();
		}
	}

}
