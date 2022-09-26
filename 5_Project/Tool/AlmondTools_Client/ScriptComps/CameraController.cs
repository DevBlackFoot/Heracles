using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;
using SharpDX.Windows;
using ImGuiNET;
using AlmondToolEngine.SceneElements.Components.Base;
using AlmondToolEngine.SceneElements.Components;
using AlmondToolEngine.Manager;

namespace AlmondTools_Client.ScriptComps
{
    public class CameraController : CollisionEventComp
    {
        private RenderControlManager? _manager;
        private Transform? _transform;
        private const float _movespeed = 120.0f;
        private bool _mouseClicked = false;


        // 작동하기 위해 반드시 해줘야함. 컨트롤이랑 카메라 받고, 명령에 따른 조작을 입력해주자.
        public void Initialize(RenderControlManager manager)
        {
            _manager = manager;
            _transform = GameOBJ!.GetComponent<Transform>()!;
            SetEvent(manager.AlRenderer.ControlView);
        }

        // 이벤트 처리 입력
        public void SetEvent(RenderControl _renderControl)
        {
            _renderControl.KeyDown += OnKeyboardPush;
            _renderControl.MouseDown += OnMouseClick;
            _renderControl.MouseMove += MoveCameraUsingMouse;
            _renderControl.MouseUp += OffMouseClick;
            _renderControl.MouseWheel += ZoomCameraUsingMouse;
        }

        // 키보드 입력 이벤트
        private void OnKeyboardPush(object? sencder, KeyEventArgs e)
        {
            if (_transform != null)
                switch (e.KeyCode)
                {
                    case Keys.Q:
                        MoveCamera(_transform.Look);
                        break;

                    case Keys.W:
                        MoveCamera(_transform.UP);
                        break;

                    case Keys.A:
                        MoveCamera(-_transform.Right);
                        break;

                    case Keys.S:
                        MoveCamera(-_transform.UP);
                        break;

                    case Keys.D:
                        MoveCamera(_transform.Right);
                        break;

                    case Keys.E:
                        MoveCamera(-_transform.Look);
                        break;
                }
        }
        private void MoveCamera(Vector3 value, float correctionValue = 10.0f)
        {
            _transform!.SetPosition(_transform!.WorldPosition + (value * correctionValue * _movespeed * _manager!.AlTimer.Delta), SPACE.WORLD);
        }

        private void OnMouseClick(object? sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Middle && !ImGui.GetIO().WantCaptureMouse)
                _mouseClicked = true;
        }

        private void OffMouseClick(object? sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Middle)
                _mouseClicked = false;
        }

        private void MoveCameraUsingMouse(object? sender, MouseEventArgs e)
        {
            if (_mouseClicked)
            {
                var rot = _transform!.WorldRotation;

                var deltaMouseMove = 0.15f *
                        (_manager!.MouseTracker.PrevMousePosition - _manager.MouseTracker.CurrMousePosition);
                Vector3 worldXY = new Vector3(deltaMouseMove.Y, deltaMouseMove.X, 0.0f);
                _transform.SetRotation(rot - worldXY, SPACE.WORLD);
            }
        }

        private void ZoomCameraUsingMouse(object? sender, MouseEventArgs e)
        {
            if (_transform != null)
            {
                var collection = (float)e.Delta * 0.1f;
                MoveCamera(_transform.Look, collection);
            }
        }

        public override void OnColTriggerEnter(ColliderComp other) 
        {
            int a = 0;
        }
        public override void OnColTriggerStay(ColliderComp other) 
        { 
            int b = 0;
        }
        public override void OnColTriggerExit(ColliderComp other) 
        { 
            int c = 0;
        }


    }
}
