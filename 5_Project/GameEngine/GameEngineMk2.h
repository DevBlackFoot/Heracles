#pragma once


namespace GameEngineSpace
{
	/**
	 * \brief 매니저들을 이용해서 씬을 업데이트 시키는 게임엔진!
	 *
	 * 2022.08.02(화) DevBlackFoot
	 */
	class GameEngineMk2
	{
	private:
		// 인스턴스화 해서 관리해줄 스태틱 변수..
		static std::shared_ptr<GameEngineMk2> gameEngineInstance;

	public:
		void Init(tstring gameTitle, HINSTANCE hInst, int screenWidth, int screenHeight);

		INT Tick();

		void Release();

		static std::shared_ptr<GameEngineMk2> GetEngineInstnace() { return gameEngineInstance; }
	};

}
