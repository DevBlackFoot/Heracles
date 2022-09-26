using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SharpDX;

namespace AlmondDxGraphics.GraphicResources.Data
{
    public enum Interpolation
    {        
        Undefined = 0,        
        Linear = 1,        
        Bezier = 2
    }

    public class AnimationData
    {        
        public List<AnimationNode> Nodes { get; set; }
             
        public AnimationData()
        {
            Nodes = new List<AnimationNode>();
        }
    }

    public class AnimationNode
    {
        public List<AnimationNode> Children { get; set; }
        public string Target { get; set; }
        public List<float> Input { get; set; }
        public List<Matrix> Output { get; set; }
        public List<Matrix> In_Tangent { get; set; }
        public List<Matrix> Out_Tangent { get; set; }
        public Interpolation Interpolation { get; set; }

        public AnimationNode()
        {
            Target = "";
            Input = new List<float>();
            Output = new List<Matrix>();
            In_Tangent = new List<Matrix>();
            Out_Tangent = new List<Matrix>();
            Interpolation = 0;
            Children = new List<AnimationNode>();
        }
    }
}
