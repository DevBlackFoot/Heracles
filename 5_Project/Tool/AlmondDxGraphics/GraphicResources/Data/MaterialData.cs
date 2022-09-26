using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;

namespace AlmondDxGraphics.GraphicResources.Data
{
    public class MaterialData
    {
        public Vector4 Ambient { get; set; }
        public Vector4 Diffuse { get; set; }
        public Vector4 Specular { get; set; }
        public float SpecularDensity { get; set; }

        public Vector4 Emissive { get; set; }

        public string DiffuseTextureFilePath { get; set; }
        public string NormalTextureFilePath { get; set; }
        public string? HeightTextureFilePath { get; set; }

        public MaterialData()
        {
            Ambient = new Vector4(0, 0, 0, 0);
            Diffuse = new Vector4();
            Specular = new Vector4(0, 0, 0, 0);
            SpecularDensity = 0;
            Emissive = new Vector4();

            DiffuseTextureFilePath = "None";
            NormalTextureFilePath = "None";
        }

    }
}
