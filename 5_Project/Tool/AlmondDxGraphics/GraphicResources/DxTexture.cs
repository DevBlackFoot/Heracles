using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX.Direct3D11;

namespace AlmondDxGraphics.GraphicResources
{
    public class DxTexture : IDResourceBase
    {
        public ShaderResourceView Texture { get; private set; }
        public TEXTURE_TYPE TextureType { get; private set; }

        public DxTexture(string name, ShaderResourceView texture, TEXTURE_TYPE type) : base(name, RESOURCE_TYPE.TEXTURE)
        {
            TextureType = type;
            Texture = texture;
        }        

    }
    public enum TEXTURE_TYPE
    {
        Diffuse,
        Normal,
        Height
    }
}
