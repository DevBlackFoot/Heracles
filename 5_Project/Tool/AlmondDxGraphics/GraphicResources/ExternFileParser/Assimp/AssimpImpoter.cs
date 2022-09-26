using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SharpDX;

using Assimp;
using Assimp.Configs;

using AlmondDxGraphics.GraphicResources.Data;

namespace AlmondDxGraphics.GraphicResources.ExternFileParser.Assimp
{
    public class AssimpImpoter
    {
        private List<PropertyConfig> _configurations = new List<PropertyConfig>()
        {
            new NoSkeletonMeshesConfig(true),      // true to disable dummy-skeleton mesh
            new FBXImportCamerasConfig(false),     // true would import cameras
            new SortByPrimitiveTypeConfig(PrimitiveType.Point | PrimitiveType.Line), // primitive types we should remove
            new VertexBoneWeightLimitConfig(4),    // max weights per vertex (4 is very common - our shader will use 4)
            new NormalSmoothingAngleConfig(66.0f), // if no normals, generate (threshold 66 degrees) 
            new FBXStrictModeConfig(false),        // true only for fbx-strict-mode
        };

        // loadingLevelPreset : 0 = 맥시멈 퀄리티, 1, 타임 퀄리티, 2 타임 페스트, 그외 기본
        public static ModelData LoadModelData(string filepath, string texDirectory, int loadingLevelPreset = 3)
        {       
            var assimpContext = new AssimpContext();

            assimpContext.Scale = 1.0f; // 스케일은 원본사이즈로.

            Scene assimpScene;

            // AssimpScene 인스턴스에 파일을 로드한다." (an assimp imported model is in a thing called Scene)
            try
            {
                switch (loadingLevelPreset)
                {
                    case 0: assimpScene = assimpContext.ImportFile(filepath, PostProcessPreset.TargetRealTimeMaximumQuality); break;
                    case 1: assimpScene = assimpContext.ImportFile(filepath, PostProcessPreset.TargetRealTimeQuality); break;
                    case 2: assimpScene = assimpContext.ImportFile(filepath, PostProcessPreset.TargetRealTimeFast); break;
                    default:
                        assimpScene = assimpContext.ImportFile(filepath,
                                                  PostProcessSteps.FlipUVs                // currently need
                                                | PostProcessSteps.JoinIdenticalVertices  // optimizes indexed
                                                | PostProcessSteps.Triangulate            // precaution
                                                | PostProcessSteps.FindInvalidData        // sometimes normals export wrong (remove & replace:)
                                                | PostProcessSteps.GenerateSmoothNormals  // smooths normals after identical verts removed (or bad normals)
                                                | PostProcessSteps.ImproveCacheLocality   // possible better cache optimization                                        
                                                | PostProcessSteps.FixInFacingNormals     // doesn't work well with planes - turn off if some faces go dark                                       
                                                | PostProcessSteps.CalculateTangentSpace  // use if you'll probably be using normal mapping 
                                                | PostProcessSteps.GenerateUVCoords       // useful for non-uv-map export primitives                                                
                                                | PostProcessSteps.ValidateDataStructure
                                                | PostProcessSteps.FindInstances
                                                | PostProcessSteps.GlobalScale            // use with AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY (if need)                                                
                                                | PostProcessSteps.FlipWindingOrder       // (CCW to CW) Depends on your rasterizing setup (need clockwise to fix inside-out problem?)                                                                         
                                                );
                        break;
                }
            }
            catch (AssimpException ex) { throw new Exception("Problem reading file: " + filepath + " (" + ex.Message + ")"); }

            var modelData = new ModelData(Path.GetFileName(filepath));

            ProcessNode(assimpScene.RootNode, assimpScene, ref modelData, texDirectory);

            return modelData;

        }

        // 불러온 씬의 루트노드를 통해 내 메쉬 형태로 변경
        public static void ProcessNode(Node assimpRootNode, Scene assimpScene, ref ModelData model, string texDirectory)
        {
            // 새로운 메쉬노드 형성. 이걸 모델데이터에 푸쉬한다.
            var meshNode = new MeshNode();

            // 입력된 노드의 메쉬정보를 불러온다.
            for (int i = 0; i < assimpRootNode.MeshCount; i++)
            {
                // 노드안의 메쉬 정보를 불러와서..
                Mesh aimesh = assimpScene.Meshes[assimpRootNode.MeshIndices[i]];

                // 이제 내 메쉬 지오메트리 형태로 변형해서 버퍼에 푸쉬해준다.
                meshNode.Geometries.Add(ProcessMeshGeom(aimesh, assimpScene, texDirectory));
            }

            meshNode.Name = assimpRootNode.Name;

            // 임시 좌표데이터 입력.
            Matrix matrix = Matrix.Identity;
            matrix *= Matrix.Translation(new Vector3(0, 0, 0));
            meshNode.World = matrix;

            model.Nodes.Add(meshNode);

            // 노드의 자식이 있다면, 자식노드 또한 계산해라. (재귀)
            for (int i = 0; i < assimpRootNode.ChildCount; i++)
            {
                ProcessNode(assimpRootNode.Children[i], assimpScene, ref model, texDirectory);
            }


        }

        private static MeshGeometry ProcessMeshGeom(Mesh aimesh, Scene aiScene, string texDirectory)
        {
            MeshGeometry geometry = new MeshGeometry();
            // 버텍스 정보를 받아온다..
            List<VertexFormat> verticies = new List<VertexFormat>();
            for (int i = 0; i < aimesh.Vertices.Count; i++)
            {
                VertexFormat vertex = new VertexFormat();
                if (aimesh.HasVertices)
                {
                    vertex.Position.X = aimesh.Vertices[i].X;
                    vertex.Position.Y = aimesh.Vertices[i].Z;
                    vertex.Position.Z = aimesh.Vertices[i].Y;
                    vertex.Position.W = 1.0f;
                }
                if (aimesh.HasNormals)
                {
                    vertex.Normal.X = aimesh.Normals[i].X;
                    vertex.Normal.Y = aimesh.Normals[i].Z;
                    vertex.Normal.Z = aimesh.Normals[i].Y;
                }
                if (aimesh.TextureCoordinateChannelCount != 0)
                {
                    vertex.TexCoord1.X = aimesh.TextureCoordinateChannels[0][i].X;
                    vertex.TexCoord1.Y = aimesh.TextureCoordinateChannels[0][i].Y;
                }
                verticies.Add(vertex);
            }
            geometry.Vertices = verticies;

            // 인덱스 정보를 받아온다.
            List<int> indices = new List<int>();
            for (int i = 0; i < aimesh.FaceCount; i++)
            {
                var face = aimesh.Faces[i];
                for (int j = 0; j < face.IndexCount; j++)
                {
                    indices.Add(face.Indices[j]);
                }
            }
            geometry.Indices = indices;

            // 머티리얼 데이터를 받아온다.
            geometry.Material = ProcessMaterialData(aiScene.Materials[aimesh.MaterialIndex], texDirectory);
            geometry.CreateSubData();

            return geometry;
        }

        private static MaterialData ProcessMaterialData(Material aiMaterial, string texdirectory)
        {
            MaterialData materialData = new MaterialData();
            var assimpMaterialTextures = aiMaterial.GetAllMaterialTextures(); // 텍스쳐 리스트를 가져온다.

            materialData.Diffuse = new Vector4(aiMaterial.ColorDiffuse.R, aiMaterial.ColorDiffuse.G, 
                                               aiMaterial.ColorDiffuse.B, aiMaterial.ColorDiffuse.A);

            materialData.Ambient = new Vector4(aiMaterial.ColorAmbient.R, aiMaterial.ColorAmbient.G,
                                               aiMaterial.ColorAmbient.B, aiMaterial.ColorAmbient.A);

            materialData.Specular = new Vector4(aiMaterial.ColorSpecular.R, aiMaterial.ColorSpecular.G,
                                                aiMaterial.ColorSpecular.B, aiMaterial.ColorSpecular.A);
            materialData.SpecularDensity = aiMaterial.Shininess;

            materialData.Emissive = new Vector4(aiMaterial.ColorEmissive.R, aiMaterial.ColorEmissive.G,
                                                aiMaterial.ColorEmissive.B, aiMaterial.ColorEmissive.A);

            //var modelIndex = directory.IndexOf("Model");
            //var texturedirectory = directory.Substring(0, modelIndex) + "Texture";
            //var texturedirectory = Path.Combine(directory, "Textures");

            foreach (var texture in assimpMaterialTextures)
            {
                switch(texture.TextureType)
                {
                    case TextureType.Diffuse:
                        materialData.DiffuseTextureFilePath = Path.Combine(texdirectory, Path.GetFileName(texture.FilePath));
                        break;
                    case TextureType.Height:
                        materialData.NormalTextureFilePath = Path.Combine(texdirectory, Path.GetFileName(texture.FilePath));
                        break;
                    case TextureType.Normals:
                        materialData.NormalTextureFilePath = Path.Combine(texdirectory, Path.GetFileName(texture.FilePath));
                        break;
                }                               
            }
            return materialData;
        }


    }
}
