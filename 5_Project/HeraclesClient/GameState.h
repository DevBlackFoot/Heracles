#pragma once


namespace Heracles
{
	enum class SceneType
	{
		MAIN_MENU,
		VILLAGE_START,
		VILLAGE_UPGRADE,
		MONSTER_NORMAL,
		MONSTER_BOSS,
	};

	struct PlayerInfo
	{
		float currHp = 100;
		float maxHp = 100;
		float armor = 0.f;

		float attackPower = 10.f;
		float attackDelay = 1.f;

		float moveSpeed = 10.f;		  //
		float rotateSpeed = 3.f;      // ȸ���ӵ�

		float dodgeSpeed = 80.f;
		float dodgeDelay = 1.f;       //sec

		bool isInvincable = false;

		bool canMove = true;

		float attackTimer = 0;
		float dodgetimer = 0;

		int memoryPieceNum = 0; // �����۵�
		int money = 0;
	};

	/// <summary>
	/// ���ӻ��� ������ ���� Ŭ����
	/// �̱������� �����߰� ��� ���� ������Ʈ���� ���ٰ����ϴ�
	/// �÷��̾� ��ȭ/ ��������(����,�÷��̾�) / ���ӿ���
	/// ��Ȱ�� �������� ���� ������ �Ѵ�
	/// </summary>
	class GameState final
	{
	private:

		static std::shared_ptr<GameState> instance;
		std::shared_ptr<GameObject> player;
		std::shared_ptr<GameObject> boss;

	public:
		tstring nextMap;
		tstring nextMap2;

		int rewardIdx1; // map1 �� ����
		int rewardIdx2; // map2 �� ����

		std::vector<tstring> mapList;
		std::vector<tstring> mapMonster;

	public:
		SceneType currMap;

		bool mapCleared = false; // ��� ���͸� ������ true -> sceneController���� üũ

		int playNum = 0; // ������ ����

		float totalPlayTime = 0.f; // �� �÷��� �ð� : �� ������ ������

		int killedMonsterNum = 0; // monster state-> death���� +1

		int currMonsterNum = 0;  // createMonster ���� +

		int totalMonsterNumInMap = 100; // ������ �ʱ�ȭ

		bool interacting = false;

		bool cursePain = true;
		bool curseMadness =true;
		bool curseSight = true;

		bool skillUpgraded = false;
		
		float bossHp = 1000.f;
		float bossMaxHP = 1000.0f;

	public:
		//�÷��̾� ���� ����
		// ���ַ� ���� ȿ���� ������������ ����� ����
		PlayerInfo* playerInfo;


	public:
		int clearedMapNum = 0;

		int dieStage = 0;

		bool exitType; // 0�̸� A 1�̸� B


	public:
		GameState() = default;

		~GameState() = default;

		GameState(const GameState& _other) = delete;
		GameState& operator= (const GameState& _other) = delete;
		GameState(GameState&& _other) = delete;
		GameState& operator= (GameState&& _other) = delete;

		static std::shared_ptr<GameState> GetInstance();

		void Init();
		void Release();

		inline tstring GetNextMap1() { return nextMap; }
		inline tstring GetNextMap2() { return nextMap2; }

		inline std::shared_ptr<GameObject> GetPlayer() { return this->player; }
		inline void SetPlayer(std::shared_ptr<GameObject> player) { this->player = player; }

		inline std::shared_ptr<GameObject> GetBoss() { return this->boss; }
		inline void SetBoss(std::shared_ptr<GameObject> boss) { this->boss = boss; }

		void ResetMapVector();

		void GameClear();
	};
	
}

