using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using SharpDX;
using SharpDX.Direct3D11;
using SharpDX.DXGI;

namespace AlmondDxGraphics.DXResources
{
    internal class DxRenderTarget : IDisposable
    {
        private RenderTargetView _backbufferView;
        private DepthStencilView _depthStencilView;
        private ShaderResourceView _resourceView;

        // private필드 접근 프로퍼티
        public RenderTargetView Target { get { return _backbufferView; } set { _backbufferView = value; } }
        public DepthStencilView DepthStencilView { get { return _depthStencilView; } }
        public ShaderResourceView ResourceView { get { return _resourceView; } }

        // Device Pointer
        public DxDevice Device { get; private set; }

        // 랜더타겟의 가로세로 크기 프로퍼티
        public int Width { get; private set; }
        public int Height { get; private set; }

        // 랜더타겟 생성할 때 타겟뷰, 리소스뷰, 댑스스탠실뷰 생성.
        public DxRenderTarget(DxDevice device, int width, int height, Format format)
        {
            Device = device;
            Width = width;
            Height = height;

            // 랜더타겟 생성
            Texture2D target = new Texture2D(device.Device, new Texture2DDescription()
            {
                Format = format,
                Width = width,
                Height = height,
                ArraySize = 1,
                BindFlags = BindFlags.RenderTarget | BindFlags.ShaderResource,
                CpuAccessFlags = CpuAccessFlags.None,
                MipLevels = 1,
                OptionFlags = ResourceOptionFlags.None,
                SampleDescription = new SampleDescription(1, 0),
                Usage = ResourceUsage.Default,
            });

            // 랜더타겟(백버퍼) 뷰, 리소스 뷰 생성
            _backbufferView = new RenderTargetView(device.Device, target);
            _resourceView = new ShaderResourceView(device.Device, target);
            target.Dispose();

            // 댑스 버퍼 생성
            var _zbufferTexture = new Texture2D(device.Device, new Texture2DDescription()
            {
                Format = Format.D16_UNorm,
                ArraySize = 1,
                MipLevels = 1,
                Width = width,
                Height = height,
                SampleDescription = new SampleDescription(1, 0),
                Usage = ResourceUsage.Default,
                BindFlags = BindFlags.DepthStencil,
                CpuAccessFlags = CpuAccessFlags.None,
                OptionFlags = ResourceOptionFlags.None
            });

            // 댑스스탠실 뷰 생성
            _depthStencilView = new DepthStencilView(Device.Device, _zbufferTexture);
            _zbufferTexture.Dispose();

        }

        // 디바이스 컨텍스트에 이 렌더타겟을 적용한다.
        public void Apply()
        {
            Device.DeviceContext.Rasterizer.SetViewport(0, 0, Width, Height);
            Device.DeviceContext.OutputMerger.SetTargets(_depthStencilView, _backbufferView);
        }

        // 백버퍼와 댑스스탠실 버퍼를 클리어한다.
        public void Clear(Color4 color)
        {
            Device.DeviceContext.ClearRenderTargetView(_backbufferView, color);
            Device.DeviceContext.ClearDepthStencilView(_depthStencilView, DepthStencilClearFlags.Depth, 1.0f, 0);
        }

        public void Dispose()
        {
            _backbufferView.Dispose();
            _depthStencilView.Dispose();
            _resourceView.Dispose();
        }
    }
}
