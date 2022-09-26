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
    public static class ModelDataFactory
    {
        public static ModelData CreateBoxModelData(DxDevice device, float x, float y, float z)
        {
            MeshNode node = new MeshNode();
            // 메쉬노드의 위치
            Matrix matrix = Matrix.Identity;
            matrix *= Matrix.Translation(new Vector3(0, 0, 0));
            node.World = matrix;

            // 메쉬노드의 형태
            node.Geometries.Add(MeshGeometryFactory.CreateBoxGeometry(device, x, y, z));

            // 메쉬노드를 모델데이터로 래핑.
            ModelData modelData = new ModelData("Box");
            modelData.Nodes.Add(node);

            return modelData;
        }

        public static ModelData CreateSkyBoxModelData(DxDevice device, float x, float y, float z, string cubeMapPath)
        {
            MeshNode node = new MeshNode();
            // 메쉬노드의 위치
            Matrix matrix = Matrix.Identity;
            matrix *= Matrix.Translation(new Vector3(0, 0, 0));
            node.World = matrix;

            var meshGeom = MeshGeometryFactory.CreateBoxGeometry(device, x, y, z);
            meshGeom.Material = new MaterialData();
            meshGeom.Material.DiffuseTextureFilePath = cubeMapPath;

            // 메쉬노드의 형태
            node.Geometries.Add(meshGeom);

            // 메쉬노드를 모델데이터로 래핑.
            ModelData modelData = new ModelData("Box");
            modelData.Nodes.Add(node);

            return modelData;
        }

        public static ModelData CreateGridTerrainModelData(DxDevice device, int seqX, int seqY, float size,
            string diffuseTexPath, string normalTexPath, string heightTexPath)
        {
            MeshNode node = new MeshNode();
            Matrix matrix = Matrix.Identity;
            matrix *= Matrix.Translation(new Vector3(0, 0, 0));
            node.World = matrix;

            // 메쉬노드
            node.Geometries.Add(MeshGeometryFactory.CreateTerrainGeometry(device, seqX, seqY, size, diffuseTexPath, normalTexPath, heightTexPath));

            ModelData modelData = new ModelData("Terrain");
            modelData.Nodes.Add(node);

            return modelData;
        }

    }
}
