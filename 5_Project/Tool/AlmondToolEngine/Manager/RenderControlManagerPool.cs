using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX.Windows;
using AlmondDxGraphics.DXResources;

namespace AlmondToolEngine.Manager
{
    public class RenderControlManagerPool
    {
        public string Name { get; set; }

        Dictionary<string, RenderControlManager> _sceneManagerForControl;   // 폼에 있는 랜더컨트롤 각각의 씬 매니저들.

        public RenderControlManagerPool(string name)
        {
            _sceneManagerForControl = new Dictionary<string, RenderControlManager>();
            Name = name;    
        }

        public RenderControlManager? CreateRenderControlManager(DxRenderControl winformUserControl)
        {
            if (_sceneManagerForControl.ContainsKey(winformUserControl.Name))
                return null;

            var sceneMananger = new RenderControlManager(winformUserControl);
            _sceneManagerForControl.Add(winformUserControl.Name, sceneMananger);
            return sceneMananger;

        }

        public RenderControlManager? GetManager(DxRenderControl winformUserControl)
        {
            if (!_sceneManagerForControl.ContainsKey(winformUserControl.Name))
                return null;

            return _sceneManagerForControl[winformUserControl.Name];
        }


    }
}
