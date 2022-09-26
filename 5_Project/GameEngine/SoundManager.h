#pragma once

#include <windows.h>
#include "../Fmod/fmod.hpp"
#include <string>
#include <map>

#pragma comment(lib, "../Fmod/fmod_vc.lib")

namespace GameEngineSpace
{
	enum class PlayMode
	{
		BGM,
		Effect,
		PlayerWalk
	};

	class SoundManager
	{
	private:
		FMOD::System* system;
		FMOD::Channel* bgmChannel = 0;
		FMOD::Channel* effectChannel[10];

		FMOD::Channel* playerWalkChannel = 0;
		FMOD::Channel* bossWalkChannel = 0;

		FMOD_RESULT result;
		void* extradriverdata = 0;

		float delayTime = 1;
		float effectSoundVolume = 1.f;

		std::map<std::string, FMOD::Sound*> soundList;

		std::string currBgm;

		static std::shared_ptr<SoundManager> instance;

	private:

		// FMOD�� System�� Update�ϱ� ���ؼ�
		// ���������� �𸣳� �� ������ ������Ʈ�� �ϸ� ���ø�����Ʈ��
		// ������ �� ó�� ���尡 ������.
		float m_ElapsedTime;

	public:
		static std::shared_ptr<SoundManager> GetInstance();

		void PlayLoop();

		void PlayWalkSound(std::string name, bool play);

		void PlayBossWalkSound(std::string name, bool play);

		void Initialize();
		void LoadSoundFile(std::string name, std::string filePath, unsigned int mode);


		void Update();
		void Play(std::string name, PlayMode mode);
		void Delay(float delayTime, PlayMode mode);

		void Pause(PlayMode mode, bool isPause);

		void PlayBGM(std::string name);
		void PlayEffect(std::string name);

		void SetSoundSpeed(float speed, PlayMode mode);

		void SetSoundVoulume(float volume, PlayMode mode);
		void SetEffectSoundVoulume(float volume);

		void Finalize();
	};

}


