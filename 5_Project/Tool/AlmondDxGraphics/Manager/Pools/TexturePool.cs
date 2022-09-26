using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using AlmondDxGraphics.DXResources;
using AlmondDxGraphics.GraphicResources;
using AlmondDxGraphics.GraphicResources.Factories;

namespace AlmondDxGraphics.Manager.Pools
{
    public class TexturePool : IDisposable
    {
        Dictionary<ulong, DxTexture> _textures;
        Dictionary<string, ulong> _uniqueIds;

        public TexturePool()
        {
            _textures = new Dictionary<ulong, DxTexture>();
            _uniqueIds = new Dictionary<string, ulong>();
        }

        public DxTexture? LoadTexture(DxDevice device, string filepath, TEXTURE_TYPE type)
        {
            DxTexture? texture = GetTexture(filepath);
            
            if (texture == null)
            {
                AddTexture(device, filepath, type);
                texture = GetTexture(filepath);
            }          
            
            return texture;
        }


        public void AddTexture(DxDevice device, string filepath, TEXTURE_TYPE type)
        {
            // 먼저 이 파일위치의 텍스쳐가 풀에 있는지 체크, 있으면 아무것도 안함.
            if (_uniqueIds.ContainsKey(filepath))
                return;

            DxTexture texture = new DxTexture(
                filepath,
                TextureFactory.LoadTextureFromFile(device, filepath),
                type);

            _textures.Add(texture.ResourceUniqueID, texture);
            _uniqueIds.Add(filepath, texture.ResourceUniqueID);
        }

        public DxTexture? GetTexture(ulong resourceID)
        {
            return _textures.ContainsKey(resourceID) ? _textures[resourceID] : null;
        }

        public DxTexture? GetTexture(string resourcePath)
        {         
            var resourceId = _uniqueIds.ContainsKey(resourcePath) ? _uniqueIds[resourcePath] : ulong.MaxValue;
            return resourceId != ulong.MaxValue ? GetTexture(resourceId) : null;
        }

        public void Dispose()
        {
            _textures.Clear();
            _uniqueIds.Clear();
        }
    }
}
