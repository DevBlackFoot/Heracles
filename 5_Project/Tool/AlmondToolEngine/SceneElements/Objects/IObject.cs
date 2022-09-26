using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using AlmondToolEngine.SceneElements.Scenes;

namespace AlmondToolEngine.SceneElements.Objects
{
    public interface IObject
    {
        void Awake(SceneBase scene); // 이 오브젝트를 갖고있는 씬.
        void OnEnable();
        void OnDisable();
        void OnDestroy();
        void PreUpdate();
        void Update();
        void LateUpdate();
    }
}
