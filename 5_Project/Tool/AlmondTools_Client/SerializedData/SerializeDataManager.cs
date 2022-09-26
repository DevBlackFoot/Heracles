using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;
using Newtonsoft.Json;


using AlmondTools_Client.SerializedData.Data;
namespace AlmondTools_Client.SerializedData
{
    public class SerializeDataManager
    {
        public MapToolSetting MaptoolSetting { get; set; }

        public SceneData? SceneData { get; set; }

        public List<MapObjectData>? MapObjects { get; set; }
        public List<MapObjectPrefabData>? MapObjectPrefabs { get; set; }
        public Dictionary<int, int> MapObjectPrefabCounts { get; set; }        

        public int LastGameObjectID { get; set; }

        public SerializeDataManager()
        {
            MapObjects = new List<MapObjectData>();
            MapObjectPrefabCounts = new Dictionary<int, int>();            
            MaptoolSetting = ImportDataFromFile<MapToolSetting>("Resources/ToolSetting/Toolsetting.json");
        }

        public void LoadSceneData(string filePath)
        {
            SceneData = ImportDataFromFile<SceneData>(filePath);
        }

        public void LoadMapObjects(string filePath)
        {
            MapObjects = ImportDataFromFile<List<MapObjectData>>(filePath);
        }

        public void LoadMapObjectPrefabs(string filePath)
        {
            MapObjectPrefabs = ImportDataFromFile<List<MapObjectPrefabData>>(filePath);
        }


        public void SaveMapObjects(string filePath)
        {
            ExportDataToFile(MapObjects, filePath);
        }

        public void SaveSceneData(string filePath)
        {
            ExportDataToFile(SceneData, filePath);
        }


        public static T? ImportDataFromFile<T>(string filePath)
        {
            string jsondata = File.ReadAllText(filePath);
            if (jsondata == null)
                return default;
            return JsonConvert.DeserializeObject<T>(jsondata);

        }

        public static void ExportDataToFile<T>(T data, string filePath)
        {
            string alltext = JsonConvert.SerializeObject(data, Formatting.Indented, new JsonSerializerSettings { NullValueHandling = NullValueHandling.Ignore });
            File.WriteAllText(filePath, alltext, Encoding.UTF8);

        }

        public MapObjectData CreateMapObjectData(TreeNode nodeSource)
        {
            int selectedItemID = int.Parse(nodeSource.Name);
            var prefData = MapObjectPrefabs!.Find(x => x.Id == selectedItemID); // 선택한녀석은 이녀석이다.

            // 이제 이걸 기반으로 데이터를 만들자.
            MapObjectData mapObjectToAdd = new MapObjectData();
            mapObjectToAdd.Id = ++LastGameObjectID;
            mapObjectToAdd.ObjectId = selectedItemID;
            mapObjectToAdd.Transform = new List<float>(new float[] { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f });            

            int itemcount = MapObjectPrefabCounts.ContainsKey(selectedItemID) ? MapObjectPrefabCounts[selectedItemID] : 0;
            mapObjectToAdd.GameObjectName = prefData!.ObjectName + string.Format(" ({0:0})", itemcount);

            // 데이터를 시리얼라이즈데이터에 추가한다.
            MapObjects!.Add(mapObjectToAdd);

            return mapObjectToAdd; // 만든건 이제 리턴해준다. 씬에다가 넣어주자.
        }

        public void CountObjects(int gobjID)
        {
            // 자동이름 생성을 위한 오브젝트 숫자 카운트
            if (MapObjectPrefabCounts.ContainsKey(gobjID))
            {
                // 키값이 컨테이너에 있으면.. 숫자 ++
                MapObjectPrefabCounts[gobjID]++;
            }
            else
            {
                // 없으면 추가해주지 뭐.
                MapObjectPrefabCounts.Add(gobjID, 1);
            }

        }



    }
}
