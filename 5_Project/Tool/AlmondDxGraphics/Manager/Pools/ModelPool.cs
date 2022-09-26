using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using AlmondDxGraphics.GraphicResources;
using AlmondDxGraphics.DXResources;
using AlmondDxGraphics.GraphicResources.Data;
using AlmondDxGraphics.GraphicResources.Factories;

namespace AlmondDxGraphics.Manager.Pools
{
    public class ModelPool : IDisposable
    {
        private Dictionary<ulong, DxModel> _models;
        private Dictionary<string, ulong> _uniqueIds;
        private TexturePool _texturePool;

        public ModelPool(TexturePool texpool)
        {
            _models = new Dictionary<ulong, DxModel>();
            _uniqueIds = new Dictionary<string, ulong>();
            _texturePool = texpool;
        }

        public void AddModel(DxDevice device, ModelData data, List<DxShader> shaders)
        {
            // 임포터에서 modelData에 이름 들어가는거 재확인. 유니크ID 터뜨리지 않나? -> 안터뜨림. 해결.
            DxModel model = new DxModel(device, _texturePool, data);
            foreach (var m in model.Meshes)
            {
                m.Shaders = shaders;
            }
            _models.Add(model.ResourceUniqueID, model);
            _uniqueIds.Add(model.Name, model.ResourceUniqueID);
        }

        public DxModel? GetModel(ulong resourceID)
        {
            return _models.ContainsKey(resourceID) ? _models[resourceID] : null;
        }

        public DxModel? GetModel(string modelName)
        {
            var resourceId = _uniqueIds.ContainsKey(modelName) ? _uniqueIds[modelName] : ulong.MaxValue;
            return resourceId != ulong.MaxValue ? GetModel(resourceId) : null;
        }


        public void Dispose()
        {
            _models.Clear();
            _uniqueIds.Clear();
        }
    }
}
