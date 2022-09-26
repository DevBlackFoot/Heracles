using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;
using SharpDX.Direct3D11;
using SharpDX.D3DCompiler;
using SharpDX.DXGI;

using AlmondDxGraphics.GraphicResources.Data;

namespace AlmondDxGraphics.DXResources
{
    using Device11 = SharpDX.Direct3D11.Device;
    using Buffer11 = SharpDX.Direct3D11.Buffer;

    public enum EShaderType
    {
        VertexShader,
        PixelShader,
        GeometryShader,
        HullShader,
        DomainShader
    }

    public interface IDxConstantBuffer
    {
        public void ApplyChanges(DeviceContext dc);
        public void Initialize(Device11 device, ConstantBufferDescription description, int slot, EShaderType type);

        public string GetTypeName();


    }
    public class DxConstantBuffer<T> : IDxConstantBuffer where T : struct
    {
        public T Data { get { return _data; } set { _data = value; } }

        private T _data;

        private string _typeName;

        public EShaderType ShaderType { get; set; }

        public Buffer11? CBForBinding { get; private set; }
        private int _slot;

        public DxConstantBuffer()
        {
            _slot = 0;
            _typeName = typeof(T).Name;
        }

        public string GetTypeName()
        {
            return _typeName;
        }

        public void Initialize(Device11 device, ConstantBufferDescription description, int slot, EShaderType type)
        {
            ShaderType = type;
            _slot = slot;

            CBForBinding = new Buffer11(device,
            Utilities.SizeOf<T>(), ResourceUsage.Default, BindFlags.ConstantBuffer,
            CpuAccessFlags.None, ResourceOptionFlags.None, 0);



        }

        public void ApplyChanges(DeviceContext dc)
        {
            if (CBForBinding == null)
                return;
            dc.UpdateSubresource<T>(ref _data, CBForBinding);
            switch (ShaderType)
            {
                case EShaderType.VertexShader:
                    dc.VertexShader.SetConstantBuffer(_slot, CBForBinding);
                    break;
                case EShaderType.PixelShader:
                    dc.PixelShader.SetConstantBuffer(_slot, CBForBinding);
                    break;
                case EShaderType.GeometryShader:
                    dc.GeometryShader.SetConstantBuffer(_slot, CBForBinding);
                    break;
                case EShaderType.HullShader:
                    dc.HullShader.SetConstantBuffer(_slot, CBForBinding);
                    break;
                case EShaderType.DomainShader:
                    dc.DomainShader.SetConstantBuffer(_slot, CBForBinding);
                    break;
            }
        }
    }
}
