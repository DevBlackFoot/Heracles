#include "pch.h"
#include "Animator.h"

#include "GameObject.h"
#include "SkinnedMeshRenderer.h"
#include "GraphicsManager.h"

namespace GameEngineSpace
{
	Animator::Animator(std::weak_ptr<GameObject> gameObj)
		: ComponentBase(gameObj, ComponentType::ANIMATOR)
		, animationClips({})
		, totalAnimationTime(0.0f)
		, currentAnimationClip(nullptr)
		, isPlaying(false)
	{
		// 모델을 캐싱합니다. 그 전에 스킨 컴포넌트가 있는지 확인합니다.
		assert(GetGameObject()->GetComponent<SkinnedMeshRenderer>() != nullptr);

		// 있다면 모델을 가져와 줍니다.
		prefab = GetGameObject()->GetComponent<SkinnedMeshRenderer>()->GetPrefab();
		model = GetGameObject()->GetComponent<SkinnedMeshRenderer>()->GetModel();
	}

	void Animator::AddAnimation(const std::string& _path, const std::string& _animName, float animationSpeed, bool isLoop)
	{
		// 이미 있는 애니메이션인지 검사합니다
		for (auto animation : animationClips)
		{
			if (animation.name == _animName)
			{
				animation.isLoop = isLoop;
				animation.animationSpeed = animationSpeed;

				return;
			}
		}

		// 클립에 등록은 안되어있는데 모델이 갖고 있는 애니메이션이라면 등록합니다.
		if (model->IsAlreadyHaveAnimation(_animName) == true)
		{
			animationClips.emplace_back(AnimationClip{ _animName, isLoop, animationSpeed });

			return;
		}

		// 모델의 이름을 가져와서 해당 모델에 애니메이션을 추가해줍니다.
		Factory* factory = GraphicsManager::GetInstance()->GetRenderer()->CreateFactory();
		ResourceManager* resourceManger = GraphicsManager::GetInstance()->GetRenderer()->GetResourceManager();

		// 모델의 이름과 게임오브젝트의 이름이 같지 않다. => 모델 이름을 받아오면 만들 수 있다...
		std::string modelName = model->GetName();

		factory->CreateAnimationFromASEFile(modelName, _path, _animName);

		// 애니메이션을 추가해줍니다.
		animationClips.emplace_back(AnimationClip{ _animName, isLoop, animationSpeed });
	}

	void Animator::Update(float tick)
	{
		if (currentAnimationClip == nullptr)
			return;

		if (isPlaying == true)
			totalAnimationTime += tick;
	}

	void Animator::Pause()
	{
		isPlaying = false;
	}

	void Animator::Resume()
	{
		isPlaying = true;
	}

	void Animator::Play()
	{
		if (currentAnimationClip == nullptr)
			return;

		if (prefab->SetAnimationKey(currentAnimationClip->name) == true)
		{
			isPlaying = true;
			totalAnimationTime = 0.0f;
		}

		auto& children = GetGameObject()->GetChildren();

		for (auto& iter : children)
		{
			auto animator = iter->GetComponent<Animator>();

			if (animator != nullptr)
				animator->Play(currentAnimationClip->name);
		}
	}

	void Animator::Play(const std::string& _animName)
	{
		for (auto& iter : animationClips)
		{
			if (iter.name == _animName)
				currentAnimationClip = &iter;
		}

		Play();
	}

	void Animator::Stop()
	{
		isPlaying = false;
		totalAnimationTime = 0.0f;
	}

	void Animator::SetLoop(const std::string& _animName, bool isLoop)
	{
		if (currentAnimationClip == nullptr)
			return;

		currentAnimationClip->isLoop = isLoop;
	}

	void Animator::SetAnimationSpeed(const std::string& _animName, float animationSpeed)
	{
		if (currentAnimationClip == nullptr)
			return;

		currentAnimationClip->animationSpeed = animationSpeed;
	}

	bool Animator::IsLoop()
	{
		if (currentAnimationClip == nullptr)
			return false;

		return currentAnimationClip->isLoop;
	}

	bool Animator::IsPlaying()
	{
		return isPlaying;
	}

	float Animator::GetTotalAnimationTime()
	{
		if (currentAnimationClip == nullptr)
			return 0.0f;

		return totalAnimationTime * currentAnimationClip->animationSpeed;
	}
}
