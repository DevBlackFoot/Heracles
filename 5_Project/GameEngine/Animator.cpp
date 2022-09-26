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
		// ���� ĳ���մϴ�. �� ���� ��Ų ������Ʈ�� �ִ��� Ȯ���մϴ�.
		assert(GetGameObject()->GetComponent<SkinnedMeshRenderer>() != nullptr);

		// �ִٸ� ���� ������ �ݴϴ�.
		prefab = GetGameObject()->GetComponent<SkinnedMeshRenderer>()->GetPrefab();
		model = GetGameObject()->GetComponent<SkinnedMeshRenderer>()->GetModel();
	}

	void Animator::AddAnimation(const std::string& _path, const std::string& _animName, float animationSpeed, bool isLoop)
	{
		// �̹� �ִ� �ִϸ��̼����� �˻��մϴ�
		for (auto animation : animationClips)
		{
			if (animation.name == _animName)
			{
				animation.isLoop = isLoop;
				animation.animationSpeed = animationSpeed;

				return;
			}
		}

		// Ŭ���� ����� �ȵǾ��ִµ� ���� ���� �ִ� �ִϸ��̼��̶�� ����մϴ�.
		if (model->IsAlreadyHaveAnimation(_animName) == true)
		{
			animationClips.emplace_back(AnimationClip{ _animName, isLoop, animationSpeed });

			return;
		}

		// ���� �̸��� �����ͼ� �ش� �𵨿� �ִϸ��̼��� �߰����ݴϴ�.
		Factory* factory = GraphicsManager::GetInstance()->GetRenderer()->CreateFactory();
		ResourceManager* resourceManger = GraphicsManager::GetInstance()->GetRenderer()->GetResourceManager();

		// ���� �̸��� ���ӿ�����Ʈ�� �̸��� ���� �ʴ�. => �� �̸��� �޾ƿ��� ���� �� �ִ�...
		std::string modelName = model->GetName();

		factory->CreateAnimationFromASEFile(modelName, _path, _animName);

		// �ִϸ��̼��� �߰����ݴϴ�.
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
