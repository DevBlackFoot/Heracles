using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;

using AlmondToolEngine.SceneElements.Components.Base;

namespace AlmondToolEngine.SceneElements.Components
{
    public class ColliderSphere : ColliderComp
    {
        public Vector3 LocalCenter { get; set; }
        public Vector3 WorldCenter { get; internal set; }
        public float Radius { get; set; }

        public override bool CheckCollision(ColliderComp other)
        {
            return false;
        }
    }
}
