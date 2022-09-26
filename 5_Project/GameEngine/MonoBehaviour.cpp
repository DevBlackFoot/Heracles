#include "pch.h"
#include "MonoBehaviour.h"

namespace GameEngineSpace
{

	MonoBehaviour::MonoBehaviour(std::weak_ptr<GameObject> gameObj)
		: ComponentBase(gameObj)
	{
	}

	MonoBehaviour::~MonoBehaviour()
	{
	}
}
