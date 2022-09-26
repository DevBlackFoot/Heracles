#pragma once
#include "ComponentBase.h"
#include "../GraphicsEngine/GraphicsEngineFramework.h"

namespace GameEngineSpace
{

	/**
	 * \brief �ϳ��� ���ӿ�����Ʈ�� ������ �ִ� ��� �ִϸ��̼� Ŭ���� �����ϴ� �ִϸ����� ������Ʈ
	 *
	 * �ִϸ��̼��� ��ȯ�� �����ϵ��� �������Ѵ�. ���Ŀ� �ð��� �ȴٸ� �ִϸ��̼� ������ �����Ѵ�.
	 *
	 * Add Clip�� �ϸ� => ���� ASEAnimation�� �ҷ��´�..
	 *
	 * �ִϸ����ʹ� �ִϸ��̼��� ���¸� ��ȯ�ϵ��� �ϴ� ����
	 *	=> �ִϸ����� ���ο��� ���� �ִϸ��̼� Ŭ���� �ִ�, Ư�� �𵨿� ���� ����(Ű)�� ������ �־�� �Ѵ�.
	 * �ִϸ��̼� Ŭ���� Ư�� �𵨿� Ư�� �ִϸ��̼��� �߰��ϴ� ����
	 *	=> �ִϸ��̼� ������Ʈ�� �ش��ϴ� ������ ��� �׷��Ƚ� ���ο� �ֱ� ������ ���� ���������� ���ø� ���ش�.
	 * 2022.08.04(��) Dev.BlackFoot
	 */
	class Animator : public ComponentBase
	{
		struct AnimationClip
		{
			std::string name;
			bool isLoop;
			float animationSpeed;
		};

	private:
		// ���� ���� ������Ʈ�� ������ �ִ� ��� �ִϸ��̼� Ŭ��
			// �̸� ������ �����ϴ� ���̱� ������ �ִϸ��̼��� �̸� ������ ������ �ֽ��ϴ�.
			// �ߺ��� ������� �ʱ� ���ؼ� set���� �������ݴϴ�.
		std::vector<AnimationClip> animationClips;

		// ��ǻ� �� ������ ������ ������ ������ ��������� �����ϴ�..

		// ���ο��� ��� �� �� ����
		GraphicsEngineSpace::PrefabBase* prefab;
		GraphicsEngineSpace::ModelBase* model;

		// ���⼭ �ִϸ��̼� �ð��� ������ �־���Ѵ�.
		float totalAnimationTime;

		// ���� �ִϸ��̼�
		AnimationClip* currentAnimationClip;

		// �ִϸ��̼� ��� Ȯ��
		bool isPlaying;

	public:
		// �ִϸ����� �ʱ�ȭ -> ���� ĳ�����ݴϴ�.
		Animator(std::weak_ptr<GameObject> gameObj);
		virtual ~Animator() = default;

		// �ִϸ��̼� �߰�
			// ���� �̸��� �ٽ��Դϴ�.
			// �׸��� �� ���� �̸��� �ִϸ����� ������Ʈ�� ������ �ִ� ���� ������Ʈ�� �̸��� �����մϴ�.
		void AddAnimation(const std::string& _path, const std::string& _animName, float animationSpeed = 1.0f, bool isLoop = false);

		void Update(float tick) override;

		void Pause();
		void Resume();
		void Play();
		void Play(const std::string& _animName);
		void Stop();

		// ����� �ִϸ��̼��� �������ݴϴ�.
		void SetLoop(const std::string& _animName, bool isLoop);
		void SetAnimationSpeed(const std::string& _animName, float animationSpeed);
		bool IsLoop();
		bool IsPlaying();

		float GetTotalAnimationTime();
	};

}
