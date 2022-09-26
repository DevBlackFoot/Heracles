using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using AlmondToolEngine;
using AlmondTools_Client.SerializedData;

namespace AlmondTools_Client.Forms
{
    internal class FormManager
    {
        public EngineForWinform Engine { get; private set; }
        public SerializeDataManager DataManager { get; private set; }

        public MainForm DefaultForm { get; set; }

        public FormManager()
        {
            Engine = new EngineForWinform();
            DataManager = new SerializeDataManager();
            DefaultForm = new MainForm(Engine, DataManager);
            Engine.CreateRenderControlPool(DefaultForm.Name);            
        }

    }
}
