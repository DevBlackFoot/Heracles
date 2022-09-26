using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SharpDX;

namespace AlmondDxGraphics.GraphicResources.Data
{
    // Todo: 만들다 말았음. 딱히 스키닝이 필요할 것 같지 않다.
    public class SkinInformation
    {
        public Matrix BindMatrix { get; set; }

        public List<string> BoneNames { get; set; }

        public List<Matrix> InverseBinding { get; set; }

        public SkinInformation()
        {
            BoneNames = new List<string>();
            InverseBinding = new List<Matrix>();
        }
    }
}
