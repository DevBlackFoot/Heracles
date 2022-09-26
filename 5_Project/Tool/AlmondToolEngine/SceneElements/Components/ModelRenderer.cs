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
    public class ModelRenderer : RenderComp
    {
        // 트랜스폼은 Render컴포넌트에. _trasnform
        private DxModel? _model;
        private Camera? _camera;
        private DirectionalLight? _directionalLight;

        public void SetDxModel(DxModel model)
        {
            _model = model;
        }

        public override void Awake(SceneBase scene)
        {
            base.Awake(scene);
            _camera = scene.MainCamera.GetComponent<Camera>();
            _directionalLight = scene.DirectionalLight.GetComponent<DirectionalLight>();
            
        }

        public override void Render()
        {  
            var view = _camera!.ViewMatrix;
            var projection = _camera!.ProjMatrix;
            var world = _transform!.WorldTM;

            var worldInv = Matrix.Invert(world);
            var transpose = Matrix.Transpose(worldInv);            

            _model?.Draw(new RenderingQuary
            {
                WorldViewProjection = world * view * projection,
                WorldInvTranspose = transpose,
                World = world,
                CameraPosition = _camera!.GameOBJ!.GetComponent<Transform>()!.WorldPosition,
                DirectionalLight = _directionalLight.DirLightInfo
            });
        }


    }
}
