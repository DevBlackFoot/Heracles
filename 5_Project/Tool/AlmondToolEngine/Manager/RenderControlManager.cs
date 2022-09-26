using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX.Windows;
using AlmondDxGraphics;
using AlmondDxGraphics.DXResources;
using AlmondDxGraphics.GraphicResources;
using AlmondDxGraphics.GraphicResources.Data;
using AlmondToolEngine.SceneElements.Scenes;
using AlmondDxGraphics.GraphicResources.ExternFileParser.Assimp;

using AlmondToolEngine.Manager;

namespace AlmondToolEngine.Manager
{
    public class RenderControlManager
    {
        // 랜더컨트롤의 이름.
        public string Name { get; set; }

        // 이 랜더 컨트롤에 그려지고 있는 씬
        private Dictionary<uint, SceneBase> _sceneList;
        private bool _hasReservedScene;
        private uint _reservedSceneID;
        private SceneBase? _currentScene;
        private MouseTracker _mouseTracker;

        public AlTimer AlTimer { get; set; }

        public SceneBase? CurrentScene { get { return _currentScene; } }

        // 씬을 재생해 줄 dx 그래픽 랜더러
        AlRenderer _rendererForControl;

        public AlRenderer AlRenderer { get { return _rendererForControl; } }
        public MouseTracker MouseTracker { get { return _mouseTracker; } }

        public RenderControlManager(DxRenderControl control)
        {
            Name = control.Name;
            _rendererForControl = new AlRenderer(control);
            _sceneList = new Dictionary<uint, SceneBase>();
            _hasReservedScene = false;
            AlTimer = new AlTimer();
            _mouseTracker = new MouseTracker(control);
        }

        public void LoadScene(uint sceneID)
        {
            if (!_sceneList.ContainsKey(sceneID))
                return;
            _reservedSceneID = sceneID;
            _hasReservedScene = true;
        }

        public void ChangeScene()
        {
            _currentScene = _sceneList[_reservedSceneID];
            _hasReservedScene = false;

            _currentScene.BuildScene();
            _currentScene.Awake();
            _currentScene.Start();
        }

        public void SceneLoop()
        {
            AlTimer.Tick();

            if (_hasReservedScene)
                ChangeScene();

            if (_currentScene == null)
                return;            

            _currentScene.PreUpdate();
            _currentScene.Update();
            _currentScene.LateUpdate();

            _rendererForControl.BeginRender();

            _currentScene.PreDrawScene();
            _currentScene.DrawScene();
            _currentScene.EndDrawScene();

            _rendererForControl.Present(); // Todo: 뭔가 래핑이 필요할것같기도..
        }

        public void AddScene(uint sceneID, SceneBase scene)
        {
            _sceneList.Add(sceneID, scene);
        }

        public void ClearAllScene()
        {
            _sceneList.Clear();
        }

        public DxShader? GetShader(string shaderName)
        {
            var shaderPool = _rendererForControl.GraphicResources.Shaders;
            return shaderPool.GetShader(shaderName);
        }

        public void AddModel(ModelData modelData, List<DxShader> shaders)
        {
            var modelPool = _rendererForControl.GraphicResources.Models;
            var device = _rendererForControl.Device;

            modelPool.AddModel(device,
                modelData,
                shaders);
        }

        public DxModel? GetModel(string modelName)
        {
            var modelPool = _rendererForControl.GraphicResources.Models;
            return modelPool.GetModel(modelName);
        }




    }
}
