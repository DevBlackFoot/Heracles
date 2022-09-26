using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AlmondDxGraphics.GraphicResources.Data;
using SharpDX;
using SharpDX.Direct3D11;
using AlmondDxGraphics.DXResources;
using AlmondDxGraphics.Manager.Pools;
using AlmondDxGraphics.GraphicResources.Factories;

namespace AlmondDxGraphics.GraphicResources
{
    public class DxMaterial
    {
        public Vector4 Ambient { get; set; }
        public Vector4 Diffuse { get; set; }
        public Vector4 Specular { get; set; }
        public float SpecularDensity { get; set; }

        public Vector4 Emissive { get; set; }

        public string DiffuseTexturePath { get; set; }
        public string? NormalTexturePath { get; set; }

        public string? HeightTexturePath { get; set; }

        // 쉐이더 리소스 포인터
        public ShaderResourceView? DiffuseTexture { get; set; }
        public ShaderResourceView? NormalTexture { get; set; }

        public ShaderResourceView? HeightTexture { get; set; }

        public DxMaterial(MaterialData material)
        {
            Diffuse = material.Diffuse;
            Ambient = material.Ambient;
            Specular = material.Specular;
            SpecularDensity = material.SpecularDensity;
            Emissive = material.Emissive;

            DiffuseTexturePath = material.DiffuseTextureFilePath;
            NormalTexturePath = material.NormalTextureFilePath;           

        }

        public void LoadTexture(DxDevice dxDevice, TexturePool pool)
        {
            if (DiffuseTexturePath != "None")
            {
                DiffuseTexture = pool.LoadTexture(dxDevice, DiffuseTexturePath, TEXTURE_TYPE.Diffuse)!.Texture;                
            }                
            if (NormalTexturePath != "None")
            {
                NormalTexture = pool.LoadTexture(dxDevice, NormalTexturePath, TEXTURE_TYPE.Normal)!.Texture;
            }

        }


    }
}
