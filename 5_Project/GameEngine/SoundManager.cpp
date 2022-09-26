#include "pch.h"
#include "SoundManager.h"

#include "soundDefine.h"

namespace GameEngineSpace
{
	std::shared_ptr<SoundManager> SoundManager::instance = nullptr;

	std::shared_ptr<SoundManager> SoundManager::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = std::make_shared<SoundManager>();
			instance->Initialize();
		}
		return instance;
	}

	void SoundManager::Initialize()
	{
		//Common_Init(&extradriverdata);
		//HRESULT _result = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

		result = FMOD::System_Create(&system);
		ERRCHECK(result);

		result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);
		ERRCHECK(result);

		//AudioClip* _atk1 = ResourceManager::Instance()->GetAudioClip("Atk1");
		//result = system->createSound(_atk1->m_MetaFilePath.c_str(), FMOD_DEFAULT, 0, &sound_atk1);
		//ERRCHECK(result);

		m_ElapsedTime = 0;
	}

	void SoundManager::LoadSoundFile(std::string name, std::string filePath, unsigned int mode)
	{
		//FMOD_DEFAULT 0
		//FMOD_LOOP_OFF 1
		FMOD::Sound* _newSound = nullptr;
		result = system->createSound(filePath.c_str(), mode, 0, &_newSound);
		_newSound->setLoopCount(1000);
		ERRCHECK(result);

		soundList.insert({ name, _newSound });
	}

	void SoundManager::PlayLoop()
	{
		bool playing = false;
		this->bgmChannel->isPlaying(&playing);
		if (playing == false)
		{
			PlayBGM(currBgm);
		}
	}

	void SoundManager::Update()
	{
		PlayLoop();
	}

	void SoundManager::Play(std::string name, PlayMode mode)
	{
		switch (mode)
		{
		case PlayMode::BGM:
			PlayBGM(name);
			break;
		case PlayMode::Effect:
			PlayEffect(name);
			break;
		}
	}

	void SoundManager::Delay(float delayTime, PlayMode mode)
	{
		//switch (mode)
		//{
		//case PlayMode::BGM:
		//	bgmChannel->setDelay(1, 2);
		//	break;
		//case PlayMode::Effect:
		//	effectChannel->setDelay(1, (unsigned long)delayTime);
		//	break;
		//default:
		//	break;
		//}
	}

	void SoundManager::Pause(PlayMode mode, bool isPause)
	{
		switch (mode)
		{
		case PlayMode::BGM:
			bgmChannel->setPaused(isPause);
			break;
		case PlayMode::Effect:
			//effectchannel->setpaused(ispause);
			break;
		default:
			break;
		}
	}

	void SoundManager::PlayBGM(std::string name)
	{
		auto kvp = soundList.find(name);
		if (kvp != soundList.end())
		{
			bool isPlay = false;
			bgmChannel->isPlaying(&isPlay);
			if (isPlay)
			{
				bgmChannel->stop();
			}
			result = system->playSound(kvp->second, 0, false, &bgmChannel);
			ERRCHECK(result);
			currBgm = name;
		}

	}

	void SoundManager::PlayEffect(std::string name)
	{
		auto kvp = soundList.find(name);
		if (kvp != soundList.end())
		{
			bool playing = false;
			// 재생중이지 않은 배경음악 체널 탐색
			for (int i = 0; i < 10; i++)
			{
				effectChannel[i]->isPlaying(&playing);
				if (!playing)
				{
					auto result = system->playSound(kvp->second, 0, false, &effectChannel[i]);
					ERRCHECK(result);
					effectChannel[i]->setVolume(effectSoundVolume);
					break;
				}
			}
		}
	}

	void SoundManager::PlayWalkSound(std::string name, bool play)
	{
		auto kvp = soundList.find(name);
		if (kvp != soundList.end())
		{
			bool temp = false;
			this->playerWalkChannel->isPlaying(&temp);
			if (temp)
			{
				playerWalkChannel->setPaused(play);
			}
			else
			{
				system->playSound(kvp->second, 0, false, &playerWalkChannel);
			}
		}
	}

	void SoundManager::PlayBossWalkSound(std::string name, bool play)
	{
		auto kvp = soundList.find(name);
		if (kvp != soundList.end())
		{
			bool temp = false;
			this->bossWalkChannel->isPlaying(&temp);
			if (temp)
			{
				bossWalkChannel->setPaused(play);
			}
			else
			{
				system->playSound(kvp->second, 0, false, &bossWalkChannel);
			}
		}
	}


	void SoundManager::SetSoundSpeed(float speed, PlayMode mode)
	{
		switch (mode)
		{
		case PlayMode::BGM:
			bgmChannel->setPitch(speed);
			break;
		case PlayMode::Effect:
			//effectChannel->setPitch(speed);
			break;
		default:
			break;
		}
	}

	void SoundManager::SetSoundVoulume(float volume, PlayMode mode)
	{
		switch (mode)
		{
		case PlayMode::BGM:
			bgmChannel->setVolume(volume);
			break;
		case PlayMode::Effect:
			effectSoundVolume = volume;
			break;
		default:
			break;
		}
	}

	void SoundManager::SetEffectSoundVoulume(float volume)
	{
		//effectChannel->setVolume(m_ElapsedTime * volume);
	}

	void SoundManager::Finalize()
	{
		for (auto _fmodSound : soundList)
		{
			result = _fmodSound.second->release();
			ERRCHECK(result);
		}

		result = system->close();
		ERRCHECK(result);

		result = system->release();
		ERRCHECK(result);

		//Common_Close();
		//CoUninitialize();
	}
}