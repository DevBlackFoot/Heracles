using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;
using AlmondToolEngine.SceneElements.Components.Base;

namespace AlmondToolEngine.SceneElements.Components
{
    public class Transform : Component
    {      

        Matrix _localTM;        
        Vector3 _localPosition;
        Vector3 _localRotation;
        Vector3 _localScale;
        const float _degreeToRadian = (float)Math.PI / 180.0f;

        public Matrix LocalTM { get { return _localTM; }  }
        public Vector3 LocalPosition { get { return _localPosition; } set { _localPosition = value; } }
        public Vector3 LocalRotation { get { return _localRotation; } set { _localRotation = value; } }
        public Vector3 LocalScale { get { return _localScale; } set { _localScale = value; } }

        Matrix _worldTM;
        Vector3 _worldPosition;
        Vector3 _worldRotation;
        Vector3 _worldScale;                   
        Quaternion _worldRotationQuat;

        public Matrix WorldTM { get { if (_needWorldTMUpdate) UpdateWorldTM(); return _worldTM; }}
        public Vector3 WorldPosition { get { return _worldPosition; } }
        public Vector3 WorldRotation { get { return _worldRotation; } }

        public Vector3 WorldScale { get { return _worldScale; } }

        public Quaternion WorldRotationQuatanion { get { return _worldRotationQuat; } }

        Vector3 _look;
        Vector3 _up;
        Vector3 _right;
        public Vector3 Look { get { return _look; } private set { _look = value; } }
        public Vector3 UP { get { return _up; } private set { _up = value; } }
        public Vector3 Right { get { return _right; } private set { _right = value; } }

        private Vector3 _unitZ = Vector3.UnitZ;
        private Vector3 _unitY = Vector3.UnitY;
        private Vector3 _unitX = Vector3.UnitX;

        private bool _needWorldTMUpdate;    // 더럽혀졌다.. 월드포지션을 찾을 때 이걸 보고 TM업데이트함.

        public Transform()
        {
            _localTM = Matrix.Identity;
            _localPosition = Vector3.Zero;
            _localRotation = Vector3.Zero;
            _localScale = Vector3.One;

            _worldTM = Matrix.Identity;
            _worldPosition = Vector3.Zero;
            _worldRotation = Vector3.Zero;
            _worldScale = Vector3.One;
            _worldRotationQuat = Quaternion.Identity;
            _needWorldTMUpdate = true;
        }

        private void UpdateLocalTM()
        {
            // Z : Roll / X :Pitch / Y : Yaw 순으로 회전하는 행렬을 사용
            _localTM = Matrix.Scaling(LocalScale)
                        * Matrix.RotationZ(_degreeToRadian * LocalRotation.Z) * Matrix.RotationX(_degreeToRadian * LocalRotation.X) * Matrix.RotationY(_degreeToRadian * LocalRotation.Y)
                            * Matrix.Translation(LocalPosition);

            MarkTMDirtCheckerForUpdate(); // 자식 트랜스폼 컴포넌트를 순회하며 월드 TM 업데이트 플래그를 On 해준다.
        }


        private void UpdateWorldTM()
        {
            var parent = GameOBJ!.Parent;
            if (parent != null)
            {
                _worldTM = LocalTM * parent.GetComponent<Transform>()!._worldTM;      // 트랜스폼은 모든 오브젝트가 갖고있다.(Todo: 그래도 코드가 맘에 안든다. 예외처리라도 넣자. 어떻게??)
                _worldTM.Decompose(out _worldScale, out _worldRotationQuat, out _worldPosition);
            }
            else
            {
                _worldTM = LocalTM;
                _worldTM.Decompose(out _worldScale, out _worldRotationQuat, out _worldPosition);
            }

            // 월드 기준 내 스페이스 회전 정도를 vector3 변수로 저장해둔다.
            Vector3.TransformNormal(ref _unitZ, ref _worldTM, out _look);
            Vector3.TransformNormal(ref _unitY, ref _worldTM, out _up);
            Vector3.TransformNormal(ref _unitX, ref _worldTM, out _right);

            _needWorldTMUpdate = false;
        }


        // Todo : bool로 변화를 감지하지만... 시간 있을때? 옵저버 패턴으로 변경하자.
        private void MarkTMDirtCheckerForUpdate()
        {
            _needWorldTMUpdate = true;

            var childs = GameOBJ!.Childs;

            foreach (var child in childs)
            {
                child.GetComponent<Transform>()!.MarkTMDirtCheckerForUpdate();
            }
        }

        public void SetPosition(Vector3 newPosition, SPACE relativeTo = SPACE.LOCAL)
        {
            if (relativeTo == SPACE.LOCAL)
            {
                LocalPosition = newPosition;
                UpdateLocalTM();
            }
            else
            {
                _worldPosition = newPosition;
                var parent = GameOBJ!.Parent;
                Vector3 parentPosition = Vector3.Zero;
                if (parent != null)
                {
                    parentPosition = parent.GetComponent<Transform>()!._worldPosition;
                }
                Vector3 newLocalPosition = newPosition - parentPosition;
                SetPosition(newLocalPosition);
            }
        }

        public void SetRotation(Vector3 eulerAngle, SPACE relativeTo = SPACE.LOCAL)
        {
            if (relativeTo == SPACE.LOCAL)
            {
                LocalRotation = eulerAngle;
                UpdateLocalTM();
            }
            else
            {
                _worldRotation = eulerAngle;
                var parent = GameOBJ!.Parent;
                Vector3 parentRotation = Vector3.Zero;
                if (parent != null)
                {
                    parentRotation = parent.GetComponent<Transform>()!._worldRotation;
                }

                Vector3 newLocalRoation = eulerAngle - parentRotation;
                SetRotation(newLocalRoation);
            }
        }

        public void SetScale(Vector3 newLocalScale)
        {
            LocalScale = newLocalScale;
            UpdateLocalTM();
        }

        public void SetLocalTM(Matrix localTM)
        {
            _localTM = localTM;

            Quaternion tempLocalQuat;

            _localTM.Decompose(out _localScale, out tempLocalQuat, out _localPosition);

            _localRotation = QuatanionToEular(tempLocalQuat);

            MarkTMDirtCheckerForUpdate();
        }

        public void SetWorldTM(Matrix worldTM)
        {
            _worldTM = worldTM;
            var parent = GameOBJ!.Parent;            
            if (parent != null)
            {
                Matrix parentWorldTM = parent.GetComponent<Transform>()!.WorldTM;
                _localTM = _worldTM * Matrix.Invert(parentWorldTM);
            }
            else
            {
                _localTM = _worldTM;
            }
            
            Quaternion tempQuat;
            _worldTM.Decompose(out _worldScale, out tempQuat, out _worldPosition);
            _worldRotation = QuatanionToEular(tempQuat);
            _localTM.Decompose(out _localScale, out tempQuat, out _localPosition);
            _localRotation = QuatanionToEular(tempQuat);

            MarkTMDirtCheckerForUpdate();
        }


        // 쿼터니안을 오일러 벡터로 변환하는 변환 함수.
        // transform 이외에서도 쓸 것 같지만.. 이게 필요한 곳에서는 transform을 분명 using할 것 같다.
        // 위치가 영 맘에 안들 때는 추후 이동시켜야지.
        public static Vector3 QuatanionToEular(Quaternion quat)
        {
            double x = quat.X;
            double y = quat.Y;
            double z = quat.Z;
            double w = quat.W;

            double sqx = x * x;
            double sqy = y * y;
            double sqz = z * z;
            double sqw = w * w;

            double exceptCheck = 2.0f * (w * x - y * z);
            float eulerX;

            if (Math.Abs(exceptCheck) >= 1.0f)
                eulerX = (float)Math.CopySign(Math.PI, exceptCheck);
            else
                eulerX = (float)exceptCheck;

            float eulerY = (float)Math.Atan2(2.0f * (x * z + w * y), (-sqx - sqy + sqz + sqw));
            float eulerZ = (float)Math.Atan2(2.0f * (x * y + z * w), (-sqx + sqy - sqz + sqw));

            return new Vector3(eulerX, eulerY, eulerZ);
        }
    }

    public enum SPACE
    {
        WORLD,
        LOCAL
    }

}
