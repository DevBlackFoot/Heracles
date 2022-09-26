using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;
using AlmondToolEngine.SceneElements.Components;
using AlmondToolEngine.SceneElements.Objects;
using AlmondDxGraphics;
using AlmondDxGraphics.DXResources;

namespace AlmondToolEngine.SceneElements.Factories
{
    public static class GameObjectFactory
    {
        public static GameObject CreateEmpty(string name)
        {
            var obj = new GameObject(name);
            obj.AddComponent<Transform>();  // 모든 오브젝트는 Transform이 있다..            

            return obj;
        }

        public static GameObject CreateCamera(string name, AlRenderer targetRenderer)
        {
            var obj = CreateEmpty(name);
            obj.AddComponent<Camera>();
            obj.GetComponent<Camera>()!.Initialize(targetRenderer);
            obj.AddComponent<ColliderRay>();

            return obj;
        }

        public static GameObject CreateDirLight(string name, AlRenderer targetRenderer)
        {
            var obj = CreateEmpty(name);
            obj.AddComponent<DirectionalLight>();
            obj.GetComponent<DirectionalLight>()!.LegacyLightInfo = new LegacyLight
            {
                Ambient = new Vector4(0.2f, 0.2f, 0.2f, 1.0f),
                Diffuse = new Vector4(0.5f, 0.5f, 0.5f, 1.0f),
                Specular = new Vector4(0.3f, 0.3f, 0.3f, 5.0f)
            };
            obj.GetComponent<DirectionalLight>()!.Direction = new Vector3(0.58f, 0.58f, 0.58f);
            obj.GetComponent<DirectionalLight>()!.SpecularPower = 1.0f;

            return obj;
        }

        public static GameObject CreateTerrainLand(string name)
        {
            var obj = CreateEmpty(name);
            obj.AddComponent<TerrainRenderer>();
            obj.GetComponent<Transform>()!.SetRotation(new Vector3(90.0f, 0.0f, 0.0f));
            return obj;
        }

        public static GameObject CreateSkyBox(string name)
        {
            var obj = CreateEmpty(name);
            obj.AddComponent<SkyBoxRenderer>();            
            return obj;
        }
    }
}
