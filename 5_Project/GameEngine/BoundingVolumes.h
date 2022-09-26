#pragma once

namespace GameEngineSpace
{
	/// �浹 ó���� ���� �ٿ�� ���� ����ü���� �����ص� ��� ����.
	///	3���� ť��, 2���� Rect, 3���� ���, 3���� ����ü�� ���� bounding volume�� �ִ�.

	/// <summary>
	/// 3���� ť�� �ٿ�� ����
	/// </summary>
	struct BoundingCube
	{
		// �ڽ��� �������� ���� ���� ������
		Vector localScale;
		// �ڽ��� �������� ���� ���� �߽�(��ġ ����)
		Vector localCenter;
		// �ڽ��� �������� ���� ���� ��Ʈ����(ȸ�� ���� ���..?)
		Matrix localMatrix;
		// �� ���� ��ġ ����.
		Vector localPoint[8];

		// �߽��� �������� scale ��ŭ�� ũ��� ������ ��´�.
		BoundingCube(const Vector& _localScale, const Vector& _localCenter, const Matrix& _localMatrix)
			: localScale(_localScale)
			, localCenter(_localCenter)
			, localMatrix(_localMatrix)
		{
			// ������ ũ�⸦ �������� ������ ��´�.
			float halfX = localScale.x / 2;
			float halfY = localScale.y / 2;
			float halfZ = localScale.z / 2;

			// ť�긦 �̷�� ������ ��ġ�� �ʱ�ȭ �����ݴϴ�.
			localPoint[0] = localCenter + Vector{ -halfX, -halfY, -halfZ };
			localPoint[1] = localCenter + Vector{ -halfX, halfY, -halfZ };
			localPoint[2] = localCenter + Vector{ halfX, -halfY, -halfZ };
			localPoint[3] = localCenter + Vector{ -halfX, -halfY, halfZ };
			localPoint[4] = localCenter + Vector{ halfX, -halfY, halfZ };
			localPoint[5] = localCenter + Vector{ halfX, halfY, -halfZ };
			localPoint[6] = localCenter + Vector{ -halfX, halfY, halfZ };
			localPoint[7] = localCenter + Vector{ halfX, halfY, halfZ };

		}
	};

	// �Ϲ����� 2���� rect
	struct BoundingRect
	{
		// lefttop�� rightbottom
		Vector leftTop;
		Vector rightBottom;

		BoundingRect(Vector _leftTop, Vector _rightBottom)
			: leftTop(_leftTop)
			, rightBottom(_rightBottom)
		{

		}
		BoundingRect(Vector rect)
			: leftTop(rect.x, rect.y)
			, rightBottom(rect.z, rect.w)
		{
		}
	};

	// 3���� ���
	struct BoundingPlane
	{
		Vector normal;

		float distance;

		BoundingPlane()
			: normal(Vector::Zero)
			, distance()
		{

		}

		// 3�������� �� �� ���� �־����� �� ����� ������ݴϴ�.
		BoundingPlane(Vector first, Vector second, Vector third)
		{
			// ù ������ second�� ���ϴ� ����
			Vector e1 = first - second;
			Vector e2 = first - third;

			// �ݽð�� ������ �̾Ƴ��ϴ�. CCW
				// => �ش� ���� �� ����� �븻�� �˴ϴ�.
			normal = Vector3Cross(e1, e2);
			normal = Vector3Normalize(normal);

			// ������� �ǹ��� dot����
				// ����� ������ => ax + by + cz = - d;
				// ���⼭ a, b, c�� normal, x, y, z�� first
			distance = -Vector3Dot(normal, first);
		}
	};

	// �ٿ�� �������� (����ü..!)
	struct BoundingFrustum
	{
		BoundingPlane planes[6];

		// far plane�� �ڳ� �� ��ǥ
			// lefttop�� x, y, z, 1
			// rightTop�� x, y, z, 1
			// leftbottom�� x, y, z, 1
			// rightbottom�� x, y, z, 1.
		Vector frustumCorner[4];
	};
}
