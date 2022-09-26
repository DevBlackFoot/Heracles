using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// using SharpDX;
using SharpDX.Windows;

namespace AlmondToolEngine.Manager
{
    public class MouseTracker
    {
        private RenderControl _control;

        private Point _currMousePosition;
        private Point _prevMousePosition;

        public SharpDX.Vector2 CurrMousePosition { get { return new SharpDX.Vector2(_currMousePosition.X, _currMousePosition.Y); } }
        public SharpDX.Vector2 PrevMousePosition { get { return new SharpDX.Vector2(_prevMousePosition.X, _prevMousePosition.Y); } }


        public MouseTracker(RenderControl control)
        {
            _control = control;
            _control.MouseMove += (sender, e) =>
            {
                _prevMousePosition = _currMousePosition;
                _currMousePosition = e.Location;
            };
              
        }        
    }
}
