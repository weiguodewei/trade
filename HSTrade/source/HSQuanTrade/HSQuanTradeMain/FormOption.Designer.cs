using System.Windows.Forms;

namespace HSQuanTradeMain
{
    partial class FormOption
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
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.panel2 = new System.Windows.Forms.Panel();
            this.lsthold = new HSQuanTradeMain.ListViewNF();
            this.num = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.time = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.account = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.code1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.p1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.code2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.p2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.code3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.p3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.code4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.P4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.yk2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.yk3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.condition = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.holdmenu1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.对价全平ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.对价开平ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.数据修改ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.lvOpt = new HSQuanTradeMain.ListViewNF();
            this.buy1price = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.buy1count = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.saleprice1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.sale1count = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.ccode = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.price = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.pcode = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.bprice1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.b1count = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.sprice1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.scount1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.rongquan = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.rongzi = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.diff = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.diff2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.panel1 = new System.Windows.Forms.Panel();
            this.chktest = new System.Windows.Forms.CheckBox();
            this.label1 = new System.Windows.Forms.Label();
            this.chkNotOrder = new System.Windows.Forms.CheckBox();
            this.combOpt = new System.Windows.Forms.ComboBox();
            this.tdiff = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.comboBox4 = new System.Windows.Forms.ComboBox();
            this.tCount = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.comboBox2 = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.listView2 = new System.Windows.Forms.ListView();
            this.columnHeader19 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader20 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader21 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader22 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader23 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader24 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.tjiacha = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.holdmenu1.SuspendLayout();
            this.panel1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.Location = new System.Drawing.Point(0, 0);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(1184, 562);
            this.tabControl1.TabIndex = 4;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.panel2);
            this.tabPage1.Controls.Add(this.panel1);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(1176, 536);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "50ETF和期权套利监控";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.lsthold);
            this.panel2.Controls.Add(this.lvOpt);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel2.Location = new System.Drawing.Point(3, 54);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(1170, 479);
            this.panel2.TabIndex = 23;
            // 
            // lsthold
            // 
            this.lsthold.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.num,
            this.time,
            this.account,
            this.code1,
            this.p1,
            this.code2,
            this.p2,
            this.code3,
            this.p3,
            this.code4,
            this.P4,
            this.yk2,
            this.yk3,
            this.condition});
            this.lsthold.ContextMenuStrip = this.holdmenu1;
            this.lsthold.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.lsthold.FullRowSelect = true;
            this.lsthold.LabelEdit = true;
            this.lsthold.Location = new System.Drawing.Point(0, 282);
            this.lsthold.Name = "lsthold";
            this.lsthold.Size = new System.Drawing.Size(1170, 197);
            this.lsthold.TabIndex = 3;
            this.lsthold.UseCompatibleStateImageBehavior = false;
            this.lsthold.View = System.Windows.Forms.View.Details;
            this.lsthold.DoubleClick += new System.EventHandler(this.lsthold_DoubleClick);
            // 
            // num
            // 
            this.num.Text = "序号";
            this.num.Width = 87;
            // 
            // time
            // 
            this.time.Text = "生成时间";
            this.time.Width = 92;
            // 
            // account
            // 
            this.account.Text = "账户";
            // 
            // code1
            // 
            this.code1.Text = "融资认购";
            this.code1.Width = 102;
            // 
            // p1
            // 
            this.p1.Text = "买价";
            this.p1.Width = 90;
            // 
            // code2
            // 
            this.code2.Text = "融资认沽";
            this.code2.Width = 97;
            // 
            // p2
            // 
            this.p2.Text = "卖价";
            this.p2.Width = 90;
            // 
            // code3
            // 
            this.code3.Text = "融券认购";
            this.code3.Width = 98;
            // 
            // p3
            // 
            this.p3.Text = "卖价";
            this.p3.Width = 90;
            // 
            // code4
            // 
            this.code4.Text = "融券认沽";
            this.code4.Width = 99;
            // 
            // P4
            // 
            this.P4.Text = "买价";
            this.P4.Width = 90;
            // 
            // yk2
            // 
            this.yk2.Text = "对价盈亏";
            this.yk2.Width = 99;
            // 
            // yk3
            // 
            this.yk3.Text = "排队价盈亏";
            this.yk3.Width = 99;
            // 
            // condition
            // 
            this.condition.Text = "对价平仓点";
            this.condition.Width = 99;
            // 
            // holdmenu1
            // 
            this.holdmenu1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.对价全平ToolStripMenuItem,
            this.对价开平ToolStripMenuItem,
            this.数据修改ToolStripMenuItem});
            this.holdmenu1.Name = "holdmenu1";
            this.holdmenu1.Size = new System.Drawing.Size(125, 70);
            // 
            // 对价全平ToolStripMenuItem
            // 
            this.对价全平ToolStripMenuItem.Name = "对价全平ToolStripMenuItem";
            this.对价全平ToolStripMenuItem.Size = new System.Drawing.Size(124, 22);
            this.对价全平ToolStripMenuItem.Text = "对价全平";
            this.对价全平ToolStripMenuItem.Click += new System.EventHandler(this.对价全平ToolStripMenuItem_Click);
            // 
            // 对价开平ToolStripMenuItem
            // 
            this.对价开平ToolStripMenuItem.Name = "对价开平ToolStripMenuItem";
            this.对价开平ToolStripMenuItem.Size = new System.Drawing.Size(124, 22);
            this.对价开平ToolStripMenuItem.Text = "对价开平";
            this.对价开平ToolStripMenuItem.Click += new System.EventHandler(this.对价开平ToolStripMenuItem_Click);
            // 
            // 数据修改ToolStripMenuItem
            // 
            this.数据修改ToolStripMenuItem.Name = "数据修改ToolStripMenuItem";
            this.数据修改ToolStripMenuItem.Size = new System.Drawing.Size(124, 22);
            this.数据修改ToolStripMenuItem.Text = "数据修改";
            this.数据修改ToolStripMenuItem.Click += new System.EventHandler(this.数据修改ToolStripMenuItem_Click);
            // 
            // lvOpt
            // 
            this.lvOpt.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.buy1price,
            this.buy1count,
            this.saleprice1,
            this.sale1count,
            this.ccode,
            this.price,
            this.pcode,
            this.bprice1,
            this.b1count,
            this.sprice1,
            this.scount1,
            this.rongquan,
            this.rongzi,
            this.diff,
            this.diff2});
            this.lvOpt.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lvOpt.FullRowSelect = true;
            this.lvOpt.Location = new System.Drawing.Point(0, 0);
            this.lvOpt.Name = "lvOpt";
            this.lvOpt.Size = new System.Drawing.Size(1170, 479);
            this.lvOpt.TabIndex = 2;
            this.lvOpt.UseCompatibleStateImageBehavior = false;
            this.lvOpt.View = System.Windows.Forms.View.Details;
            // 
            // buy1price
            // 
            this.buy1price.Text = "买一价";
            this.buy1price.Width = 71;
            // 
            // buy1count
            // 
            this.buy1count.Text = "买一量";
            this.buy1count.Width = 78;
            // 
            // saleprice1
            // 
            this.saleprice1.Text = "卖一价";
            this.saleprice1.Width = 78;
            // 
            // sale1count
            // 
            this.sale1count.Text = "卖一量";
            this.sale1count.Width = 73;
            // 
            // ccode
            // 
            this.ccode.Text = "认购合约";
            this.ccode.Width = 85;
            // 
            // price
            // 
            this.price.Text = "行权价";
            this.price.Width = 85;
            // 
            // pcode
            // 
            this.pcode.Text = "认沽合约";
            this.pcode.Width = 85;
            // 
            // bprice1
            // 
            this.bprice1.Text = "买一价";
            this.bprice1.Width = 73;
            // 
            // b1count
            // 
            this.b1count.Text = "买一量";
            this.b1count.Width = 73;
            // 
            // sprice1
            // 
            this.sprice1.Text = "卖一价";
            this.sprice1.Width = 76;
            // 
            // scount1
            // 
            this.scount1.Text = "卖一量";
            this.scount1.Width = 73;
            // 
            // rongquan
            // 
            this.rongquan.Text = "融券";
            this.rongquan.Width = 71;
            // 
            // rongzi
            // 
            this.rongzi.Text = "融资";
            // 
            // diff
            // 
            this.diff.Text = "差值1";
            this.diff.Width = 81;
            // 
            // diff2
            // 
            this.diff2.Text = "差值2";
            this.diff2.Width = 90;
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.label3);
            this.panel1.Controls.Add(this.tjiacha);
            this.panel1.Controls.Add(this.chktest);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Controls.Add(this.chkNotOrder);
            this.panel1.Controls.Add(this.combOpt);
            this.panel1.Controls.Add(this.tdiff);
            this.panel1.Controls.Add(this.label6);
            this.panel1.Controls.Add(this.comboBox4);
            this.panel1.Controls.Add(this.tCount);
            this.panel1.Controls.Add(this.label5);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Location = new System.Drawing.Point(3, 3);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(1170, 51);
            this.panel1.TabIndex = 22;
            // 
            // chktest
            // 
            this.chktest.AutoSize = true;
            this.chktest.Location = new System.Drawing.Point(701, 13);
            this.chktest.Name = "chktest";
            this.chktest.Size = new System.Drawing.Size(48, 16);
            this.chktest.TabIndex = 22;
            this.chktest.Text = "测试";
            this.chktest.UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(3, 14);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(77, 12);
            this.label1.TabIndex = 5;
            this.label1.Text = "选择期权月份";
            // 
            // chkNotOrder
            // 
            this.chkNotOrder.AutoSize = true;
            this.chkNotOrder.Location = new System.Drawing.Point(631, 13);
            this.chkNotOrder.Name = "chkNotOrder";
            this.chkNotOrder.Size = new System.Drawing.Size(60, 16);
            this.chkNotOrder.TabIndex = 21;
            this.chkNotOrder.Text = "不报单";
            this.chkNotOrder.UseVisualStyleBackColor = true;
            // 
            // combOpt
            // 
            this.combOpt.FormattingEnabled = true;
            this.combOpt.Location = new System.Drawing.Point(86, 11);
            this.combOpt.Name = "combOpt";
            this.combOpt.Size = new System.Drawing.Size(189, 20);
            this.combOpt.TabIndex = 4;
            this.combOpt.DropDown += new System.EventHandler(this.comboBox1_DropDown);
            this.combOpt.SelectedIndexChanged += new System.EventHandler(this.combOpt_SelectedIndexChanged);
            // 
            // tdiff
            // 
            this.tdiff.Location = new System.Drawing.Point(575, 12);
            this.tdiff.Name = "tdiff";
            this.tdiff.Size = new System.Drawing.Size(50, 21);
            this.tdiff.TabIndex = 20;
            this.tdiff.Text = "0";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(534, 16);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(35, 12);
            this.label6.TabIndex = 19;
            this.label6.Text = "差值>";
            // 
            // comboBox4
            // 
            this.comboBox4.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox4.FormattingEnabled = true;
            this.comboBox4.Location = new System.Drawing.Point(294, 11);
            this.comboBox4.Name = "comboBox4";
            this.comboBox4.Size = new System.Drawing.Size(84, 20);
            this.comboBox4.TabIndex = 16;
            // 
            // tCount
            // 
            this.tCount.Location = new System.Drawing.Point(468, 11);
            this.tCount.Name = "tCount";
            this.tCount.Size = new System.Drawing.Size(49, 21);
            this.tCount.TabIndex = 17;
            this.tCount.Text = "0";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(400, 17);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(53, 12);
            this.label5.TabIndex = 18;
            this.label5.Text = "交易次数";
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.textBox1);
            this.tabPage2.Controls.Add(this.comboBox2);
            this.tabPage2.Controls.Add(this.label2);
            this.tabPage2.Controls.Add(this.checkBox1);
            this.tabPage2.Controls.Add(this.listView2);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(1176, 536);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "50ETF和IH套利监控";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(294, 20);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(111, 21);
            this.textBox1.TabIndex = 7;
            // 
            // comboBox2
            // 
            this.comboBox2.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox2.FormattingEnabled = true;
            this.comboBox2.Items.AddRange(new object[] {
            "大于",
            "小于"});
            this.comboBox2.Location = new System.Drawing.Point(185, 21);
            this.comboBox2.Name = "comboBox2";
            this.comboBox2.Size = new System.Drawing.Size(90, 20);
            this.comboBox2.TabIndex = 6;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(137, 26);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(29, 12);
            this.label2.TabIndex = 5;
            this.label2.Text = "价差";
            // 
            // checkBox1
            // 
            this.checkBox1.AutoSize = true;
            this.checkBox1.Location = new System.Drawing.Point(39, 25);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.Size = new System.Drawing.Size(72, 16);
            this.checkBox1.TabIndex = 4;
            this.checkBox1.Text = "告警提醒";
            this.checkBox1.UseVisualStyleBackColor = true;
            // 
            // listView2
            // 
            this.listView2.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader19,
            this.columnHeader20,
            this.columnHeader21,
            this.columnHeader22,
            this.columnHeader23,
            this.columnHeader24});
            this.listView2.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.listView2.Location = new System.Drawing.Point(3, -59);
            this.listView2.Name = "listView2";
            this.listView2.Size = new System.Drawing.Size(1170, 592);
            this.listView2.TabIndex = 3;
            this.listView2.UseCompatibleStateImageBehavior = false;
            this.listView2.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader19
            // 
            this.columnHeader19.Text = "IH合约";
            // 
            // columnHeader20
            // 
            this.columnHeader20.Text = "卖一价";
            // 
            // columnHeader21
            // 
            this.columnHeader21.Text = "买一价";
            this.columnHeader21.Width = 70;
            // 
            // columnHeader22
            // 
            this.columnHeader22.Text = "卖一量";
            this.columnHeader22.Width = 75;
            // 
            // columnHeader23
            // 
            this.columnHeader23.Text = "买一量";
            this.columnHeader23.Width = 91;
            // 
            // columnHeader24
            // 
            this.columnHeader24.Text = "价差";
            this.columnHeader24.Width = 141;
            // 
            // tjiacha
            // 
            this.tjiacha.Location = new System.Drawing.Point(825, 10);
            this.tjiacha.Name = "tjiacha";
            this.tjiacha.Size = new System.Drawing.Size(100, 21);
            this.tjiacha.TabIndex = 23;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(790, 14);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(29, 12);
            this.label3.TabIndex = 24;
            this.label3.Text = "价差";
            // 
            // FormOption
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1184, 562);
            this.Controls.Add(this.tabControl1);
            this.Name = "FormOption";
            this.Text = "50ETF套利监控";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FormOption_FormClosing);
            this.Load += new System.EventHandler(this.FormOption_Load);
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.panel2.ResumeLayout(false);
            this.holdmenu1.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        //private System.Windows.Forms.ListView lvOpt;
        ListViewNF lvOpt;
        private System.Windows.Forms.ColumnHeader price;
        private System.Windows.Forms.ColumnHeader ccode;
        private System.Windows.Forms.ColumnHeader sale1count;
        private System.Windows.Forms.ColumnHeader buy1count;
        private System.Windows.Forms.ColumnHeader pcode;
        private System.Windows.Forms.ColumnHeader scount1;
        private System.Windows.Forms.ColumnHeader b1count;
        private System.Windows.Forms.ColumnHeader rongquan;
        private System.Windows.Forms.ColumnHeader diff;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox combOpt;
        private System.Windows.Forms.ListView listView2;
        private System.Windows.Forms.ColumnHeader columnHeader19;
        private System.Windows.Forms.ColumnHeader columnHeader20;
        private System.Windows.Forms.ColumnHeader columnHeader21;
        private System.Windows.Forms.ColumnHeader columnHeader22;
        private System.Windows.Forms.ColumnHeader columnHeader23;
        private System.Windows.Forms.ColumnHeader columnHeader24;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.ComboBox comboBox2;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.CheckBox checkBox1;
        private ColumnHeader buy1price;
        private ColumnHeader saleprice1;
        private ColumnHeader bprice1;
        private ColumnHeader sprice1;
        private ColumnHeader diff2;
        private ColumnHeader rongzi;
        private ComboBox comboBox4;
        private TextBox tCount;
        private Label label5;
        private TextBox tdiff;
        private Label label6;
        private CheckBox chkNotOrder;
        private Panel panel2;
        private Panel panel1;
        //private ListView lsthold;
        ListViewNF lsthold;
        private ColumnHeader num;
        private ColumnHeader time;
        private ColumnHeader code1;
        private ColumnHeader code2;
        private ColumnHeader code3;
        private ColumnHeader code4;
        private ColumnHeader account;
        private CheckBox chktest;
        private ColumnHeader yk2;
        private ColumnHeader yk3;
        private ColumnHeader p1;
        private ColumnHeader p2;
        private ColumnHeader p3;
        private ColumnHeader P4;
        private ContextMenuStrip holdmenu1;
        private ToolStripMenuItem 对价全平ToolStripMenuItem;
        private ToolStripMenuItem 对价开平ToolStripMenuItem;
        private ToolStripMenuItem 数据修改ToolStripMenuItem;
        private ColumnHeader condition;
        private Label label3;
        private TextBox tjiacha;
    }

/*
    class ListViewNF : System.Windows.Forms.ListView
    {
        public ListViewNF()
        {
            // 开启双缓冲
            this.SetStyle(ControlStyles.OptimizedDoubleBuffer | ControlStyles.AllPaintingInWmPaint, true);

            // Enable the OnNotifyMessage event so we get a chance to filter out 
            // Windows messages before they get to the form's WndProc
            this.SetStyle(ControlStyles.EnableNotifyMessage, true);
        }

        protected override void OnNotifyMessage(Message m)
        {
            //Filter out the WM_ERASEBKGND message
            if (m.Msg != 0x14)
            {
                base.OnNotifyMessage(m);
            }

        }


    }
 */

}