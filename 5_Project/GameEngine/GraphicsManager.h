#pragma once

// �׷��Ƚ� ���� ����� ���� ���
#include "../GraphicsEngine/GraphicsEngineFramework.h"

namespace GameEngineSpace
{
	using namespace GraphicsEngineSpace;

	// dll Import�� ����.. �߰�



	/**
	 * \brief ������ �׷��Ƚ� ������ �ʱ�ȭ �ϰ�, ������ �ִ� �Ŵ��� Ŭ����
	 *
	 * �׷��Ƚ� ������ ����� �ʿ��� �� �ش� Ŭ������ �Ű��� �̿��Ѵ�. => ���������� ȭ�鿡 �׸��� �׸��� ģ��..
	 *
	 * 2022.08.02 Dev.BlackFoot
	 */
	class GraphicsManager final
	{
	private:
		static std::shared_ptr<GraphicsManager> instance;

		// ������
		std::shared_ptr<GraphicsEngine> renderer;
		HMODULE graphicsDLL;

		// �� �������� ���� ť..
			// ���� ������ DirLit �ϳ���
		DirectionalLight* dLight;

	public:
		GraphicsManager() = default;
		~GraphicsManager() = default;
		GraphicsManager(const GraphicsManager& other) = delete;
		GraphicsManager& operator= (const GraphicsManager& other) = delete;
		GraphicsManager(const GraphicsManager&& other) = delete;
		GraphicsManager& operator= (const GraphicsManager&& other) = delete;

	private:
		// ���� ť�� �ִ� ��� �����͸� �׸��ϴ�.
		void RenderPrefab();

	public:
		bool Init(HWND hWnd, uint32 screenWidth, uint32 screenHeight);
		void Update();
		void Release();

		// �׷��Ƚ� �Ŵ����� ������.. �������� �����ִ°ɷ� �غ���..
		std::shared_ptr<GraphicsEngine> GetRenderer() { return renderer; }

		// ���� �����ݽô�..
		DirectionalLight* GetDirectionalLight() { return dLight; }

	public:
		static std::shared_ptr<GraphicsManager> GetInstance();

	};
}
