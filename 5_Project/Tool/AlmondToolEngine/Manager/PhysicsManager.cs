using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;
using AlmondToolEngine.SceneElements.Components;
using AlmondToolEngine.SceneElements.Components.Base;


namespace AlmondToolEngine.Manager
{
    public static class PhysicsManager
    {
        public static void Update(ref List<ColliderComp> colliderList)
        {
            // 업데이트 전, 지난 프레임을 초기화한다.
            ResetColliders(ref colliderList);

            // 충돌체크
            CheckCollisionAll(ref colliderList);

            // 이후, 체크된 것을 브로드캐스트
            BroadCastTriggerEvent(ref colliderList);

        }

        private static void CheckCollisionAll(ref List<ColliderComp> coliderList)
        {
            // 모든 영역 돌면서 충돌처리.
            // Todo : 쿼드트리
            for (int i = 0; i < coliderList.Count; i++)
            {
                for (int j = i + 1; j < coliderList.Count; j++)
                {
                    var target = coliderList[i];
                    var other = coliderList[j];

                    if (target.GameOBJ == other.GameOBJ)
                        continue;

                    bool targetIsOtherParent = (target.GameOBJ == other.GameOBJ!.Parent);
                    bool otherIsTargetsParent = (other.GameOBJ == target.GameOBJ!.Parent);

                    if (targetIsOtherParent || otherIsTargetsParent)
                        continue;

                    if (target.CheckCollision(other))
                    {
                        // 충돌했으면 각각의 콜라이더의 충돌한 물체벡터에 other를 넣어줌.
                        target.CurrColliedColList.Add(other);
                        other.CurrColliedColList.Add(target);

                        // 디버그컬러 변경
                        target.DebugColor = new Color4(1.0f, 0.0f, 0.0f, 1.0f);
                        other.DebugColor = new Color4(1.0f, 0.0f, 0.0f, 1.0f);
                    }
                }
            }
        }

        private static void ResetColliders(ref List<ColliderComp> colList)
        {
            foreach (var col in colList)
            {
                // 현재 프레임에 있던 콜라이더들은 이전 프레임에 넘겨주고, 클리어
                col.PrevColliedColList = col.CurrColliedColList;
                col.CurrColliedColList.Clear();

                // 레이라면 Hit도 클리어.
                if (col.GetType() == typeof(ColliderRay))
                {
                    var raycol = (ColliderRay)col;
                    raycol.ClearHit();
                }
            }
        }

        private static void BroadCastTriggerEvent(ref List<ColliderComp> colList)
        {
            foreach (var col in colList)
            {
                // 접촉한거 없으면 디버그 컬러는 흰색으로.
                if (col.CurrColliedColList.Count == 0)
                    col.DebugColor = Color4.White;

                //  레이면, Hit처리하고, 컨티뉴
                if (col.GetType() == typeof(ColliderRay))
                {
                    var ray = (ColliderRay)col;
                    ray.SetHit();
                    continue;
                }

                // 현재 프레임에 충돌이 있는데 과거에 없으면 Enter, 둘다 있으면 Stay
                foreach (var currCollider in col.CurrColliedColList)
                {
                    if (currCollider.GetType() == typeof(ColliderRay))
                        continue;
                    
                    if (col.PrevColliedColList.Find(x => x == currCollider) != null)
                        col.GameOBJ!.OnColTriggerStay(currCollider);
                    else
                        col.GameOBJ!.OnColTriggerEnter(currCollider);
                }

                // 반대로 현재에 없는데 과거에 있으면 Exit
                foreach (var prevCollider in col.PrevColliedColList)
                {
                    if (prevCollider.GetType() == typeof(ColliderRay))
                        continue;

                    if (col.CurrColliedColList.Find(x => x == prevCollider) == null)
                        col.GameOBJ!.OnColTriggerExit(prevCollider);
                }


            }
        }


    }
}
