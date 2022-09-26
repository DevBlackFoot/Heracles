using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AlmondDxGraphics.GraphicResources.Data
{
    // 파일에서 파싱된 3D 모델 리소스 (실제 그리기용 인스턴스에 올리기 전 리소스데이터 덩어리)
    // 모델데이터구조 - MeshNode 리스트
    //               |     ㄴ---- MeshGeometry & 자식정보
    //               |     |          ㄴ---- 버텍스, 인덱스
    //               |     |          ㄴ---- MeterialData
    //               |     |                     ㄴ- 레거시라이트관련 변수, 텍스쳐이름등
    //               |     ㄴ---- SkinInformation
    //               ㄴ AnimationData  
    // 
    public class ModelData
    {
        public string Name { get; set; }
        public List<MeshNode> Nodes { get; set; }
        public List<AnimationData> Animations { get; set; }

        public ModelData(string name)
        {
            Name = name;
            Nodes = new List<MeshNode>();
            Animations = new List<AnimationData>();
        }

    }
}
