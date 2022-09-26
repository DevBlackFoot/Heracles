using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Newtonsoft.Json;

namespace AlmondTools_Client.SerializedData.Data
{
    // [JsonProperty()]은 Attribute
    // https://docs.microsoft.com/en-us/dotnet/csharp/programming-guide/concepts/attributes/
    // attribute를 잘 써서 리플렉션을 사용할 수 있다는데... 시간날 때 리팩토링해서 할 수 있게 코드 바꿔봐야할 듯?
    
    public class SceneData
    {
        public int Id { get; set; }
        public List<float>? CameraTransform { get; set; }
        public List<float>? DirLightAmbient { get; set; }
        public List<float>? DirLightDiffuse { get; set; }
        public List<float>? DirLightSpecular { get; set; }
        public List<float>? DirLightDirection { get; set; }
        public string? SkyBoxTexturePath { get; set; }

        public string? MapObjectDataListPath { get; set; }


    }
    
    
    public class MapToolSetting
    {
        [JsonProperty("DefualtShaderPath")]
        public string? DefualtShaderPath { get; set; }

    }
    public class MapObjectData
    {
        [JsonProperty("Id")]
        public int Id { get; set; }

        [JsonProperty("ObjectId")]
        public int ObjectId { get; set; }

        [JsonProperty("GameObjectName")]
        public string? GameObjectName { get; set; }

        [JsonProperty("Transform")]
        public List<float>? Transform { get; set; }
    }

    public class MapObjectPrefabData
    {
        [JsonProperty("Id")]
        public int Id { get; set; }

        [JsonProperty("ObjectName")]
        public string? ObjectName { get; set; }

        [JsonProperty("Shader")]
        public string? Shader { get; set; }

        [JsonProperty("Path")]
        public string? Path { get; set; }

        [JsonProperty("TextureDirectory")]
        public string? TextureDirectory { get; set; }
    }

    
}
