using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX.Windows;
using ImGuiNET;

namespace AlmondDxGraphics.DXResources
{
    public class DxRenderControl : RenderControl
    {
        private bool _isUseImGui;

        public bool IsUseImGui { get { return _isUseImGui; } }

        public DxRenderControl(bool isUseImGui)
        {
            _isUseImGui = isUseImGui;
        }


        protected override unsafe void WndProc(ref Message m)
        {
            base.WndProc(ref m);
            if (_isUseImGui)
            {                
                ImGui.ImGui_ImplWin32_WndProcHandler(Handle.ToPointer(), (uint)m.Msg, m.WParam, m.LParam);
            }
        }
    }
}
