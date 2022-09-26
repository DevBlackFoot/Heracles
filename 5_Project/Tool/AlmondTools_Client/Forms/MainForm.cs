using SharpDX.Windows;
using AlmondToolEngine;
using AlmondToolEngine.Manager;
using AlmondTools_Client.Scenes;

using AlmondTools_Client.SerializedData;
using AlmondTools_Client.SerializedData.Data;

using AlmondDxGraphics.DXResources;
using AlmondDxGraphics.GraphicResources;
using AlmondDxGraphics.GraphicResources.ExternFileParser.Assimp;

namespace AlmondTools_Client.Forms
{
    public partial class MainForm : Form
    {
        private RenderControlManager? _sceneScreen;
        private RenderControlManager? _mapObjScreen;
        private RenderControlManager? _mapObjPrefabScreen;

        private string? _currentLoadedMapFilePath;

        private EngineForWinform _engine;
        public SerializeDataManager DataManager { get; private set; }

        public MainForm(EngineForWinform engine, SerializeDataManager dataManager)
        {
            InitializeComponent();

            dxRenderControl_mainScene = new DxRenderControl(true);
            dxRenderControl_mainScene.Name = "MainScene";
            dxRenderControl_mainScene.Location = new System.Drawing.Point(1, 27);
            dxRenderControl_mainScene.Size = new System.Drawing.Size(1280, 720);
            dxRenderControl_mainScene.TabIndex = 1;
            dxRenderControl_mainScene.AutoSize = true;
            dxRenderControl_mainScene.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            dxRenderControl_mainScene.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);


            dxRenderControl_mapObjPreview = new DxRenderControl(false);
            dxRenderControl_mapObjPreview.Name = "MapObjScene";
            dxRenderControl_mapObjPreview.Location = new System.Drawing.Point(1283, 27);
            dxRenderControl_mapObjPreview.Size = new System.Drawing.Size(196, 255);
            dxRenderControl_mapObjPreview.TabIndex = 0;
            dxRenderControl_mapObjPreview.AutoSize = true;
            dxRenderControl_mapObjPreview.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            dxRenderControl_mapObjPreview.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);

            dxRenderControl_batchObjPreview = new DxRenderControl(false);
            dxRenderControl_batchObjPreview.Name = "ObjPrefabScene";
            dxRenderControl_batchObjPreview.Location = new System.Drawing.Point(1481, 27);
            dxRenderControl_batchObjPreview.Size = new System.Drawing.Size(179, 255);
            dxRenderControl_batchObjPreview.TabIndex = 6;
            dxRenderControl_batchObjPreview.AutoSize = true;
            dxRenderControl_batchObjPreview.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            dxRenderControl_batchObjPreview.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);

            this.Controls.Add(this.dxRenderControl_mainScene);
            this.Controls.Add(this.dxRenderControl_mapObjPreview);
            this.Controls.Add(this.dxRenderControl_batchObjPreview);

            _engine = engine;
            DataManager = dataManager;
        }

        private void MainForm_Load(object sender, EventArgs e)
        {            
            DataManager.LoadMapObjectPrefabs("Resources/ToolSetting/ToolMapObjects.json");

            _sceneScreen = _engine.GetRenderControlPool(Name).CreateRenderControlManager(dxRenderControl_mainScene)!;
            var testScene = new MapScene(0, _sceneScreen, DataManager);
            _sceneScreen.AddScene(0, testScene);


            _mapObjScreen = _engine.GetRenderControlPool(Name).CreateRenderControlManager(dxRenderControl_mapObjPreview)!;
            var testScene2 = new MapScene(0, _mapObjScreen, DataManager);
            var previewScene = new ObjectPreviewScene(0, _mapObjScreen, DataManager);
            _mapObjScreen.AddScene(0, previewScene);

            _mapObjPrefabScreen = _engine.GetRenderControlPool(Name).CreateRenderControlManager(dxRenderControl_batchObjPreview)!;
            var previewScene2 = new ObjectPreviewScene(0, _mapObjPrefabScreen, DataManager);
            _mapObjPrefabScreen.AddScene(0, previewScene2);

            BuildObject(DataManager.MapObjectPrefabs!); // todo : 예외처리 해라.

            _sceneScreen.LoadScene(0);
            _mapObjScreen.LoadScene(0);
            _mapObjPrefabScreen.LoadScene(0);

            CreateMapObjTreeView();
            CreateMapObjPrefabTreeView();

        }

        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);

            var renderer1 = _sceneScreen!.AlRenderer;
            var renderer2 = _mapObjScreen!.AlRenderer;
            var renderer3 = _mapObjPrefabScreen!.AlRenderer;

            RenderLoop.Run(this, () =>
            {
                if (renderer1.MustResize)
                {
                    renderer1.Resize();
                    renderer2.Resize();
                    renderer3.Resize();
                }
                renderer1.Clear(SharpDX.Color.DarkBlue);
                renderer2.Clear(SharpDX.Color.DarkBlue);
                renderer3.Clear(SharpDX.Color.DarkBlue);

                _mapObjScreen.SceneLoop();
                _sceneScreen.SceneLoop();
                _mapObjPrefabScreen.SceneLoop();

            });

        }

        private void LoadMapFromFile(string fileName)
        {
            DataManager.LoadMapObjects(fileName);
            var testScene = new MapScene(0, _sceneScreen, DataManager);
            _sceneScreen.ClearAllScene();
            _sceneScreen.AddScene(0, testScene);
            _sceneScreen.LoadScene(0);

            _currentLoadedMapFilePath = fileName;

            ClearTreeView(treeView_mapObj);  // 트리도 지우고.
            CreateMapObjTreeView();          // 다시 새 정보로 그려준다.
        }


        // 맵파일로드.
        private void LoadFileMenuItem_Click(object sender, EventArgs e)
        {
            string fileName;
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Title = "불러올 맵데이터 파일을 지정하세요";
            openFileDialog.InitialDirectory = Directory.GetCurrentDirectory() + "\\Contents";
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                fileName = openFileDialog.FileName;
                LoadMapFromFile(fileName);
            }
        }

        private void SaveFileMenuItem_Click(object sender, EventArgs e)
        {
            OpenSaveFileDialog();
        }

        // 맵 세이브
        private void OpenSaveFileDialog()
        {
            string fileName;
            SaveFileDialog saveFileDialog = new SaveFileDialog();
            saveFileDialog.Title = "저장할 파일 이름을 지정하세요";
            saveFileDialog.OverwritePrompt = true;
            saveFileDialog.InitialDirectory = Directory.GetCurrentDirectory() + "\\Contents";
            saveFileDialog.Filter = "JSON File(*.json)|*.json";

            if (saveFileDialog.ShowDialog() == DialogResult.OK)
            {
                fileName = saveFileDialog.FileName;
                DataManager.SaveMapObjects(fileName);
            }
        }


        private void MapObjNodeItem_Click(object sender, TreeViewEventArgs e)
        {
            if (e.Node == null)
                return;

            string clickedObjectID = e.Node.Name;
            int clickedID = int.Parse(clickedObjectID);

            if (_mapObjScreen!.CurrentScene != null)
            {
                var prevScene = (ObjectPreviewScene)_mapObjScreen!.CurrentScene;
                var mapScene = (MapScene)_sceneScreen!.CurrentScene;
                prevScene.ChangeViewingModel(prevScene.FindMapObjPrefabID(clickedID));
                mapScene!.SelectGameObject(clickedID);
            }
        }


        private void MapObjPrefabNodeItem_Click(object sender, TreeViewEventArgs e)
        {
            if (e.Node == null)
                return;

            string clickedObjectID = e.Node.Name;
            int clickedID = int.Parse(clickedObjectID);

            if (_mapObjPrefabScreen!.CurrentScene != null)
            {
                var scene = (ObjectPreviewScene)_mapObjPrefabScreen!.CurrentScene;
                scene.ChangeViewingModel(clickedID);
            }
        }

        private void CreateMapObjTreeView()
        {
            var mapObjList = DataManager.MapObjects;
            if (mapObjList != null)
            {
                foreach (MapObjectData data in mapObjList)
                {
                    treeView_mapObj.Nodes.Add(data.Id.ToString(), data.GameObjectName);
                }
            }            
            treeView_mapObj.ExpandAll();
            treeView_mapObj.AllowDrop = true;
        }

        private void CreateMapObjPrefabTreeView()
        {
            var mapObjPrefabList = DataManager.MapObjectPrefabs;

            foreach (var data in mapObjPrefabList)
            {
                treeView_mapObjPrefab.Nodes.Add(data.Id.ToString(), data.ObjectName);
            }
            treeView_mapObjPrefab.ExpandAll();
            treeView_mapObjPrefab.AllowDrop = true;

        }

        private void ClearTreeView(TreeView targetTree)
        {
            targetTree.Nodes.Clear();
        }


        private void BuildObject(List<MapObjectPrefabData> mapObjects)
        {
            foreach (MapObjectPrefabData data in mapObjects)
            {                 
                string modelPath = data.Path;
                string texturePath = data.TextureDirectory;

                var modelGeom = AssimpImpoter.LoadModelData(modelPath, texturePath);
                var sceneShaderPool = _sceneScreen.AlRenderer.GraphicResources.Shaders;
                var prevShaderPool = _mapObjScreen.AlRenderer.GraphicResources.Shaders;
                var objShaderPool = _mapObjPrefabScreen.AlRenderer.GraphicResources.Shaders;

                var defaultSceneVS = sceneShaderPool.GetShader("Default_StaticVS.hlsl");
                DxShader defaultScenePS;
                if (Path.GetExtension(modelPath) == ".fbx")                
                    defaultScenePS = sceneShaderPool.GetShader("Default_StaticFbxPS.hlsl");                
                else                
                    defaultScenePS = sceneShaderPool.GetShader("Default_StaticObjPS.hlsl");
                                 
                List<DxShader> defaultSceneShaders = new List<DxShader>();
                defaultSceneShaders.Add(defaultSceneVS);
                defaultSceneShaders.Add(defaultScenePS);                

                var defaultPrevVS = prevShaderPool.GetShader("Default_StaticVS.hlsl");

                DxShader defaultPrevPS;
                if (Path.GetExtension(modelPath) == ".fbx")
                    defaultPrevPS = prevShaderPool.GetShader("Default_StaticFbxPS.hlsl");
                else
                    defaultPrevPS = prevShaderPool.GetShader("Default_StaticObjPS.hlsl");                
                List<DxShader> defaultPrevShaders = new List<DxShader>();
                defaultPrevShaders.Add(defaultPrevVS);
                defaultPrevShaders.Add(defaultPrevPS);
                
                var defaultobjVS = objShaderPool.GetShader("Default_StaticVS.hlsl");
                DxShader defaultobjPS;
                if (Path.GetExtension(modelPath) == ".fbx")
                    defaultobjPS = objShaderPool.GetShader("Default_StaticFbxPS.hlsl");
                else
                    defaultobjPS = objShaderPool.GetShader("Default_StaticObjPS.hlsl");
                List<DxShader> defaultobjShaders = new List<DxShader>();
                defaultobjShaders.Add(defaultobjVS);
                defaultobjShaders.Add(defaultobjPS);

                _sceneScreen.AddModel(modelGeom, defaultSceneShaders);
                _mapObjScreen.AddModel(modelGeom, defaultPrevShaders);
                _mapObjPrefabScreen.AddModel(modelGeom, defaultobjShaders);
            }
        }

        private void Tree_OpenContextMenu(object sender, MouseEventArgs e)
        {
            TreeView tree = (TreeView)sender;

            if (e.Button == MouseButtons.Right)
            {
                // 선택된 아이템의 노드를 저장해둔다.
                TreeNode selectedNode = tree.GetNodeAt(e.X, e.Y);
                tree.SelectedNode = selectedNode;

                if (selectedNode != null)
                {
                    OpenMapObjContextMenu(tree, e.X, e.Y);
                }                
            }
        }

        private void OpenMapObjContextMenu(TreeView tree, int x, int y)
        {
            ToolStripMenuItem deleteObjMenuItem = new ToolStripMenuItem();
            deleteObjMenuItem.Text = "삭제";
            deleteObjMenuItem.Name = "delete";
            deleteObjMenuItem.Click += (senders, es) =>
            {
                // 오브젝트를 삭제한다.
                int selectedItemId = int.Parse(tree.SelectedNode.Name);

                var mapScene = (MapScene)_sceneScreen!.CurrentScene!;
                mapScene.DeleteGameObject(selectedItemId);

                // 시리얼라이즈 데이터를 삭제한다.
                var dataToDelete = DataManager.MapObjects!.Find(x => x.Id == selectedItemId);
                if (dataToDelete != null)
                    DataManager.MapObjects.Remove(dataToDelete);

                // 트리에서 노드를 삭제한다.
                tree.Nodes.Remove(tree.SelectedNode);
            };

            ContextMenuStrip menuStrip = new ContextMenuStrip();
            menuStrip.Items.Add(deleteObjMenuItem);

            menuStrip.Show(tree, new Point(x, y));


        }

        // 드래그 앤 드롭을 위한 아이템 클릭 이벤트
        private void Tree_NodeItemSelect(object sender, MouseEventArgs e)
        {
            TreeView tree = (TreeView)sender;
            TreeNode node = tree.GetNodeAt(e.X, e.Y);
            tree.SelectedNode = node;

            if (node != null)
            {
                tree.DoDragDrop(node, DragDropEffects.Copy);
            }
        }

        private void Tree_NodeItemDragOver(object sender, DragEventArgs e)
        {
            e.Effect = DragDropEffects.Copy;

        }

        private void Tree_NodeItemDragOut(object sender, DragEventArgs e)
        {
            TreeView targetTree = (TreeView)sender;
            Point pt = new Point(e.X, e.Y);
            pt = targetTree.PointToClient(pt);
            TreeNode nodeTarget = targetTree.GetNodeAt(pt);
            TreeNode nodeSource = (TreeNode)e.Data!.GetData(typeof(TreeNode));
            var mapobjData = DataManager.CreateMapObjectData(nodeSource);

            // 씬에 해당 아이템의 오브젝트를 생성해서 추가.
            var mapScene = (MapScene)_sceneScreen!.CurrentScene;
            mapScene.AddGameObject(mapobjData);

            // 타겟 트리에 노드 추가.
            targetTree.Nodes.Add(mapobjData.Id.ToString(), mapobjData.GameObjectName);

        }

        private void MainForm_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Modifiers == Keys.Control)
            {
                switch (e.KeyCode)
                {
                    case Keys.S:
                        {
                            if (_currentLoadedMapFilePath == null)
                            {
                                OpenSaveFileDialog();
                            }
                            else
                            {
                                DataManager.SaveMapObjects(_currentLoadedMapFilePath);
                                MessageBox.Show("저장되었습니다.", "확인", MessageBoxButtons.OK);
                            }
                        }
                        break;
                }
            }
        }

    }
}