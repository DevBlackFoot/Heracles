using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;

namespace AlmondDxGraphics.GraphicResources.Data
{
    public class MeshGeometry
    {
        public string? Name { get; set; }
        public List<VertexFormat> Vertices { get; set; }
        public List<int> Indices { get; set; }

        public Vector3 MinVertex { get; private set; }
        public Vector3 MaxVertex { get; private set; }

        public MaterialData? Material { get; set; }

        public MeshGeometry()
        {
            Vertices = new List<VertexFormat>();
            Indices = new List<int>();
            MinVertex = Vector3.Zero;
            MaxVertex = Vector3.Zero;
        }

        public void CreateSubData()
        {
            // assimp에서는 필요없다.. 텍스쳐위치나 인덱스 최적화는 알아서 해준다.
            // 자체포멧을 사용할 때 다시 사용할 수 있으니 코드는 남겨둔다..
            // FixTextureCoord();
            // GenerateIndices();
            GenerateNormal();
            GenerateTangentBinormal();
        }

        private void FixTextureCoord()
        {
            for (int i = 0; i < Vertices.Count; i++)
            {
                VertexFormat v = Vertices[i];
                v.TexCoord1.Y = 1.0F - v.TexCoord1.Y;
                v.TexCoord2.Y = 1.0F - v.TexCoord2.Y;
                Vertices[i] = v;
            }
        }

        private void GenerateIndices()
        {
            List<VertexFormat> tempVertices = new List<VertexFormat>();
            List<int> tempIndices = new List<int>();

            foreach (VertexFormat v in Vertices)
            {
                // 중복된 버텍스를 찾는다
                int i = 0;
                bool found = false;
                foreach (VertexFormat v2 in tempVertices)
                {
                    if (VertexFormat.Compare(v, v2))
                    {
                        // 찾았다!
                        found = true;
                        break;
                    }
                    i++;
                }

                // In found join the normals
                if (found)
                {
                    tempIndices.Add(i);
                }
                else
                {
                    i = tempVertices.Count;
                    tempVertices.Add(v);
                    tempIndices.Add(i);
                }

                // normal
                VertexFormat vTemp = tempVertices[i];
                vTemp.Normal += v.Normal;
                tempVertices[i] = vTemp;
            }

            // 모든 버텍스를 노멀라이즈 한다.
            Vertices.Clear();
            foreach (VertexFormat v in tempVertices)
            {
                v.Normal.Normalize();
                Vertices.Add(v);
            }
            Vertices.AddRange(tempVertices);

            Indices.Clear();
            Indices.AddRange(tempIndices);
        }

        private void GenerateNormal()
        {
            float minx = 0.0f, miny = 0.0f, minz = 0.0f;
            float maxx = 0.0f, maxy = 0.0f, maxz = 0.0f;
            for (int i = 0; i < Vertices.Count; i++)
            {
                VertexFormat v = Vertices[i];
                v.Normal = new Vector3();
                Vertices[i] = v;
                RenewMinVertex(v.Position, ref minx, ref miny, ref minz);
                RenewMaxVertex(v.Position, ref maxx, ref maxy, ref maxz);
            }
            MinVertex = new Vector3(minx, miny, minz);
            MaxVertex = new Vector3(maxx, maxy, maxz);

            for (int i = 0; i < Indices.Count; i += 3)
            {
                if (i > Indices.Count - 3)
                    break;

                VertexFormat p1 = Vertices[Indices[i]];
                VertexFormat p2 = Vertices[Indices[i + 1]];
                VertexFormat p3 = Vertices[Indices[i + 2]];

                var temp1 = p2.Position - p1.Position;
                var temp2 = p3.Position - p1.Position;

                Vector3 V1 = new(temp1.X, temp1.Y, temp1.Z);
                Vector3 V2 = new(temp2.X, temp2.Y, temp2.Z);

                Vector3 N = Vector3.Cross(V1, V2);
                N.Normalize();

                p1.Normal += N;
                p2.Normal += N;
                p3.Normal += N;

                Vertices[Indices[i]] = p1;
                Vertices[Indices[i + 1]] = p2;
                Vertices[Indices[i + 2]] = p3;
            }

            // 노멀라이즈
            for (int i = 0; i < Vertices.Count; i++)
            {
                VertexFormat v = Vertices[i];
                v.Normal.Normalize();
                Vertices[i] = v;
            }
        }

        private void GenerateTangentBinormal()
        {
            // 버텍스 리셋
            for (int i = 0; i < Vertices.Count; i++)
            {
                VertexFormat v = Vertices[i];
                v.Normal = new Vector3();
                v.Tangent = new Vector3();
                v.Binormal = new Vector3();
                Vertices[i] = v;
            }

            for (int i = 0; i < Indices.Count; i += 3)
            {
                if (i > Indices.Count - 3)
                    break;

                VertexFormat P0 = Vertices[Indices[i]];
                VertexFormat P1 = Vertices[Indices[i + 1]];
                VertexFormat P2 = Vertices[Indices[i + 2]];


                Vector4 e0 = P1.Position - P0.Position;
                Vector4 e1 = P2.Position - P0.Position;

                Vector3 en0 = new(e0.X, e0.Y, e0.Z);
                Vector3 en1 = new(e1.X, e1.Y, e1.Z);

                Vector3 normal = Vector3.Cross(en0, en1);

                // 객체 공간에 있는 벡터가 탄젠트 스페이스의 벡터로 변환하게 한다.
                // X축은 s 방향과 일치하도록.
                // Y축은 t 방향과 일치하도록
                // Z축은 텍스쳐 맵의 바로 위에 해당하는 <0, 0, 1>로.

                Vector4 P = P1.Position - P0.Position;
                Vector4 Q = P2.Position - P0.Position;

                float s1 = P1.TexCoord1.X - P0.TexCoord1.X;
                float t1 = P1.TexCoord1.Y - P0.TexCoord1.Y;
                float s2 = P2.TexCoord1.X - P0.TexCoord1.X;
                float t2 = P2.TexCoord1.Y - P0.TexCoord1.Y;


                // 이제 방정식을 풀어야 한다..
                // P = s1*T + t1*B
                // Q = s2*T + t2*B
                // for T and B


                // this is a linear system with six unknowns and six equatinos, for TxTyTz BxByBz
                // [px,py,pz] = [s1,t1] * [Tx,Ty,Tz]
                //  qx,qy,qz     s2,t2     Bx,By,Bz

                // multiplying both sides by the inverse of the s,t matrix gives
                // [Tx,Ty,Tz] = 1/(s1t2-s2t1) *  [t2,-t1] * [px,py,pz]
                //  Bx,By,Bz                      -s2,s1	    qx,qy,qz  

                // solve this for the unormalized T and B to get from tangent to object space

                float tmp;
                if (Math.Abs(s1 * t2 - s2 * t1) <= 0.0001f)
                {
                    tmp = 1.0f;
                }
                else
                {
                    tmp = 1.0f / (s1 * t2 - s2 * t1);
                }

                Vector3 tangent = new Vector3();
                Vector3 binormal = new Vector3();

                tangent.X = t2 * P.X - t1 * Q.X;
                tangent.Y = t2 * P.Y - t1 * Q.Y;
                tangent.Z = t2 * P.Z - t1 * Q.Z;

                tangent = tangent * tmp;

                binormal.X = s1 * Q.X - s2 * P.X;
                binormal.Y = s1 * Q.Y - s2 * P.Y;
                binormal.Z = s1 * Q.Z - s2 * P.Z;

                binormal = binormal * tmp;

                normal.Normalize();
                tangent.Normalize();
                binormal.Normalize();

                // 노멀 추가
                P0.Normal += normal;
                P1.Normal += normal;
                P2.Normal += normal;

                P0.Tangent += tangent;
                P1.Tangent += tangent;
                P2.Tangent += tangent;

                P0.Binormal += binormal;
                P1.Binormal += binormal;
                P2.Binormal += binormal;

                Vertices[Indices[i]] = P0;
                Vertices[Indices[i + 1]] = P1;
                Vertices[Indices[i + 2]] = P2;
            }
        }

        private void RenewMinVertex(Vector4 vertexPosition, ref float x, ref float y, ref float z)
        {
            if (vertexPosition.X < x)
                x = vertexPosition.X;
            if (vertexPosition.Y < y)
                y = vertexPosition.Y;
            if (vertexPosition.Z < z)
                z = vertexPosition.Z;
        }

        private void RenewMaxVertex(Vector4 vertexPosition, ref float x, ref float y, ref float z)
        {
            if (vertexPosition.X > x)
                x = vertexPosition.X;
            if (vertexPosition.Y > y)
                y = vertexPosition.Y;
            if (vertexPosition.Z > z)
                z = vertexPosition.Z;
        }


    }


}
