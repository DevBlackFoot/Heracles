#pragma once
#include "ComponentBase.h"

#include "BoundingVolumes.h"

namespace GameEngineSpace
{
	class Transform;

	// ���� Ÿ��
		// ����Ÿ�Կ� ���� View, Proj ���� �޶���.
	enum class ProjType
	{
		PERSPECTIVE,
		ORTHOGRAPHIC
	};

	/**
	 * \brief ������Ʈ�� ��ӹ��� ���ӽ����̽� ���� Camera
	 */
	class Camera final : public ComponentBase
	{
		// ��ġ ����
			// ��ǻ� ���⿡ ���� �ۼ��� ��ġ������ look up right ���Ͱ� ��� ����ִ�.
		std::shared_ptr<Transform> transform;

		// View, Proj ���
		Matrix view;
		Matrix proj;

		ProjType projType;

		// ����ü�� ���õ� ������.
		float nearZ;
		float farZ;
		float aspect;
		float fovY;

		// ���Ŀ� ViewFrustum ���� ����, �Լ� �߰�.
			// ViewFrustum �ø�, ��ī�� �ڽ� ���� ��� �߰��� �ʿ�.
		// Frustum
		std::shared_ptr<BoundingFrustum> viewFrustum;

		// Shaking factor
		float shakingIntensity;
		float originalZ;

	public:
		Camera(std::weak_ptr<GameObject> _obj);
		virtual ~Camera();

		virtual void Release() override;

		// ���� Getter Setter
		const float& GetAspectRatio() { return aspect; }
		// ȭ�� �� ����Ǹ� ProjMatrix�� ����Ǿ����.
		void SetAspectRatio(float _ratio) { aspect = _ratio; UpdateProjMatrix(); }
		const float& GetNearZ() { return nearZ; }
		void SetNearZ(float _nearZ) { nearZ = _nearZ; UpdateProjMatrix(); }
		const float& GetFarZ() { return farZ; }
		void SetFarZ(float _farZ) { farZ = _farZ; UpdateProjMatrix(); }
		const float& GetFov() { return fovY; }
		void SetFov(float _fov) { fovY = _fov; UpdateProjMatrix(); }

		// TM ��ȯ.
			// View�� Proj ����� ���� ���ϴ�.
			// Proj * View * SRT ������ View, Proj
		const Matrix& GetViewMatrix();
		const Matrix& GetProjMatrix();

		virtual void Update(float tick) override;

		// �������� ����.
		void SetLens(float _fovY, float _aspect, float _nearZ, float _farZ);

		void SetShaking(float intensity);

		void UpdateViewMatrix();

		// ViewFrustumCulling�� ���� �Լ� ����
		bool ViewFrustumCulling(std::shared_ptr<BoundingCube> boundingCube, const Matrix& worldMatrix);

	private:
		void UpdateProjMatrix();
		void UpdateViewFrustum();
	};
}
