using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;
using AlmondDxGraphics.DXResources;
using AlmondDxGraphics.GraphicResources.Data;

namespace AlmondDxGraphics.GraphicResources.Factories
{
    public static class MeshGeometryFactory
    {
        public static MeshGeometry CreateBoxGeometry(DxDevice device, float x, float y, float z)
        {
            MeshGeometry geometry = new MeshGeometry();

            List<VertexFormat> verticies = new List<VertexFormat>();
            // 앞면
            verticies.Add(new VertexFormat() { Position = new Vector4(-x, -y, -z, 1), TexCoord1 = new Vector2(0, 1), Normal = new Vector3(0, 0, -1), Tangent = new Vector3(1, 0, 0) });
            verticies.Add(new VertexFormat() { Position = new Vector4(-x, +y, -z, 1), TexCoord1 = new Vector2(0, 0), Normal = new Vector3(0, 0, -1), Tangent = new Vector3(1, 0, 0) });
            verticies.Add(new VertexFormat() { Position = new Vector4(+x, +y, -z, 1), TexCoord1 = new Vector2(1, 0), Normal = new Vector3(0, 0, -1), Tangent = new Vector3(1, 0, 0) });
            verticies.Add(new VertexFormat() { Position = new Vector4(+x, -y, -z, 1), TexCoord1 = new Vector2(1, 1), Normal = new Vector3(0, 0, -1), Tangent = new Vector3(1, 0, 0) });
            // 뒷면
            verticies.Add(new VertexFormat() { Position = new Vector4(-x, -y, +z, 1), TexCoord1 = new Vector2(1, 1), Normal = new Vector3(0, 0, 1), Tangent = new Vector3(-1, 0, 0) });
            verticies.Add(new VertexFormat() { Position = new Vector4(+x, -y, +z, 1), TexCoord1 = new Vector2(0, 1), Normal = new Vector3(0, 0, 1), Tangent = new Vector3(-1, 0, 0) });
            verticies.Add(new VertexFormat() { Position = new Vector4(+x, +y, +z, 1), TexCoord1 = new Vector2(0, 0), Normal = new Vector3(0, 0, 1), Tangent = new Vector3(-1, 0, 0) });
            verticies.Add(new VertexFormat() { Position = new Vector4(-x, +y, +z, 1), TexCoord1 = new Vector2(1, 0), Normal = new Vector3(0, 0, 1), Tangent = new Vector3(-1, 0, 0) });
            // 윗면
            verticies.Add(new VertexFormat() { Position = new Vector4(-x, +y, -z, 1), TexCoord1 = new Vector2(0, 1), Normal = new Vector3(0, 1, 0), Tangent = new Vector3(1, 0, 0) });
            verticies.Add(new VertexFormat() { Position = new Vector4(-x, +y, +z, 1), TexCoord1 = new Vector2(0, 0), Normal = new Vector3(0, 1, 0), Tangent = new Vector3(1, 0, 0) });
            verticies.Add(new VertexFormat() { Position = new Vector4(+x, +y, +z, 1), TexCoord1 = new Vector2(1, 0), Normal = new Vector3(0, 1, 0), Tangent = new Vector3(1, 0, 0) });
            verticies.Add(new VertexFormat() { Position = new Vector4(+x, +y, -z, 1), TexCoord1 = new Vector2(1, 1), Normal = new Vector3(0, 1, 0), Tangent = new Vector3(1, 0, 0) });
            // 아랫면
            verticies.Add(new VertexFormat() { Position = new Vector4(-x, -y, -z, 1), TexCoord1 = new Vector2(1, 1), Normal = new Vector3(0, -1, 0), Tangent = new Vector3(-1, 0, 0) });
            verticies.Add(new VertexFormat() { Position = new Vector4(+x, -y, -z, 1), TexCoord1 = new Vector2(0, 1), Normal = new Vector3(0, -1, 0), Tangent = new Vector3(-1, 0, 0) });
            verticies.Add(new VertexFormat() { Position = new Vector4(+x, -y, +z, 1), TexCoord1 = new Vector2(0, 0), Normal = new Vector3(0, -1, 0), Tangent = new Vector3(-1, 0, 0) });
            verticies.Add(new VertexFormat() { Position = new Vector4(-x, -y, +z, 1), TexCoord1 = new Vector2(1, 0), Normal = new Vector3(0, -1, 0), Tangent = new Vector3(-1, 0, 0) });
            // 왼쪽면
            verticies.Add(new VertexFormat() { Position = new Vector4(-x, -y, +z, 1), TexCoord1 = new Vector2(0, 1), Normal = new Vector3(-1, 0, 0), Tangent = new Vector3(0, 0, -1) });
            verticies.Add(new VertexFormat() { Position = new Vector4(-x, +y, +z, 1), TexCoord1 = new Vector2(0, 0), Normal = new Vector3(-1, 0, 0), Tangent = new Vector3(0, 0, -1) });
            verticies.Add(new VertexFormat() { Position = new Vector4(-x, +y, -z, 1), TexCoord1 = new Vector2(1, 0), Normal = new Vector3(-1, 0, 0), Tangent = new Vector3(0, 0, -1) });
            verticies.Add(new VertexFormat() { Position = new Vector4(-x, -y, -z, 1), TexCoord1 = new Vector2(1, 1), Normal = new Vector3(-1, 0, 0), Tangent = new Vector3(0, 0, -1) });
            // 오른쪽면
            verticies.Add(new VertexFormat() { Position = new Vector4(+x, -y, -z, 1), TexCoord1 = new Vector2(0, 1), Normal = new Vector3(1, 0, 0), Tangent = new Vector3(0, 0, 1) });
            verticies.Add(new VertexFormat() { Position = new Vector4(+x, +y, -z, 1), TexCoord1 = new Vector2(0, 0), Normal = new Vector3(1, 0, 0), Tangent = new Vector3(0, 0, 1) });
            verticies.Add(new VertexFormat() { Position = new Vector4(+x, +y, +z, 1), TexCoord1 = new Vector2(1, 0), Normal = new Vector3(1, 0, 0), Tangent = new Vector3(0, 0, 1) });
            verticies.Add(new VertexFormat() { Position = new Vector4(+x, -y, +z, 1), TexCoord1 = new Vector2(1, 1), Normal = new Vector3(1, 0, 0), Tangent = new Vector3(0, 0, 1) });

            geometry.Vertices = verticies;

            geometry.Indices = new List<int>
            {
                // 앞면
                0, 1, 2,
                0, 2, 3,
                // 뒷면
                4, 5, 6,
                4, 6, 7,
                // 윗면
                8, 9, 10,
                8, 10, 11,
                // 아랫면
                12, 13, 14,
                12, 14, 15,
                // 왼쪽면
                16, 17, 18,
                16, 18, 19,
                // 오른쪽면
                20, 21, 22,
                20, 22, 23
            };

            geometry.Name = "BoxGeom";


            return geometry;
        }

        public static MeshGeometry CreateTerrainGeometry(DxDevice device, int seqX, int seqY, float size,
            string diffuseTexPath, string normalTexPath, string heightTexPath)
        {
            MeshGeometry geometry = new MeshGeometry();
            float sizeW = seqX * size;
            float sizeH = seqY * size;

            int tesslationVertexNumber = (seqX + 2) * (seqY + 2);

            List<VertexFormat> verticies = new List<VertexFormat>(tesslationVertexNumber);            

            int k = 0;
            for (int y = -1; y < seqY + 1; y++)
            {
                for (int x = -1; x < seqX + 1; x++)
                {
                    float vX = x * size - (seqX / 2.0F) * size;
                    float vY = y * size - (seqY / 2.0F) * size;
                    float vZ = 0;

                    VertexFormat v = new VertexFormat();
                    v.Position.X = vX;
                    v.Position.Y = vY;
                    v.Position.Z = vZ;
                    v.Position.W = 1.0f;
                    v.TexCoord1.X = (float)x * 0.25f;
                    v.TexCoord1.Y = (float)y * 0.25f;
                    verticies.Add(v);                   
                    
                    k++;
                }
            }

            //indici

            int indicesNumber = seqX * seqY * 4 * 4;
            List<int> indices = new List<int>(indicesNumber);
            
            k = 0;
            for (int y = 0; y < seqY - 1; y++)
            {
                for (int x = 0; x < seqX - 1; x++)
                {
                    int startX = x + 1;
                    int startY = y + 1;

                    for (int j = -1; j < 3; j++)
                    {
                        for (int i = -1; i < 3; i++)
                        {
                            indices.Add( (i + startX + (seqX + 2) * (j + startY)) );
                            k++;
                        }
                    }
                }
            }
            
            geometry.Vertices = verticies;
            geometry.Indices = indices;
            geometry.Name = "GridGeomForTerrain";

            geometry.Material = new MaterialData();

            geometry.Material.DiffuseTextureFilePath = diffuseTexPath;
            geometry.Material.NormalTextureFilePath = normalTexPath;
            geometry.Material.HeightTextureFilePath = heightTexPath;

            return geometry;
        }
    }
}
