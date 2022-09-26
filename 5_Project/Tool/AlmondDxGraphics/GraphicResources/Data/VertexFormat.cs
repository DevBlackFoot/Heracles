using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
// .Net 기반으로 Com구성요소와 상호작용 할 수 있도록 하는 기능을 제공
using System.Runtime.InteropServices;

// Sharp.Vector사용을 위해.
using SharpDX;

namespace AlmondDxGraphics.GraphicResources.Data
{
    // 버텍스 버퍼에 저장할 버텍스 하나의 정보
    // 이 포멧은 비관리메모리와 외부 통신(쉐이더와 바인드) 하기 때문에,
    // 데이터가 순서대로 전달되도록 Sequential로 설정한다.
    [StructLayout(LayoutKind.Sequential)]
    public struct VertexFormat
    {
        public Vector4 Position;
        public Vector3 Normal;
        public Vector2 TexCoord1;
        public Vector2 TexCoord2;
        public Vector3 Binormal;
        public Vector3 Tangent;
        public Vector4 Joint;
        public Vector4 Weight;        

        // float 4바이트 * 29 = 100 즉 내용추가되면 늘려야 함. 이거 리플렉션 못하나 귀찮은데?
        public const int Size = 100;

        // 필드의 버텍스들을 Float배열로 반환한다.
        internal float[] GetArray()
        {
            List<float> v = new List<float>();
            v.AddRange(Position.ToArray());
            v.AddRange(Normal.ToArray());
            v.AddRange(TexCoord1.ToArray());
            v.AddRange(TexCoord2.ToArray());
            v.AddRange(Tangent.ToArray());
            v.AddRange(Binormal.ToArray());
            v.AddRange(Joint.ToArray());
            v.AddRange(Weight.ToArray());            
            return v.ToArray();
        }

        // 두 버텍스가 같은지 내부 필드를 비교한다.
        public static bool Compare(VertexFormat a, VertexFormat b)
        {
            return a.Position == b.Position &&
                a.TexCoord1 == b.TexCoord1 &&
                a.TexCoord2 == b.TexCoord2 &&
                a.Weight == b.Weight &&
                a.Joint == b.Joint;
        }
    }
}
