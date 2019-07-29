namespace AnalysisAnalog
{
    partial class AddParametr
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
            DevExpress.XtraEditors.DXErrorProvider.ConditionValidationRule conditionValidationRule1 = new DevExpress.XtraEditors.DXErrorProvider.ConditionValidationRule();
            DevExpress.XtraEditors.DXErrorProvider.ConditionValidationRule conditionValidationRule2 = new DevExpress.XtraEditors.DXErrorProvider.ConditionValidationRule();
            DevExpress.XtraEditors.DXErrorProvider.ConditionValidationRule conditionValidationRule3 = new DevExpress.XtraEditors.DXErrorProvider.ConditionValidationRule();
            DevExpress.XtraEditors.DXErrorProvider.ConditionValidationRule conditionValidationRule4 = new DevExpress.XtraEditors.DXErrorProvider.ConditionValidationRule();
            DevExpress.XtraEditors.DXErrorProvider.ConditionValidationRule conditionValidationRule5 = new DevExpress.XtraEditors.DXErrorProvider.ConditionValidationRule();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AddParametr));
            this.labelControl1 = new DevExpress.XtraEditors.LabelControl();
            this.labelControl2 = new DevExpress.XtraEditors.LabelControl();
            this.spinCountArray = new DevExpress.XtraEditors.SpinEdit();
            this.spinEditCMR = new DevExpress.XtraEditors.SpinEdit();
            this.labelControl3 = new DevExpress.XtraEditors.LabelControl();
            this.textEditAddress = new DevExpress.XtraEditors.TextEdit();
            this.textEditName = new DevExpress.XtraEditors.TextEdit();
            this.labelControl4 = new DevExpress.XtraEditors.LabelControl();
            this.simpleButton1 = new DevExpress.XtraEditors.SimpleButton();
            this.dxValidationProvider1 = new DevExpress.XtraEditors.DXErrorProvider.DXValidationProvider(this.components);
            this.textEditMask = new DevExpress.XtraEditors.TextEdit();
            this.simpleButton2 = new DevExpress.XtraEditors.SimpleButton();
            this.labelControl5 = new DevExpress.XtraEditors.LabelControl();
            this.simpleButton3 = new DevExpress.XtraEditors.SimpleButton();
            ((System.ComponentModel.ISupportInitialize)(this.spinCountArray.Properties)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.spinEditCMR.Properties)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.textEditAddress.Properties)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.textEditName.Properties)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dxValidationProvider1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.textEditMask.Properties)).BeginInit();
            this.SuspendLayout();
            // 
            // labelControl1
            // 
            this.labelControl1.Location = new System.Drawing.Point(31, 41);
            this.labelControl1.Name = "labelControl1";
            this.labelControl1.Size = new System.Drawing.Size(36, 13);
            this.labelControl1.TabIndex = 0;
            this.labelControl1.Text = "Адресс";
            // 
            // labelControl2
            // 
            this.labelControl2.Location = new System.Drawing.Point(31, 67);
            this.labelControl2.Name = "labelControl2";
            this.labelControl2.Size = new System.Drawing.Size(78, 13);
            this.labelControl2.TabIndex = 1;
            this.labelControl2.Text = "Размер массива";
            // 
            // spinCountArray
            // 
            this.spinCountArray.EditValue = new decimal(new int[] {
            0,
            0,
            0,
            0});
            this.spinCountArray.Location = new System.Drawing.Point(126, 60);
            this.spinCountArray.Name = "spinCountArray";
            this.spinCountArray.Properties.Buttons.AddRange(new DevExpress.XtraEditors.Controls.EditorButton[] {
            new DevExpress.XtraEditors.Controls.EditorButton(DevExpress.XtraEditors.Controls.ButtonPredefines.Combo)});
            this.spinCountArray.Properties.IsFloatValue = false;
            this.spinCountArray.Properties.Mask.EditMask = "N00";
            this.spinCountArray.Size = new System.Drawing.Size(109, 20);
            this.spinCountArray.TabIndex = 2;
            conditionValidationRule1.ConditionOperator = DevExpress.XtraEditors.DXErrorProvider.ConditionOperator.IsNotBlank;
            conditionValidationRule1.ErrorText = "This value is not valid";
            this.dxValidationProvider1.SetValidationRule(this.spinCountArray, conditionValidationRule1);
            // 
            // spinEditCMR
            // 
            this.spinEditCMR.EditValue = new decimal(new int[] {
            0,
            0,
            0,
            0});
            this.spinEditCMR.Location = new System.Drawing.Point(126, 86);
            this.spinEditCMR.Name = "spinEditCMR";
            this.spinEditCMR.Properties.Buttons.AddRange(new DevExpress.XtraEditors.Controls.EditorButton[] {
            new DevExpress.XtraEditors.Controls.EditorButton(DevExpress.XtraEditors.Controls.ButtonPredefines.Combo)});
            this.spinEditCMR.Size = new System.Drawing.Size(109, 20);
            this.spinEditCMR.TabIndex = 4;
            conditionValidationRule2.ConditionOperator = DevExpress.XtraEditors.DXErrorProvider.ConditionOperator.IsNotBlank;
            conditionValidationRule2.ErrorText = "This value is not valid";
            this.dxValidationProvider1.SetValidationRule(this.spinEditCMR, conditionValidationRule2);
            // 
            // labelControl3
            // 
            this.labelControl3.Location = new System.Drawing.Point(31, 93);
            this.labelControl3.Name = "labelControl3";
            this.labelControl3.Size = new System.Drawing.Size(79, 13);
            this.labelControl3.TabIndex = 3;
            this.labelControl3.Text = "Цена младшего";
            // 
            // textEditAddress
            // 
            this.textEditAddress.Location = new System.Drawing.Point(126, 34);
            this.textEditAddress.Name = "textEditAddress";
            this.textEditAddress.Size = new System.Drawing.Size(109, 20);
            this.textEditAddress.TabIndex = 5;
            conditionValidationRule3.ConditionOperator = DevExpress.XtraEditors.DXErrorProvider.ConditionOperator.IsNotBlank;
            conditionValidationRule3.ErrorText = "This value is not valid";
            this.dxValidationProvider1.SetValidationRule(this.textEditAddress, conditionValidationRule3);
            // 
            // textEditName
            // 
            this.textEditName.Location = new System.Drawing.Point(126, 8);
            this.textEditName.Name = "textEditName";
            this.textEditName.Size = new System.Drawing.Size(109, 20);
            this.textEditName.TabIndex = 7;
            conditionValidationRule4.ConditionOperator = DevExpress.XtraEditors.DXErrorProvider.ConditionOperator.IsNotBlank;
            conditionValidationRule4.ErrorText = "This value is not valid";
            this.dxValidationProvider1.SetValidationRule(this.textEditName, conditionValidationRule4);
            // 
            // labelControl4
            // 
            this.labelControl4.Location = new System.Drawing.Point(31, 15);
            this.labelControl4.Name = "labelControl4";
            this.labelControl4.Size = new System.Drawing.Size(76, 13);
            this.labelControl4.TabIndex = 6;
            this.labelControl4.Text = "Имя параметра";
            // 
            // simpleButton1
            // 
            this.simpleButton1.Location = new System.Drawing.Point(90, 151);
            this.simpleButton1.Name = "simpleButton1";
            this.simpleButton1.Size = new System.Drawing.Size(161, 23);
            this.simpleButton1.TabIndex = 8;
            this.simpleButton1.Text = "Добавить параметр";
            this.simpleButton1.Click += new System.EventHandler(this.SimpleButton1_Click);
            // 
            // textEditMask
            // 
            this.textEditMask.Location = new System.Drawing.Point(126, 112);
            this.textEditMask.Name = "textEditMask";
            this.textEditMask.Size = new System.Drawing.Size(109, 20);
            this.textEditMask.TabIndex = 11;
            conditionValidationRule5.ConditionOperator = DevExpress.XtraEditors.DXErrorProvider.ConditionOperator.IsNotBlank;
            conditionValidationRule5.ErrorText = "This value is not valid";
            this.dxValidationProvider1.SetValidationRule(this.textEditMask, conditionValidationRule5);
            // 
            // simpleButton2
            // 
            this.simpleButton2.Location = new System.Drawing.Point(90, 183);
            this.simpleButton2.Name = "simpleButton2";
            this.simpleButton2.Size = new System.Drawing.Size(161, 23);
            this.simpleButton2.TabIndex = 9;
            this.simpleButton2.Text = "Редактировать  параметр";
            this.simpleButton2.Click += new System.EventHandler(this.SimpleButton2_Click);
            // 
            // labelControl5
            // 
            this.labelControl5.Location = new System.Drawing.Point(31, 119);
            this.labelControl5.Name = "labelControl5";
            this.labelControl5.Size = new System.Drawing.Size(31, 13);
            this.labelControl5.TabIndex = 10;
            this.labelControl5.Text = "Маска";
            // 
            // simpleButton3
            // 
            this.simpleButton3.Location = new System.Drawing.Point(90, 221);
            this.simpleButton3.Name = "simpleButton3";
            this.simpleButton3.Size = new System.Drawing.Size(161, 23);
            this.simpleButton3.TabIndex = 12;
            this.simpleButton3.Text = "Удалить параметр";
            this.simpleButton3.Click += new System.EventHandler(this.SimpleButton3_Click);
            // 
            // AddParametr
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(355, 256);
            this.Controls.Add(this.simpleButton3);
            this.Controls.Add(this.textEditMask);
            this.Controls.Add(this.labelControl5);
            this.Controls.Add(this.simpleButton2);
            this.Controls.Add(this.simpleButton1);
            this.Controls.Add(this.textEditName);
            this.Controls.Add(this.labelControl4);
            this.Controls.Add(this.textEditAddress);
            this.Controls.Add(this.spinEditCMR);
            this.Controls.Add(this.labelControl3);
            this.Controls.Add(this.spinCountArray);
            this.Controls.Add(this.labelControl2);
            this.Controls.Add(this.labelControl1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "AddParametr";
            this.Text = "AddParametr";
            ((System.ComponentModel.ISupportInitialize)(this.spinCountArray.Properties)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.spinEditCMR.Properties)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.textEditAddress.Properties)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.textEditName.Properties)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dxValidationProvider1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.textEditMask.Properties)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private DevExpress.XtraEditors.LabelControl labelControl1;
        private DevExpress.XtraEditors.LabelControl labelControl2;
        private DevExpress.XtraEditors.SpinEdit spinCountArray;
        private DevExpress.XtraEditors.DXErrorProvider.DXValidationProvider dxValidationProvider1;
        private DevExpress.XtraEditors.SpinEdit spinEditCMR;
        private DevExpress.XtraEditors.LabelControl labelControl3;
        private DevExpress.XtraEditors.TextEdit textEditAddress;
        private DevExpress.XtraEditors.TextEdit textEditName;
        private DevExpress.XtraEditors.LabelControl labelControl4;
        private DevExpress.XtraEditors.SimpleButton simpleButton1;
        private DevExpress.XtraEditors.SimpleButton simpleButton2;
        private DevExpress.XtraEditors.TextEdit textEditMask;
        private DevExpress.XtraEditors.LabelControl labelControl5;
        private DevExpress.XtraEditors.SimpleButton simpleButton3;
    }
}