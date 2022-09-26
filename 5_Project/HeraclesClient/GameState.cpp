#include "pch.h"
#include "GameState.h"
#include "GameObjectTemplate.h"
#include "SoundManager.h"


namespace Heracles
{
	std::shared_ptr<GameState> GameState::instance = nullptr;

	std::shared_ptr<GameState> GameState::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = std::make_shared<GameState>();
			instance->playerInfo = new PlayerInfo;
		}
		return instance;
	}

	void GameState::Init()
	{
		//���� BGM �ε�
		SoundManager::GetInstance()->LoadSoundFile("MainMenu", "Resources/Sound/Title.mp3", 0); //main menu
		SoundManager::GetInstance()->LoadSoundFile("CutScene", "Resources/Sound/tragedy.mp3", 0); //cutScene
		SoundManager::GetInstance()->LoadSoundFile("StartCurseSceneMap", "Resources/Sound/Chris Rede - mamadrama.mp3", 0); //startCurseScene
		SoundManager::GetInstance()->LoadSoundFile("MonsterMap", "Resources/Sound/Grand Attack of The Trolls.mp3", 0); // monsterScene
		SoundManager::GetInstance()->LoadSoundFile("WeaponUpgradeMap", "Resources/Sound/Aldebaran.mp3", 0); // waeponUpgrade
		SoundManager::GetInstance()->LoadSoundFile("BeforeBossMap", "Resources/Sound/enterboss_stage1.mp3", 0); // beforeboss
		SoundManager::GetInstance()->LoadSoundFile("BossAfterEnter", "Resources/Sound/boss_stage_last_battle.mp3", 0); // boss��������
		SoundManager::GetInstance()->LoadSoundFile("BossBattle", "Resources/Sound/Epic Boss Battle.mp3", 0); // ���� ���� 
		SoundManager::GetInstance()->LoadSoundFile("BossLastPhase", "Resources/Sound/boss_stage_last_battle.mp3", 0); // ���� ������ ������
		SoundManager::GetInstance()->LoadSoundFile("GameClear", "Resources/Sound/Hero.mp3", 0); // Ŭ����

		//���� effect �ε�

		SoundManager::GetInstance()->LoadSoundFile("BossEnter", "Resources/Sound/intro_boss.mp3", 1); // boss���� ����

		SoundManager::GetInstance()->LoadSoundFile("MonsterAttacked1", "Resources/Sound/club_attack1.mp3", 1); // ���� �ǰ�
		SoundManager::GetInstance()->LoadSoundFile("MonsterAttacked2", "Resources/Sound/club_attack2.mp3", 1); // ���� ���� �ǰ�
		SoundManager::GetInstance()->LoadSoundFile("MonsterAttacked3", "Resources/Sound/club_attack3.mp3", 1); // ū ���� �ǰ� MonsterAttack.wav
		SoundManager::GetInstance()->LoadSoundFile("MonsterAttack", "Resources/Sound/MonsterAttack.wav", 1); //���� ����
		SoundManager::GetInstance()->LoadSoundFile("MonsterDeath", "Resources/Sound/MonsterDeath.wav", 1);

		SoundManager::GetInstance()->LoadSoundFile("playerAttack1", "Resources/Sound/FX_Swing_Blade2Hand_01.wav", 1); // �÷��̾� ����1
		SoundManager::GetInstance()->LoadSoundFile("playerAttack2", "Resources/Sound/FX_Swing_Blade2Hand_02.wav", 1); // �÷��̾� ����1
		SoundManager::GetInstance()->LoadSoundFile("playerAttack3", "Resources/Sound/FX_Swing_Blade2Hand_03.wav", 1); // �÷��̾� ����1
		SoundManager::GetInstance()->LoadSoundFile("playerHit", "Resources/Sound/PlayerHit.mp3", 1); // �÷��̾� �ǰ�

		SoundManager::GetInstance()->LoadSoundFile("playerSkill", "Resources/Sound/skill.wav", 1); // �÷��̾� ��ų1

		SoundManager::GetInstance()->LoadSoundFile("PlayerRun", "Resources/Sound/run4.wav", 1); // �÷��̾� �̵�
		SoundManager::GetInstance()->LoadSoundFile("PlayerDash", "Resources/Sound/00Add_Dash.wav", 1); // �÷��̾� �뽬
		
		//������
		SoundManager::GetInstance()->LoadSoundFile("GetHpItem", "Resources/Sound/Hp_Potion.wav", 1); // hp������ 
		SoundManager::GetInstance()->LoadSoundFile("GetMemoryGem", "Resources/Sound/MemoryGem.wav", 1); // �������
		SoundManager::GetInstance()->LoadSoundFile("GetMoney", "Resources/Sound/Coin(God's_marble).wav", 1); // ���Ⱝȭ��ȭ
		SoundManager::GetInstance()->LoadSoundFile("ItemDrop", "Magic Chimes 02.wav", 1); // ������ ���


		SoundManager::GetInstance()->LoadSoundFile("DoorOpen", "Resources/Sound/Add_Gate_Open_01.mp3", 1); // �� ����
		SoundManager::GetInstance()->LoadSoundFile("DoorClose", "Resources/Sound/Close.mp3", 1); // �� ����

		// ����
		SoundManager::GetInstance()->LoadSoundFile("AltarActivate", "Resources/Sound/Altar_interact_ui_open.mp3", 1); // ���� on
		SoundManager::GetInstance()->LoadSoundFile("AltarClose", "Resources/Sound/Altar_interact_ui_close.mp3", 1);
		SoundManager::GetInstance()->LoadSoundFile("AltarWrong", "Resources/Sound/wrong_click1.mp3", 1);
		SoundManager::GetInstance()->LoadSoundFile("PlayerUpgrade", "Resources/Sound/Character_Upgrade.mp3", 1); // �÷��̾� ���׷��̵�
		SoundManager::GetInstance()->LoadSoundFile("CurseOff", "Resources/Sound/Curse_Free.mp3", 1); // ���� ����
		SoundManager::GetInstance()->LoadSoundFile("WeaponUpgrade", "Resource/Sound/Weapon_Upgrade.mp3", 1);

		// UI
		SoundManager::GetInstance()->LoadSoundFile("TitleUIHover", "Resources/Sound/Main_ui_hover.mp3", 1); // main ui hover -> ����ȭ�� ���� �ε�?
		SoundManager::GetInstance()->LoadSoundFile("TitleUISelect1", "Resources/Sound/Main_ui_select0.wav", 1); // main ui select1
		SoundManager::GetInstance()->LoadSoundFile("TitleUISelect2", "Resources/Sound/Main_ui_select1.mp3", 1); // main ui select2
		SoundManager::GetInstance()->LoadSoundFile("TutorialUI", "Resources/Sound/Tutorial_ui.mp3", 1); // tutorial ui
		SoundManager::GetInstance()->LoadSoundFile("TutorialPage", "Resources/Sound/Tutorial_page.wav", 1); // tutorial ui

		SoundManager::GetInstance()->LoadSoundFile("UIClose", "Resources/Sound/UI_close.wav", 1); // ui close
		SoundManager::GetInstance()->LoadSoundFile("UIHover", "Resources/Sound/UI_Hover.wav", 1); // ui hover
		SoundManager::GetInstance()->LoadSoundFile("UIOpen", "Resources/Sound/UI_open.wav", 1); // ui open
		SoundManager::GetInstance()->LoadSoundFile("UIAltarSelect", "Resources/Sound/ui_select(Upgrade_shop).mp3", 1); //upgrade ���� ����?
		SoundManager::GetInstance()->LoadSoundFile("TextType", "Resources/Sound/TextType.mp3", 1); // text �Է� �� ������ �Ҹ�.

		//����
		SoundManager::GetInstance()->LoadSoundFile("TrapOpen", "Resources/Sound/Open.wav", 1); // trap open
		SoundManager::GetInstance()->LoadSoundFile("TrampClose", "Resources/Sound/Close.wav", 1); // trap close 

		//����
		SoundManager::GetInstance()->LoadSoundFile("BossRoar", "Resources/Sound/BossRoar.wav", 1);
		SoundManager::GetInstance()->LoadSoundFile("BossStartRoar", "Resources/Sound/FX_Lion_Emote_Roar_06.mp3", 1);
		SoundManager::GetInstance()->LoadSoundFile("BossJumpStart", "Resources/Sound/BOSS_KoalaKong_Jump_01.mp3", 1);
		SoundManager::GetInstance()->LoadSoundFile("BossJumpLand", "Resources/Sound/BOSS_KoalaKong_Land_01.mp3", 1); 
		SoundManager::GetInstance()->LoadSoundFile("BossDashStart", "Resources/Sound/BossDashStart.wav", 1);  
		SoundManager::GetInstance()->LoadSoundFile("BossAttack1", "Resources/Sound/BossAttack1.mp3", 1); 
		SoundManager::GetInstance()->LoadSoundFile("BossAttack2", "Resources/Sound/BossAttack2.mp3", 1); 
		SoundManager::GetInstance()->LoadSoundFile("BossAttack3", "Resources/Sound/BossAttack3.mp3", 1); 
		SoundManager::GetInstance()->LoadSoundFile("BossDeath", "Resources/Sound/BossDeath.wav", 1);
		SoundManager::GetInstance()->LoadSoundFile("BossWalk", "Resources/Sound/BossWalk.wav", 1);

		

		this->mapList.emplace_back(TEXT("UpgradeMap"));
		this->mapList.emplace_back(TEXT("BeforeBossMap"));
		this->mapList.emplace_back(TEXT("BossMap"));
		this->mapList.emplace_back(TEXT("StartCurseMap"));

		this->ResetMapVector();
	}
	void GameState::ResetMapVector()
	{
		this->mapMonster.clear();
		this->mapMonster.emplace_back(TEXT("MapA"));
		this->mapMonster.emplace_back(TEXT("MapB"));
		this->mapMonster.emplace_back(TEXT("MapC"));
		this->mapMonster.emplace_back(TEXT("MapE"));
		this->mapMonster.emplace_back(TEXT("MapF"));
		this->mapMonster.emplace_back(TEXT("MapI"));
		this->mapMonster.emplace_back(TEXT("StartScene"));

		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		shuffle(this->mapMonster.begin(), this->mapMonster.end(), std::default_random_engine(seed));
		//std::random_shuffle(this->mapMonster.begin(), this->mapMonster.end());
	}
	void GameState::GameClear()
	{
		//SceneManager::GetInstance()->LoadScene(TEXT("ClearScene"));
	}
}