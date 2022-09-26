using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using AlmondDxGraphics;

using AlmondToolEngine.Manager;

namespace AlmondToolEngine
{
    public class EngineForWinform
    {
        // 렌더컨트롤을 제어하는 컨트롤매니저 풀. (폼단위로 모은다.)
        private Dictionary<string, RenderControlManagerPool> _rcmPool;


        public EngineForWinform()
        {
            _rcmPool = new Dictionary<string, RenderControlManagerPool>();
        }

        public void CreateRenderControlPool(string name)
        {
            _rcmPool.Add(name, new RenderControlManagerPool(name));
            
        }

        public RenderControlManagerPool GetRenderControlPool(string name)
        {
            return _rcmPool[name];
        }
        

    }
}
