using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX.DXGI;
using SharpDX.D3DCompiler;
using SharpDX.Direct3D11;

using AlmondDxGraphics.DXResources;

namespace AlmondDxGraphics.GraphicResources.Factories
{
    public static class ShaderFactory
    {        
        public static DxShader CompileHullShader(DxDevice device, string filePath)
        {           
            DxShader hullShader = new DxShader(device, filePath,
                new DxShaderDescription()
                {                    
                    HullShaderFunctionName = "HSMain"
                });

            return hullShader;
        }

        public static DxShader CompileDomainShader(DxDevice device, string filePath)
        {
            DxShader domainShader = new DxShader(device, filePath,
                new DxShaderDescription()
                {                    
                    DomainShaderFunctionName = "DSMain"                    
                });

            return domainShader;
        }

        public static DxShader CompileVertexShader(DxDevice device, string filePath)
        {
            DxShader vs = new DxShader(device, filePath,
                new DxShaderDescription()
                {
                    VertexShaderFunctionName = "VSMain"                    
                });

            return vs;
        }

        public static DxShader CompilePixelShader(DxDevice device, string filePath)
        {
            DxShader ps = new DxShader(device, filePath,
                new DxShaderDescription()
                {
                    PixelShaderFunctionName = "PSMain"
                });

            return ps;
        }

        public static InputElement[] CreateInputElement(ShaderReflection reflection)
        {
            List<InputElement> description = new List<InputElement>();

            var desc = reflection.Description;
            int byteOffset = 0;

            for (int i =0; i< desc.InputParameters; i++)
            {                
                var paramDesc = reflection.GetInputParameterDescription(i);
                InputElement element = new InputElement();
                element.SemanticName = paramDesc.SemanticName;
                element.SemanticIndex = paramDesc.SemanticIndex;
                element.AlignedByteOffset = byteOffset;
                element.InstanceDataStepRate = 0;
                element.Classification = InputClassification.PerVertexData;

                if (paramDesc.UsageMask == RegisterComponentMaskFlags.ComponentX)
                {
                    if (paramDesc.ComponentType == RegisterComponentType.UInt32) element.Format = Format.R32_UInt;
                    else if (paramDesc.ComponentType == RegisterComponentType.SInt32) element.Format = Format.R32_SInt;
                    else if (paramDesc.ComponentType == RegisterComponentType.Float32) element.Format = Format.R32_Float;
                    byteOffset += 4;
                }
                else if (paramDesc.UsageMask < RegisterComponentMaskFlags.ComponentZ)
                {
                    if (paramDesc.ComponentType == RegisterComponentType.UInt32) element.Format = Format.R32G32_UInt;
                    else if (paramDesc.ComponentType == RegisterComponentType.SInt32) element.Format = Format.R32G32_SInt;
                    else if (paramDesc.ComponentType == RegisterComponentType.Float32) element.Format = Format.R32G32_Float;
                    byteOffset += 8;
                }
                else if (paramDesc.UsageMask < RegisterComponentMaskFlags.ComponentW)
                {
                    if (paramDesc.ComponentType == RegisterComponentType.UInt32) element.Format = Format.R32G32B32_UInt;
                    else if (paramDesc.ComponentType == RegisterComponentType.SInt32) element.Format = Format.R32G32B32_SInt;
                    else if (paramDesc.ComponentType == RegisterComponentType.Float32) element.Format = Format.R32G32B32_Float;
                    byteOffset += 12;
                }
                else if (paramDesc.UsageMask <= RegisterComponentMaskFlags.All)
                {
                    if (paramDesc.ComponentType == RegisterComponentType.UInt32) element.Format = Format.R32G32B32A32_UInt;
                    else if (paramDesc.ComponentType == RegisterComponentType.SInt32) element.Format = Format.R32G32B32A32_SInt;
                    else if (paramDesc.ComponentType == RegisterComponentType.Float32) element.Format = Format.R32G32B32A32_Float;
                    byteOffset += 16;
                }
                description.Add(element);
            }
            return description.ToArray();
        }

        public static IDxConstantBuffer[] CreateConstantBuffer(DxDevice device, ShaderReflection reflection, EShaderType type)
        {            
            var desc = reflection.Description;
            IDxConstantBuffer[] cbList = new IDxConstantBuffer[desc.ConstantBuffers];

            for (int i = 0; i < desc.ConstantBuffers; i++)
            {
                var reflectedbuffer = reflection.GetConstantBuffer(i);
                var rbsDesc = reflectedbuffer.Description;
                var inputBindingDesc = reflection.GetResourceBindingDescription(rbsDesc.Name);

                // 버퍼 타입을 찾아 해당 버퍼를 생성한다.
                if (rbsDesc.Name == "CB_Matrix")
                    cbList[inputBindingDesc.BindPoint] = new DxConstantBuffer<CB_Matrix>();                
                else if (rbsDesc.Name == "CB_Pallete")
                    cbList[inputBindingDesc.BindPoint] = new DxConstantBuffer<CB_PaletteMatrices>();
                else if (rbsDesc.Name == "CB_ViewPos")
                    cbList[inputBindingDesc.BindPoint] = new DxConstantBuffer<CB_ViewPos>();
                else if (rbsDesc.Name == "CB_FxDirectionalLight")
                    cbList[inputBindingDesc.BindPoint] = new DxConstantBuffer<CB_FxDirectionalLight>();
                else if (rbsDesc.Name == "CB_FxPointLight")
                    cbList[inputBindingDesc.BindPoint] = new DxConstantBuffer<CB_FxPointLight>();
                else if (rbsDesc.Name == "CB_FxSpotLight")
                    cbList[inputBindingDesc.BindPoint] = new DxConstantBuffer<CB_FxSpotLight>();
                else if (rbsDesc.Name == "CB_Material")
                    cbList[inputBindingDesc.BindPoint] = new DxConstantBuffer<LegacyLight>();
                else if (rbsDesc.Name == "CB_MaterialReflect")
                    cbList[inputBindingDesc.BindPoint] = new DxConstantBuffer<LegacyReflect>();
                else if (rbsDesc.Name == "CB_TerrainData")
                    cbList[inputBindingDesc.BindPoint] = new DxConstantBuffer<CB_TerrainData>();

                cbList[inputBindingDesc.BindPoint].Initialize(device.Device, rbsDesc, inputBindingDesc.BindPoint, type);
            }


            return cbList;

        }
    }
}