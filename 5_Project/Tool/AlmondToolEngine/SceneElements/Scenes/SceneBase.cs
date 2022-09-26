using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using AlmondDxGraphics;
using AlmondToolEngine.SceneElements.Objects;
using AlmondToolEngine.SceneElements.Components;
using AlmondToolEngine.SceneElements.Factories;
using AlmondToolEngine.SceneElements.Components.Base;
using AlmondToolEngine.Manager;

namespace AlmondToolEngine.SceneElements.Scenes
{
    public abstract class SceneBase
    {
        public uint SceneID { get; private set; }

        protected List<GameObject> _objectsInScene;

        protected List<RenderComp> _renderCompInScene;
        
        protected List<ColliderComp> _collidersInScene;

        protected RenderControlManager _managerForThis;

        public GameObject MainCamera { get; set; }
        public GameObject DirectionalLight { get; set; }

        public SceneBase(uint sceneID, RenderControlManager manager)
        {
            SceneID = sceneID;
            _managerForThis = manager;
            _objectsInScene = new List<GameObject>();
            _renderCompInScene = new List<RenderComp>();
            _collidersInScene = new List<ColliderComp>();
            MainCamera = GameObjectFactory.CreateCamera("MainCamera", _managerForThis.AlRenderer);
            DirectionalLight = GameObjectFactory.CreateDirLight("DirLight", _managerForThis.AlRenderer);

        }

        // 랜더러 컴포넌트를 추가했을 때, 호출되어 해당 컴포넌트가 씬에 등록되어 따로 관리될 수 있게 한다.
        public void RegisterRenderComp(RenderComp renderComp)
        {
            if (_renderCompInScene.Find(x => x.Equals(renderComp)) != null)
                return;
            _renderCompInScene.Add(renderComp);
        }

        public void RegisterColliderComp(ColliderComp colComp)
        {
            if (_collidersInScene.Find(x => x.Equals(colComp)) != null)
                return;
            _collidersInScene.Add(colComp);
        }


        virtual public void BuildScene()
        {
            _managerForThis.AlRenderer.ImguiSetUp();
        }

        virtual public void Awake() 
        {
            MainCamera.Awake(this); // 트랜스폼 컴포넌트에서 카메라컴포넌트에 필요한 변수를 받아온다.
            foreach(GameObject obj in _objectsInScene)            
                obj.Awake(this);
            
        }
        virtual public void PreUpdate() 
        {
            foreach (GameObject obj in _objectsInScene)            
                obj.PreUpdate();
            
        }
        virtual public void Update() 
        {
            PhysicsManager.Update(ref _collidersInScene);
            MainCamera.Update();
            DirectionalLight.Update();
            foreach (GameObject obj in _objectsInScene)            
                obj.Update();            
        }
        virtual public void LateUpdate() 
        {
            foreach (GameObject obj in _objectsInScene)           
                obj.LateUpdate();            
        }

        virtual public void PreDrawScene() { }

        virtual public void DrawScene()
        {
            foreach (var renderComp in _renderCompInScene)
                renderComp.Render();
            //foreach (var colComp in _collidersInScene)
            //    colComp.ColliderRender();
        }

        virtual public void EndDrawScene() { } 

        virtual public void Start() { }

    }
}
