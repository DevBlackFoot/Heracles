using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SharpDX;
using SharpDX.Direct3D11;

namespace AlmondDxGraphics.DXResources
{
    internal class DxStateSampler : IDisposable
    {
        private SamplerState _samplerState;

        public SamplerState State { get { return _samplerState; } }

        public DxStateSampler(Device device)
        {
            SamplerStateDescription description = SamplerStateDescription.Default();
            description.Filter = Filter.MinMagMipLinear;
            description.AddressU = TextureAddressMode.Wrap;
            description.AddressV = TextureAddressMode.Wrap;            
            _samplerState = new SamplerState(device, description);
        }

        public void Dispose()
        {
            _samplerState.Dispose();
        }

    }
}
