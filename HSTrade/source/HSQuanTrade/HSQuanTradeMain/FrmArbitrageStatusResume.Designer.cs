namespace HSQuanTradeMain
{
    partial class FrmArbitrageStatusResume
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
            this.txtPrice2 = new System.Windows.Forms.TextBox();
            this.txtPrice1 = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.rdoBK = new System.Windows.Forms.RadioButton();
            this.label16 = new System.Windows.Forms.Label();
            this.rdoSK = new System.Windows.Forms.RadioButton();
            this.rdoInit = new System.Windows.Forms.RadioButton();
            this.btnOk = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.txtPos1 = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.txtPos2 = new System.Windows.Forms.TextBox();
            this.txtCode1 = new System.Windows.Forms.TextBox();
            this.txtCode2 = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.dtOpen1 = new System.Windows.Forms.DateTimePicker();
            this.dtOpen2 = new System.Windows.Forms.DateTimePicker();
            this.SuspendLayout();
            // 
            // txtPrice2
            // 
            this.txtPrice2.Enabled = false;
            this.txtPrice2.Location = new System.Drawing.Point(209, 153);
            this.txtPrice2.Name = "txtPrice2";
            this.txtPrice2.Size = new System.Drawing.Size(79, 21);
            this.txtPrice2.TabIndex = 9;
            // 
            // txtPrice1
            // 
            this.txtPrice1.Enabled = false;
            this.txtPrice1.Location = new System.Drawing.Point(210, 117);
            this.txtPrice1.Name = "txtPrice1";
            this.txtPrice1.Size = new System.Drawing.Size(79, 21);
            this.txtPrice1.TabIndex = 5;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(23, 157);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(41, 12);
            this.label1.TabIndex = 11;
            this.label1.Text = "合约二";
            // 
            // rdoBK
            // 
            this.rdoBK.AutoSize = true;
            this.rdoBK.Location = new System.Drawing.Point(23, 50);
            this.rdoBK.Name = "rdoBK";
            this.rdoBK.Size = new System.Drawing.Size(95, 16);
            this.rdoBK.TabIndex = 2;
            this.rdoBK.Text = "BK，开仓成功";
            this.rdoBK.UseVisualStyleBackColor = true;
            this.rdoBK.CheckedChanged += new System.EventHandler(this.rdoBK_CheckedChanged);
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(23, 121);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(41, 12);
            this.label16.TabIndex = 12;
            this.label16.Text = "合约一";
            // 
            // rdoSK
            // 
            this.rdoSK.AutoSize = true;
            this.rdoSK.Location = new System.Drawing.Point(23, 83);
            this.rdoSK.Name = "rdoSK";
            this.rdoSK.Size = new System.Drawing.Size(95, 16);
            this.rdoSK.TabIndex = 3;
            this.rdoSK.Text = "SK，开仓成功";
            this.rdoSK.UseVisualStyleBackColor = true;
            this.rdoSK.CheckedChanged += new System.EventHandler(this.rdoSK_CheckedChanged);
            // 
            // rdoInit
            // 
            this.rdoInit.AutoSize = true;
            this.rdoInit.Checked = true;
            this.rdoInit.Location = new System.Drawing.Point(23, 19);
            this.rdoInit.Name = "rdoInit";
            this.rdoInit.Size = new System.Drawing.Size(71, 16);
            this.rdoInit.TabIndex = 1;
            this.rdoInit.TabStop = true;
            this.rdoInit.Text = "初始状态";
            this.rdoInit.UseVisualStyleBackColor = true;
            this.rdoInit.CheckedChanged += new System.EventHandler(this.rdoInit_CheckedChanged);
            // 
            // btnOk
            // 
            this.btnOk.Location = new System.Drawing.Point(460, 197);
            this.btnOk.Name = "btnOk";
            this.btnOk.Size = new System.Drawing.Size(75, 23);
            this.btnOk.TabIndex = 12;
            this.btnOk.Text = "确定";
            this.btnOk.UseVisualStyleBackColor = true;
            this.btnOk.Click += new System.EventHandler(this.btnOk_Click);
            // 
            // btnCancel
            // 
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(550, 197);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(75, 23);
            this.btnCancel.TabIndex = 13;
            this.btnCancel.Text = "取消";
            this.btnCancel.UseVisualStyleBackColor = true;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(153, 121);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(53, 12);
            this.label2.TabIndex = 12;
            this.label2.Text = "开仓价格";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(152, 157);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(53, 12);
            this.label3.TabIndex = 12;
            this.label3.Text = "开仓价格";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(302, 121);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(41, 12);
            this.label4.TabIndex = 12;
            this.label4.Text = "持仓量";
            // 
            // txtPos1
            // 
            this.txtPos1.Enabled = false;
            this.txtPos1.Location = new System.Drawing.Point(349, 117);
            this.txtPos1.Name = "txtPos1";
            this.txtPos1.Size = new System.Drawing.Size(79, 21);
            this.txtPos1.TabIndex = 6;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(301, 157);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(41, 12);
            this.label5.TabIndex = 12;
            this.label5.Text = "持仓量";
            // 
            // txtPos2
            // 
            this.txtPos2.Enabled = false;
            this.txtPos2.Location = new System.Drawing.Point(348, 153);
            this.txtPos2.Name = "txtPos2";
            this.txtPos2.Size = new System.Drawing.Size(79, 21);
            this.txtPos2.TabIndex = 10;
            // 
            // txtCode1
            // 
            this.txtCode1.Enabled = false;
            this.txtCode1.Location = new System.Drawing.Point(70, 117);
            this.txtCode1.Name = "txtCode1";
            this.txtCode1.ReadOnly = true;
            this.txtCode1.Size = new System.Drawing.Size(79, 21);
            this.txtCode1.TabIndex = 4;
            // 
            // txtCode2
            // 
            this.txtCode2.Enabled = false;
            this.txtCode2.Location = new System.Drawing.Point(70, 153);
            this.txtCode2.Name = "txtCode2";
            this.txtCode2.ReadOnly = true;
            this.txtCode2.Size = new System.Drawing.Size(79, 21);
            this.txtCode2.TabIndex = 8;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(441, 121);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(53, 12);
            this.label6.TabIndex = 12;
            this.label6.Text = "开仓时间";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(440, 157);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(53, 12);
            this.label7.TabIndex = 12;
            this.label7.Text = "开仓时间";
            // 
            // dtOpen1
            // 
            this.dtOpen1.CustomFormat = "yyyy-MM-dd HH:mm:ss";
            this.dtOpen1.Enabled = false;
            this.dtOpen1.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
            this.dtOpen1.Location = new System.Drawing.Point(500, 117);
            this.dtOpen1.Name = "dtOpen1";
            this.dtOpen1.Size = new System.Drawing.Size(159, 21);
            this.dtOpen1.TabIndex = 7;
            // 
            // dtOpen2
            // 
            this.dtOpen2.CustomFormat = "yyyy-MM-dd HH:mm:ss";
            this.dtOpen2.Enabled = false;
            this.dtOpen2.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
            this.dtOpen2.Location = new System.Drawing.Point(499, 153);
            this.dtOpen2.Name = "dtOpen2";
            this.dtOpen2.Size = new System.Drawing.Size(160, 21);
            this.dtOpen2.TabIndex = 11;
            // 
            // FrmArbitrageStatusResume
            // 
            this.AcceptButton = this.btnOk;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.btnCancel;
            this.ClientSize = new System.Drawing.Size(682, 232);
            this.Controls.Add(this.dtOpen2);
            this.Controls.Add(this.dtOpen1);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnOk);
            this.Controls.Add(this.txtPrice2);
            this.Controls.Add(this.txtPos2);
            this.Controls.Add(this.txtPos1);
            this.Controls.Add(this.txtCode2);
            this.Controls.Add(this.txtCode1);
            this.Controls.Add(this.txtPrice1);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.rdoBK);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label16);
            this.Controls.Add(this.rdoSK);
            this.Controls.Add(this.rdoInit);
            this.Name = "FrmArbitrageStatusResume";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "状态恢复";
            this.Load += new System.EventHandler(this.FrmArbitrageStatusResume_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox txtPrice2;
        private System.Windows.Forms.TextBox txtPrice1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.RadioButton rdoBK;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.RadioButton rdoSK;
        private System.Windows.Forms.RadioButton rdoInit;
        private System.Windows.Forms.Button btnOk;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox txtPos1;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox txtPos2;
        private System.Windows.Forms.TextBox txtCode1;
        private System.Windows.Forms.TextBox txtCode2;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.DateTimePicker dtOpen1;
        private System.Windows.Forms.DateTimePicker dtOpen2;

    }
}