/********************************
*								*
*	Constant Buffer.h			*
*								*
*	Created : 2022/07/02		*
*	Updated : 2022/07/20		*
*********************************/

#pragma once

#include "../GraphicsEngine/Bases/BufferBase.h"
#include <d3d11_1.h>

namespace DX11
{
	using GraphicsEngineSpace::BufferBase;
	using GraphicsEngineSpace::ShaderType;

	class BufferBuilder;

	class ConstantBuffer : public BufferBase
	{
		ID3D11DeviceContext* deviceContext;
		ID3D11Buffer* buffer;
		UINT size;

	public:
		ConstantBuffer(ID3D11DeviceContext* deviceContext);
		~ConstantBuffer() override;

		bool SetUpBuffer(unsigned int slot, void* data, ShaderType targetType) override;

	private:
		bool CreateBuffer(ID3D11Device* device, D3D11_USAGE usage, UINT CPUflag, UINT typeSize);

		friend BufferBuilder;
	};
}