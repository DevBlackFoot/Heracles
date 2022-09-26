using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SharpDX;
using AlmondDxGraphics.DXResources;
using AlmondDxGraphics.GraphicResources.Data;
using AlmondDxGraphics.Manager.Pools;

namespace AlmondDxGraphics.GraphicResources
{
    // 메쉬의 트랜스폼 정보를 가진 객체
    public class DxNode
    {
        public string? Name { get; set; }

        public Matrix World { get; set; }

        // 미리 연산해둔 좌표 매트릭스
        internal Matrix PreComputed { get; set; }

        public List<DxNode> Children { get; private set; }
        public DxNode? Parent { get; private set; }
        public DxSkinData? Skinning { get; private set; }

        public DxNode (DxDevice device, TexturePool pool, MeshNode node, DxModel model)
        {
            Name = node.Name;
            World = node.World;
            PreComputed = this.World;
            Children = new List<DxNode>();

            // 자식 노드를 만든다.
            foreach (var childMesh in node.ChildNodes)
            {
                var childNode = new DxNode(device, pool, childMesh, model);
                childNode.Parent = this;
                Children.Add(childNode);
            }

            // 모델에 메쉬를 추가한다.
            foreach (var geom in node.Geometries)
            {
                var g3d = new DxMesh(device, pool, geom, node.Skinning != null);
                g3d.Node = this;
                model.Meshes.Add(g3d);
                model.BoundingBoxies.Add(g3d.BoundingCube);
            }

            // 스키닝 정보를 추가한다.
            if (node.Skinning != null)
            {
                Skinning = new DxSkinData()
                {
                    BindMatrix = node.Skinning.BindMatrix,
                    InverseBindMatrix = new List<Matrix>(node.Skinning.InverseBinding),
                    BoneNames = new List<string>(node.Skinning.BoneNames)
                };
            }

        }

        // 재귀를 통해 이름으로 자식 노드를 찾는 함수
        public DxNode? SearchChildNodeByName(string name)
        {
            var val = Children.FirstOrDefault(n => n.Name == name);
            if (val != null)
                return val;

            val = Children.Select(n => n.SearchChildNodeByName(name)).Where(n => n != null).FirstOrDefault();
            if (val == null)
                return null;

            return val;

            //foreach (var node in Children)
            //{
            //    if (node.Name == name)
            //    {
            //        return node;
            //    }
            //    else
            //    {
            //        var cnode = node.SearchChildNodeByName(name);
            //        if (cnode != null)
            //            return cnode;
            //    }
            //}
            //return null;
        }
        // Matrix pallete를 생성한다.
        // 이 그래픽 엔진은 Matrix Palette Skinning을 사용한다.
        internal void CreatePalettes(DxModel model)
        {
            if (Skinning != null)
                Skinning.Init(model);

            foreach (var n in Children)
            {
                if (n.Skinning != null)
                    n.Skinning.Init(model);
            }
        }

        public Matrix GetNodeMatrix()
        {
            var model = this;
            Matrix currentMat = model.PreComputed;
            while (model.Parent != null)
            {
                model = model.Parent;
                currentMat *= model.PreComputed;
            }

            return currentMat;
        }

    }
}
