using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;
using AlmondDxGraphics.DXResources;

namespace AlmondDxGraphics.GraphicResources.Data
{
    public class RenderingQuary
    {        
        public Matrix World;
        public Matrix WorldInvTranspose;
        public Matrix WorldViewProjection;
        public Vector3 CameraPosition;
        public CB_FxDirectionalLight DirectionalLight;
    }

    // 그릴 때 Constant Buffer에 들어갈 내용이 모두 여기에 있다.
    public class RenderData_PerObject
    {
        public CB_Matrix cb_Matrix;        
        public CB_PaletteMatrices cb_PaletteMatrices;
        public LegacyLight cb_objLegacyLight;
        public LegacyReflect cb_objLegacyReflect;
    }

    public class RenderData_PerFrame
    {
        public CB_ViewPos cb_ViewPos;
        public CB_FxDirectionalLight cb_FxDirectionalLight;
        public CB_FxPointLight cb_FxPointLight;
        public CB_FxSpotLight cb_FxSpotLight;
    }
}
