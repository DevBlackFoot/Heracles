using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;
using ImGuiNET;
using AlmondToolEngine.Manager;
using AlmondToolEngine.SceneElements.Scenes;
using AlmondToolEngine.SceneElements.Objects;
using AlmondToolEngine.SceneElements.Factories;
using AlmondToolEngine.SceneElements.Components;
using AlmondDxGraphics.DXResources;
using AlmondDxGraphics.GraphicResources.Factories;
using AlmondDxGraphics.GraphicResources.ExternFileParser.Assimp;
using AlmondTools_Client.ScriptComps;
using AlmondTools_Client.SerializedData;
using AlmondTools_Client.SerializedData.Data;

namespace AlmondTools_Client.Scenes
{
    public class ObjectPreviewScene : SceneBase
    {

        private GameObject _selectedObject;
        private SerializeDataManager _sridata;
        private bool _isFindMapObj;
        private MapObjectData? _selectedMapObjData;

        public ObjectPreviewScene(uint id, RenderControlManager manager, SerializeDataManager data) : base(id, manager)
        {
            _sridata = data;
            _isFindMapObj = false;
        }

        public override void BuildScene()
        {
            base.BuildScene();
            MainCamera.GetComponent<Transform>()!.SetPosition(new Vector3(0, 0.0f, -30.0f), SPACE.WORLD);
            MainCamera.AddComponent<CameraController>();
            MainCamera.GetComponent<CameraController>()!.Initialize(_managerForThis);                                            
            

            _selectedObject = GameObjectFactory.CreateEmpty("SelectedModel");
            _selectedObject.AddComponent<ModelRenderer>();
            _objectsInScene.Add(_selectedObject);
        }

        // Todo: 리팩토링 필요하다. 오브젝트를 찾을 때, 데이터의 포인터를 따서 중간변수에 저장함. 매우 안좋은 코드..
        public int FindMapObjPrefabID(int mapObjID)
        {
            _isFindMapObj = true;
            _selectedMapObjData = _sridata.MapObjects.Find(x => x.Id == mapObjID);

            return _selectedMapObjData!.ObjectId;
        }

        public void ChangeViewingModel(int mapObjPrefabID)
        {
            string selectedItemModelPath;

            selectedItemModelPath = _sridata.MapObjectPrefabs.Find(x => x.Id == mapObjPrefabID).Path;


            var selectedModel = _managerForThis.GetModel(Path.GetFileName(selectedItemModelPath));
            if (selectedModel != null)
            {
                foreach (var mesh in selectedModel.Meshes)
                {
                    mesh.SetRasterizerState(RSState.SOLID);
                }
            }

            if (selectedModel != null)
            {
                _selectedObject!.GetComponent<ModelRenderer>()!.SetDxModel(selectedModel);
            }


            // 스케일과 회전값을 적용해 보여준다.
            if (_isFindMapObj == true)
            {
                var transform = _selectedObject!.GetComponent<Transform>();
                transform!.SetRotation(new Vector3(_selectedMapObjData!.Transform[3], _selectedMapObjData.Transform[4], _selectedMapObjData.Transform[5]));
                transform!.SetScale(new Vector3(_selectedMapObjData.Transform[6], _selectedMapObjData.Transform[7], _selectedMapObjData.Transform[8]));

            }

        }


    }
}
