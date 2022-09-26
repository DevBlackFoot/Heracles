using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SharpDX.Direct3D;
using SharpDX.Direct3D11;
using SharpDX.DXGI;
using SharpDX.Windows;

namespace AlmondDxGraphics.DXResources
{
    using Device11 = SharpDX.Direct3D11.Device;

    public enum RSState
    {
        SOLID,
        WIRE_FRAME
    }

    public class DxStateRasterizer : IDisposable
    {
        private RasterizerState _solidState;
        private RasterizerState _wireFrameState;
        private RSState _presentState;

        public DxStateRasterizer(Device11 device)
        {
            RasterizerStateDescription rasterDescription = RasterizerStateDescription.Default();
            rasterDescription.FillMode = FillMode.Solid;
            rasterDescription.CullMode = CullMode.None;
            rasterDescription.IsFrontCounterClockwise = true;
            rasterDescription.IsDepthClipEnabled = true;
            rasterDescription.IsMultisampleEnabled = true;
            _solidState = new RasterizerState(device, rasterDescription);

            rasterDescription.FillMode = FillMode.Wireframe;
            _wireFrameState = new RasterizerState(device, rasterDescription);

            _presentState = RSState.SOLID;
        }

        public RasterizerState State
        {
            get
            {
                switch (_presentState)
                {
                    case RSState.WIRE_FRAME:
                        return _wireFrameState;
                    default:
                        return _solidState;
                }
            }
        }

        // 현재 레스터라이즈 상태를 바꾼다.
        public void SetState(RSState state) {  _presentState = state; }


        public void Dispose()
        {
            _solidState.Dispose();
            _wireFrameState.Dispose();
        }

    }
}
