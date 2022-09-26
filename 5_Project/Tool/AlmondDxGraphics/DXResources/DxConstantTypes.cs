using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using SharpDX;

namespace AlmondDxGraphics.DXResources
{      
    [StructLayout(LayoutKind.Sequential)]
    public struct CB_ViewPos
    {
        public Vector3 ViewPosition;
        public float pad;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct CB_Matrix
    {        
        public Matrix World;
        public Matrix WorldInvTranspose;
        public Matrix WorldViewProjection;
    }    

    [StructLayout(LayoutKind.Sequential)]
    public struct CB_PaletteMatrices
    {
        public Matrix[] Pallete;
        
        public CB_PaletteMatrices()
        {
            Pallete = new Matrix[256];
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct LegacyLight
    {
        public Vector4 Ambient;
        public Vector4 Diffuse;
        public Vector4 Specular;

        public LegacyLight()
        {
            Ambient = Vector4.Zero;
            Diffuse = Vector4.Zero;
            Specular = Vector4.Zero;
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct LegacyReflect
    {
        public Vector4 Reflect;
    }


    [StructLayout(LayoutKind.Sequential)]
    public struct CB_TerrainData
    {
        public Vector3 CameraPosition;
        public Vector3 LightDirection;
        public float pad1;
        public float pad2;
    }


    [StructLayout(LayoutKind.Sequential)]
    public struct CB_FxDirectionalLight
    {
        public LegacyLight Light;

        public Vector3 Direction;
        public float power;

        public CB_FxDirectionalLight()
        {
            Light = new LegacyLight();
            Direction = Vector3.One;
            power = 0;
        }

    }

    [StructLayout(LayoutKind.Sequential)]
    public struct CB_FxPointLight
    {
        public LegacyLight Light;

        public Vector3 Position;
        public float Range;
        public Vector3 Att;
        public float pad;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct CB_FxSpotLight
    {
        public LegacyLight Light;

        public Vector3 Position;
        public float Range;

        public Vector3 Direction;
        public float Spot;

        public Vector3 Att;
        public float pad;
    }

}
