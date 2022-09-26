using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


using SharpDX;

using AlmondToolEngine.SceneElements.Objects;
using AlmondToolEngine.SceneElements.Components.Base;
using AlmondToolEngine.SceneElements.Scenes;

namespace AlmondToolEngine.SceneElements.Components
{
    public class RaycastHit
    {
        public GameObject? GameObject { get; set; }
        public Transform? Transform { get; set; }
        public float Distance { get; set; }
        public string Name { get; set; }

        public RaycastHit()
        {
            Distance = 0;
            Name = "HitNone";
        }
    }

    public class ColliderRay : ColliderComp
    {
        public Vector3 LocalOrigin { get; set; }
        public Vector3 WorldOrigin { get; set; }
        public Vector3 Direction { get; set; }

        // 가장 가까운 충돌오브젝트 정보
        public RaycastHit Hit { get; internal set; }

        // 충돌 오브젝트를 키로 해서 거리를 모아둔 중간변수 컨테이너
        private List<KeyValuePair<GameObject, float>> _currHitObjects;        

        public ColliderRay()
        {
            LocalOrigin = Vector3.Zero;            
            _currHitObjects = new List<KeyValuePair<GameObject, float>>();
            Hit = new RaycastHit();
        }

        public override void Awake(SceneBase scene)
        {
            base.Awake(scene);
            WorldOrigin = _transform!.WorldPosition;
            Direction = _transform!.Look;
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

        public bool CheckForSphere(ColliderSphere other)
        {
            float t0, t1;                                                   // 만날 수 있는 점의 위치
            Vector3 rayToSphere = other.WorldCenter - WorldOrigin;          // 원의 중심으로 향하는 벡터
            float projRayToSphere = Vector3.Dot(rayToSphere, Direction);    // 해당 벡터를 dir에 사영

            // 원의 중심에서 레이까지 수직인 선분의 길이
            float d = Vector3.Dot(rayToSphere, rayToSphere) - projRayToSphere * projRayToSphere;

            // 이 길이가 반지름보다 크면 만나지 않는 것이다.
            if (d > other.Radius)
                return false;

            // 중심에서 벡터로 내린 수선에서 원과 부딪히는 부분까지의 거리
            float solDist = (float)Math.Sqrt((double)(other.Radius - d));
            t0 = projRayToSphere - solDist;
            t1 = projRayToSphere + solDist;
            var distance = GetDistanceforSphereCollision(t0, t1);

            if (distance < 0)
                return false;

            // 이젠 충돌된거니.. 거리를 넣어준다.
            _currHitObjects.Add(new KeyValuePair<GameObject, float>(other.GameOBJ!, distance));

            return true;
        }

        private static float GetDistanceforSphereCollision(float t0, float t1)
        {
            if (t0 > t1)
            {
                var temp = t0;
                t0 = t1;
                t1 = temp;
            }

            if (t0 < 0)
            {
                t0 = t1;                
            }
            return t0;
        }


        private bool CheckForBox(ColliderBox other)
        {
            Matrix obbRotInv = Matrix.Invert(other.ObjWorldRotTM);

            // 포지션과 방향을 other의 영역으로 변환
            Vector3 rayLocalOrigin = MultiplyVec3Matrix(WorldOrigin, obbRotInv);
            Vector3 rayLocalDir = Vector3Reciprocal(
                Vector3.Normalize(
                    MultiplyVec3Matrix(Direction, obbRotInv)));

            Vector3 boxLocalCenter = MultiplyVec3Matrix(other.WorldCenter, obbRotInv);

            var vertices = CreateVerticesForCheckBox(boxLocalCenter, other);

            Vector3 rayMin, rayMax;
            CreateRayFromVertices(out rayMin, out rayMax, vertices, rayLocalOrigin, rayLocalDir);

            float distance;
            if (CheckRayOverlab(rayMin, rayMax, out distance))
            {
                // 오버랩 되었으면.. 겹친것 페어에 등록하고 충돌 반환.
                _currHitObjects.Add(new KeyValuePair<GameObject, float>(other.GameOBJ!, distance));
                return true;
            }
            else
            {
                return false;
            }

        }

        private static bool CheckRayOverlab(Vector3 rayMin, Vector3 rayMax, out float distance)
        {
            // 각 좌표축의 최소값, 최대값을 구한다.
            Vector3 tMin = Vector3.Zero;
            Vector3 tMax = Vector3.Zero;

            for (int i = 0; i < 3; i++)
            {
                if (rayMin[i] > rayMax[i])
                {
                    tMin[i] = rayMax[i];
                    tMax[i] = rayMin[i];
                }
                else
                {
                    tMin[i] = rayMin[i];
                    tMax[i] = rayMax[i];
                }
            }

            float maxOftMin = float.MinValue;
            float minOftMax = float.MaxValue;

            // tMin에서 가장 큰 값과 tMax에서 가장 작은 값을 찾는다.
            for (int i = 0; i < 3; i++)
            {
                if (maxOftMin < tMin[i])
                    maxOftMin = tMin[i];
                if (minOftMax > tMax[i])
                    minOftMax = tMax[i];
            }

            distance = maxOftMin;

            if (maxOftMin > minOftMax) // 만약 tMin의 Max가 tMax의 min보다 크다면, 겹쳐지지 않은거다.
                return false;

            if (minOftMax < 0.0f)      // 0보다 작아도 겹쳐지지 않은 것.
                return false;

            return true;
        }

        private static void CreateRayFromVertices(out Vector3 rayMin, out Vector3 rayMax, List<Vector3> vertices, Vector3 rayLocalOrigin, Vector3 rayLocalDir)
        {
            // Box의 버텍스들을 비교할... x, y, z의 최소, 최대 값 초기화. (역으로 최대, 최소값을 넣어둔다)
            var boxMin = new Vector3(float.MaxValue, float.MaxValue, float.MaxValue);
            var boxMax = new Vector3(float.MinValue, float.MinValue, float.MinValue);

            foreach (var vertex in vertices)
            {
                for (int i = 0; i < 3; i++)
                {
                    if (boxMin[i] > vertex[i])
                        boxMin[i] = vertex[i];
                    if (boxMax[i] > vertex[i])
                        boxMax[i] = vertex[i];
                }
            }
            // 레이의 위치로 뽑아낸 벡터
            boxMin = boxMin - rayLocalOrigin;
            boxMax = boxMax - rayLocalOrigin;

            // 레이에 투영
            rayMin = boxMin * rayLocalDir;
            rayMax = boxMax * rayLocalDir;
        }

        private static List<Vector3> CreateVerticesForCheckBox(Vector3 boxLC, ColliderBox other)
        {
            var vertices = new List<Vector3>();
            // 중심에서 각 extents(각 축의 half) 별로 더하고 빼줘서 vertice정보를 만든다.
            float right = boxLC.X + other.Extents[0]; // 점과 면과의 거리. 어딘지는 정확히 몰?루. Todo: 디버그 후 변수명 변경
            float left = boxLC.X - other.Extents[0];
            float top = boxLC.Y + other.Extents[1];
            float bottom = boxLC.Y - other.Extents[1];
            float back = boxLC.Z + other.Extents[2];
            float front = boxLC.Z - other.Extents[2];

            vertices.Add(new Vector3(right, top, back)); // rightTopBack?
            vertices.Add(new Vector3(right, top, front));  // rightTopFront
            vertices.Add(new Vector3(right, bottom, back));
            vertices.Add(new Vector3(left, top, back));
            vertices.Add(new Vector3(right, bottom, front));
            vertices.Add(new Vector3(left, top, front));
            vertices.Add(new Vector3(left, bottom, back));
            vertices.Add(new Vector3(left, bottom, front));

            return vertices;
        }

        public void SetHit()
        {
            if (_currHitObjects.Count == 0)
                return;

            int idx = 0;
            float min = float.MaxValue;            

            // 안쪽에 있는 distance들을 모두 비교하고..
            for (int i = 0; i < _currHitObjects.Count; i++)
            {
                var hitDist = _currHitObjects[i].Value;
                if (min > hitDist && hitDist > 0)
                {
                    min = hitDist;
                    idx = i;
                }
            }

            Hit.GameObject = _currHitObjects[idx].Key;
            Hit.Transform = _currHitObjects[idx].Key.GetComponent<Transform>();
            Hit.Distance = _currHitObjects[idx].Value;
            Hit.Name = _currHitObjects[idx].Key.Name;
        }

        public void ClearHit()
        {
            _currHitObjects.Clear();
        }

        public override void LateUpdate()
        {
            base.LateUpdate();
            WorldOrigin = LocalOrigin + _transform!.WorldPosition;
        }

    }
}
