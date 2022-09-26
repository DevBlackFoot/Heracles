using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX.Direct3D11;
using SharpDX.DXGI;
using SharpDX.Windows;
using Buffer11 = SharpDX.Direct3D11.Buffer;

using ImGuiNET;

using AlmondDxGraphics.DXResources;
using AlmondDxGraphics.Manager;

namespace AlmondDxGraphics
{
    public class AlRenderer : IDisposable
    {
        // 윈폼 컨트롤에 그릴 때 필요한 DirectX 리소스의 래퍼 인스턴스들        
        private DxDevice _device;
        private DxStateDepthStencil _dxDepthStencilState;
        private DxStateBlend _dxBlendState;
        private DxStateSampler _dxSamplerState;

        // Todo: 포스트프로세싱 추가시, 멀티랜더 타겟이 필요할 때 그때 늘리고 리팩토링할 예정.
        private DxRenderTarget? _dxRenderTarget;

        private DxResourceManager _dxResourceManager;

        // 랜더할 대상인 윈폼 유저 컨트롤
        public DxRenderControl ControlView { get; private set; }
        public DxResourceManager GraphicResources { get { return _dxResourceManager; } }

        // 리사이즈 필요성을 지시해 줌. 이게 True면, Resize() 메소드 실행 필요. (호출자에서 체크해서 사용할 것)
        public bool MustResize { get; private set; }

        // 임시. 원랜 이렇게 쓸일은 없을거임.
        public DxDevice Device { get { return _device; } }

        public AlRenderer(DxRenderControl winformUserControl)
        {
            ControlView = winformUserControl;

            _device = new DxDevice(winformUserControl);
            _dxDepthStencilState = new DxStateDepthStencil(_device.Device);
            _dxBlendState = new DxStateBlend(_device.Device);
            _dxSamplerState = new DxStateSampler(_device.Device);
            _dxResourceManager = new DxResourceManager(_device, winformUserControl.Name);

            ControlView.Resize += (sender, args) => MustResize = true;
            Resize();
        }

        public unsafe void ImguiSetUp()
        {
            if (ControlView.IsUseImGui)
            {
                ImGui.CreateContext();
                ImGui.ImGui_ImplWin32_Init(ControlView.Handle);
                ImGui.ImGui_ImplDX11_Init((void*)_device.Device.NativePointer, (void*)_device.DeviceContext.NativePointer);
                ImGui.ImGui_ImplDX11_CreateDeviceObjects();
                ImGui.StyleColorsDark();
            }

        }

        public void SetZbuffer(bool ontrue)
        {
            if (ontrue == true)
                _dxDepthStencilState.On();
            else
                _dxDepthStencilState.Off();
        }
        public void SetBlend(BlendOperation operation, BlendOption source, BlendOption destination)
        {
            _dxBlendState.SetBlend(operation, source, destination);
        }

        public void Clear(SharpDX.Color color)
        {
            _dxRenderTarget?.Clear(color);
        }

        // 컨트롤 사이즈가 바뀔 때, 사이즈와 관련있는 리소스들을 재설정해준다.
        public void Resize()
        {
            if (ControlView.ClientSize.Width == 0 || ControlView.ClientSize.Height == 0)
                return;

            _device.ResizeSwapChainBuffer(ControlView);

            _dxRenderTarget = new DxRenderTarget(_device, ControlView.ClientSize.Width, ControlView.ClientSize.Height, Format.R8G8B8A8_UNorm);
            _dxRenderTarget.Apply();

            // Get the backbuffer from the swapchain
            var _backBufferTexture = _device.SwapChain.GetBackBuffer<Texture2D>(0);

            // Backbuffer
            _dxRenderTarget.Target = new RenderTargetView(_device.Device, _backBufferTexture);
            _backBufferTexture.Dispose();

            MustResize = false;
        }

        public void BeginRender()
        {
            if (ControlView.IsUseImGui)
            {
                ImGui.ImGui_ImplDX11_NewFrame();
                ImGui.ImGui_ImplWin32_NewFrame();
                ImGui.NewFrame();
            }
        }

        public void Present()
        {
            if (ControlView.IsUseImGui)
            {
                ImGui.Render();
                ImGui.ImGui_ImplDX11_RenderDrawData(ImGui.GetDrawData());
            }

            // 임시. 나중에 호출자로 옮겨준다.
            UpdateAllStates();
            _dxRenderTarget.Apply();

            _device.Present();
        }

        public void UpdateAllStates()
        {
            _device.DeviceContext.OutputMerger.SetDepthStencilState(_dxDepthStencilState.State);
            _device.DeviceContext.OutputMerger.SetBlendState(_dxBlendState.State);
            _device.DeviceContext.PixelShader.SetSampler(0, _dxSamplerState.State);
        }

        public void UpdateData<T>(Buffer11 buffer, T data) where T : struct
        {
            _device.DeviceContext.UpdateSubresource(ref data, buffer);
        }

        public void Dispose()
        {
            _device.Dispose();
            _dxDepthStencilState.Dispose();
            _dxBlendState.Dispose();
            _dxSamplerState.Dispose();
            _dxRenderTarget?.Dispose();
        }

    }
}
