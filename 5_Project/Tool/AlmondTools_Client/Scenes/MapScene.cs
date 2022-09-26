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

using AlmondToolEngine.SceneElements.Components.Base;

using AlmondTools_Client.ScriptComps;
using AlmondTools_Client.SerializedData;
using AlmondTools_Client.SerializedData.Data;

using AlmondDxGraphics.GraphicResources;
using AlmondDxGraphics.GraphicResources.Data;

namespace AlmondTools_Client.Scenes
{
    public class MapScene : SceneBase
    {
        private string? _cameraposition;
        private string? _camerarotation;

        private SerializeDataManager _sridata;
        private GameObject? _selectedMapObj;
        private int _selectedItemID;
        private System.Numerics.Vector3 _selectedItemPosition;
        private System.Numerics.Vector3 _selectedItemRotation;
        private System.Numerics.Vector3 _selectedItemScale;

        private System.Numerics.Vector4 _dirLightAmbient;
        private System.Numerics.Vector4 _dirLightDiffuse;
        private System.Numerics.Vector4 _dirLightSpecular;
        private System.Numerics.Vector3 _dirLightDirection;
        private float _dirLightpower;

        private GameObject _terrainMap;
        private GameObject _skyBox;

        public MapScene(uint id, RenderControlManager manager, SerializeDataManager data) : base(id, manager)
        {
            _sridata = data;
        }

        public override void BuildScene()
        {
            base.BuildScene();

            MainCamera.GetComponent<Transform>()!.SetPosition(new Vector3(-7.11f, 10.32f, -70.0f), SPACE.WORLD);
            MainCamera.GetComponent<Transform>()!.SetRotation(new Vector3(0.25f, 0.32f, 0.0f), SPACE.WORLD);
            MainCamera.AddComponent<CameraController>();
            MainCamera.GetComponent<CameraController>()!.Initialize(_managerForThis);

            CreateMapObjecUsingSriData();

            var dirLightComp = DirectionalLight.GetComponent<DirectionalLight>();

            var sharpAmbient = dirLightComp.LegacyLightInfo.Ambient;
            var sharpDiffuse = dirLightComp.LegacyLightInfo.Diffuse;
            var sharpSpecular = dirLightComp.LegacyLightInfo.Specular;
            var sharpDirection = dirLightComp.Direction;

            _dirLightAmbient = new System.Numerics.Vector4(sharpAmbient.X, sharpAmbient.Y, sharpAmbient.Z, sharpAmbient.W);
            _dirLightDiffuse = new System.Numerics.Vector4(sharpDiffuse.X, sharpDiffuse.Y, sharpDiffuse.Z, sharpDiffuse.W);
            _dirLightSpecular = new System.Numerics.Vector4(sharpSpecular.X, sharpSpecular.Y, sharpSpecular.Z, sharpSpecular.W);
            _dirLightDirection = new System.Numerics.Vector3(sharpDirection.X, sharpDirection.Y, sharpDirection.Z);
            _dirLightpower = dirLightComp.SpecularPower;

            var device = _managerForThis.AlRenderer.Device;
            var shaderPool = _managerForThis.AlRenderer.GraphicResources.Shaders;
            var texPool = _managerForThis.AlRenderer.GraphicResources.Textures;

            //texPool.AddTexture(device, "Resources/TerrainTexture/Terrain_diffuse.dds", TEXTURE_TYPE.Diffuse);
            //texPool.AddTexture(device, "Resources/TerrainTexture/Terrain_normal.dds", TEXTURE_TYPE.Normal);
            //texPool.AddTexture(device, "Resources/TerrainTexture/Terrain_height.dds", TEXTURE_TYPE.Height);
            //texPool.AddTexture(device, "Resources/SkyBoxTexture/snowcube1024.dds", TEXTURE_TYPE.Diffuse);

            _terrainMap = GameObjectFactory.CreateTerrainLand("Land");

            var map = new DxTerrain();
            map.Initialize(device, texPool, shaderPool,
                "Resources/TerrainTexture/Terrain_diffuse.dds",
                "Resources/TerrainTexture/Terrain_normal.dds",
                "Resources/TerrainTexture/Terrain_height.dds");
            map.LoadHeightMap(texPool, "Resources/TerrainTexture/Terrain_height.dds");

            _terrainMap.GetComponent<TerrainRenderer>()!.SetDxTerrain(map);
            _terrainMap.Awake(this);

            _skyBox = GameObjectFactory.CreateSkyBox("SkyBox");
            var skyBox = new DxSkyBox();
            skyBox.Initialize(device, texPool, shaderPool, "Resources/SkyBoxTexture/snowcube1024.dds");
            skyBox.LoadCubeMap(texPool, "Resources/SkyBoxTexture/snowcube1024.dds");
            _skyBox.GetComponent<SkyBoxRenderer>()!.SetDxSkyBox(skyBox);
            _skyBox.Awake(this);
        }

        public void CreateMapObjecUsingSriData()
        {

            if (_sridata.MapObjects != null)
            {
                foreach (var gobj in _sridata.MapObjects)
                {
                    AddGameObject(gobj);
                }
            }
        }

        public void AddGameObject(MapObjectData data)
        {
            var objID = data.ObjectId;
            var position = new Vector3(data.Transform[0], data.Transform[1], data.Transform[2]);
            var rotation = new Vector3(data.Transform[3], data.Transform[4], data.Transform[5]);
            var scale = new Vector3(data.Transform[6], data.Transform[7], data.Transform[8]);

            string modelPath;
            modelPath = _sridata.MapObjectPrefabs.Find(x => x.Id == data.ObjectId).Path;


            var model = _managerForThis.GetModel(Path.GetFileName(modelPath));
            foreach (var mesh in model.Meshes)
            {
                mesh.SetRasterizerState(RSState.SOLID);
            }
            // ID를 이름으로 해서 오브젝트를 생성한다.
            GameObject obj = GameObjectFactory.CreateEmpty(data.Id.ToString());
            obj.AddComponent<ModelRenderer>();
            obj.GetComponent<ModelRenderer>()!.SetDxModel(model);
            obj.AddComponent<ColliderBox>();

            var dxdevice = _managerForThis.AlRenderer.Device;
            var texturepool = _managerForThis.AlRenderer.GraphicResources.Textures;
            var shaderpool = _managerForThis.AlRenderer.GraphicResources.Shaders;

            obj.GetComponent<ColliderBox>()!.Initialize(dxdevice, texturepool, shaderpool, model.BoundingBox);

            // 마지막 ID를 저장한다. 이걸로 ++ 해서 맵오브젝트 생성할거다.
            _sridata.LastGameObjectID = data.Id;

            // 오브젝트 몇개 만든건지 ID와 카운트를 모아둔다.                
            _sridata.CountObjects(objID);

            var transform = obj.GetComponent<Transform>();
            transform!.SetPosition(position);
            transform!.SetRotation(rotation);
            transform!.SetScale(scale);
            _objectsInScene.Add(obj);
            obj.Awake(this);
        }

        public void DeleteGameObject(int objectID)
        {
            var targetobj = _objectsInScene.Find(x => x.Name == objectID.ToString());
            if (targetobj != null)
            {
                var rendercomp = targetobj.GetComponent<ModelRenderer>();
                if (rendercomp != null)
                    _renderCompInScene.Remove(rendercomp);

                // 만약 충돌체컴포넌트가 있다면, 씬에 박혀있는 녀석을 해제해준다.
                ColliderComp? colcomp;
                colcomp = targetobj.GetComponent<ColliderBox>();
                if (colcomp != null)
                    _collidersInScene.Remove(colcomp);
                colcomp = targetobj.GetComponent<ColliderRay>();
                if (colcomp != null)
                    _collidersInScene.Remove(colcomp);
                colcomp = targetobj.GetComponent<ColliderRay>();
                if (colcomp != null)
                    _collidersInScene.Remove(colcomp);

                _objectsInScene.Remove(targetobj);
            }
        }


        public override void Update()
        {
            base.Update();
            var position = MainCamera.GetComponent<Transform>().WorldPosition;
            var rotation = MainCamera.GetComponent<Transform>().WorldRotation;
            _cameraposition = string.Format("Position : {0:0.00}, {1:0.00}, {2:0.00}", position.X, position.Y, position.Z);
            _camerarotation = string.Format("Rotation : {0:0.00}, {1:0.00}, {2:0.00}", rotation.X, rotation.Y, rotation.Z);
        }

        public override void PreDrawScene()
        {
            base.PreDrawScene();

            ImGui.Begin("Camera");
            ImGui.Text(_cameraposition);
            ImGui.Text(_camerarotation);
            if (ImGui.Button("Reset", new System.Numerics.Vector2(200, 30)))
            {
                var transform = MainCamera.GetComponent<Transform>();
                transform.SetPosition(new Vector3(-7.11f, 10.32f, -70.0f), SPACE.WORLD);
                transform.SetRotation(new Vector3(0.25f, 0.32f, 0.0f), SPACE.WORLD);
            }
            ImGui.Text("Directional Light Setting");

            ImGui.InputFloat4("Light_Ambient", ref _dirLightAmbient);
            ImGui.InputFloat4("Light_Diffuse", ref _dirLightDiffuse);
            ImGui.InputFloat4("Light_Specular", ref _dirLightSpecular);
            ImGui.InputFloat3("Direction x,y,z", ref _dirLightDirection);            
            if (ImGui.Button("Change Transform", new System.Numerics.Vector2(200, 30)))
            {
                var dirLight = DirectionalLight.GetComponent<DirectionalLight>();
                dirLight.LegacyLightInfo = new()
                {
                    Ambient = new Vector4(_dirLightAmbient.X, _dirLightAmbient.Y, _dirLightAmbient.Z, _dirLightAmbient.W),
                    Diffuse = new Vector4(_dirLightDiffuse.X, _dirLightDiffuse.Y, _dirLightDiffuse.Z, _dirLightDiffuse.W),
                    Specular = new Vector4(_dirLightSpecular.X, _dirLightSpecular.Y, _dirLightSpecular.Z, _dirLightSpecular.W)
                };
                dirLight.Direction = new(_dirLightDirection.X, _dirLightDirection.Y, _dirLightDirection.Z);                
            }
            // DirectionalLight 부분 float 하나를 컨트롤용 변수로 사용했음. 물론 임시..
            if (ImGui.Button("LightOn/Off", new System.Numerics.Vector2(100, 30)))
            {
                var dirLight = DirectionalLight.GetComponent<DirectionalLight>();
                if (dirLight!.SpecularPower < 1.0f)
                {
                    dirLight.SpecularPower = 2.0f;
                }
                else
                {
                    dirLight.SpecularPower = 0.8f;
                }                
            }            

            ImGui.End();

            DrawSelectItemImGuiSetting();
        }

        public override void DrawScene()
        {
            base.DrawScene();
            var collider = _selectedMapObj?.GetComponent<ColliderBox>();
            if (collider != null)
                collider.ColliderRender();

            _terrainMap.GetComponent<TerrainRenderer>()!.Render();
            _skyBox.GetComponent<SkyBoxRenderer>()!.Render();

        }

        public void SelectGameObject(int mapObjID)
        {
            _selectedItemID = mapObjID;
            _selectedMapObj = _objectsInScene.Find(x => x.Name == mapObjID.ToString());
            var selectobjTransform = _selectedMapObj.GetComponent<Transform>();
            _selectedItemPosition = new System.Numerics.Vector3(selectobjTransform!.WorldPosition.X, selectobjTransform!.WorldPosition.Y, selectobjTransform!.WorldPosition.Z);
            _selectedItemRotation = new System.Numerics.Vector3(selectobjTransform!.LocalRotation.X, selectobjTransform!.LocalRotation.Y, selectobjTransform!.LocalRotation.Z);
            _selectedItemScale = new System.Numerics.Vector3(selectobjTransform!.LocalScale.X, selectobjTransform!.LocalScale.Y, selectobjTransform!.LocalScale.Z);
        }

        public void DeselectGameObject()
        {
            _selectedMapObj = null;
        }

        private void DrawSelectItemImGuiSetting()
        {
            if (_selectedMapObj == null)
                return;

            ImGui.Begin("Selected Item Transform");
            ImGui.InputFloat3("Position", ref _selectedItemPosition);
            ImGui.InputFloat3("Rotation", ref _selectedItemRotation);
            ImGui.InputFloat3("Scale", ref _selectedItemScale);

            var selectobjTransform = _selectedMapObj.GetComponent<Transform>();
            if (ImGui.Button("Change Transform", new System.Numerics.Vector2(200, 30)))
            {
                selectobjTransform.SetPosition(new Vector3(_selectedItemPosition.X, _selectedItemPosition.Y, _selectedItemPosition.Z), SPACE.WORLD);
                selectobjTransform.SetRotation(new Vector3(_selectedItemRotation.X, _selectedItemRotation.Y, _selectedItemRotation.Z), SPACE.LOCAL);
                selectobjTransform.SetScale(new Vector3(_selectedItemScale.X, _selectedItemScale.Y, _selectedItemScale.Z));

                var transformData = _sridata.MapObjects.Find(x => x.Id == _selectedItemID)!.Transform;
                transformData[0] = _selectedItemPosition.X;
                transformData[1] = _selectedItemPosition.Y;
                transformData[2] = _selectedItemPosition.Z;
                transformData[3] = _selectedItemRotation.X;
                transformData[4] = _selectedItemRotation.Y;
                transformData[5] = _selectedItemRotation.Z;
                transformData[6] = _selectedItemScale.X;
                transformData[7] = _selectedItemScale.Y;
                transformData[8] = _selectedItemScale.Z;
            }
            ImGui.End();
        }






    }


}
