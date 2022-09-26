using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;
using SharpDX.Direct3D11;

namespace AlmondDxGraphics.DXResources
{
    using Device11 = SharpDX.Direct3D11.Device;

    internal class DxStateDepthStencil : IDisposable
    {
        private DepthStencilState _onState;
        private DepthStencilState _offState;
        private bool _presentState;

        public DxStateDepthStencil(Device11 device)
        {
            DepthStencilStateDescription description = DepthStencilStateDescription.Default();
            description.DepthComparison = Comparison.LessEqual;
            description.IsDepthEnabled = true;
            description.FrontFace.FailOperation = StencilOperation.Keep;
            description.FrontFace.DepthFailOperation = StencilOperation.Increment;
            description.FrontFace.PassOperation = StencilOperation.Keep;
            description.FrontFace.Comparison = Comparison.Always;
            description.BackFace.FailOperation = StencilOperation.Keep;
            description.BackFace.DepthFailOperation = StencilOperation.Decrement;
            description.BackFace.PassOperation = StencilOperation.Keep;
            description.BackFace.Comparison = Comparison.Always;

            _onState = new DepthStencilState(device, description);

            description.DepthComparison = Comparison.LessEqual;
            description.IsDepthEnabled = false;
            _offState = new DepthStencilState(device, description);

            // 일단 처음 상태는 댑스스탠실 On 상태
            _presentState = true;
        }

        public DepthStencilState State { get { return (_presentState == true) ? _onState : _offState; } }

        public void On() { _presentState = true; }

        public void Off() { _presentState = false; }

        public void Dispose()
        {
            Utilities.Dispose(ref _onState);
            Utilities.Dispose(ref _offState);
        }


    }
}
