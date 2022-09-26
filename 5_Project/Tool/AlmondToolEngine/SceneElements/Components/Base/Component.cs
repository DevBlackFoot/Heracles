using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using AlmondToolEngine.SceneElements.Scenes;
using AlmondToolEngine.SceneElements.Objects;

namespace AlmondToolEngine.SceneElements.Components.Base
{
    public abstract class Component : IObject
    {                       
        public GameObject? GameOBJ { get; set; }
               

        public virtual void Awake(SceneBase scene) { }
        public virtual void OnEnable() { }
        public virtual void OnDisable() { }
        public virtual void OnDestroy() { }
        public virtual void PreUpdate() { }
        public virtual void Update() { }
        public virtual void LateUpdate() { }
    }

}
