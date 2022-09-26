/********************************
*								*
*	Layout Base.h				*
*								*
*	Created : 2022/06/30		*
*	Updated : 2022/06/30		*
*********************************/

#pragma once

#include <string>
#include "Define/GraphicsFormat.h"
#include "Bases/ResourceBase.h"

namespace GraphicsEngineSpace
{
	typedef unsigned int UINT;

	struct InputLayout
	{
		int unused;
	};

	/// <summary>
	/// Vertex Shader �����ÿ� �ʿ��� Elements�� ������ Ŭ����
	/// 
	/// 2022. 06. 30
	/// </summary>
	class LayoutBase : public ResourceBase<LayoutBase>
	{
	public:
		LayoutBase() = default;
		virtual ~LayoutBase() = default;

		virtual void AddElements(const char* semantic, UINT index, GraphicsFormat format, UINT slot, UINT offset /* instancing ������ �߰����� ����*/) abstract;

		virtual InputLayout* GetLayout() abstract;
		virtual UINT GetLayoutSize() abstract;
	};
}