using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using AlmondDxGraphics.GraphicResources;
using AlmondDxGraphics.DXResources;
using AlmondDxGraphics.Manager.Pools;

namespace AlmondDxGraphics.Manager
{
    using Device11 = SharpDX.Direct3D11.Device;

    public class DxResourceManager : IDisposable
    {
        private DxDevice _dxDevice;

        public ModelPool Models { get; private set; }
        public TexturePool Textures { get; private set; }

        public ShaderPool Shaders { get; private set; }

        private string _renderControlName;

        public DxResourceManager(DxDevice device, string renderControlName)
        {
            Textures = new TexturePool();
            Models = new ModelPool(Textures);
            Shaders = new ShaderPool();

            _dxDevice = device;
            _renderControlName = renderControlName;

            CreateBasePrimitiveResource();
        }

        private void CreateBasePrimitiveResource()
        {          

            Shaders.AddShader(_dxDevice, "Resources/ShaderResources/Default_StaticVS.hlsl", ShaderType.VertexShader);
            Shaders.AddShader(_dxDevice, "Resources/ShaderResources/Default_StaticFbxPS.hlsl", ShaderType.PixelShader);
            Shaders.AddShader(_dxDevice, "Resources/ShaderResources/Default_StaticObjPS.hlsl", ShaderType.PixelShader);
            Shaders.AddShader(_dxDevice, "Resources/ShaderResources/Default_PrimitivePS.hlsl", ShaderType.PixelShader);
            if (_renderControlName == "MainScene")
            {
                Shaders.AddShader(_dxDevice, "Resources/ShaderResources/TerrainVS.hlsl", ShaderType.VertexShader);
                Shaders.AddShader(_dxDevice, "Resources/ShaderResources/TerrainHS.hlsl", ShaderType.HullShader);
                Shaders.AddShader(_dxDevice, "Resources/ShaderResources/TerrainDS.hlsl", ShaderType.DomainShader);
                Shaders.AddShader(_dxDevice, "Resources/ShaderResources/TerrainPS.hlsl", ShaderType.PixelShader);
                Shaders.AddShader(_dxDevice, "Resources/ShaderResources/SkyBoxVS.hlsl", ShaderType.VertexShader);
                Shaders.AddShader(_dxDevice, "Resources/ShaderResources/SkyBoxPS.hlsl", ShaderType.PixelShader);
            }

        }

        public void Dispose()
        {
            Models.Dispose();
            Textures.Dispose();
        }


    }
}
