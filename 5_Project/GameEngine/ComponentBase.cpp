#include "pch.h"
#include "ComponentBase.h"

GameEngineSpace::ComponentBase::ComponentBase(std::weak_ptr<GameObject> _gameObject, ComponentType _componentType) :
	Object()
	, gameObject(_gameObject)
	, componentType(_componentType)
	, isStarted(false)
{

}

GameEngineSpace::ComponentBase::~ComponentBase()
{

}

void GameEngineSpace::ComponentBase::Release()
{
	if (gameObject.expired() != true)
	{
		gameObject.lock().reset();
	}
}
