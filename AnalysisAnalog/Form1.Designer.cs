namespace AnalysisAnalog
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
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
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            DevExpress.XtraSplashScreen.SplashScreenManager splashScreenManager1 = new DevExpress.XtraSplashScreen.SplashScreenManager(this, typeof(global::AnalysisAnalog.SplashScreen1), true, true);
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.barManager1 = new DevExpress.XtraBars.BarManager(this.components);
            this.bar1 = new DevExpress.XtraBars.Bar();
            this.btnOpen = new DevExpress.XtraBars.BarButtonItem();
            this.btnSave = new DevExpress.XtraBars.BarButtonItem();
            this.btnRun = new DevExpress.XtraBars.BarButtonItem();
            this.chRUNStop = new DevExpress.XtraBars.BarCheckItem();
            this.btnAddParametr = new DevExpress.XtraBars.BarButtonItem();
            this.barCheckRec = new DevExpress.XtraBars.BarCheckItem();
            this.bar3 = new DevExpress.XtraBars.Bar();
            this.barStaticItem1 = new DevExpress.XtraBars.BarStaticItem();
            this.barStaticItem2 = new DevExpress.XtraBars.BarStaticItem();
            this.bar2 = new DevExpress.XtraBars.Bar();
            this.barStaticItem3 = new DevExpress.XtraBars.BarStaticItem();
            this.barEditCounterPocket = new DevExpress.XtraBars.BarEditItem();
            this.repositoryItemSpinEdit1 = new DevExpress.XtraEditors.Repository.RepositoryItemSpinEdit();
            this.barStaticItem4 = new DevExpress.XtraBars.BarStaticItem();
            this.barDockControlTop = new DevExpress.XtraBars.BarDockControl();
            this.barDockControlBottom = new DevExpress.XtraBars.BarDockControl();
            this.barDockControlLeft = new DevExpress.XtraBars.BarDockControl();
            this.barDockControlRight = new DevExpress.XtraBars.BarDockControl();
            this.gridControl1 = new DevExpress.XtraGrid.GridControl();
            this.analysisBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.gridView1 = new DevExpress.XtraGrid.Views.Grid.GridView();
            this.colName = new DevExpress.XtraGrid.Columns.GridColumn();
            this.colAddress = new DevExpress.XtraGrid.Columns.GridColumn();
            this.colCmr = new DevExpress.XtraGrid.Columns.GridColumn();
            this.colSrednie = new DevExpress.XtraGrid.Columns.GridColumn();
            this.colSko = new DevExpress.XtraGrid.Columns.GridColumn();
            this.colFizika = new DevExpress.XtraGrid.Columns.GridColumn();
            this.colCounter = new DevExpress.XtraGrid.Columns.GridColumn();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.saveFileDialog2 = new System.Windows.Forms.SaveFileDialog();
            ((System.ComponentModel.ISupportInitialize)(this.barManager1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.repositoryItemSpinEdit1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.gridControl1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.analysisBindingSource)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.gridView1)).BeginInit();
            this.SuspendLayout();
            // 
            // splashScreenManager1
            // 
            splashScreenManager1.ClosingDelay = 500;
            // 
            // barManager1
            // 
            this.barManager1.Bars.AddRange(new DevExpress.XtraBars.Bar[] {
            this.bar1,
            this.bar3,
            this.bar2});
            this.barManager1.DockControls.Add(this.barDockControlTop);
            this.barManager1.DockControls.Add(this.barDockControlBottom);
            this.barManager1.DockControls.Add(this.barDockControlLeft);
            this.barManager1.DockControls.Add(this.barDockControlRight);
            this.barManager1.Form = this;
            this.barManager1.Items.AddRange(new DevExpress.XtraBars.BarItem[] {
            this.btnRun,
            this.btnAddParametr,
            this.barStaticItem1,
            this.chRUNStop,
            this.btnSave,
            this.btnOpen,
            this.barCheckRec,
            this.barStaticItem2,
            this.barStaticItem3,
            this.barEditCounterPocket,
            this.barStaticItem4});
            this.barManager1.MaxItemId = 13;
            this.barManager1.RepositoryItems.AddRange(new DevExpress.XtraEditors.Repository.RepositoryItem[] {
            this.repositoryItemSpinEdit1});
            this.barManager1.StatusBar = this.bar3;
            // 
            // bar1
            // 
            this.bar1.BarName = "Tools";
            this.bar1.DockCol = 0;
            this.bar1.DockRow = 0;
            this.bar1.DockStyle = DevExpress.XtraBars.BarDockStyle.Top;
            this.bar1.LinksPersistInfo.AddRange(new DevExpress.XtraBars.LinkPersistInfo[] {
            new DevExpress.XtraBars.LinkPersistInfo(this.btnOpen),
            new DevExpress.XtraBars.LinkPersistInfo(this.btnSave),
            new DevExpress.XtraBars.LinkPersistInfo(this.btnRun),
            new DevExpress.XtraBars.LinkPersistInfo(this.chRUNStop),
            new DevExpress.XtraBars.LinkPersistInfo(this.btnAddParametr),
            new DevExpress.XtraBars.LinkPersistInfo(this.barCheckRec)});
            this.bar1.Text = "Tools";
            // 
            // btnOpen
            // 
            this.btnOpen.Caption = "Open";
            this.btnOpen.Id = 7;
            this.btnOpen.ImageOptions.Image = ((System.Drawing.Image)(resources.GetObject("btnOpen.ImageOptions.Image")));
            this.btnOpen.Name = "btnOpen";
            this.btnOpen.ItemClick += new DevExpress.XtraBars.ItemClickEventHandler(this.BtnOpen_ItemClick);
            // 
            // btnSave
            // 
            this.btnSave.Caption = "Save";
            this.btnSave.Id = 6;
            this.btnSave.ImageOptions.Image = ((System.Drawing.Image)(resources.GetObject("btnSave.ImageOptions.Image")));
            this.btnSave.Name = "btnSave";
            this.btnSave.ItemClick += new DevExpress.XtraBars.ItemClickEventHandler(this.BtnSave_ItemClick);
            // 
            // btnRun
            // 
            this.btnRun.Caption = "Connect";
            this.btnRun.Id = 0;
            this.btnRun.ImageOptions.Image = ((System.Drawing.Image)(resources.GetObject("btnRun.ImageOptions.Image")));
            this.btnRun.Name = "btnRun";
            this.btnRun.ItemClick += new DevExpress.XtraBars.ItemClickEventHandler(this.BtnRun_ItemClick);
            // 
            // chRUNStop
            // 
            this.chRUNStop.Caption = "RUN/STOP";
            this.chRUNStop.Id = 5;
            this.chRUNStop.ImageOptions.DisabledImage = ((System.Drawing.Image)(resources.GetObject("chRUNStop.ImageOptions.DisabledImage")));
            this.chRUNStop.ImageOptions.Image = ((System.Drawing.Image)(resources.GetObject("chRUNStop.ImageOptions.Image")));
            this.chRUNStop.Name = "chRUNStop";
            this.chRUNStop.CheckedChanged += new DevExpress.XtraBars.ItemClickEventHandler(this.ChRUNStop_CheckedChanged);
            // 
            // btnAddParametr
            // 
            this.btnAddParametr.Caption = "AddParametr";
            this.btnAddParametr.Id = 2;
            this.btnAddParametr.ImageOptions.Image = ((System.Drawing.Image)(resources.GetObject("btnAddParametr.ImageOptions.Image")));
            this.btnAddParametr.Name = "btnAddParametr";
            this.btnAddParametr.ItemClick += new DevExpress.XtraBars.ItemClickEventHandler(this.BtnAddParametr_ItemClick);
            // 
            // barCheckRec
            // 
            this.barCheckRec.Caption = "REC";
            this.barCheckRec.Id = 8;
            this.barCheckRec.ImageOptions.Image = ((System.Drawing.Image)(resources.GetObject("barCheckRec.ImageOptions.Image")));
            this.barCheckRec.Name = "barCheckRec";
            this.barCheckRec.CheckedChanged += new DevExpress.XtraBars.ItemClickEventHandler(this.BarCheckItem1_CheckedChanged);
            // 
            // bar3
            // 
            this.bar3.BarName = "Status bar";
            this.bar3.CanDockStyle = DevExpress.XtraBars.BarCanDockStyle.Bottom;
            this.bar3.DockCol = 0;
            this.bar3.DockRow = 0;
            this.bar3.DockStyle = DevExpress.XtraBars.BarDockStyle.Bottom;
            this.bar3.LinksPersistInfo.AddRange(new DevExpress.XtraBars.LinkPersistInfo[] {
            new DevExpress.XtraBars.LinkPersistInfo(this.barStaticItem1),
            new DevExpress.XtraBars.LinkPersistInfo(this.barStaticItem2)});
            this.bar3.OptionsBar.AllowQuickCustomization = false;
            this.bar3.OptionsBar.DrawDragBorder = false;
            this.bar3.OptionsBar.UseWholeRow = true;
            this.bar3.Text = "Status bar";
            // 
            // barStaticItem1
            // 
            this.barStaticItem1.Caption = "Adapter Status";
            this.barStaticItem1.Id = 4;
            this.barStaticItem1.Name = "barStaticItem1";
            // 
            // barStaticItem2
            // 
            this.barStaticItem2.Caption = "Status Write LOG";
            this.barStaticItem2.Id = 9;
            this.barStaticItem2.Name = "barStaticItem2";
            // 
            // bar2
            // 
            this.bar2.BarName = "Пользовательская 4";
            this.bar2.DockCol = 1;
            this.bar2.DockRow = 0;
            this.bar2.DockStyle = DevExpress.XtraBars.BarDockStyle.Top;
            this.bar2.FloatLocation = new System.Drawing.Point(225, 131);
            this.bar2.LinksPersistInfo.AddRange(new DevExpress.XtraBars.LinkPersistInfo[] {
            new DevExpress.XtraBars.LinkPersistInfo(this.barStaticItem3),
            new DevExpress.XtraBars.LinkPersistInfo(this.barEditCounterPocket),
            new DevExpress.XtraBars.LinkPersistInfo(this.barStaticItem4)});
            this.bar2.Offset = 305;
            this.bar2.Text = "Пользовательская 4";
            // 
            // barStaticItem3
            // 
            this.barStaticItem3.Border = DevExpress.XtraEditors.Controls.BorderStyles.NoBorder;
            this.barStaticItem3.Caption = "Обработка каждого";
            this.barStaticItem3.Id = 10;
            this.barStaticItem3.Name = "barStaticItem3";
            // 
            // barEditCounterPocket
            // 
            this.barEditCounterPocket.Caption = "barEditItem1";
            this.barEditCounterPocket.Edit = this.repositoryItemSpinEdit1;
            this.barEditCounterPocket.EditValue = "3";
            this.barEditCounterPocket.Id = 11;
            this.barEditCounterPocket.Name = "barEditCounterPocket";
            // 
            // repositoryItemSpinEdit1
            // 
            this.repositoryItemSpinEdit1.AutoHeight = false;
            this.repositoryItemSpinEdit1.Buttons.AddRange(new DevExpress.XtraEditors.Controls.EditorButton[] {
            new DevExpress.XtraEditors.Controls.EditorButton(DevExpress.XtraEditors.Controls.ButtonPredefines.Combo)});
            this.repositoryItemSpinEdit1.IsFloatValue = false;
            this.repositoryItemSpinEdit1.Mask.EditMask = "N00";
            this.repositoryItemSpinEdit1.Name = "repositoryItemSpinEdit1";
            // 
            // barStaticItem4
            // 
            this.barStaticItem4.Caption = "пакета";
            this.barStaticItem4.Id = 12;
            this.barStaticItem4.Name = "barStaticItem4";
            // 
            // barDockControlTop
            // 
            this.barDockControlTop.CausesValidation = false;
            this.barDockControlTop.Dock = System.Windows.Forms.DockStyle.Top;
            this.barDockControlTop.Location = new System.Drawing.Point(0, 0);
            this.barDockControlTop.Manager = this.barManager1;
            this.barDockControlTop.Size = new System.Drawing.Size(841, 47);
            // 
            // barDockControlBottom
            // 
            this.barDockControlBottom.CausesValidation = false;
            this.barDockControlBottom.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.barDockControlBottom.Location = new System.Drawing.Point(0, 339);
            this.barDockControlBottom.Manager = this.barManager1;
            this.barDockControlBottom.Size = new System.Drawing.Size(841, 25);
            // 
            // barDockControlLeft
            // 
            this.barDockControlLeft.CausesValidation = false;
            this.barDockControlLeft.Dock = System.Windows.Forms.DockStyle.Left;
            this.barDockControlLeft.Location = new System.Drawing.Point(0, 47);
            this.barDockControlLeft.Manager = this.barManager1;
            this.barDockControlLeft.Size = new System.Drawing.Size(0, 292);
            // 
            // barDockControlRight
            // 
            this.barDockControlRight.CausesValidation = false;
            this.barDockControlRight.Dock = System.Windows.Forms.DockStyle.Right;
            this.barDockControlRight.Location = new System.Drawing.Point(841, 47);
            this.barDockControlRight.Manager = this.barManager1;
            this.barDockControlRight.Size = new System.Drawing.Size(0, 292);
            // 
            // gridControl1
            // 
            this.gridControl1.DataSource = this.analysisBindingSource;
            this.gridControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gridControl1.Location = new System.Drawing.Point(0, 47);
            this.gridControl1.MainView = this.gridView1;
            this.gridControl1.MenuManager = this.barManager1;
            this.gridControl1.Name = "gridControl1";
            this.gridControl1.Size = new System.Drawing.Size(841, 292);
            this.gridControl1.TabIndex = 4;
            this.gridControl1.ViewCollection.AddRange(new DevExpress.XtraGrid.Views.Base.BaseView[] {
            this.gridView1});
            // 
            // analysisBindingSource
            // 
            this.analysisBindingSource.DataSource = typeof(AnalysisAnalog.Form1.Analysis);
            // 
            // gridView1
            // 
            this.gridView1.Columns.AddRange(new DevExpress.XtraGrid.Columns.GridColumn[] {
            this.colName,
            this.colAddress,
            this.colCmr,
            this.colSrednie,
            this.colSko,
            this.colFizika,
            this.colCounter});
            this.gridView1.GridControl = this.gridControl1;
            this.gridView1.Name = "gridView1";
            this.gridView1.OptionsBehavior.AllowDeleteRows = DevExpress.Utils.DefaultBoolean.True;
            this.gridView1.DoubleClick += new System.EventHandler(this.GridView1_DoubleClick);
            // 
            // colName
            // 
            this.colName.FieldName = "Name";
            this.colName.Name = "colName";
            this.colName.Visible = true;
            this.colName.VisibleIndex = 0;
            // 
            // colAddress
            // 
            this.colAddress.FieldName = "Address";
            this.colAddress.Name = "colAddress";
            this.colAddress.Visible = true;
            this.colAddress.VisibleIndex = 1;
            // 
            // colCmr
            // 
            this.colCmr.FieldName = "Cmr";
            this.colCmr.Name = "colCmr";
            this.colCmr.Visible = true;
            this.colCmr.VisibleIndex = 2;
            // 
            // colSrednie
            // 
            this.colSrednie.DisplayFormat.FormatString = "0.00";
            this.colSrednie.DisplayFormat.FormatType = DevExpress.Utils.FormatType.Numeric;
            this.colSrednie.FieldName = "Srednie";
            this.colSrednie.Name = "colSrednie";
            this.colSrednie.Visible = true;
            this.colSrednie.VisibleIndex = 3;
            // 
            // colSko
            // 
            this.colSko.FieldName = "Sko";
            this.colSko.Name = "colSko";
            this.colSko.Visible = true;
            this.colSko.VisibleIndex = 4;
            // 
            // colFizika
            // 
            this.colFizika.DisplayFormat.FormatString = "0.0000";
            this.colFizika.DisplayFormat.FormatType = DevExpress.Utils.FormatType.Numeric;
            this.colFizika.FieldName = "Fizika";
            this.colFizika.Name = "colFizika";
            this.colFizika.Visible = true;
            this.colFizika.VisibleIndex = 5;
            // 
            // colCounter
            // 
            this.colCounter.FieldName = "Counter";
            this.colCounter.Name = "colCounter";
            this.colCounter.Visible = true;
            this.colCounter.VisibleIndex = 6;
            // 
            // saveFileDialog1
            // 
            this.saveFileDialog1.Filter = "an|*.an";
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            this.openFileDialog1.Filter = "an|*.an";
            // 
            // saveFileDialog2
            // 
            this.saveFileDialog2.FileName = "SaveLog";
            this.saveFileDialog2.Filter = "TXT|*.txt";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(841, 364);
            this.Controls.Add(this.gridControl1);
            this.Controls.Add(this.barDockControlLeft);
            this.Controls.Add(this.barDockControlRight);
            this.Controls.Add(this.barDockControlBottom);
            this.Controls.Add(this.barDockControlTop);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Form1";
            this.Text = "AnalogAnalysis";
            ((System.ComponentModel.ISupportInitialize)(this.barManager1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.repositoryItemSpinEdit1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.gridControl1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.analysisBindingSource)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.gridView1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private DevExpress.XtraBars.BarManager barManager1;
        private DevExpress.XtraBars.Bar bar1;
        private DevExpress.XtraBars.Bar bar3;
        private DevExpress.XtraBars.BarDockControl barDockControlTop;
        private DevExpress.XtraBars.BarDockControl barDockControlBottom;
        private DevExpress.XtraBars.BarDockControl barDockControlLeft;
        private DevExpress.XtraBars.BarDockControl barDockControlRight;
        private DevExpress.XtraGrid.GridControl gridControl1;
        private DevExpress.XtraGrid.Views.Grid.GridView gridView1;
        private DevExpress.XtraBars.BarButtonItem btnRun;
        private DevExpress.XtraBars.BarButtonItem btnAddParametr;
        private System.Windows.Forms.BindingSource analysisBindingSource;
        private DevExpress.XtraBars.BarStaticItem barStaticItem1;
        private DevExpress.XtraBars.BarCheckItem chRUNStop;
        private DevExpress.XtraGrid.Columns.GridColumn colAddress;
        private DevExpress.XtraGrid.Columns.GridColumn colSrednie;
        private DevExpress.XtraGrid.Columns.GridColumn colFizika;
        private DevExpress.XtraGrid.Columns.GridColumn colCounter;
        private DevExpress.XtraGrid.Columns.GridColumn colName;
        private DevExpress.XtraBars.BarButtonItem btnSave;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private DevExpress.XtraBars.BarButtonItem btnOpen;
        private DevExpress.XtraBars.BarCheckItem barCheckRec;
        private DevExpress.XtraBars.BarStaticItem barStaticItem2;
        private System.Windows.Forms.SaveFileDialog saveFileDialog2;
        private DevExpress.XtraBars.Bar bar2;
        private DevExpress.XtraBars.BarStaticItem barStaticItem3;
        private DevExpress.XtraBars.BarEditItem barEditCounterPocket;
        private DevExpress.XtraEditors.Repository.RepositoryItemSpinEdit repositoryItemSpinEdit1;
        private DevExpress.XtraBars.BarStaticItem barStaticItem4;
        private DevExpress.XtraGrid.Columns.GridColumn colCmr;
        private DevExpress.XtraGrid.Columns.GridColumn colSko;
    }
}

