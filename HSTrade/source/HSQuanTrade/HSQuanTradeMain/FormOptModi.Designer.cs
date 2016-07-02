namespace HSQuanTradeMain
{
    partial class FormOptModi
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
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.t1 = new System.Windows.Forms.TextBox();
            this.t2 = new System.Windows.Forms.TextBox();
            this.t3 = new System.Windows.Forms.TextBox();
            this.t4 = new System.Windows.Forms.TextBox();
            this.b1 = new System.Windows.Forms.Button();
            this.b2 = new System.Windows.Forms.Button();
            this.label5 = new System.Windows.Forms.Label();
            this.t5 = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(22, 27);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(77, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "融资认购价格";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(22, 69);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(77, 12);
            this.label2.TabIndex = 1;
            this.label2.Text = "融资认沽价格";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(22, 105);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(77, 12);
            this.label3.TabIndex = 2;
            this.label3.Text = "融券认购价格";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(22, 144);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(77, 12);
            this.label4.TabIndex = 3;
            this.label4.Text = "融券认沽价格";
            // 
            // t1
            // 
            this.t1.Location = new System.Drawing.Point(119, 18);
            this.t1.Name = "t1";
            this.t1.Size = new System.Drawing.Size(100, 21);
            this.t1.TabIndex = 4;
            // 
            // t2
            // 
            this.t2.Location = new System.Drawing.Point(119, 60);
            this.t2.Name = "t2";
            this.t2.Size = new System.Drawing.Size(100, 21);
            this.t2.TabIndex = 5;
            // 
            // t3
            // 
            this.t3.Location = new System.Drawing.Point(119, 96);
            this.t3.Name = "t3";
            this.t3.Size = new System.Drawing.Size(100, 21);
            this.t3.TabIndex = 6;
            // 
            // t4
            // 
            this.t4.Location = new System.Drawing.Point(119, 135);
            this.t4.Name = "t4";
            this.t4.Size = new System.Drawing.Size(100, 21);
            this.t4.TabIndex = 7;
            // 
            // b1
            // 
            this.b1.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.b1.Location = new System.Drawing.Point(54, 209);
            this.b1.Name = "b1";
            this.b1.Size = new System.Drawing.Size(75, 23);
            this.b1.TabIndex = 8;
            this.b1.Text = "取消";
            this.b1.UseVisualStyleBackColor = true;
            // 
            // b2
            // 
            this.b2.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.b2.Location = new System.Drawing.Point(144, 209);
            this.b2.Name = "b2";
            this.b2.Size = new System.Drawing.Size(75, 23);
            this.b2.TabIndex = 9;
            this.b2.Text = "修改";
            this.b2.UseVisualStyleBackColor = true;
            this.b2.Click += new System.EventHandler(this.b2_Click);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(22, 180);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(77, 12);
            this.label5.TabIndex = 10;
            this.label5.Text = "对价平仓点位";
            // 
            // t5
            // 
            this.t5.Location = new System.Drawing.Point(119, 171);
            this.t5.Name = "t5";
            this.t5.Size = new System.Drawing.Size(100, 21);
            this.t5.TabIndex = 11;
            // 
            // FormOptModi
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 244);
            this.Controls.Add(this.t5);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.b2);
            this.Controls.Add(this.b1);
            this.Controls.Add(this.t4);
            this.Controls.Add(this.t3);
            this.Controls.Add(this.t2);
            this.Controls.Add(this.t1);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Name = "FormOptModi";
            this.Text = "FormOptModi";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox t1;
        private System.Windows.Forms.TextBox t2;
        private System.Windows.Forms.TextBox t3;
        private System.Windows.Forms.TextBox t4;
        private System.Windows.Forms.Button b1;
        private System.Windows.Forms.Button b2;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox t5;
    }
}