#include "pch.h"
#include "Object.h"

uint32 GameEngineSpace::ObjectIDGenerator::nextID = 0;

GameEngineSpace::Object::Object() :
	isEnable(true)
	, objectID(GameEngineSpace::ObjectIDGenerator::IDGenerate())
{
}

GameEngineSpace::Object::~Object()
{
}

void GameEngineSpace::Object::Destroy()
{
}

void GameEngineSpace::Object::DontDestroyOnLoad(std::shared_ptr<Object> targetObject)
{
}

void GameEngineSpace::Object::Awake()
{
}

void GameEngineSpace::Object::Start()
{
}

void GameEngineSpace::Object::PreUpdate(float tick)
{
}

void GameEngineSpace::Object::Update(float tick)
{
}

void GameEngineSpace::Object::LateUpdate(float tick)
{
}

void GameEngineSpace::Object::OnEnable()
{
}

void GameEngineSpace::Object::OnDisable()
{
}

void GameEngineSpace::Object::OnDestroy()
{
}
