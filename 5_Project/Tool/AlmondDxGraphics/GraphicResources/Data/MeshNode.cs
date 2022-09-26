using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;

namespace AlmondDxGraphics.GraphicResources.Data
{
    public enum NodeType
    {
        Bone,       // Bone, Joint 뭐든지. 암튼 보여주지 않는 스켈레톤 형성하는 뼈대 노드일 경우 이걸.
        Node        // 이건 일반적인 메쉬 노드.
    }
    // 
    public class MeshNode
    {
        public string? Name { get; set; }

        public NodeType Type { get; set; }
        public Matrix World { get; set; }                   // 노드의 월드좌표 매트릭스
        public List<MeshGeometry> Geometries { get; set; }  // 노드 안에 있는 메쉬 기하학 정보

        public List<MeshNode> ChildNodes { get; set; }      // 자식 노드 포인터 리스트

        public SkinInformation? Skinning { get; set; }       // 스키닝 정보

        public MeshNode()
        {
            Geometries = new List<MeshGeometry>();
            ChildNodes = new List<MeshNode>();
            World = Matrix.Identity;
        }

    }
}
