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
    public class SkyBoxRenderer : Component
    {
        private DxSkyBox? _skybox;
        private Camera? _camera;
        private DirectionalLight? _directionalLight;

        public void SetDxSkyBox(DxSkyBox skybox)
        {
            _skybox = skybox;
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
            var cameraworld = _camera!.GameOBJ.GetComponent<Transform>()!.WorldTM;
                        
            Vector3 scale;
            Quaternion rot;
            Vector3 trans;
            cameraworld.Decompose(out scale, out rot, out trans);

            // 카메라 위치만큼 이동, 큐브를 view공간으로 옮긴 후 프로젝션
            var WVP = Matrix.Translation(trans) * view * projection;            

            var worldInv = Matrix.Invert(cameraworld);
            var transpose = Matrix.Transpose(worldInv);            

            _skybox?.Draw(new RenderingQuary
            {
                WorldViewProjection = WVP,
                WorldInvTranspose = transpose,
                World = cameraworld,
                CameraPosition = _camera!.GameOBJ!.GetComponent<Transform>()!.WorldPosition,
                DirectionalLight = _directionalLight!.DirLightInfo
            });
        }

    }
}
