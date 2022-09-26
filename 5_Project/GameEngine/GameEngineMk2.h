#pragma once


namespace GameEngineSpace
{
	/**
	 * \brief �Ŵ������� �̿��ؼ� ���� ������Ʈ ��Ű�� ���ӿ���!
	 *
	 * 2022.08.02(ȭ) DevBlackFoot
	 */
	class GameEngineMk2
	{
	private:
		// �ν��Ͻ�ȭ �ؼ� �������� ����ƽ ����..
		static std::shared_ptr<GameEngineMk2> gameEngineInstance;

	public:
		void Init(tstring gameTitle, HINSTANCE hInst, int screenWidth, int screenHeight);

		INT Tick();

		void Release();

		static std::shared_ptr<GameEngineMk2> GetEngineInstnace() { return gameEngineInstance; }
	};

}
