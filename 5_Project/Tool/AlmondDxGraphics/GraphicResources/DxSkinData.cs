using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SharpDX;

namespace AlmondDxGraphics.GraphicResources
{
    public class DxSkinData
    {
        public Matrix BindMatrix { get; set; }
        public List<Matrix> InverseBindMatrix { get; set; }
        public List<DxNode> BoneNodes { get; private set; }

        public List<string> BoneNames { get; set; }

        internal void Init(DxModel model)
        {
            Matrix[] m = new Matrix[256];
            m = m.Select(me => Matrix.Identity).ToArray();

            List<Matrix> tempMatrices = new List<Matrix>();
            for (int i = 0; i < BoneNames.Count; i++)
            {
                string s = BoneNames[i];
                var node = model.SearchChildNodeByName(s);
                if (node != null)
                    BoneNodes.Add(node);
            }
        }

        internal Matrix[] GetPalette()
        {
            Matrix[] m = new Matrix[256];
            m = m.Select(me => Matrix.Identity).ToArray();
            int i = 0;

            foreach (var n in BoneNodes)
            {
                var node = n;

                Matrix currentMat = node.PreComputed;
                while (node.Parent != null)
                {
                    node = node.Parent;
                    currentMat *= node.PreComputed;
                }

                m[i] = BindMatrix * InverseBindMatrix[i] * currentMat;

                i++;
            }
            return m;
        }

        public DxSkinData()
        {
            BindMatrix = Matrix.Identity;
            InverseBindMatrix = new List<Matrix>();
            BoneNodes = new List<DxNode>();
            BoneNames = new List<string>();
        }

    }
}
