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
    // Adaptive Tessalation을 이용해서 지형을 그림.
    public class DxTerrain
    {
        private DxDevice _device;
        private DxModel _terrainModel;

        //private ShaderResourceView _diffuseMap;
        //private ShaderResourceView _normalMap;
        private ShaderResourceView _heightMap;

        public DxTerrain()
        {
            
        }

        public void Initialize(DxDevice device, TexturePool tPool, ShaderPool sPool, string diffuseTexPath, string normalTexPath, string heightTexPath)
        {
            _device = device;

            _terrainModel = new DxModel(device, tPool, ModelDataFactory.CreateGridTerrainModelData(device, 64, 64, 80, diffuseTexPath, normalTexPath, heightTexPath));
            var vS = sPool.GetShader("TerrainVS.hlsl");
            var hS = sPool.GetShader("TerrainHS.hlsl");
            var dS = sPool.GetShader("TerrainDS.hlsl");
            var pS = sPool.GetShader("TerrainPS.hlsl");
            List<DxShader> terrainShaders = new List<DxShader>();
            if (vS != null && hS != null && dS != null && pS != null)
            {
                terrainShaders.Add(vS);
                terrainShaders.Add(hS);
                terrainShaders.Add(dS);
                terrainShaders.Add(pS);
            }

            foreach (var m in _terrainModel.Meshes)
            {
                m.Shaders = terrainShaders;
                m.SetRasterizerState(RSState.SOLID);
            }
        }

        //public void LoadDiffuseMap(TexturePool pool, string diffuseMapPath)
        //{
        //    _diffuseMap = pool.LoadTexture(_device, diffuseMapPath, TEXTURE_TYPE.Diffuse)!.Texture;
        //    foreach (var m in _terrainModel.Meshes)
        //    {
        //        m.Material.DiffuseTexturePath = diffuseMapPath;
        //        m.Material.DiffuseTexture = _diffuseMap;                
        //    }
        //}

        //public void LoadNormalMap(TexturePool pool, string normalMapPath)
        //{
        //    _normalMap = pool.LoadTexture(_device, normalMapPath, TEXTURE_TYPE.Normal)!.Texture;
        //    foreach (var m in _terrainModel.Meshes)
        //    {
        //        m.Material.NormalTexturePath = normalMapPath;
        //        m.Material.NormalTexture = _normalMap;
        //    }
        //}

        public void LoadHeightMap(TexturePool pool, string heightMapPath)
        {
            _heightMap = pool.LoadTexture(_device, heightMapPath, TEXTURE_TYPE.Height)!.Texture;
            foreach (var m in _terrainModel.Meshes)
            {
                m.Material.HeightTexturePath = heightMapPath;
                m.Material.HeightTexture = _heightMap;
            }
        }

        public void Draw(RenderingQuary renderingQuary)
        {         
            _terrainModel.DrawPatch(renderingQuary);
        }


    }
}
