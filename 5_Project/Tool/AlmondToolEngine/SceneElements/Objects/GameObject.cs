using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using AlmondToolEngine.SceneElements.Scenes;
using AlmondToolEngine.SceneElements.Components;
using AlmondToolEngine.SceneElements.Components.Base;

namespace AlmondToolEngine.SceneElements.Objects
{
    public class GameObject : IObject
    {
        public string Name { get; set; }
        public GameObject? Parent { get; set; }
        public List<GameObject> Childs { get; set; }
        public List<Component> Components { get; set; }
        public List<CollisionEventComp> CollisionEventComps { get; set; }
        

        public GameObject(string name)
        {
            Name = name;
            Components = new List<Component>();
            Childs = new List<GameObject>();
            CollisionEventComps = new List<CollisionEventComp>();
        }

        public void Awake(SceneBase scene) 
        { 
            foreach(var component in Components)
            {
                component.Awake(scene);
            }
        }
        public void OnEnable() { }
        public void OnDisable() { }
        public void OnDestroy() { }
        public void PreUpdate() 
        {
            foreach (var comp in Components)
            {
                comp.PreUpdate();
            }
        }
        public void Update() 
        {
            foreach (var comp in Components)
            {
                comp.Update();
            }
        }
        public void LateUpdate() 
        {
            foreach (var comp in Components)
            {
                comp.LateUpdate();
            }
        }

        public void OnColTriggerEnter(ColliderComp other)
        {
            foreach(var comp in CollisionEventComps)
                comp.OnColTriggerEnter(other);
        }

        public void OnColTriggerStay(ColliderComp other)
        {
            foreach (var comp in CollisionEventComps)
                comp.OnColTriggerStay(other);
        }

        public void OnColTriggerExit(ColliderComp other)
        {
            foreach (var comp in CollisionEventComps)
                comp.OnColTriggerExit(other);
        }

        // 사실상 중복을 허용하지 않는다. 컨테이너가 List로 되어있으니, 호출에서 주의할것.
        // 같은 형식의 컴포넌트를 추가하면, 후속 컴포넌트를 찾을 수 없다.
        public void AddComponent<TComponent>() where TComponent : Component, new()
        {
            Component component = new TComponent() { GameOBJ = this };
            Components.Add(component);
            if (typeof(TComponent).BaseType == typeof(CollisionEventComp))
                CollisionEventComps.Add((CollisionEventComp)component);
        }

        // Todo : list find에 RTTI까지... 느리므로 개선 필요.
        public TComponent? GetComponent<TComponent>() where TComponent : Component
        {
            var component = Components.Find(x => x.GetType() == typeof(TComponent));
            if (component != null)
                return (TComponent)component;
            else
                return null;
        }

    }

}

