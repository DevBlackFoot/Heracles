using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;
using AlmondDxGraphics.DXResources;
using AlmondDxGraphics.Manager.Pools;
using AlmondDxGraphics.GraphicResources.Data;

namespace AlmondDxGraphics.GraphicResources
{
    // 그래픽라이브러리에서 그릴 때 사용할 3D 모델 정보.
    // 최적화 연산을 마친, 그릴 때 필요한 최종 정보를 들고 있다.
    // Renderer클래스의 최종 인스턴스 풀에 저장한다. (버퍼 때문에 디바이스 종속적 리소스)
    public class DxModel : IDResourceBase, IDisposable
    {
        public DxDevice DxDevice { get; set; }

        public List<DxMesh> Meshes { get; set; }

        public List<BoundingBox> BoundingBoxies { internal get; set; }

        public BoundingBox BoundingBox { get; internal set; }

        public List<DxNode> Children { get; private set; }

        // Todo: 애니메이션 추가

        public DxModel(DxDevice device, TexturePool pool, Data.ModelData model) : base(model.Name, RESOURCE_TYPE.MODEL)
        {
            DxDevice = device;
            Meshes = new List<DxMesh>();
            Children = new List<DxNode>();
            BoundingBoxies = new List<BoundingBox>();

            foreach (var m in model.Nodes)
            {
                var c = new DxNode(DxDevice, pool, m, this);
                Children.Add(c);
            }

            Initialize();

        }

        public DxNode? SearchChildNodeByName(string name)
        {
            // 이 노드의 자식노드에 이름이 같은 노드가 있으면 그녀석을 반환.
            var val = Children.FirstOrDefault(n => n.Name == name);
            if (val != null)
                return val;

            // 없으면 자식노드에서 재귀함수를 돌려 자식노드를 찾고, 그래도 없으면 null을 반환.
            val = Children.Select(n => n.SearchChildNodeByName(name)).Where(n => n != null).FirstOrDefault();
            if (val == null)
                return null;

            return val;
        }

        internal void Initialize()
        {
            foreach (DxNode n in Children)
            {
                n.CreatePalettes(this);
            }
            // 바운딩 박스들을 통합해서 큰 바운딩박스 한개로 전환한다.
            BoundingBox = MakeOneBoundingBox(BoundingBoxies);
        }

        public void Draw(RenderingQuary information)
        {
            //Iterate each geometry
            foreach (var mesh in Meshes)
            {
                mesh.Apply(information);
                mesh.Draw(DxDevice.DeviceContext);
            }
        }

        public void DrawPatch(RenderingQuary information)
        {
            //Iterate each geometry
            foreach (var mesh in Meshes)
            {
                mesh.ApplyPatch(information);
                mesh.DrawPatch(DxDevice.DeviceContext);
            }
        }


        public void Dispose()
        {
            foreach (var mesh in Meshes)
                mesh.Dispose();
        }

        private BoundingBox MakeOneBoundingBox(List<BoundingBox> BoundingBoxies)
        {           
            float minX = float.MaxValue, minY = float.MaxValue, minZ = float.MaxValue;
            float maxX = float.MinValue, maxY = float.MinValue, maxZ = float.MinValue;

            foreach (var bBox in BoundingBoxies)
            {
                if (minX > bBox.Minimum.X)
                    minX = bBox.Minimum.X;
                if (minY > bBox.Minimum.Y)
                    minY = bBox.Minimum.Y;
                if (minZ > bBox.Minimum.Z)
                    minZ = bBox.Minimum.Z;

                if (maxX < bBox.Maximum.X)
                    maxX = bBox.Maximum.X;
                if (maxY < bBox.Maximum.Y)
                    maxY = bBox.Maximum.Y;
                if (maxZ < bBox.Maximum.Z)
                    maxZ = bBox.Maximum.Z;
            }

            Vector3 minvector = new Vector3(minX, minY, minZ);
            Vector3 maxvector = new Vector3(maxX, maxY, maxZ);

            BoundingBox maxSizedbox = new(minvector, maxvector);

            return maxSizedbox;
        }
    }
}
