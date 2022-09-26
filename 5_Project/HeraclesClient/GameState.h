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
		float rotateSpeed = 3.f;      // 회전속도

		float dodgeSpeed = 80.f;
		float dodgeDelay = 1.f;       //sec

		bool isInvincable = false;

		bool canMove = true;

		float attackTimer = 0;
		float dodgetimer = 0;

		int memoryPieceNum = 0; // 아이템들
		int money = 0;
	};

	/// <summary>
	/// 게임상태 공유를 위한 클래스
	/// 싱글톤으로 구현했고 모든 게임 오브젝트에서 접근가능하다
	/// 플레이어 강화/ 저주적용(몬스터,플레이어) / 게임오버
	/// 부활시 정보저장 등의 역할을 한다
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

		int rewardIdx1; // map1 의 보상
		int rewardIdx2; // map2 의 보상

		std::vector<tstring> mapList;
		std::vector<tstring> mapMonster;

	public:
		SceneType currMap;

		bool mapCleared = false; // 모든 몬스터를 잡으면 true -> sceneController에서 체크

		int playNum = 0; // 죽으면 증가

		float totalPlayTime = 0.f; // 총 플레이 시간 : 매 프레임 더해줌

		int killedMonsterNum = 0; // monster state-> death에서 +1

		int currMonsterNum = 0;  // createMonster 에서 +

		int totalMonsterNumInMap = 100; // 씬에서 초기화

		bool interacting = false;

		bool cursePain = true;
		bool curseMadness =true;
		bool curseSight = true;

		bool skillUpgraded = false;
		
		float bossHp = 1000.f;
		float bossMaxHP = 1000.0f;

	public:
		//플레이어 상태 저장
		// 저주로 인한 효과는 다음스테이지 입장시 적용
		PlayerInfo* playerInfo;


	public:
		int clearedMapNum = 0;

		int dieStage = 0;

		bool exitType; // 0이면 A 1이면 B


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

