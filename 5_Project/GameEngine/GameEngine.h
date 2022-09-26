#pragma once
#include <vector>
#include "FootEngineTypedef.h"

class IRenderer;
class Camera;
class Timer;
class IDXObject;

/**
 * \brief ������ ������ ������ �� Engine�Դϴ�.
 *
 * ���Ŵ����� ��������� ���� ������ ������Ʈ�ϰ� �����ϴ� ������ ����մϴ�
 *
 * 2022.07.19(ȭ) Dev.BlackFoot
 */
namespace FootEngine
{
	class GameEngine
	{
	public:
		GameEngine();
		~GameEngine();

		bool Initialize(HWND _hWnd, int _clientWidth, int _clientHeight);
		void Finalize();

		void SceneUpdate(float _dt);

		void SceneRender();

		// ���콺 �Է��� �޾ƾ��� ��
		void OnMouseDown(int x, int y);
		void OnMouseUp(int x, int y);
		void MouseMove(int btnState, int x, int y);

		void OnResize();

		// ������ ��������
		bool IsVaildDevice();
		void SetClientSize(int _width, int _height);
		void SetWinMinMax(bool _isMinimized, bool _isMaximized);

		// ���������� �������� ������ �ְ�..
	private:
		// �����͸� ����Ʈ ������ ���ַ� ����غ���..
		//IRenderer* m_Renderer;
		std::unique_ptr<IRenderer> m_Renderer;
		HWND m_hWnd;

		POINT m_LastMousePos;

		// ������ �׸��� �׷��� ���� ������Ʈ���� ������ ���� ������ �ʿ��ϴ�..
		// ������Ʈ �������� ���� ���� ���̸�. ObjBase�� �����͸� ������ �ִ� ���� or list�� �ʿ��� �� �ϴ�.
			// Scene���� ��ü�� �� ���� ������ �̰���..
		//Camera* m_Camera;
		std::unique_ptr<Camera> m_Camera;
		float m_DeltaTime;
		// ���⿡ �ڽ��� �ѹ� ������..
		IDXObject* m_Box;

		// ������Ʈ ����Ʈ�� ������.
		std::vector<IDXObject*> m_ObjVector;
	};

}