using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

using AlmondDxGraphics.DXResources;
using AlmondDxGraphics.Manager.Pools;
using SharpDX;
using SharpDX.Direct3D;
using SharpDX.Direct3D11;
using Buffer11 = SharpDX.Direct3D11.Buffer;
using AlmondDxGraphics.GraphicResources.Data;

namespace AlmondDxGraphics.GraphicResources
{
    public class DxMesh : IDisposable
    {
        // 디바이스 포인터
        public DxDevice Device { get; protected set; }

        public List<DxShader>? Shaders { get; set; }

        // 아. 이건 메모리 낭비야. 그냥 넘어감. 시간없다.
        private DxStateRasterizer _dxRasterizerState;

        public string? Name { get; protected set; }

        public BoundingBox BoundingCube { get; protected set; }

        // 버텍스 버퍼
        public Buffer11 VertexBuffer { get; private set; }

        public Buffer11 IndexBuffer { get; private set; }

        public int VertexCount { get; private set; }
        public int IndexCount { get; private set; }

        public DxMaterial? Material { get; private set; }

        public DxNode? Node { get; set; }

        public bool IsAnimated { get; private set; }

        // 이 생성자는 DxNode가 생성될 때 호출된다. (그리고 Node 프로퍼티에 참조값이 전해짐)
        internal DxMesh(DxDevice device, TexturePool texturePool, MeshGeometry data, bool animated)
        {
            Name = data.Name;
            Device = device;
            _dxRasterizerState = new DxStateRasterizer(device.Device);

            // 데이터 복사
            List<VertexFormat> vertices = new List<VertexFormat>(data.Vertices);
            List<int> indices = new List<int>(data.Indices);

            VertexCount = vertices.Count;
            IndexCount = indices.Count;

            // 바운딩 박스 형성
            BoundingCube = new BoundingBox(data.MinVertex, data.MaxVertex);

            // 버퍼
            VertexBuffer = Buffer11.Create<VertexFormat>(Device.Device, BindFlags.VertexBuffer, vertices.ToArray());
            IndexBuffer = Buffer11.Create<int>(Device.Device, BindFlags.IndexBuffer, indices.ToArray());

            if (data.Material != null)
            {
                Material = new DxMaterial(data.Material);
                Material.LoadTexture(device, texturePool);
            }

            IsAnimated = animated;

        }

        public void SetRasterizerState(RSState state)
        {
            _dxRasterizerState.SetState(state);
        }

        public void Apply(RenderingQuary information)
        {
            RenderData_PerObject objCdata = new();
            RenderData_PerFrame objCFrame = new RenderData_PerFrame();
            if (IsAnimated)
            {
                //objCdata.cb_Matrix.World = Node!.PreComputed * information.World;
                objCdata.cb_Matrix.World = information.World;
                objCdata.cb_Matrix.WorldViewProjection = information.WorldViewProjection;
                objCdata.cb_Matrix.WorldInvTranspose = information.WorldInvTranspose;

                objCdata.cb_PaletteMatrices.Pallete = Node.Skinning!.GetPalette();
                objCFrame.cb_ViewPos.ViewPosition = information.CameraPosition;

                objCFrame.cb_FxDirectionalLight = information.DirectionalLight;

                objCdata.cb_objLegacyLight = new LegacyLight
                {
                    Ambient = Material.Ambient,
                    Diffuse = Material.Diffuse,
                    Specular = Material.Specular
                };
            }
            else
            {
                Matrix currentMat = Node!.GetNodeMatrix();
                //objCdata.cb_Matrix.World = currentMat * information.World;
                //objCdata.cb_Matrix.Transform = currentMat * information.ObjectTransform;
                objCdata.cb_Matrix.World = information.World;
                objCdata.cb_Matrix.WorldViewProjection = information.WorldViewProjection;
                objCdata.cb_Matrix.WorldInvTranspose = information.WorldInvTranspose;
                objCFrame.cb_ViewPos.ViewPosition = information.CameraPosition;

                objCFrame.cb_FxDirectionalLight = information.DirectionalLight;

                if (Material != null) // 머티리얼 없는 프리미티브..
                {
                    objCdata.cb_objLegacyLight = new LegacyLight
                    {
                        Ambient = Material.Ambient,
                        Diffuse = Material.Diffuse,
                        Specular = Material.Specular
                    };
                }

            }
            if (Shaders != null)
            {
                foreach (var shader in Shaders)
                {
                    shader!.Apply(objCdata, objCFrame);
                }
            }
            Device.DeviceContext.PixelShader.SetShaderResource(0, Material?.DiffuseTexture);
            Device.DeviceContext.PixelShader.SetShaderResource(1, Material?.NormalTexture);
        }



        public void ApplyPatch(RenderingQuary information)
        {
            RenderData_PerObject objCdata = new();
            RenderData_PerFrame objCFrame = new RenderData_PerFrame();
            if (IsAnimated)
            {
                //objCdata.cb_Matrix.World = Node!.PreComputed * information.World;
                objCdata.cb_Matrix.World = information.World;
                objCdata.cb_Matrix.WorldViewProjection = information.WorldViewProjection;
                objCdata.cb_Matrix.WorldInvTranspose = information.WorldInvTranspose;

                objCdata.cb_PaletteMatrices.Pallete = Node.Skinning!.GetPalette();
                objCFrame.cb_ViewPos.ViewPosition = information.CameraPosition;

                objCFrame.cb_FxDirectionalLight = information.DirectionalLight;

                objCdata.cb_objLegacyLight = new LegacyLight
                {
                    Ambient = Material.Ambient,
                    Diffuse = Material.Diffuse,
                    Specular = Material.Specular
                };
            }
            else
            {
                Matrix currentMat = Node!.GetNodeMatrix();
                //objCdata.cb_Matrix.World = currentMat * information.World;
                //objCdata.cb_Matrix.Transform = currentMat * information.ObjectTransform;
                objCdata.cb_Matrix.World = information.World;
                objCdata.cb_Matrix.WorldViewProjection = information.WorldViewProjection;
                objCdata.cb_Matrix.WorldInvTranspose = information.WorldInvTranspose;
                objCFrame.cb_ViewPos.ViewPosition = information.CameraPosition;

                objCFrame.cb_FxDirectionalLight = information.DirectionalLight;

                if (Material != null) // 머티리얼 없는 프리미티브..
                {
                    objCdata.cb_objLegacyLight = new LegacyLight
                    {
                        Ambient = Material.Ambient,
                        Diffuse = Material.Diffuse,
                        Specular = Material.Specular
                    };
                }

            }
            if (Shaders != null)
            {
                foreach (var shader in Shaders)
                {
                    shader!.Apply(objCdata, objCFrame);
                }
            }
            Device.DeviceContext.PixelShader.SetShaderResource(0, Material?.DiffuseTexture);
            Device.DeviceContext.PixelShader.SetShaderResource(1, Material?.NormalTexture);
            Device.DeviceContext.DomainShader.SetShaderResource(0, Material?.HeightTexture);            
        }

        internal void Draw(DeviceContext context)
        {
            context.Rasterizer.State = _dxRasterizerState.State;
            context.InputAssembler.PrimitiveTopology = PrimitiveTopology.TriangleList;
            context.InputAssembler.SetVertexBuffers(0, new VertexBufferBinding(VertexBuffer, VertexFormat.Size, 0));
            context.InputAssembler.SetIndexBuffer(IndexBuffer, SharpDX.DXGI.Format.R32_UInt, 0);
            context.DrawIndexed(IndexCount, 0, 0);
        }

        internal void DrawPatch(DeviceContext context)
        {
            context.Rasterizer.State = _dxRasterizerState.State;
            context.InputAssembler.PrimitiveTopology = PrimitiveTopology.PatchListWith16ControlPoints;
            context.InputAssembler.SetVertexBuffers(0, new VertexBufferBinding(VertexBuffer, VertexFormat.Size, 0));
            context.InputAssembler.SetIndexBuffer(IndexBuffer, SharpDX.DXGI.Format.R32_UInt, 0);
            context.DrawIndexed(IndexCount, 0, 0);
        }

        public void Dispose()
        {
            _dxRasterizerState.Dispose();
            VertexBuffer.Dispose();
            IndexBuffer.Dispose();
        }
    }
}
