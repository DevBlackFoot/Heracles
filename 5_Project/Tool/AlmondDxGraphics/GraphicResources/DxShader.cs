using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;
using SharpDX.Direct3D11;
using SharpDX.D3DCompiler;

using Buffer = SharpDX.Direct3D11.Buffer;

using AlmondDxGraphics.DXResources;
using AlmondDxGraphics.GraphicResources.Factories;
using AlmondDxGraphics.GraphicResources.Data;

namespace AlmondDxGraphics.GraphicResources
{
    // 여긴 D3D_COMPILE_STANDARD_FILE_INCLUDE 매크로가 없다. 포인터를 넣어 해당역할을 하도록 하는 클래스
    public class StandardIncludeHandler : CppObject, Include
    {
        public StandardIncludeHandler() : base(new IntPtr(1)) { }
        public void Close(Stream stream) { }
        public Stream Open(IncludeType type, string fileName, Stream parentStream)
        {
            throw new NotImplementedException();
        }
    }

    // 쉐이더 파일에서 정의한 함수 이름을 받는다.
    // Ex ) VertexShaderFunction = "VSMain"
    public class DxShaderDescription
    {
        public string? VertexShaderFunctionName { get; set; }
        public string? PixelShaderFunctionName { get; set; }
        public string? GeometryShaderFunctionName { get; set; }
        public string? HullShaderFunctionName { get; set; }
        public string? DomainShaderFunctionName { get; set; }
        // 스트림 아웃풋
        public StreamOutputElement[]? GeometrySO { get; set; }

    }

    public class DxShader : IDResourceBase, IDisposable
    {
        #region Shaders
        public VertexShader? VertexShader { get; set; }
        public PixelShader? PixelShader { get; set; }
        public GeometryShader? GeometryShader { get; set; }
        public HullShader? HullShader { get; set; }
        public DomainShader? DomainShader { get; set; }
        #endregion 

        #region ConstantBuffers
        private IDxConstantBuffer[]? _VSconstantBuffers;
        private IDxConstantBuffer[]? _PSconstantBuffers;
        private IDxConstantBuffer[]? _GSconstantBuffers;
        private IDxConstantBuffer[]? _HSconstantBuffers;
        private IDxConstantBuffer[]? _DSconstantBuffers;
        #endregion 

        public InputLayout? InputLayout { get; set; }

        public DxDevice Device { get; set; }

        public DxShader(DxDevice device, string filename, DxShaderDescription description) : base(Path.GetFileName(filename), RESOURCE_TYPE.SHADER)
        {
            Device = device;

            var vertexShaderName = description.VertexShaderFunctionName;
            var pixelShaderName = description.PixelShaderFunctionName;
            var geometryShaderName = description.GeometryShaderFunctionName;
            var hullShaderName = description.HullShaderFunctionName;
            var domainShaderName = description.DomainShaderFunctionName;

            bool hasVertexShader = !string.IsNullOrEmpty(vertexShaderName);
            bool hasPixelShader = !string.IsNullOrEmpty(pixelShaderName);
            bool hasGeometryShader = !string.IsNullOrEmpty(geometryShaderName);
            bool hasHullShader = !string.IsNullOrEmpty(hullShaderName);
            bool hasDomainShader = !string.IsNullOrEmpty(domainShaderName);

            // 디스크립션 내용에 따라 쉐이더 컴파일하여 프로퍼티에 넣음.
            if (hasVertexShader)    // 버텍스
            {
                var vertexShaderByteCode = ShaderBytecode.CompileFromFile(filename, vertexShaderName, "vs_5_0", include: new StandardIncludeHandler());
                VertexShader = new VertexShader(Device.Device, vertexShaderByteCode);

                // 인풋 레이아웃 (리플렉션으로 레이아웃엘레먼트 생성)
                InputLayout = new InputLayout(Device.Device,
                    ShaderSignature.GetInputSignature(vertexShaderByteCode),
                    ShaderFactory.CreateInputElement(new ShaderReflection(vertexShaderByteCode)));
                _VSconstantBuffers = ShaderFactory.CreateConstantBuffer(device, new ShaderReflection(vertexShaderByteCode), EShaderType.VertexShader);
            }
            if (hasPixelShader)     // 픽셀
            {
                var pixelShaderByteCode = ShaderBytecode.CompileFromFile(filename, pixelShaderName, "ps_5_0", include: new StandardIncludeHandler());
                PixelShader = new PixelShader(Device.Device, pixelShaderByteCode);
                _PSconstantBuffers = ShaderFactory.CreateConstantBuffer(device, new ShaderReflection(pixelShaderByteCode), EShaderType.PixelShader);
            }
            if (hasGeometryShader)  // 지오메트리
            {
                var geometryShaderByteCode = ShaderBytecode.CompileFromFile(filename, geometryShaderName, "gs_5_0", include: new StandardIncludeHandler());
                if (description.GeometrySO == null)
                {
                    GeometryShader = new GeometryShader(Device.Device, geometryShaderByteCode);
                }
                else
                {
                    int[] size = new int[] { description.GeometrySO.Select(e => e.ComponentCount * 4).Sum() };
                    GeometryShader = new GeometryShader(Device.Device, geometryShaderByteCode, description.GeometrySO, size, -1);
                }
                _GSconstantBuffers = ShaderFactory.CreateConstantBuffer(device, new ShaderReflection(geometryShaderByteCode), EShaderType.GeometryShader);
            }
            if (hasHullShader)      // 헐
            {
                var hullShaderByteCode = ShaderBytecode.CompileFromFile(filename, hullShaderName, "hs_5_0", include: new StandardIncludeHandler());
                HullShader = new HullShader(Device.Device, hullShaderByteCode);
                _HSconstantBuffers = ShaderFactory.CreateConstantBuffer(device, new ShaderReflection(hullShaderByteCode), EShaderType.HullShader);
            }
            if (hasDomainShader)    // 도메인
            {
                var domainShaderByteCode = ShaderBytecode.CompileFromFile(filename, domainShaderName, "ds_5_0", include: new StandardIncludeHandler());
                DomainShader = new DomainShader(Device.Device, domainShaderByteCode);
                _DSconstantBuffers = ShaderFactory.CreateConstantBuffer(device, new ShaderReflection(domainShaderByteCode), EShaderType.DomainShader);
            }

        }

        // Todo : 상수버퍼 클래스 만들고, 리플렉션으로 생성하도록 하자.
        public Buffer CreateBuffer<T>() where T : struct
        {
            return new Buffer(Device.Device, Utilities.SizeOf<T>(), ResourceUsage.Default, BindFlags.ConstantBuffer, CpuAccessFlags.None, ResourceOptionFlags.None, 0);
        }

        // 쉐이더를 적용할 때 호출자에서 불러주면 OK
        public void Apply(RenderData_PerObject objectCdata, RenderData_PerFrame frameCData)
        {
            if (VertexShader != null)
            {
                foreach (var cb in _VSconstantBuffers!)
                    BindingBuffer(Device, objectCdata, frameCData, cb);
                Device.DeviceContext.InputAssembler.InputLayout = InputLayout;
                Device.DeviceContext.VertexShader.Set(VertexShader);
            }
            if (PixelShader != null)
            {
                foreach (var cb in _PSconstantBuffers!)
                    BindingBuffer(Device, objectCdata, frameCData, cb);
                Device.DeviceContext.PixelShader.Set(PixelShader);
            }
            if (GeometryShader != null)
            {
                foreach (var cb in _GSconstantBuffers!)
                    BindingBuffer(Device, objectCdata, frameCData, cb);
                Device.DeviceContext.GeometryShader.Set(GeometryShader);
            }

            if (DomainShader != null)
            {
                foreach (var cb in _DSconstantBuffers!)
                    BindingBuffer(Device, objectCdata, frameCData, cb);
                Device.DeviceContext.DomainShader.Set(DomainShader);
            }
            if (HullShader != null)
            {
                foreach (var cb in _HSconstantBuffers!)
                    BindingBuffer(Device, objectCdata, frameCData, cb);
                Device.DeviceContext.HullShader.Set(HullShader);
            }

        }

        // 버퍼의 생성 클래스 이름을 찾아서 해당 데이터를 바인딩 & 변경점을 적용한다.
        public static void BindingBuffer(DxDevice device, RenderData_PerObject objectCdata, RenderData_PerFrame frameCData, IDxConstantBuffer cbuffer)
        {
            switch (cbuffer.GetTypeName())
            {
                case "CB_Matrix":
                    var cbr = (DxConstantBuffer<CB_Matrix>)cbuffer;
                    cbr.Data = objectCdata.cb_Matrix;
                    break;
                case "CB_ViewPos":
                    var viewPos = (DxConstantBuffer<CB_ViewPos>)cbuffer;
                    viewPos.Data = frameCData.cb_ViewPos;
                    break;
                case "CB_FxDirectionalLight":
                    var dLight = (DxConstantBuffer<CB_FxDirectionalLight>)cbuffer;
                    dLight.Data = frameCData.cb_FxDirectionalLight;
                    break;
                case "CB_FxPointLight":
                    var pLight = (DxConstantBuffer<CB_FxPointLight>)cbuffer;
                    pLight.Data = frameCData.cb_FxPointLight;
                    break;
                case "CB_FxSpotLight":
                    var sLight = (DxConstantBuffer<CB_FxSpotLight>)cbuffer;
                    sLight.Data = frameCData.cb_FxSpotLight;
                    break;
                case "LegacyLight":
                    var objLight = (DxConstantBuffer<LegacyLight>)cbuffer;
                    objLight.Data = objectCdata.cb_objLegacyLight;
                    break;
                case "LegacyReflect":
                    var objReflect = (DxConstantBuffer<LegacyReflect>)cbuffer;
                    objReflect.Data = objectCdata.cb_objLegacyReflect;
                    break;
                case "CB_TerrainData":
                    {
                        var terrain = (DxConstantBuffer<CB_TerrainData>)cbuffer;
                        CB_TerrainData data = new CB_TerrainData
                        {
                            LightDirection = frameCData.cb_FxDirectionalLight.Direction,
                            CameraPosition = frameCData.cb_ViewPos.ViewPosition,
                            pad1 = 0.0f,
                            pad2 = 0.0f
                        };
                        terrain.Data = data;
                        break;
                    }

            }
            cbuffer.ApplyChanges(device.DeviceContext);
        }

        // 쉐이더를 해제할 때 불러주면 OK
        public void Clear()
        {
            Device.DeviceContext.VertexShader.Set(null);
            Device.DeviceContext.PixelShader.Set(null);
            Device.DeviceContext.GeometryShader.Set(null);
            Device.DeviceContext.DomainShader.Set(null);
            Device.DeviceContext.HullShader.Set(null);
        }

        public void Dispose()
        {
            VertexShader?.Dispose();
            PixelShader?.Dispose();
            GeometryShader?.Dispose();
            DomainShader?.Dispose();
            HullShader?.Dispose();
            InputLayout?.Dispose();
        }
    }
}
