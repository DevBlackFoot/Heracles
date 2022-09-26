using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using AlmondToolEngine.SceneElements.Scenes;

namespace AlmondToolEngine.SceneElements.Components.Base
{
    public abstract class RenderComp : Component
    {
        protected Transform? _transform;

        //protected RenderComp()
        //{            
        //}

        public override void Awake(SceneBase scene) 
        {
            base.Awake(scene);
            _transform = GameOBJ!.GetComponent<Transform>()!;
            scene.RegisterRenderComp(this);
        }

        public abstract void Render();        

    }
}
