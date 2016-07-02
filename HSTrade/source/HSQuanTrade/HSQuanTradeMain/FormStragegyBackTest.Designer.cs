namespace HSQuanTradeMain
{
    partial class FormStragegyBackTest
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
            this.label1 = new System.Windows.Forms.Label();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.rdoStock = new System.Windows.Forms.RadioButton();
            this.rdoFuture = new System.Windows.Forms.RadioButton();
            this.btnCancel = new System.Windows.Forms.Button();
            this.dateTimePicker1 = new System.Windows.Forms.DateTimePicker();
            this.dateTimePicker2 = new System.Windows.Forms.DateTimePicker();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.lstCode = new System.Windows.Forms.ListBox();
            this.btnSelCode = new System.Windows.Forms.Button();
            this.pnStock = new System.Windows.Forms.Panel();
            this.cmbCycle = new System.Windows.Forms.ComboBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.progTotal = new System.Windows.Forms.ProgressBar();
            this.progSub = new System.Windows.Forms.ProgressBar();
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.txtCode = new System.Windows.Forms.TextBox();
            this.btnStartTest = new System.Windows.Forms.Button();
            this.lblTotal = new System.Windows.Forms.Label();
            this.lblSub = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.pnStock.SuspendLayout();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(26, 30);
            this.label1.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(77, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "选择一个策略";
            // 
            // comboBox1
            // 
            this.comboBox1.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Location = new System.Drawing.Point(118, 27);
            this.comboBox1.Margin = new System.Windows.Forms.Padding(2);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(196, 20);
            this.comboBox1.TabIndex = 1;
            this.comboBox1.SelectedIndexChanged += new System.EventHandler(this.comboBox1_SelectedIndexChanged);
            // 
            // dataGridView1
            // 
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Location = new System.Drawing.Point(32, 88);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.RowTemplate.Height = 23;
            this.dataGridView1.Size = new System.Drawing.Size(280, 310);
            this.dataGridView1.TabIndex = 3;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(26, 62);
            this.label2.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(77, 12);
            this.label2.TabIndex = 4;
            this.label2.Text = "策略参数设置";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(341, 88);
            this.label3.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(53, 12);
            this.label3.TabIndex = 5;
            this.label3.Text = "测试合约";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(343, 151);
            this.label4.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(77, 12);
            this.label4.TabIndex = 6;
            this.label4.Text = "历史数据文件";
            // 
            // rdoStock
            // 
            this.rdoStock.AutoSize = true;
            this.rdoStock.Checked = true;
            this.rdoStock.Location = new System.Drawing.Point(373, 113);
            this.rdoStock.Margin = new System.Windows.Forms.Padding(2);
            this.rdoStock.Name = "rdoStock";
            this.rdoStock.Size = new System.Drawing.Size(47, 16);
            this.rdoStock.TabIndex = 7;
            this.rdoStock.TabStop = true;
            this.rdoStock.Text = "证券";
            this.rdoStock.UseVisualStyleBackColor = true;
            this.rdoStock.CheckedChanged += new System.EventHandler(this.rdoStock_CheckedChanged);
            // 
            // rdoFuture
            // 
            this.rdoFuture.AutoSize = true;
            this.rdoFuture.Location = new System.Drawing.Point(373, 133);
            this.rdoFuture.Margin = new System.Windows.Forms.Padding(2);
            this.rdoFuture.Name = "rdoFuture";
            this.rdoFuture.Size = new System.Drawing.Size(47, 16);
            this.rdoFuture.TabIndex = 8;
            this.rdoFuture.Text = "期货";
            this.rdoFuture.UseVisualStyleBackColor = true;
            this.rdoFuture.CheckedChanged += new System.EventHandler(this.rdoFuture_CheckedChanged);
            // 
            // btnCancel
            // 
            this.btnCancel.Location = new System.Drawing.Point(637, 51);
            this.btnCancel.Margin = new System.Windows.Forms.Padding(2);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(75, 23);
            this.btnCancel.TabIndex = 13;
            this.btnCancel.Text = "退出";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // dateTimePicker1
            // 
            this.dateTimePicker1.Location = new System.Drawing.Point(1, 27);
            this.dateTimePicker1.Name = "dateTimePicker1";
            this.dateTimePicker1.Size = new System.Drawing.Size(117, 21);
            this.dateTimePicker1.TabIndex = 14;
            // 
            // dateTimePicker2
            // 
            this.dateTimePicker2.Location = new System.Drawing.Point(143, 27);
            this.dateTimePicker2.Name = "dateTimePicker2";
            this.dateTimePicker2.Size = new System.Drawing.Size(117, 21);
            this.dateTimePicker2.TabIndex = 14;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(126, 33);
            this.label6.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(11, 12);
            this.label6.TabIndex = 6;
            this.label6.Text = "~";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(-1, 60);
            this.label7.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(53, 12);
            this.label7.TabIndex = 6;
            this.label7.Text = "合约代码";
            // 
            // lstCode
            // 
            this.lstCode.FormattingEnabled = true;
            this.lstCode.ItemHeight = 12;
            this.lstCode.Location = new System.Drawing.Point(1, 83);
            this.lstCode.Name = "lstCode";
            this.lstCode.Size = new System.Drawing.Size(136, 112);
            this.lstCode.TabIndex = 15;
            // 
            // btnSelCode
            // 
            this.btnSelCode.Location = new System.Drawing.Point(62, 54);
            this.btnSelCode.Name = "btnSelCode";
            this.btnSelCode.Size = new System.Drawing.Size(75, 23);
            this.btnSelCode.TabIndex = 16;
            this.btnSelCode.Text = "选择";
            this.btnSelCode.UseVisualStyleBackColor = true;
            this.btnSelCode.Click += new System.EventHandler(this.btnSelCode_Click);
            // 
            // pnStock
            // 
            this.pnStock.Controls.Add(this.cmbCycle);
            this.pnStock.Controls.Add(this.label5);
            this.pnStock.Controls.Add(this.dateTimePicker2);
            this.pnStock.Controls.Add(this.lstCode);
            this.pnStock.Controls.Add(this.btnSelCode);
            this.pnStock.Controls.Add(this.label6);
            this.pnStock.Controls.Add(this.dateTimePicker1);
            this.pnStock.Controls.Add(this.label11);
            this.pnStock.Controls.Add(this.label7);
            this.pnStock.Location = new System.Drawing.Point(373, 166);
            this.pnStock.Name = "pnStock";
            this.pnStock.Size = new System.Drawing.Size(264, 232);
            this.pnStock.TabIndex = 17;
            // 
            // cmbCycle
            // 
            this.cmbCycle.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbCycle.FormattingEnabled = true;
            this.cmbCycle.Items.AddRange(new object[] {
            "1 分钟",
            "5分钟",
            "15分钟",
            "1小时",
            "1日"});
            this.cmbCycle.Location = new System.Drawing.Point(32, 211);
            this.cmbCycle.Name = "cmbCycle";
            this.cmbCycle.Size = new System.Drawing.Size(105, 20);
            this.cmbCycle.TabIndex = 18;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(2, 12);
            this.label5.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(53, 12);
            this.label5.TabIndex = 17;
            this.label5.Text = "时间范围";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(-2, 214);
            this.label11.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(29, 12);
            this.label11.TabIndex = 6;
            this.label11.Text = "周期";
            // 
            // progTotal
            // 
            this.progTotal.Location = new System.Drawing.Point(88, 435);
            this.progTotal.Name = "progTotal";
            this.progTotal.Size = new System.Drawing.Size(549, 23);
            this.progTotal.Step = 1;
            this.progTotal.Style = System.Windows.Forms.ProgressBarStyle.Continuous;
            this.progTotal.TabIndex = 18;
            // 
            // progSub
            // 
            this.progSub.Location = new System.Drawing.Point(88, 462);
            this.progSub.Name = "progSub";
            this.progSub.Size = new System.Drawing.Size(549, 23);
            this.progSub.Step = 1;
            this.progSub.Style = System.Windows.Forms.ProgressBarStyle.Continuous;
            this.progSub.TabIndex = 18;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(30, 413);
            this.label8.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(53, 12);
            this.label8.TabIndex = 5;
            this.label8.Text = "当前合约";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(30, 441);
            this.label9.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(41, 12);
            this.label9.TabIndex = 5;
            this.label9.Text = "总进度";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(30, 469);
            this.label10.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(41, 12);
            this.label10.TabIndex = 5;
            this.label10.Text = "分进度";
            // 
            // txtCode
            // 
            this.txtCode.Location = new System.Drawing.Point(88, 410);
            this.txtCode.Name = "txtCode";
            this.txtCode.ReadOnly = true;
            this.txtCode.Size = new System.Drawing.Size(100, 21);
            this.txtCode.TabIndex = 19;
            // 
            // btnStartTest
            // 
            this.btnStartTest.Location = new System.Drawing.Point(637, 19);
            this.btnStartTest.Name = "btnStartTest";
            this.btnStartTest.Size = new System.Drawing.Size(75, 23);
            this.btnStartTest.TabIndex = 20;
            this.btnStartTest.Text = "开始";
            this.btnStartTest.UseVisualStyleBackColor = true;
            this.btnStartTest.Click += new System.EventHandler(this.btnStartTest_Click);
            // 
            // lblTotal
            // 
            this.lblTotal.AutoSize = true;
            this.lblTotal.Location = new System.Drawing.Point(642, 441);
            this.lblTotal.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.lblTotal.Name = "lblTotal";
            this.lblTotal.Size = new System.Drawing.Size(0, 12);
            this.lblTotal.TabIndex = 21;
            // 
            // lblSub
            // 
            this.lblSub.AutoSize = true;
            this.lblSub.Location = new System.Drawing.Point(642, 469);
            this.lblSub.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.lblSub.Name = "lblSub";
            this.lblSub.Size = new System.Drawing.Size(0, 12);
            this.lblSub.TabIndex = 21;
            // 
            // FormStragegyBackTest
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(723, 506);
            this.Controls.Add(this.lblSub);
            this.Controls.Add(this.lblTotal);
            this.Controls.Add(this.btnStartTest);
            this.Controls.Add(this.txtCode);
            this.Controls.Add(this.progSub);
            this.Controls.Add(this.progTotal);
            this.Controls.Add(this.pnStock);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.rdoFuture);
            this.Controls.Add(this.rdoStock);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.dataGridView1);
            this.Controls.Add(this.comboBox1);
            this.Controls.Add(this.label1);
            this.Margin = new System.Windows.Forms.Padding(2);
            this.Name = "FormStragegyBackTest";
            this.Text = "交易策略测试";
            this.Load += new System.EventHandler(this.FormStragegyBackTest_Load);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.pnStock.ResumeLayout(false);
            this.pnStock.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.RadioButton rdoStock;
        private System.Windows.Forms.RadioButton rdoFuture;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.DateTimePicker dateTimePicker1;
        private System.Windows.Forms.DateTimePicker dateTimePicker2;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.ListBox lstCode;
        private System.Windows.Forms.Button btnSelCode;
        private System.Windows.Forms.Panel pnStock;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.ProgressBar progTotal;
        private System.Windows.Forms.ProgressBar progSub;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.TextBox txtCode;
        private System.Windows.Forms.Button btnStartTest;
        private System.Windows.Forms.Label lblTotal;
        private System.Windows.Forms.Label lblSub;
        private System.Windows.Forms.ComboBox cmbCycle;
        private System.Windows.Forms.Label label11;
    }
}