#include "pch.h"
#include "GameEngine.h"
#include "Renderer.h"
#include "Camera.h"
#include "Input.h"
#include "Factory.h"
#include "BuilderManger.h"
#include "Basic32Obj.h"
#include "PosColorObj.h"

FootEngine::GameEngine::GameEngine() :
	m_Renderer(nullptr)
	, m_DeltaTime(0.0f)
	, m_Box(nullptr)
	, m_LastMousePos()
	, m_hWnd(nullptr)
{
	m_Camera = make_unique<::Camera>();
	m_Camera->LookAt(Vector3(0.0f, 15.0f, -25.0f), Vector3(0.0f, 7.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));

}

FootEngine::GameEngine::~GameEngine()
{
}

bool FootEngine::GameEngine::Initialize(HWND _hWnd, int _clientWidth, int _clientHeight)
{
	m_hWnd = _hWnd;
	// ������ ���� �� �ʱ�ȭ.
		// �������� ������ ��, ���ο��� �׷��Ƚ� �ھ�, ���� Ÿ��, ���� Effect�� Factory, BuildManger�� �����ȴ�.
		// ���� �����͸� �޾� ����ũ ptr�� ������ش�.
	m_Renderer.reset(CreateRenderer());

	//���丮 �ʱ�ȭ.
	if (m_Renderer->Initialize(_hWnd, _clientWidth, _clientHeight) != true)
	{
		return false;
	}

#pragma region Object Create

	// �ڽ� �����.
	m_Box = Factory::GetInstance()->CreateDXObject<Basic32Obj>(
		BuilderManger::GetInstance()->GetBuilder("Basic32Builder"), "Box");

	m_Renderer->AddRenderObj(m_Box);

	IDXObject* _tmpObj = Factory::GetInstance()->CreateDXObject<PosColorObj>(
		BuilderManger::GetInstance()->GetBuilder("PosColorBuilder"), "Grid"
		);
	m_ObjVector.push_back(_tmpObj);
	m_Renderer->AddRenderObj(_tmpObj);

	_tmpObj = Factory::GetInstance()->CreateDXObject<PosColorObj>(
		BuilderManger::GetInstance()->GetBuilder("PosColorBuilder"), "Axis"
		);
	m_ObjVector.push_back(_tmpObj);
	m_Renderer->AddRenderObj(_tmpObj);

	// texture ���� ���� ����
	_tmpObj = Factory::GetInstance()->CreateDXObject<Basic32Obj>(
		BuilderManger::GetInstance()->GetBuilder("Basic32Builder"), "../ASE/box.ASE"
		);
	m_ObjVector.push_back(_tmpObj);
	m_Renderer->AddRenderObj(_tmpObj);

	_tmpObj = Factory::GetInstance()->CreateDXObject<Basic32Obj>(
		BuilderManger::GetInstance()->GetBuilder("Basic32Builder"), "../ASE/teapot.ASE",
		L"../Textures/000000002476.dds"
		);
	m_ObjVector.push_back(_tmpObj);
	m_Renderer->AddRenderObj(_tmpObj);

#pragma endregion

	// ������ ������ ������Ʈ�� �ʱ�ȭ �����ݴϴ�.
	m_Renderer->InitObject();

	return true;
}

void FootEngine::GameEngine::Finalize()
{
	m_Renderer->Finalize();
}

void FootEngine::GameEngine::SceneUpdate(float _dt)
{
	// ������Ʈ �� ��..
		// ī�޶� �־�����.
	m_DeltaTime = _dt;

	// ���⼭ Ű ������Ʈ�� �ϸ� �Ƹ� �ɰǵ�..
		// dll���� common library�� �̱��� ��ü�� �������� ����
		// exe���� common library�� ��ü�� �������� ���� �޸� �ּҰ� �ٸ��� ���´�.
		// �׷��� ����� ����..
	Input::GetInstance()->Update();

	// ī�޶� �̵�. (������ ����)
	if (Input::GetInstance()->GetInputState('W', KeyState::PRESS))
		m_Camera->Walk(10.0f * _dt);

	if (Input::GetInstance()->GetInputState('S', KeyState::PRESS)) // s
		m_Camera->Walk(-10.0f * _dt);

	if (Input::GetInstance()->GetInputState('A', KeyState::PRESS)) // a
		m_Camera->Strafe(-10.0f * _dt);

	if (Input::GetInstance()->GetInputState('D', KeyState::PRESS)) // d
		m_Camera->Strafe(10.0f * _dt);

	if (Input::GetInstance()->GetInputState('Q', KeyState::PRESS)) // q
		m_Camera->WorldUpDown(-10.0f * _dt);

	if (Input::GetInstance()->GetInputState('E', KeyState::PRESS)) // e
		m_Camera->WorldUpDown(10.0f * _dt);

	// ViewTM �����
	m_Camera->UpdateViewMatrix();
	// ȸ���� ���� ����
	static XMVECTOR rotate = {};
	rotate.m128_f32[1] += 0.005f;

	XMMATRIX rotateMx = XMMatrixRotationRollPitchYawFromVector(rotate);

	// Ʈ������
	XMVECTOR translate = { 0.0f, 0.0f, 0.0f, 1.0f };
	XMMATRIX _translationMx = XMMatrixTranslationFromVector(translate);

	XMMATRIX world = rotateMx * _translationMx;

	// �׸��� Axis ������Ʈ
		// �� �� ������ �༮�̹Ƿ� �̷��� �ص� �ȴ�..
	for (auto obj : m_ObjVector)
	{
		obj->Update(XMMatrixIdentity(), m_Camera->View(), m_Camera->Proj());
		// ������ ������Ʈ�� ã�Ƽ� �������ش�..
		if (obj->GetObjName() == "../ASE/box.ASE")
		{
			obj->Update(world, m_Camera->View(), m_Camera->Proj());
		}

		if (obj->GetObjName() == "../ASE/teapot.ASE")
		{
			translate.m128_f32[0] = -5.0f;
			_translationMx = XMMatrixTranslationFromVector(translate);

			world = rotateMx * _translationMx;
			obj->Update(world, m_Camera->View(), m_Camera->Proj());
		}
	}

	translate.m128_f32[0] = 5.0f;
	_translationMx = XMMatrixTranslationFromVector(translate);

	world = rotateMx * _translationMx;

	// box �������..
	m_Box->Update(world, m_Camera->View(), m_Camera->Proj());

	DirectionalLight _dirLit;
	_dirLit.Ambient = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	_dirLit.Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	_dirLit.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	_dirLit.Direction = XMFLOAT3(0.0f, -0.5f, 0.57735f);

	m_Box->LightUpdate(_dirLit, {}, {}, m_Camera->GetPosition());

	// �������� �� ������Ʈ ������մϴ�..
	for (auto obj : m_ObjVector)
	{
		obj->LightUpdate(_dirLit, {}, {}, m_Camera->GetPosition());
	}

	Input::GetInstance()->LateUpdate();
}

void FootEngine::GameEngine::SceneRender()
{
	m_Renderer->RenderAll();
}

void FootEngine::GameEngine::OnMouseDown(int x, int y)
{
	m_LastMousePos.x = x;
	m_LastMousePos.y = y;

	SetCapture(m_hWnd);
}

void FootEngine::GameEngine::OnMouseUp(int x, int y)
{
	ReleaseCapture();
}

void FootEngine::GameEngine::MouseMove(int btnState, int x, int y)
{
	if ((btnState & MK_RBUTTON) != 0)
	{
		float dx = XMConvertToRadians(0.25f * static_cast<float>(x - m_LastMousePos.x));
		float dy = XMConvertToRadians(0.25f * static_cast<float>(y - m_LastMousePos.y));

		m_Camera->Pitch(dy);
		m_Camera->RotateY(dx);
	}

	m_LastMousePos.x = x;
	m_LastMousePos.y = y;
}

void FootEngine::GameEngine::OnResize()
{
	m_Renderer->OnResize();

	// ī�޶� ���� �������ش�.
	m_Camera->SetLens(0.25f * 3.1415926535f, m_Renderer->GetAspectRatio()
		, 1.0f, 1000.0f);
}

// �⺻���ΰ� ������ �ȿ� ���� �Ǿ��ִ�..
bool FootEngine::GameEngine::IsVaildDevice()
{
	return m_Renderer->IsVaildDevice();
}

void FootEngine::GameEngine::SetClientSize(int _width, int _height)
{
	m_Renderer->SetClientSize(_width, _height);
}

void FootEngine::GameEngine::SetWinMinMax(bool _isMinimized, bool _isMaximized)
{
	m_Renderer->SetWinMinMax(_isMinimized, _isMaximized);
}
