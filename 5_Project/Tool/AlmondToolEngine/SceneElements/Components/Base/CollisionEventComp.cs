using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AlmondToolEngine.SceneElements.Components.Base
{
    // 이벤트나 컨트롤에 사용하는 컴포넌트. 얘네는 온오프가 가능하다.
    public abstract class CollisionEventComp : Component
    {
        public bool ISEnable { get; set; }

        public CollisionEventComp()
        {
            ISEnable = true;
        }

        public abstract void OnColTriggerEnter(ColliderComp other);
        public abstract void OnColTriggerStay(ColliderComp other);
        public abstract void OnColTriggerExit(ColliderComp other);
    }
}
