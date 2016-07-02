namespace HSQuanTradeMain
{
    partial class FormAddModel
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
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.comboBox2 = new System.Windows.Forms.ComboBox();
            this.lbAccount = new System.Windows.Forms.Label();
            this.rbuy = new System.Windows.Forms.RadioButton();
            this.rsale = new System.Windows.Forms.RadioButton();
            this.rkai = new System.Windows.Forms.RadioButton();
            this.rping = new System.Windows.Forms.RadioButton();
            this.button3 = new System.Windows.Forms.Button();
            this.cbtype = new System.Windows.Forms.ComboBox();
            this.button4 = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(5, 16);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(53, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "策略名称";
            // 
            // comboBox1
            // 
            this.comboBox1.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Location = new System.Drawing.Point(95, 13);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(183, 20);
            this.comboBox1.TabIndex = 1;
            this.comboBox1.SelectedIndexChanged += new System.EventHandler(this.comboBox1_SelectedIndexChanged);
            // 
            // dataGridView1
            // 
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Location = new System.Drawing.Point(7, 39);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.RowTemplate.Height = 23;
            this.dataGridView1.Size = new System.Drawing.Size(271, 325);
            this.dataGridView1.TabIndex = 2;
            // 
            // button1
            // 
            this.button1.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.button1.Location = new System.Drawing.Point(299, 300);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(62, 23);
            this.button1.TabIndex = 3;
            this.button1.Text = "确定";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // button2
            // 
            this.button2.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.button2.Location = new System.Drawing.Point(299, 271);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(62, 23);
            this.button2.TabIndex = 4;
            this.button2.Text = "取消";
            this.button2.UseVisualStyleBackColor = true;
            // 
            // comboBox2
            // 
            this.comboBox2.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox2.FormattingEnabled = true;
            this.comboBox2.Location = new System.Drawing.Point(64, 375);
            this.comboBox2.Name = "comboBox2";
            this.comboBox2.Size = new System.Drawing.Size(164, 20);
            this.comboBox2.TabIndex = 6;
            this.comboBox2.SelectedIndexChanged += new System.EventHandler(this.comboBox2_SelectedIndexChanged);
            // 
            // lbAccount
            // 
            this.lbAccount.AutoSize = true;
            this.lbAccount.Location = new System.Drawing.Point(5, 378);
            this.lbAccount.Name = "lbAccount";
            this.lbAccount.Size = new System.Drawing.Size(53, 12);
            this.lbAccount.TabIndex = 7;
            this.lbAccount.Text = "帐号选择";
            // 
            // rbuy
            // 
            this.rbuy.AutoSize = true;
            this.rbuy.Location = new System.Drawing.Point(7, 401);
            this.rbuy.Name = "rbuy";
            this.rbuy.Size = new System.Drawing.Size(35, 16);
            this.rbuy.TabIndex = 8;
            this.rbuy.TabStop = true;
            this.rbuy.Text = "买";
            this.rbuy.UseVisualStyleBackColor = true;
            this.rbuy.CheckedChanged += new System.EventHandler(this.rbuy_CheckedChanged);
            // 
            // rsale
            // 
            this.rsale.AutoSize = true;
            this.rsale.Location = new System.Drawing.Point(48, 401);
            this.rsale.Name = "rsale";
            this.rsale.Size = new System.Drawing.Size(35, 16);
            this.rsale.TabIndex = 9;
            this.rsale.TabStop = true;
            this.rsale.Text = "卖";
            this.rsale.UseVisualStyleBackColor = true;
            this.rsale.CheckedChanged += new System.EventHandler(this.rsale_CheckedChanged);
            // 
            // rkai
            // 
            this.rkai.AutoSize = true;
            this.rkai.Location = new System.Drawing.Point(104, 401);
            this.rkai.Name = "rkai";
            this.rkai.Size = new System.Drawing.Size(35, 16);
            this.rkai.TabIndex = 10;
            this.rkai.TabStop = true;
            this.rkai.Text = "开";
            this.rkai.UseVisualStyleBackColor = true;
            this.rkai.CheckedChanged += new System.EventHandler(this.rkai_CheckedChanged);
            // 
            // rping
            // 
            this.rping.AutoSize = true;
            this.rping.Location = new System.Drawing.Point(145, 401);
            this.rping.Name = "rping";
            this.rping.Size = new System.Drawing.Size(35, 16);
            this.rping.TabIndex = 11;
            this.rping.TabStop = true;
            this.rping.Text = "平";
            this.rping.UseVisualStyleBackColor = true;
            this.rping.CheckedChanged += new System.EventHandler(this.rping_CheckedChanged);
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(299, 90);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(62, 23);
            this.button3.TabIndex = 12;
            this.button3.Text = "导入数据";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // cbtype
            // 
            this.cbtype.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbtype.FormattingEnabled = true;
            this.cbtype.Items.AddRange(new object[] {
            "下单",
            "监控",
            "回购"});
            this.cbtype.Location = new System.Drawing.Point(299, 64);
            this.cbtype.Name = "cbtype";
            this.cbtype.Size = new System.Drawing.Size(62, 20);
            this.cbtype.TabIndex = 13;
            this.cbtype.SelectedIndexChanged += new System.EventHandler(this.cbtype_SelectedIndexChanged);
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(299, 119);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(62, 23);
            this.button4.TabIndex = 14;
            this.button4.Text = "加载模型";
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.button4_Click);
            // 
            // FormAddModel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(373, 429);
            this.Controls.Add(this.button4);
            this.Controls.Add(this.cbtype);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.rping);
            this.Controls.Add(this.rkai);
            this.Controls.Add(this.rsale);
            this.Controls.Add(this.rbuy);
            this.Controls.Add(this.lbAccount);
            this.Controls.Add(this.comboBox2);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.dataGridView1);
            this.Controls.Add(this.comboBox1);
            this.Controls.Add(this.label1);
            this.Name = "FormAddModel";
            this.Text = "FormAddModel";
            this.Load += new System.EventHandler(this.FormAddModel_Load);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.ComboBox comboBox2;
        private System.Windows.Forms.Label lbAccount;
        private System.Windows.Forms.RadioButton rbuy;
        private System.Windows.Forms.RadioButton rsale;
        private System.Windows.Forms.RadioButton rkai;
        private System.Windows.Forms.RadioButton rping;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.ComboBox cbtype;
        private System.Windows.Forms.Button button4;
    }
}