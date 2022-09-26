using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;
using AlmondToolEngine.SceneElements.Scenes;
using AlmondDxGraphics.GraphicResources;
using AlmondDxGraphics.GraphicResources.Data;
using AlmondToolEngine.SceneElements.Objects;

namespace AlmondToolEngine.SceneElements.Components.Base
{
    public abstract class ColliderComp : Component
    {
        // 캐싱용 트랜스폼
        protected Transform? _transform;

        protected Camera? _camera;

        protected DirectionalLight? _directionalLight;
        
        // 디버그용 모델. 시간없으니 걍 모든 컴포넌트가 다 갖고 있게. (메모리 낭비다..)
        protected DxModel? _modelForDebug;

        // 현재 충돌한 콜라이더들
        public List<ColliderComp> CurrColliedColList { get; internal set; }
        // 이전 충돌한 콜라이더들
        public List<ColliderComp> PrevColliedColList { get; internal set; }

        public Color4 DebugColor { get; set; }
        

        protected ColliderComp()
        {
            CurrColliedColList = new List<ColliderComp>();
            PrevColliedColList = new List<ColliderComp>();            
            DebugColor = Color4.White;
        }

        public override void Awake(SceneBase scene)
        {
            base.Awake(scene);
            _transform = GameOBJ!.GetComponent<Transform>()!;
            _camera = scene.MainCamera.GetComponent<Camera>()!;
            _directionalLight = scene.DirectionalLight.GetComponent<DirectionalLight>();
            scene.RegisterColliderComp(this);
        }

        // 콜리젼 체크 함수.
        public abstract bool CheckCollision(ColliderComp other);

        // 콜리젼 형태를 랜더하는 함수. 디버그용.
        public void ColliderRender() 
        {         
            var view = _camera!.ViewMatrix;
            var projection = _camera!.ProjMatrix;
            var world = _transform!.WorldTM;

            var worldInv = Matrix.Invert(world);
            var transpose = Matrix.Transpose(worldInv);

            _modelForDebug?.Draw(new RenderingQuary
            {                
                World = world,
                WorldInvTranspose = transpose,
                WorldViewProjection = world * view * projection,
                CameraPosition = _camera!.GameOBJ!.GetComponent<Transform>()!.WorldPosition,
                DirectionalLight = _directionalLight.DirLightInfo
            });
        }

        public void ClearCurrColliedCollist() { CurrColliedColList.Clear(); }
        public bool GetIsCollidedThisFrame() { return CurrColliedColList.Count() != 0; }


        // vector3 와 Matrix를 곱해서 결과 vec3반환하는 함수 (순서 중요!) 이 함수는 vec * matrix임.
        static internal Vector3 MultiplyVec3Matrix(Vector3 vector, Matrix matrix)
        {
            float fx = vector.X;
            float fy = vector.Y;
            float fz = vector.Z;

            var vec = new Vector3();

            vec.X = fx * matrix.M11 + fy * matrix.M21 + matrix.M31 * fz;
            vec.Y = fx * matrix.M12 + fy * matrix.M22 + matrix.M32 * fz;
            vec.Z = fx * matrix.M13 + fy * matrix.M23 + matrix.M33 * fz;

            return vec;
        }

        // vector의 각 변수들의 역수인 벡터를 반환한다. (제공메소드가 뭔지 모르겠다..)
        static internal Vector3 Vector3Reciprocal(Vector3 v)
        {
            return new Vector3
            (
                1.0f / v.X,
                1.0f / v.Y,
                1.0f / v.Z
            );
        }


    }
}
