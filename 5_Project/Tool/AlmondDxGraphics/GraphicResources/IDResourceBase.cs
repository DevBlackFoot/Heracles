using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AlmondDxGraphics.GraphicResources
{
    public enum RESOURCE_TYPE
    {
        MODEL,
        TEXTURE,
        SHADER
    }
    public class ResourceIDGenerator
    {
        private static ulong _nextID = 0;
        public static ulong Generate() { return _nextID++; }
    }
    public abstract class IDResourceBase
    {
        public ulong ResourceUniqueID { get; private set; }
        public string Name { get; private set; }
        public RESOURCE_TYPE Type { get; private set; }

        protected IDResourceBase(string name, RESOURCE_TYPE type) 
        {  
            Name = name;
            Type = type;
            ResourceUniqueID = ResourceIDGenerator.Generate();
        }


    }
}
