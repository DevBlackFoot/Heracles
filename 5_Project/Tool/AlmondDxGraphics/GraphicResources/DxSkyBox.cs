using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX.Direct3D11;
using AlmondDxGraphics.DXResources;
using AlmondDxGraphics.GraphicResources.Factories;
using AlmondDxGraphics.Manager.Pools;
using AlmondDxGraphics.GraphicResources.Data;

namespace AlmondDxGraphics.GraphicResources
{
    public class DxSkyBox
    {
        private DxDevice _device;
        private DxModel _SkyBoxModel;
        private ShaderResourceView _cubeMap;

        public void Initialize(DxDevice device, TexturePool tPool, ShaderPool sPool, string cubeMapPath)
        {
            _device = device;
            var boxGeom = ModelDataFactory.CreateSkyBoxModelData(device, 1.0f, 1.0f, 1.0f, cubeMapPath);
            _SkyBoxModel = new DxModel(device, tPool, boxGeom);
            var vS = sPool.GetShader("SkyBoxVS.hlsl");
            var pS = sPool.GetShader("SkyBoxPS.hlsl");
            List<DxShader> SkyBoxShaders = new List<DxShader>();
            SkyBoxShaders.Add(vS);
            SkyBoxShaders.Add(pS);

            foreach (var m in _SkyBoxModel.Meshes)
            {
                m.Shaders = SkyBoxShaders;
                m.SetRasterizerState(RSState.SOLID);
            }
        }

        public void LoadCubeMap(TexturePool pool, string cubeMapPath)
        {
            _cubeMap = pool.LoadTexture(_device, cubeMapPath, TEXTURE_TYPE.Diffuse)!.Texture;
            foreach (var m in _SkyBoxModel.Meshes)
            {
                m.Material.DiffuseTexturePath = cubeMapPath;
                m.Material.DiffuseTexture = _cubeMap;
            }
        }

        public void Draw(RenderingQuary renderingQuary)
        {
            _SkyBoxModel.Draw(renderingQuary);
        }

    }
}
