using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;
using AlmondDxGraphics.DXResources;
using AlmondToolEngine.SceneElements.Components.Base;
using AlmondToolEngine.SceneElements.Scenes;

namespace AlmondToolEngine.SceneElements.Components
{
    public class DirectionalLight : Light
    {
        public CB_FxDirectionalLight DirLightInfo {get; internal set;}
        public Vector3 Direction { get; set; }
        public float SpecularPower { get; set; }

        public override void Awake(SceneBase scene)
        {
            base.Awake(scene);            
        }

        public override void Update()
        {
            base.Update();

            // struct라 값복사 해야 값을 리프레시할 수 있다.
            var tmpDirLight = DirLightInfo;
            tmpDirLight.Light = LegacyLightInfo;
            tmpDirLight.Direction = Direction;
            tmpDirLight.power = SpecularPower;

            DirLightInfo = tmpDirLight;

        }



    }
}
