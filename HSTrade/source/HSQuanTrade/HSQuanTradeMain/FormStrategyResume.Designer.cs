namespace HSQuanTradeMain
{
    partial class FormStrategyResume
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
            this.btnResume = new System.Windows.Forms.Button();
            this.cmbStrategy = new System.Windows.Forms.ComboBox();
            this.lstStrategy = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.lstStrategyInfo = new System.Windows.Forms.ListView();
            this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader5 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader6 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader7 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.panel1 = new System.Windows.Forms.Panel();
            this.btnRemove = new System.Windows.Forms.Button();
            this.panel2 = new System.Windows.Forms.Panel();
            this.panel3 = new System.Windows.Forms.Panel();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnResume
            // 
            this.btnResume.Location = new System.Drawing.Point(264, 3);
            this.btnResume.Name = "btnResume";
            this.btnResume.Size = new System.Drawing.Size(75, 23);
            this.btnResume.TabIndex = 2;
            this.btnResume.Text = "恢复";
            this.btnResume.UseVisualStyleBackColor = true;
            this.btnResume.Click += new System.EventHandler(this.btnResume_Click);
            // 
            // cmbStrategy
            // 
            this.cmbStrategy.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbStrategy.FormattingEnabled = true;
            this.cmbStrategy.Location = new System.Drawing.Point(3, 3);
            this.cmbStrategy.Name = "cmbStrategy";
            this.cmbStrategy.Size = new System.Drawing.Size(255, 20);
            this.cmbStrategy.TabIndex = 9;
            this.cmbStrategy.SelectedIndexChanged += new System.EventHandler(this.cmbStrategy_SelectedIndexChanged);
            // 
            // lstStrategy
            // 
            this.lstStrategy.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});
            this.lstStrategy.Dock = System.Windows.Forms.DockStyle.Left;
            this.lstStrategy.FullRowSelect = true;
            this.lstStrategy.HideSelection = false;
            this.lstStrategy.Location = new System.Drawing.Point(0, 0);
            this.lstStrategy.MultiSelect = false;
            this.lstStrategy.Name = "lstStrategy";
            this.lstStrategy.Size = new System.Drawing.Size(252, 296);
            this.lstStrategy.TabIndex = 10;
            this.lstStrategy.UseCompatibleStateImageBehavior = false;
            this.lstStrategy.View = System.Windows.Forms.View.Details;
            this.lstStrategy.SelectedIndexChanged += new System.EventHandler(this.lstStrategy_SelectedIndexChanged);
            this.lstStrategy.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.lstStrategy_MouseDoubleClick);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "策略名";
            this.columnHeader1.Width = 102;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "ID";
            this.columnHeader2.Width = 120;
            // 
            // lstStrategyInfo
            // 
            this.lstStrategyInfo.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader3,
            this.columnHeader4,
            this.columnHeader5,
            this.columnHeader6,
            this.columnHeader7});
            this.lstStrategyInfo.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lstStrategyInfo.HideSelection = false;
            this.lstStrategyInfo.Location = new System.Drawing.Point(268, 0);
            this.lstStrategyInfo.Margin = new System.Windows.Forms.Padding(25, 3, 25, 3);
            this.lstStrategyInfo.MultiSelect = false;
            this.lstStrategyInfo.Name = "lstStrategyInfo";
            this.lstStrategyInfo.Size = new System.Drawing.Size(462, 296);
            this.lstStrategyInfo.TabIndex = 10;
            this.lstStrategyInfo.UseCompatibleStateImageBehavior = false;
            this.lstStrategyInfo.View = System.Windows.Forms.View.Details;
            this.lstStrategyInfo.SelectedIndexChanged += new System.EventHandler(this.lstStrategy_SelectedIndexChanged);
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "";
            this.columnHeader3.Width = 84;
            // 
            // columnHeader4
            // 
            this.columnHeader4.Text = "";
            this.columnHeader4.Width = 88;
            // 
            // columnHeader5
            // 
            this.columnHeader5.Text = "";
            this.columnHeader5.Width = 91;
            // 
            // columnHeader6
            // 
            this.columnHeader6.Text = "";
            this.columnHeader6.Width = 89;
            // 
            // columnHeader7
            // 
            this.columnHeader7.Text = "";
            this.columnHeader7.Width = 83;
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.cmbStrategy);
            this.panel1.Controls.Add(this.btnRemove);
            this.panel1.Controls.Add(this.btnResume);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(730, 35);
            this.panel1.TabIndex = 11;
            // 
            // btnRemove
            // 
            this.btnRemove.Location = new System.Drawing.Point(362, 3);
            this.btnRemove.Name = "btnRemove";
            this.btnRemove.Size = new System.Drawing.Size(75, 23);
            this.btnRemove.TabIndex = 2;
            this.btnRemove.Text = "删除";
            this.btnRemove.UseVisualStyleBackColor = true;
            this.btnRemove.Click += new System.EventHandler(this.btnRemove_Click);
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.lstStrategyInfo);
            this.panel2.Controls.Add(this.panel3);
            this.panel2.Controls.Add(this.lstStrategy);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel2.Location = new System.Drawing.Point(0, 35);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(730, 296);
            this.panel2.TabIndex = 12;
            // 
            // panel3
            // 
            this.panel3.Dock = System.Windows.Forms.DockStyle.Left;
            this.panel3.Location = new System.Drawing.Point(252, 0);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(16, 296);
            this.panel3.TabIndex = 11;
            // 
            // FormStrategyResume
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(730, 331);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.panel1);
            this.Name = "FormStrategyResume";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "策略恢复";
            this.Load += new System.EventHandler(this.FormStrategyResume_Load);
            this.panel1.ResumeLayout(false);
            this.panel2.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btnResume;
        private System.Windows.Forms.ComboBox cmbStrategy;
        private System.Windows.Forms.ListView lstStrategy;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.ListView lstStrategyInfo;
        private System.Windows.Forms.ColumnHeader columnHeader3;
        private System.Windows.Forms.ColumnHeader columnHeader4;
        private System.Windows.Forms.ColumnHeader columnHeader5;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Panel panel3;
        private System.Windows.Forms.ColumnHeader columnHeader6;
        private System.Windows.Forms.ColumnHeader columnHeader7;
        private System.Windows.Forms.Button btnRemove;
    }
}