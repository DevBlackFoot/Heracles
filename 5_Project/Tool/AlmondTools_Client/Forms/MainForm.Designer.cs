namespace AlmondTools_Client.Forms
{
    using AlmondDxGraphics.DXResources;
    partial class MainForm
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        { 
            this.tc_hierarchy = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.treeView_mapObj = new System.Windows.Forms.TreeView();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.treeView_mapObjPrefab = new System.Windows.Forms.TreeView();
            this.tc_prefab = new System.Windows.Forms.TabControl();
            this.tc_hierarchy.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.tc_prefab.SuspendLayout();
            this.SuspendLayout();
            // 
            // tc_hierarchy
            // 
            this.tc_hierarchy.Controls.Add(this.tabPage1);
            this.tc_hierarchy.Location = new System.Drawing.Point(1283, 283);
            this.tc_hierarchy.Name = "tc_hierarchy";
            this.tc_hierarchy.SelectedIndex = 0;
            this.tc_hierarchy.Size = new System.Drawing.Size(196, 464);
            this.tc_hierarchy.TabIndex = 3;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.treeView_mapObj);
            this.tabPage1.Location = new System.Drawing.Point(4, 24);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(188, 436);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "MAP Hierarchy";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // treeView_mapObj
            // 
            this.treeView_mapObj.Location = new System.Drawing.Point(4, 3);
            this.treeView_mapObj.Name = "treeView_mapObj";
            this.treeView_mapObj.Size = new System.Drawing.Size(178, 425);
            this.treeView_mapObj.TabIndex = 0;
            this.treeView_mapObj.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.MapObjNodeItem_Click);
            this.treeView_mapObj.DragDrop += new System.Windows.Forms.DragEventHandler(this.Tree_NodeItemDragOut);
            this.treeView_mapObj.DragOver += new System.Windows.Forms.DragEventHandler(this.Tree_NodeItemDragOver);
            this.treeView_mapObj.MouseClick += new System.Windows.Forms.MouseEventHandler(this.Tree_OpenContextMenu);
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1664, 24);
            this.menuStrip1.TabIndex = 4;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadToolStripMenuItem,
            this.saveToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // loadToolStripMenuItem
            // 
            this.loadToolStripMenuItem.Name = "loadToolStripMenuItem";
            this.loadToolStripMenuItem.Size = new System.Drawing.Size(100, 22);
            this.loadToolStripMenuItem.Text = "Load";
            this.loadToolStripMenuItem.Click += new System.EventHandler(this.LoadFileMenuItem_Click);
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(100, 22);
            this.saveToolStripMenuItem.Text = "Save";
            this.saveToolStripMenuItem.Click += new System.EventHandler(this.SaveFileMenuItem_Click);
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.treeView_mapObjPrefab);
            this.tabPage2.Location = new System.Drawing.Point(4, 24);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(175, 436);
            this.tabPage2.TabIndex = 0;
            this.tabPage2.Text = "배치가능 오브젝트";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // treeView_mapObjPrefab
            // 
            this.treeView_mapObjPrefab.Location = new System.Drawing.Point(3, 2);
            this.treeView_mapObjPrefab.Name = "treeView_mapObjPrefab";
            this.treeView_mapObjPrefab.Size = new System.Drawing.Size(170, 424);
            this.treeView_mapObjPrefab.TabIndex = 0;
            this.treeView_mapObjPrefab.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.MapObjPrefabNodeItem_Click);
            this.treeView_mapObjPrefab.DragOver += new System.Windows.Forms.DragEventHandler(this.Tree_NodeItemDragOver);
            this.treeView_mapObjPrefab.MouseDown += new System.Windows.Forms.MouseEventHandler(this.Tree_NodeItemSelect);
            // 
            // tc_prefab
            // 
            this.tc_prefab.Controls.Add(this.tabPage2);
            this.tc_prefab.Location = new System.Drawing.Point(1481, 283);
            this.tc_prefab.Name = "tc_prefab";
            this.tc_prefab.SelectedIndex = 0;
            this.tc_prefab.Size = new System.Drawing.Size(183, 464);
            this.tc_prefab.TabIndex = 5;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.AutoSize = true;
            this.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.ClientSize = new System.Drawing.Size(1664, 751);
            this.Controls.Add(this.tc_prefab);
            this.Controls.Add(this.tc_hierarchy);
            this.Controls.Add(this.menuStrip1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.KeyPreview = true;
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "MainForm";
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.MainForm_KeyDown);
            this.tc_hierarchy.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.tabPage2.ResumeLayout(false);
            this.tc_prefab.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private DxRenderControl dxRenderControl_mainScene;
        private DxRenderControl dxRenderControl_mapObjPreview;
        private DxRenderControl dxRenderControl_batchObjPreview;
        private TabControl tc_hierarchy;
        private TabPage tabPage1;
        private MenuStrip menuStrip1;
        private ToolStripMenuItem fileToolStripMenuItem;
        private ToolStripMenuItem loadToolStripMenuItem;
        private ToolStripMenuItem saveToolStripMenuItem;
        private TreeView treeView_mapObj;
        private TabPage tabPage2;
        private TreeView treeView_mapObjPrefab;
        private TabControl tc_prefab;
        
    }
}