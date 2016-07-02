namespace HSQuanTradeMain
{
    partial class FormStrategyAutoOrder
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
            this.panel1 = new System.Windows.Forms.Panel();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.lstTradeRec = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader5 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader6 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader7 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader8 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.lstLog = new System.Windows.Forms.ListView();
            this.columnHeader9 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader10 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.splitter1 = new System.Windows.Forms.Splitter();
            this.panel2 = new System.Windows.Forms.Panel();
            this.tjiacha = new System.Windows.Forms.TextBox();
            this.ljiacha = new System.Windows.Forms.Label();
            this.ttime = new System.Windows.Forms.TextBox();
            this.ltime = new System.Windows.Forms.Label();
            this.ttype = new System.Windows.Forms.TextBox();
            this.ltype = new System.Windows.Forms.Label();
            this.chkIsOrder = new System.Windows.Forms.CheckBox();
            this.btnRun = new System.Windows.Forms.Button();
            this.btnApply = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.panel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.splitContainer1);
            this.panel1.Controls.Add(this.splitter1);
            this.panel1.Controls.Add(this.panel2);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(1028, 600);
            this.panel1.TabIndex = 6;
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(3, 65);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.dataGridView1);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.tabControl1);
            this.splitContainer1.Size = new System.Drawing.Size(1025, 535);
            this.splitContainer1.SplitterDistance = 291;
            this.splitContainer1.TabIndex = 1;
            // 
            // dataGridView1
            // 
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dataGridView1.Location = new System.Drawing.Point(0, 0);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.RowTemplate.Height = 23;
            this.dataGridView1.Size = new System.Drawing.Size(291, 535);
            this.dataGridView1.TabIndex = 0;
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.Location = new System.Drawing.Point(0, 0);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(730, 535);
            this.tabControl1.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.lstTradeRec);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3, 3, 3, 3);
            this.tabPage1.Size = new System.Drawing.Size(722, 509);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "交易记录";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // lstTradeRec
            // 
            this.lstTradeRec.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3,
            this.columnHeader4,
            this.columnHeader5,
            this.columnHeader6,
            this.columnHeader7,
            this.columnHeader8});
            this.lstTradeRec.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lstTradeRec.FullRowSelect = true;
            this.lstTradeRec.Location = new System.Drawing.Point(3, 3);
            this.lstTradeRec.MultiSelect = false;
            this.lstTradeRec.Name = "lstTradeRec";
            this.lstTradeRec.Size = new System.Drawing.Size(716, 503);
            this.lstTradeRec.TabIndex = 0;
            this.lstTradeRec.UseCompatibleStateImageBehavior = false;
            this.lstTradeRec.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "时间";
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "股票代码";
            this.columnHeader2.Width = 103;
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "股票名称";
            this.columnHeader3.Width = 90;
            // 
            // columnHeader4
            // 
            this.columnHeader4.Text = "交易方向";
            this.columnHeader4.Width = 82;
            // 
            // columnHeader5
            // 
            this.columnHeader5.Text = "价格";
            this.columnHeader5.Width = 88;
            // 
            // columnHeader6
            // 
            this.columnHeader6.Text = "成交价格";
            this.columnHeader6.Width = 108;
            // 
            // columnHeader7
            // 
            this.columnHeader7.Text = "数量";
            this.columnHeader7.Width = 116;
            // 
            // columnHeader8
            // 
            this.columnHeader8.Text = "金额";
            this.columnHeader8.Width = 96;
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.lstLog);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3, 3, 3, 3);
            this.tabPage2.Size = new System.Drawing.Size(942, 653);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "日志";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // lstLog
            // 
            this.lstLog.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader9,
            this.columnHeader10});
            this.lstLog.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lstLog.FullRowSelect = true;
            this.lstLog.Location = new System.Drawing.Point(3, 3);
            this.lstLog.Name = "lstLog";
            this.lstLog.Size = new System.Drawing.Size(936, 647);
            this.lstLog.TabIndex = 1;
            this.lstLog.UseCompatibleStateImageBehavior = false;
            this.lstLog.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader9
            // 
            this.columnHeader9.Text = "时间";
            this.columnHeader9.Width = 182;
            // 
            // columnHeader10
            // 
            this.columnHeader10.Text = "信息";
            this.columnHeader10.Width = 843;
            // 
            // splitter1
            // 
            this.splitter1.Location = new System.Drawing.Point(0, 65);
            this.splitter1.Name = "splitter1";
            this.splitter1.Size = new System.Drawing.Size(3, 535);
            this.splitter1.TabIndex = 0;
            this.splitter1.TabStop = false;
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.tjiacha);
            this.panel2.Controls.Add(this.ljiacha);
            this.panel2.Controls.Add(this.ttime);
            this.panel2.Controls.Add(this.ltime);
            this.panel2.Controls.Add(this.ttype);
            this.panel2.Controls.Add(this.ltype);
            this.panel2.Controls.Add(this.chkIsOrder);
            this.panel2.Controls.Add(this.btnRun);
            this.panel2.Controls.Add(this.btnApply);
            this.panel2.Controls.Add(this.label1);
            this.panel2.Controls.Add(this.comboBox1);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel2.Location = new System.Drawing.Point(0, 0);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(1028, 65);
            this.panel2.TabIndex = 0;
            this.panel2.Paint += new System.Windows.Forms.PaintEventHandler(this.panel2_Paint);
            // 
            // tjiacha
            // 
            this.tjiacha.Location = new System.Drawing.Point(971, 24);
            this.tjiacha.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.tjiacha.Name = "tjiacha";
            this.tjiacha.Size = new System.Drawing.Size(46, 21);
            this.tjiacha.TabIndex = 18;
            this.tjiacha.Text = "9999";
            // 
            // ljiacha
            // 
            this.ljiacha.AutoSize = true;
            this.ljiacha.Location = new System.Drawing.Point(938, 27);
            this.ljiacha.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.ljiacha.Name = "ljiacha";
            this.ljiacha.Size = new System.Drawing.Size(29, 12);
            this.ljiacha.TabIndex = 17;
            this.ljiacha.Text = "价差";
            // 
            // ttime
            // 
            this.ttime.Location = new System.Drawing.Point(875, 24);
            this.ttime.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.ttime.Name = "ttime";
            this.ttime.Size = new System.Drawing.Size(44, 21);
            this.ttime.TabIndex = 16;
            this.ttime.Text = "9999";
            // 
            // ltime
            // 
            this.ltime.AutoSize = true;
            this.ltime.Location = new System.Drawing.Point(818, 27);
            this.ltime.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.ltime.Name = "ltime";
            this.ltime.Size = new System.Drawing.Size(53, 12);
            this.ltime.TabIndex = 15;
            this.ltime.Text = "超时时间";
            // 
            // ttype
            // 
            this.ttype.Location = new System.Drawing.Point(756, 24);
            this.ttype.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.ttype.Name = "ttype";
            this.ttype.Size = new System.Drawing.Size(42, 21);
            this.ttype.TabIndex = 14;
            this.ttype.Text = "-1";
            // 
            // ltype
            // 
            this.ltype.AutoSize = true;
            this.ltype.Location = new System.Drawing.Point(699, 27);
            this.ltype.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.ltype.Name = "ltype";
            this.ltype.Size = new System.Drawing.Size(53, 12);
            this.ltype.TabIndex = 13;
            this.ltype.Text = "下单模式";
            // 
            // chkIsOrder
            // 
            this.chkIsOrder.AutoSize = true;
            this.chkIsOrder.Location = new System.Drawing.Point(566, 35);
            this.chkIsOrder.Name = "chkIsOrder";
            this.chkIsOrder.Size = new System.Drawing.Size(72, 16);
            this.chkIsOrder.TabIndex = 12;
            this.chkIsOrder.Text = "执行下单";
            this.chkIsOrder.UseVisualStyleBackColor = true;
            this.chkIsOrder.CheckedChanged += new System.EventHandler(this.chkIsOrder_CheckedChanged);
            // 
            // btnRun
            // 
            this.btnRun.Location = new System.Drawing.Point(468, 31);
            this.btnRun.Name = "btnRun";
            this.btnRun.Size = new System.Drawing.Size(75, 23);
            this.btnRun.TabIndex = 11;
            this.btnRun.Text = "运行策略";
            this.btnRun.UseVisualStyleBackColor = true;
            this.btnRun.Click += new System.EventHandler(this.btnRun_Click);
            // 
            // btnApply
            // 
            this.btnApply.Enabled = false;
            this.btnApply.Location = new System.Drawing.Point(370, 31);
            this.btnApply.Name = "btnApply";
            this.btnApply.Size = new System.Drawing.Size(75, 23);
            this.btnApply.TabIndex = 10;
            this.btnApply.Text = "应用";
            this.btnApply.UseVisualStyleBackColor = true;
            this.btnApply.Click += new System.EventHandler(this.btnApply_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(1, 7);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(101, 12);
            this.label1.TabIndex = 9;
            this.label1.Text = "选择一个交易策略";
            // 
            // comboBox1
            // 
            this.comboBox1.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Location = new System.Drawing.Point(3, 31);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(332, 20);
            this.comboBox1.TabIndex = 8;
            this.comboBox1.SelectedIndexChanged += new System.EventHandler(this.comboBox1_SelectedIndexChanged);
            // 
            // FormStrategyAutoOrder
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1028, 600);
            this.Controls.Add(this.panel1);
            this.Name = "FormStrategyAutoOrder";
            this.Text = "策略自动买入";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FormStrategyAutoOrder_FormClosing);
            this.Load += new System.EventHandler(this.FormStrategyAutoOrder_Load);
            this.panel1.ResumeLayout(false);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage2.ResumeLayout(false);
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.Splitter splitter1;
        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.ListView lstTradeRec;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.ColumnHeader columnHeader3;
        private System.Windows.Forms.ColumnHeader columnHeader4;
        private System.Windows.Forms.ColumnHeader columnHeader5;
        private System.Windows.Forms.ColumnHeader columnHeader6;
        private System.Windows.Forms.ColumnHeader columnHeader7;
        private System.Windows.Forms.ColumnHeader columnHeader8;
        private System.Windows.Forms.ListView lstLog;
        private System.Windows.Forms.ColumnHeader columnHeader9;
        private System.Windows.Forms.ColumnHeader columnHeader10;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.CheckBox chkIsOrder;
        private System.Windows.Forms.Button btnRun;
        private System.Windows.Forms.Button btnApply;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.TextBox tjiacha;
        private System.Windows.Forms.Label ljiacha;
        private System.Windows.Forms.TextBox ttime;
        private System.Windows.Forms.Label ltime;
        private System.Windows.Forms.TextBox ttype;
        private System.Windows.Forms.Label ltype;
    }
}