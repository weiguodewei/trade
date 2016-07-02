namespace HSQuanTradeMain
{
    partial class FormMain
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FormMain));
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.SysMS = new System.Windows.Forms.ToolStripMenuItem();
            this.LoginMI = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.SetMI = new System.Windows.Forms.ToolStripMenuItem();
            this.ConnectMI = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.LogoutMI = new System.Windows.Forms.ToolStripMenuItem();
            this.TradeMS = new System.Windows.Forms.ToolStripMenuItem();
            this.StyRunMI = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.AddStyMI = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.LoadModelMI = new System.Windows.Forms.ToolStripMenuItem();
            this.TestModelMI = new System.Windows.Forms.ToolStripMenuItem();
            this.StockTradeMI = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator7 = new System.Windows.Forms.ToolStripSeparator();
            this.TaoLIMI = new System.Windows.Forms.ToolStripMenuItem();
            this.OptMI = new System.Windows.Forms.ToolStripMenuItem();
            this.FormMS = new System.Windows.Forms.ToolStripMenuItem();
            this.PingPuMI = new System.Windows.Forms.ToolStripMenuItem();
            this.CengDieMI = new System.Windows.Forms.ToolStripMenuItem();
            this.HelpMS = new System.Windows.Forms.ToolStripMenuItem();
            this.AboutMI = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator5 = new System.Windows.Forms.ToolStripSeparator();
            this.TestOrderMI = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator6 = new System.Windows.Forms.ToolStripSeparator();
            this.TestExcelMI = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.axHSQuanTrade1 = new AxHSQuanTradeLib.AxHSQuanTrade();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.timerLogin2 = new System.Windows.Forms.Timer(this.components);
            this.menuStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.axHSQuanTrade1)).BeginInit();
            this.SuspendLayout();
            // 
            // statusStrip1
            // 
            this.statusStrip1.Location = new System.Drawing.Point(0, 578);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(1028, 22);
            this.statusStrip1.TabIndex = 0;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.SysMS,
            this.TradeMS,
            this.FormMS,
            this.HelpMS});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1028, 25);
            this.menuStrip1.TabIndex = 1;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // SysMS
            // 
            this.SysMS.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.LoginMI,
            this.toolStripSeparator4,
            this.SetMI,
            this.ConnectMI,
            this.toolStripSeparator1,
            this.LogoutMI});
            this.SysMS.Name = "SysMS";
            this.SysMS.Size = new System.Drawing.Size(44, 21);
            this.SysMS.Text = "系统";
            // 
            // LoginMI
            // 
            this.LoginMI.Name = "LoginMI";
            this.LoginMI.Size = new System.Drawing.Size(136, 22);
            this.LoginMI.Text = "登录";
            this.LoginMI.Click += new System.EventHandler(this.toolStripMenuItem1_Click);
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(133, 6);
            // 
            // SetMI
            // 
            this.SetMI.Name = "SetMI";
            this.SetMI.Size = new System.Drawing.Size(136, 22);
            this.SetMI.Text = "设置";
            this.SetMI.Click += new System.EventHandler(this.设置ToolStripMenuItem_Click);
            // 
            // ConnectMI
            // 
            this.ConnectMI.Name = "ConnectMI";
            this.ConnectMI.Size = new System.Drawing.Size(136, 22);
            this.ConnectMI.Text = "连接服务器";
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(133, 6);
            // 
            // LogoutMI
            // 
            this.LogoutMI.Name = "LogoutMI";
            this.LogoutMI.Size = new System.Drawing.Size(136, 22);
            this.LogoutMI.Text = "退出";
            // 
            // TradeMS
            // 
            this.TradeMS.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.StyRunMI,
            this.toolStripSeparator2,
            this.AddStyMI,
            this.toolStripSeparator3,
            this.LoadModelMI,
            this.TestModelMI,
            this.StockTradeMI,
            this.toolStripSeparator7,
            this.TaoLIMI,
            this.OptMI});
            this.TradeMS.Name = "TradeMS";
            this.TradeMS.Size = new System.Drawing.Size(68, 21);
            this.TradeMS.Text = "量化交易";
            // 
            // StyRunMI
            // 
            this.StyRunMI.Name = "StyRunMI";
            this.StyRunMI.Size = new System.Drawing.Size(172, 22);
            this.StyRunMI.Text = "策略运行窗口";
            this.StyRunMI.Click += new System.EventHandler(this.策略运行窗口ToolStripMenuItem_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(169, 6);
            // 
            // AddStyMI
            // 
            this.AddStyMI.Name = "AddStyMI";
            this.AddStyMI.Size = new System.Drawing.Size(172, 22);
            this.AddStyMI.Text = "新建模型运行";
            this.AddStyMI.Click += new System.EventHandler(this.新建模型运行ToolStripMenuItem_Click);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(169, 6);
            // 
            // LoadModelMI
            // 
            this.LoadModelMI.Name = "LoadModelMI";
            this.LoadModelMI.Size = new System.Drawing.Size(172, 22);
            this.LoadModelMI.Text = "加载模型";
            this.LoadModelMI.Visible = false;
            this.LoadModelMI.Click += new System.EventHandler(this.加载模型ToolStripMenuItem1_Click);
            // 
            // TestModelMI
            // 
            this.TestModelMI.Name = "TestModelMI";
            this.TestModelMI.Size = new System.Drawing.Size(172, 22);
            this.TestModelMI.Text = "模型测试";
            this.TestModelMI.Click += new System.EventHandler(this.模型测试ToolStripMenuItem_Click);
            // 
            // StockTradeMI
            // 
            this.StockTradeMI.Name = "StockTradeMI";
            this.StockTradeMI.Size = new System.Drawing.Size(172, 22);
            this.StockTradeMI.Text = "证券策略化交易";
            this.StockTradeMI.Click += new System.EventHandler(this.证券策略化交易ToolStripMenuItem_Click);
            // 
            // toolStripSeparator7
            // 
            this.toolStripSeparator7.Name = "toolStripSeparator7";
            this.toolStripSeparator7.Size = new System.Drawing.Size(169, 6);
            // 
            // TaoLIMI
            // 
            this.TaoLIMI.Name = "TaoLIMI";
            this.TaoLIMI.Size = new System.Drawing.Size(172, 22);
            this.TaoLIMI.Text = "套利策略";
            this.TaoLIMI.Click += new System.EventHandler(this.套利策略ToolStripMenuItem_Click);
            // 
            // OptMI
            // 
            this.OptMI.Name = "OptMI";
            this.OptMI.Size = new System.Drawing.Size(172, 22);
            this.OptMI.Text = "股票期权套利监控";
            this.OptMI.Click += new System.EventHandler(this.股票期权套利监控ToolStripMenuItem_Click);
            // 
            // FormMS
            // 
            this.FormMS.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.PingPuMI,
            this.CengDieMI});
            this.FormMS.Name = "FormMS";
            this.FormMS.Size = new System.Drawing.Size(44, 21);
            this.FormMS.Text = "窗口";
            // 
            // PingPuMI
            // 
            this.PingPuMI.Name = "PingPuMI";
            this.PingPuMI.Size = new System.Drawing.Size(100, 22);
            this.PingPuMI.Text = "平铺";
            this.PingPuMI.Click += new System.EventHandler(this.平铺ToolStripMenuItem_Click);
            // 
            // CengDieMI
            // 
            this.CengDieMI.Name = "CengDieMI";
            this.CengDieMI.Size = new System.Drawing.Size(100, 22);
            this.CengDieMI.Text = "层叠";
            this.CengDieMI.Click += new System.EventHandler(this.层叠ToolStripMenuItem_Click);
            // 
            // HelpMS
            // 
            this.HelpMS.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.AboutMI,
            this.toolStripSeparator5,
            this.TestOrderMI,
            this.toolStripSeparator6,
            this.TestExcelMI});
            this.HelpMS.Name = "HelpMS";
            this.HelpMS.Size = new System.Drawing.Size(44, 21);
            this.HelpMS.Text = "帮助";
            // 
            // AboutMI
            // 
            this.AboutMI.Name = "AboutMI";
            this.AboutMI.Size = new System.Drawing.Size(129, 22);
            this.AboutMI.Text = "关于";
            // 
            // toolStripSeparator5
            // 
            this.toolStripSeparator5.Name = "toolStripSeparator5";
            this.toolStripSeparator5.Size = new System.Drawing.Size(126, 6);
            // 
            // TestOrderMI
            // 
            this.TestOrderMI.Name = "TestOrderMI";
            this.TestOrderMI.Size = new System.Drawing.Size(129, 22);
            this.TestOrderMI.Text = "下单测试";
            this.TestOrderMI.Click += new System.EventHandler(this.testorder_Click);
            // 
            // toolStripSeparator6
            // 
            this.toolStripSeparator6.Name = "toolStripSeparator6";
            this.toolStripSeparator6.Size = new System.Drawing.Size(126, 6);
            // 
            // TestExcelMI
            // 
            this.TestExcelMI.Name = "TestExcelMI";
            this.TestExcelMI.Size = new System.Drawing.Size(129, 22);
            this.TestExcelMI.Text = "excel测试";
            this.TestExcelMI.Click += new System.EventHandler(this.testExcel_Click);
            // 
            // toolStrip1
            // 
            this.toolStrip1.Location = new System.Drawing.Point(0, 25);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(1028, 25);
            this.toolStrip1.TabIndex = 2;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // axHSQuanTrade1
            // 
            this.axHSQuanTrade1.Enabled = true;
            this.axHSQuanTrade1.Location = new System.Drawing.Point(329, 185);
            this.axHSQuanTrade1.Name = "axHSQuanTrade1";
            this.axHSQuanTrade1.OcxState = ((System.Windows.Forms.AxHost.State)(resources.GetObject("axHSQuanTrade1.OcxState")));
            this.axHSQuanTrade1.Size = new System.Drawing.Size(125, 63);
            this.axHSQuanTrade1.TabIndex = 6;
            this.axHSQuanTrade1.Visible = false;
            // 
            // timer1
            // 
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // timerLogin2
            // 
            this.timerLogin2.Enabled = true;
            this.timerLogin2.Interval = 10000;
            this.timerLogin2.Tick += new System.EventHandler(this.timerLogin2_Tick);
            // 
            // FormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1028, 600);
            this.Controls.Add(this.axHSQuanTrade1);
            this.Controls.Add(this.toolStrip1);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.menuStrip1);
            this.IsMdiContainer = true;
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "FormMain";
            this.Text = "HSQuantitveTrade";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FormMain_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.axHSQuanTrade1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem SysMS;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripMenuItem SetMI;
        private System.Windows.Forms.ToolStripMenuItem ConnectMI;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem LogoutMI;
        private System.Windows.Forms.ToolStripMenuItem TradeMS;
        private System.Windows.Forms.ToolStripMenuItem StyRunMI;
        private System.Windows.Forms.ToolStripMenuItem HelpMS;
        private System.Windows.Forms.ToolStripMenuItem AboutMI;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripMenuItem AddStyMI;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripMenuItem FormMS;
        private System.Windows.Forms.ToolStripMenuItem PingPuMI;
        private System.Windows.Forms.ToolStripMenuItem CengDieMI;
        private System.Windows.Forms.ToolStripMenuItem LoadModelMI;

        private System.Windows.Forms.ToolStripMenuItem LoginMI;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.ToolStripMenuItem TestModelMI;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator5;
        private System.Windows.Forms.ToolStripMenuItem TestOrderMI;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator6;
        private System.Windows.Forms.ToolStripMenuItem TestExcelMI;
        private System.Windows.Forms.ToolStripMenuItem StockTradeMI;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
     
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator7;
        private System.Windows.Forms.ToolStripMenuItem TaoLIMI;
        private System.Windows.Forms.ToolStripMenuItem OptMI;
        private AxHSQuanTradeLib.AxHSQuanTrade axHSQuanTrade1;
		private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.Timer timerLogin2;


    }
}

