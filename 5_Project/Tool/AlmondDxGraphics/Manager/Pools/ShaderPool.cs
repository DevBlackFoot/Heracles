using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using AlmondDxGraphics.GraphicResources;
using AlmondDxGraphics.DXResources;
using AlmondDxGraphics.GraphicResources.Factories;

namespace AlmondDxGraphics.Manager.Pools
{
    public enum ShaderType
    {
        VertexShader,
        PixelShader,
        GeometryShader,
        HullShader,
        DomainShader
    }

    public class ShaderPool : IDisposable
    {
        private Dictionary<ulong, DxShader> _shaders;
        private Dictionary<string, ulong> _uniqueIds;

        public ShaderPool()
        {
            _shaders = new Dictionary<ulong, DxShader>();
            _uniqueIds = new Dictionary<string, ulong>();
        }

        public void AddShader(DxDevice device, string filePath, ShaderType type)
        {
            if (_uniqueIds.ContainsKey(Path.GetFileName(filePath)))
                return;

            DxShader? shader = null;

            switch (type)
            {
                case ShaderType.VertexShader:
                    shader = ShaderFactory.CompileVertexShader(device, filePath);
                    break;

                case ShaderType.PixelShader:
                    shader = ShaderFactory.CompilePixelShader(device, filePath);
                    break;
                case ShaderType.HullShader:
                    shader = ShaderFactory.CompileHullShader(device, filePath);
                    break;
                case ShaderType.DomainShader:
                    shader = ShaderFactory.CompileDomainShader(device, filePath);
                    break;
            }

            if (shader != null)
            {
                _shaders.Add(shader.ResourceUniqueID, shader);
                _uniqueIds.Add(Path.GetFileName(filePath), shader.ResourceUniqueID);
            }
        }

        public DxShader? GetShader(ulong resourceID)
        {
            return _shaders.ContainsKey(resourceID) ? _shaders[resourceID] : null;
        }

        public DxShader? GetShader(string shaderName)
        {
            var resourceId = _uniqueIds.ContainsKey(shaderName) ? _uniqueIds[shaderName] : ulong.MaxValue;
            return resourceId != ulong.MaxValue ? GetShader(resourceId) : null;
        }

        public void Dispose()
        {
            _shaders.Clear();
            _uniqueIds.Clear();
        }
    }
}
