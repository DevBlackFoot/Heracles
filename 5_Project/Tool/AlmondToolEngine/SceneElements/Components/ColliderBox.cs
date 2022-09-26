using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;

using AlmondToolEngine.SceneElements.Objects;
using AlmondToolEngine.SceneElements.Components.Base;
using AlmondToolEngine.SceneElements.Scenes;
using AlmondDxGraphics.GraphicResources;
using AlmondDxGraphics.GraphicResources.Factories;
using AlmondDxGraphics.Manager.Pools;
using AlmondDxGraphics.DXResources;

namespace AlmondToolEngine.SceneElements.Components
{
    internal class DotForCollision
    {
        public float Plane { get; set; }
        public float Radius { get; set; }
        public float Min { get; set; }
        public float Max { get; set; }
        public DotForCollision(float plane, float radius, float min, float max)
        {
            Plane = plane;
            Radius = radius;
            Min = min;
            Max = max;
        }
    }

    public class ColliderBox : ColliderComp
    {
        // 회전하니까.. 오브젝트의 로테이션 행렬 저장.
        public Matrix ObjWorldRotTM { get; set; }

        public Vector3 LocalCenter { get; set; }
        public Vector3 WorldCenter { get; internal set; }
        public Vector3 Size { get; set; } // == radius

        public List<Vector3> WorldNormals { get; internal set; }

        // 각 축이 가지고 있는 절반의 크기 (센터에서 떨어진 거리)
        public List<float>? Extents { get; internal set; }


        public ColliderBox()
        {          
            ObjWorldRotTM = Matrix.Identity;
            WorldNormals = new List<Vector3>();
            WorldNormals.Add(new Vector3(1.0f, 0.0f, 0.0f));
            WorldNormals.Add(new Vector3(0.0f, 1.0f, 0.0f));
            WorldNormals.Add(new Vector3(0.0f, 0.0f, 1.0f));            
        }

        public override void Awake(SceneBase scene)
        {
            base.Awake(scene);
            WorldCenter = _transform!.WorldPosition;            
        }

        public void Initialize(DxDevice device, TexturePool texturePool, ShaderPool shader, BoundingBox box)
        {
            Size = box.Size;
            LocalCenter = Vector3.Zero;
            Extents = new List<float> { Size.X * 0.5f, Size.Y * 0.5f, Size.Z * 0.5f };
            var model = ModelDataFactory.CreateBoxModelData(device, Size.X * 0.5f, Size.Y * 0.5f, Size.Z * 0.5f);         
            _modelForDebug = new DxModel(device, texturePool, model);        

            var defaultVS = shader.GetShader("Default_StaticVS.hlsl");
            var defaultPS = shader.GetShader("Default_PrimitivePS.hlsl");
            List<DxShader> defaultShaders = new List<DxShader>();
            defaultShaders.Add(defaultVS!);
            defaultShaders.Add(defaultPS!);

            foreach (var m in _modelForDebug.Meshes)
            {
                m.Shaders = defaultShaders;
                m.SetRasterizerState(RSState.WIRE_FRAME);
            }

        }
        


        private bool CheckForBox(ColliderBox other)
        {
            var otherCenter = other.WorldCenter;
            // 각 면까지의 거리를 가져온다.
            var otherExtents = other.Extents;
            // 검증에 필요한 축을 가져온다.
            var otherNomals = other.WorldNormals;

            // 내적 연산의 결과를 보관하는 변수            
            var myDot = new DotForCollision(0.0f, 0.0f, 0.0f, 0.0f);
            var otherDot = new DotForCollision(0.0f, 0.0f, 0.0f, 0.0f);

            // 상대 평면에 분리축이 있다면.. 충돌한게 아니니, false 리턴
            if (HasSeparatingAxisAtOtherPlane(ref myDot, ref otherDot, otherCenter, otherExtents, otherNomals))
                return false;

            // 각 모서리의 외적으로 생성되는 평면에 대해서도 충돌 검사. 축이 있으면 충돌한게 아님.
            if (HasSeparatingAxisAtEdgeCross(ref myDot, ref otherDot, otherCenter, otherExtents, otherNomals))
                return false;

            return true;
        }

        private bool HasSeparatingAxisAtEdgeCross(ref DotForCollision myDot, ref DotForCollision otherDot,
            Vector3 otherCenter, List<float> otherExtents, List<Vector3> otherNormals)
        {
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    Vector3 targetNormal = Vector3.Cross(WorldNormals[i], otherNormals[j]);

                    if (targetNormal == Vector3.Zero)
                        continue;

                    targetNormal = Vector3.Normalize(targetNormal);

                    otherDot.Plane = Vector3.Dot(otherCenter, targetNormal);
                    myDot.Plane = Vector3.Dot(WorldCenter, targetNormal);

                    // 분리축 반지름을 구한다
                    myDot.Radius = 0.0f;
                    for (int k = 0; k < 3; k++)
                        myDot.Radius += Math.Abs(Vector3.Dot(WorldNormals[k], targetNormal) * Extents![k]);

                    otherDot.Radius = 0.0f;
                    for (int k = 0; k < 3; k++)
                        otherDot.Radius += Math.Abs(Vector3.Dot(otherNormals[k], targetNormal) * otherExtents[k]);

                    otherDot.Min = otherDot.Plane - otherDot.Radius;
                    otherDot.Max = otherDot.Plane + otherDot.Radius;

                    myDot.Min = myDot.Plane - myDot.Radius;
                    myDot.Max = myDot.Plane + myDot.Radius;

                    // 분리축 찾기. 축이 있으면 true리턴
                    if ((myDot.Max < otherDot.Min) || (myDot.Max > otherDot.Max))
                        return true;
                }
            }
            // 못찾았으면 false 리턴
            return false;
        }

        // 
        private bool HasSeparatingAxisAtOtherPlane(ref DotForCollision myDot, ref DotForCollision otherDot,
            Vector3 otherCenter, List<float> otherExtents, List<Vector3> otherNormals)
        {
            // 평면에대한 검사
            for (int i = 0; i < 3; i++)
            {
                var targetNormal = WorldNormals[i];

                otherDot.Plane = Vector3.Dot(otherCenter, targetNormal);
                myDot.Plane = Vector3.Dot(WorldCenter, targetNormal);

                otherDot.Radius = 0.0f;

                // 분리 축 반지름을 찾음
                for (int j = 0; j < 3; j++)
                {
                    otherDot.Radius += Math.Abs(Vector3.Dot(otherNormals[j], targetNormal)) * otherExtents[j];
                }

                myDot.Radius = Extents![i];

                otherDot.Min = otherDot.Plane - otherDot.Radius;
                otherDot.Max = otherDot.Plane + otherDot.Radius;

                myDot.Min = myDot.Plane - myDot.Radius;
                myDot.Max = myDot.Plane + myDot.Radius;

                // 분리축 찾기. 축이 있으면 true리턴
                if ((myDot.Max < otherDot.Min) || (myDot.Max > otherDot.Max))
                    return true;
            }
            return false;
        }

        private bool CheckForSphere(ColliderSphere other)
        {
            Matrix obbRotInv = Matrix.Invert(ObjWorldRotTM);

            Vector3 sphereLocalCenter = MultiplyVec3Matrix(other.WorldCenter, obbRotInv);
            Vector3 boxLocalCenter = MultiplyVec3Matrix(WorldCenter, obbRotInv);

            Vector3 boxMin = boxLocalCenter - (Size * 0.5f);
            Vector3 boxMax = boxLocalCenter + (Size * 0.5f);

            float minX = (float)Math.Max(boxMin.X, (float)Math.Min(sphereLocalCenter.X, boxMax.X));
            float minY = (float)Math.Max(boxMin.Y, (float)Math.Min(sphereLocalCenter.Y, boxMax.Y));
            float minZ = (float)Math.Max(boxMin.Z, (float)Math.Min(sphereLocalCenter.Z, boxMax.Z));

            Vector3 minPos = new Vector3(minX, minY, minZ);
            float minDist = (minPos - sphereLocalCenter).Length();

            if (minDist <= other.Radius)
                return true;
            else
                return false;
        }


        public override bool CheckCollision(ColliderComp other)
        {
            if (other.GetType() == typeof(ColliderBox))
            {
                ColliderBox otherbox = (ColliderBox)other;
                return CheckForBox(otherbox);
            }
            else if (other.GetType() == typeof(ColliderSphere))
            {
                ColliderSphere otherSphere = (ColliderSphere)other;
                return CheckForSphere(otherSphere);
            }
            return false;
        }

        public override void LateUpdate()
        {
            base.LateUpdate();
            // 월드센터 업데이트.
            WorldCenter = LocalCenter + _transform!.WorldPosition + new Vector3(0.0f, 0.0f, Size.Z * 0.5f);

            Quaternion quat = new Quaternion(WorldCenter, 1.0f);

            Matrix objRotTM = Matrix.RotationQuaternion(quat);

            // 지난 번 프레임과 비교해서 이번 프레임 회전 값의 변화가 없으면 종료
            if (ObjWorldRotTM == objRotTM)
                return;

            // 충돌 검사에 필요한 정보 캐싱
            ObjWorldRotTM = objRotTM;

            // Transform
            WorldNormals[0] = new(1.0f, 0.0f, 0.0f);
            WorldNormals[0] = Vector3.Normalize(MultiplyVec3Matrix(WorldNormals[0], objRotTM));

            WorldNormals[1] = new(0.0f, 1.0f, 0.0f);
            WorldNormals[1] = Vector3.Normalize(MultiplyVec3Matrix(WorldNormals[1], objRotTM));

            WorldNormals[2] = new(0.0f, 0.0f, 1.0f);
            WorldNormals[2] = Vector3.Normalize(MultiplyVec3Matrix(WorldNormals[2], objRotTM));

        }
    }
}