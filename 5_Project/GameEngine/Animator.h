#pragma once
#include "ComponentBase.h"
#include "../GraphicsEngine/GraphicsEngineFramework.h"

namespace GameEngineSpace
{

	/**
	 * \brief 하나의 게임오브젝트가 가지고 있는 모든 애니메이션 클립을 관리하는 애니메이터 컴포넌트
	 *
	 * 애니메이션의 전환이 가능하도록 만들어야한다. 이후에 시간이 된다면 애니메이션 블렌딩을 연구한다.
	 *
	 * Add Clip을 하면 => 모델의 ASEAnimation을 불러온다..
	 *
	 * 애니메이터는 애니메이션의 상태를 전환하도록 하는 역할
	 *	=> 애니메이터 내부에는 여러 애니메이션 클립이 있다, 특정 모델에 대한 정보(키)도 가지고 있어야 한다.
	 * 애니메이션 클립은 특정 모델에 특정 애니메이션을 추가하는 역할
	 *	=> 애니메이션 업데이트에 해당하는 내용은 모두 그래픽스 내부에 있기 때문에 게임 엔진에서는 세팅만 해준다.
	 * 2022.08.04(목) Dev.BlackFoot
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
		// 현재 게임 오브젝트가 가지고 있는 모든 애니메이션 클립
			// 이름 정보로 세팅하는 것이기 때문에 애니메이션의 이름 정보를 가지고 있습니다.
			// 중복을 허용하지 않기 위해서 set으로 지정해줍니다.
		std::vector<AnimationClip> animationClips;

		// 사실상 모델 정보만 가지고 있으면 세팅은 자유자재로 가능하다..

		// 내부에서 사용 할 모델 정보
		GraphicsEngineSpace::PrefabBase* prefab;
		GraphicsEngineSpace::ModelBase* model;

		// 여기서 애니메이션 시간을 가지고 있어야한다.
		float totalAnimationTime;

		// 현재 애니메이션
		AnimationClip* currentAnimationClip;

		// 애니메이션 재생 확인
		bool isPlaying;

	public:
		// 애니메이터 초기화 -> 모델을 캐싱해줍니다.
		Animator(std::weak_ptr<GameObject> gameObj);
		virtual ~Animator() = default;

		// 애니메이션 추가
			// 모델의 이름이 핵심입니다.
			// 그리고 그 모델의 이름은 애니메이터 컴포넌트를 가지고 있는 게임 오브젝트의 이름과 동일합니다.
		void AddAnimation(const std::string& _path, const std::string& _animName, float animationSpeed = 1.0f, bool isLoop = false);

		void Update(float tick) override;

		void Pause();
		void Resume();
		void Play();
		void Play(const std::string& _animName);
		void Stop();

		// 재생할 애니메이션을 세팅해줍니다.
		void SetLoop(const std::string& _animName, bool isLoop);
		void SetAnimationSpeed(const std::string& _animName, float animationSpeed);
		bool IsLoop();
		bool IsPlaying();

		float GetTotalAnimationTime();
	};

}
