using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;
using AlmondDxGraphics.GraphicResources;
using AlmondToolEngine.SceneElements.Components.Base;
using AlmondToolEngine.SceneElements.Scenes;
using AlmondDxGraphics.GraphicResources.Data;

namespace AlmondToolEngine.SceneElements.Components
{
    public class TerrainRenderer : Component
    {
        // 트랜스폼은 Render컴포넌트에. _trasnform
        private DxTerrain? _terrain;
        private Camera? _camera;
        private DirectionalLight? _directionalLight;

        public void SetDxTerrain(DxTerrain land)
        {
            _terrain = land;
        }

        public override void Awake(SceneBase scene)
        {            
            _camera = scene.MainCamera.GetComponent<Camera>();
            _directionalLight = scene.DirectionalLight.GetComponent<DirectionalLight>();

        }

        public void Render()
        {
            var view = _camera!.ViewMatrix;
            var projection = _camera!.ProjMatrix;
            var world = GameOBJ!.GetComponent<Transform>()!.WorldTM;            

            var worldInv = Matrix.Invert(world);
            var transpose = Matrix.Transpose(worldInv);

            var WVP = world * view * projection;
            WVP.Transpose();            

            _terrain?.Draw(new RenderingQuary
            {
                WorldViewProjection = WVP,
                WorldInvTranspose = transpose,
                World = world,
                CameraPosition = _camera!.GameOBJ!.GetComponent<Transform>()!.WorldPosition,
                DirectionalLight = _directionalLight!.DirLightInfo
            });
        }
        
    }
}
