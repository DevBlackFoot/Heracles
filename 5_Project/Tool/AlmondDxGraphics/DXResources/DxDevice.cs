using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
// 참조로 WindowsFormDll을 추가해줬다...없으면 내부변수에 접근이 불가하다.
using SharpDX;
using SharpDX.Direct3D;
using SharpDX.Direct3D11;
using SharpDX.DXGI;
using SharpDX.Windows;

namespace AlmondDxGraphics.DXResources
{


    // DXGI 쪽이랑 겹침. 분리해준다.
    using Device11 = SharpDX.Direct3D11.Device;

    // 디바이스는 사라지는 시점이 명확하므로 IDisposable을 받아 통해 수동 해제한다.
    public class DxDevice : IDisposable
    {
        // 디바이스와 컨텍스트 및 스왑체인. 이 인스턴스는 이 3개 리소스를 관리한다.
        private Device11 _device;
        private DeviceContext _deviceContext;
        private SwapChain _swapChain;               

        // 리소스 접근용 프로퍼티
        public Device11 Device { get { return _device; } }
        public DeviceContext DeviceContext { get { return _deviceContext; } }
        public SwapChain SwapChain { get { return _swapChain; } }


        public DxDevice(RenderControl userControl, bool debug = false)
        {      
           
            FeatureLevel[] levels = new FeatureLevel[] { FeatureLevel.Level_11_1 };            

            // 스왑체인 디스크립션. 유저 컨트롤에서 핸들을 가져와서 만든다.
            var desc = new SwapChainDescription()
            {
                BufferCount = 1,//buffer count
                ModeDescription = new ModeDescription(userControl.ClientSize.Width, userControl.ClientSize.Height, new Rational(60, 1), Format.R8G8B8A8_UNorm),//sview
                IsWindowed = true,
                OutputHandle = userControl.Handle,
                SampleDescription = new SampleDescription(1, 0),
                SwapEffect = SwapEffect.Discard,
                Usage = Usage.RenderTargetOutput
            };

            // 이제 스왑체인과 디바이스를 만든다.
            DeviceCreationFlags flag = DeviceCreationFlags.None | DeviceCreationFlags.BgraSupport;
            if (debug)
                flag = DeviceCreationFlags.Debug;            

            
            Device11.CreateWithSwapChain(DriverType.Hardware, flag, levels, desc, out _device, out _swapChain);
            

            // 만들어진 디바이스로부터 디바이스 컨텍스트를 받아온다.
            _deviceContext = Device.ImmediateContext;

            // 윈도우 이벤트를 무시한다.
            var factory = SwapChain.GetParent<Factory>();
            factory.MakeWindowAssociation(userControl.Handle, WindowAssociationFlags.IgnoreAll);

        }

        public void ResizeSwapChainBuffer(RenderControl userControl)
        {
            _swapChain.ResizeBuffers(1, userControl.ClientSize.Width, userControl.ClientSize.Height, Format.R8G8B8A8_UNorm, SwapChainFlags.None);           

        }

        public void Present()
        {
            _swapChain.Present(1, PresentFlags.None);
            
        }
        

        public void Dispose()
        {
            _device.Dispose();
            _deviceContext.Dispose();
            _swapChain.Dispose();
        }
    }
}
