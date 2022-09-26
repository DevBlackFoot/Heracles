using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SharpDX;
using SharpDX.Direct3D11;

namespace AlmondDxGraphics.DXResources
{
    internal class DxStateBlend : IDisposable
    {
        private BlendState _blendState;
        private Device _device;

        public BlendState State { get { return _blendState; } }

        public DxStateBlend(Device device)
        {
            _device = device;
            BlendStateDescription description = BlendStateDescription.Default();
            _blendState = new BlendState(device, description);
        }

        // 현재 blending state를 변경한다.
        public void SetBlend(BlendOperation operation, BlendOption source, BlendOption destination)
        {
            Utilities.Dispose(ref _blendState);
            BlendStateDescription description = BlendStateDescription.Default();

            description.RenderTarget[0].BlendOperation = operation;
            description.RenderTarget[0].SourceBlend = source;
            description.RenderTarget[0].DestinationBlend = destination;
            description.RenderTarget[0].IsBlendEnabled = true;
            _blendState = new BlendState(_device, description);
        }


        public void Dispose()
        {
            _blendState.Dispose();
        }
    }
}
