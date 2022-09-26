using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;
using AlmondToolEngine.SceneElements.Components.Base;
using AlmondDxGraphics;
using AlmondToolEngine.SceneElements.Scenes;

namespace AlmondToolEngine.SceneElements.Components
{
    public class Camera : Component
    {
        private float _aspectRatio;
        private float _nearZ;
        private float _farZ;
        private float _fov;

        private int _renderControlWidth;
        private int _renderControlHeight;

        public CameraInfo Info { get; private set; }
        public BoundingFrustum Frustum { get; private set; }

        public PROJ_TYPE ProjType { get; set; }
        public Matrix ViewMatrix { get { return Info.viewMatrix; } }
        public Matrix ProjMatrix { get { return Info.projMatrix; } }

        public float NearZ { get { return _nearZ; } set { _nearZ = value; UpdateProjMatrix(); } }
        public float FarZ { get { return _farZ; } set { _farZ = value; UpdateProjMatrix(); } }
        public float Fov { get { return _fov; } set { _fov = value; UpdateProjMatrix(); }}

        private Transform? _transform;

        public override void Update()
        {
            Info.cameraWorldPosition = _transform!.WorldPosition;

            UpdateViewMatrix();
         
        }

        public override void Awake(SceneBase scene)
        {
            base.Awake(scene);
        }

        // 이거가지고 부족함. 무조건 Initialize 필요. (객체 생성 후 붙여야하는 것들 있음)
        public Camera()
        {
            ProjType = PROJ_TYPE.PERSPECTIVE;            
            
            _nearZ = 1.0f;
            _farZ = 5000.0f;
            _fov = (float)(Math.PI * 0.25);
            Info = new CameraInfo();                        
        }

        public void Initialize(AlRenderer targetRenderer)
        {
            _renderControlWidth = targetRenderer.ControlView.Width;
            _renderControlHeight = targetRenderer.ControlView.Height;
            _aspectRatio = (float)_renderControlWidth / (float)_renderControlHeight;
            _transform = GameOBJ!.GetComponent<Transform>()!; // 게임오브젝트도, 트랜스폼 컴포넌트도 무조건 있다.

            UpdateViewMatrix();
            UpdateProjMatrix();
        }

        public void UpdateViewMatrix()
        {
            Info.prevViewMatrix = Info.viewMatrix;
            Info.viewMatrix = Matrix.Invert(_transform.WorldTM);
        }

        public void UpdateProjMatrix()
        {
            Info.prevProjMatrix = Info.projMatrix;
            Info.projMatrix = Matrix.PerspectiveFovLH(_fov, _aspectRatio, _nearZ, _farZ);

            var cameraPos = _transform!.WorldPosition;
            var lookDir = _transform.Look;
            var upDir = _transform.UP;
            // 바운딩 프러스텀 업데이트
            Frustum = BoundingFrustum.FromCamera(cameraPos, lookDir, upDir, _fov, _nearZ, _farZ, _aspectRatio);            
        }

        // 피킹을 위해 카메라의 피킹된 부분에서 반직선을 구하는 함수.
        public void CalculateClickedRayInWorld(int mousePosX, int mousePosY, out Vector3 direction, out Vector3 origin)
        {
            float vx = ((2.0f * mousePosX / _renderControlWidth) - 1.0f) / Info.projMatrix.M11;
            float vy = ((-2.0f * mousePosY / _renderControlHeight) - 1.0f) / Info.projMatrix.M22;

            // View space의 반직선
            var outDirection = new Vector3(vx, vy, 1.0f);
            var outOrigin = new Vector3(0.0f, 0.0f, 0.0f);

            Matrix _viewInv = Matrix.Invert(ViewMatrix);

            // 월드 스페이스의 반직선을 구해서 인수로 내보냄.
            direction = Vector3.TransformNormal(outDirection, _viewInv);
            origin = Vector3.TransformCoordinate(outOrigin, _viewInv);
        }

    }

    public class CameraInfo
    {
        public Vector3 cameraWorldPosition;
        public Matrix viewMatrix;
        public Matrix projMatrix;
        public Matrix prevViewMatrix;
        public Matrix prevProjMatrix;
        public CameraInfo()
        {
            cameraWorldPosition = Vector3.Zero;
            viewMatrix = Matrix.Identity;
            projMatrix = Matrix.Identity;
            prevViewMatrix = Matrix.Identity;
            prevProjMatrix = Matrix.Identity;
        }
    }

    public enum PROJ_TYPE
    {
        PERSPECTIVE,
        ORTHOGRAPHIC
    }
}
