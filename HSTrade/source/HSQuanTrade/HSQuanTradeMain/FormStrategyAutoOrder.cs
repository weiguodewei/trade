using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using HSQuanTradeMain.MsgDispatch;
using HSQuanTradeMain.StrategyResume;


namespace HSQuanTradeMain
{
    public partial class FormStrategyAutoOrder : TradeFormBase
    {
       
        public delegate_getstrategyparams delegate_GetParams = null;
        public StrategryManager m_oStrategyManager = null;
        public delete_init delegate_Init = null;
        public delegate_getallstrategyname delegate_GetAllStrategyName = null;
        public delegate_AddRunStrategy delegate_AddrunStrategy = null;
        public delegate_SetStrategyParam del_SetStrategyParam = null;
        public delete_getValues del_GetStrategyParam = null;
        public delegate_GetStrategyContext del_GetStrategyContext = null;
        public delete_stop del_StopStrategy = null;
        protected string m_sStrategyId = string.Empty;

        protected string m_sStrategyName = string.Empty;
        protected bool m_bStrategyRun = false;

        //初始参数
        public Dictionary<string, string> mapInitParam = new Dictionary<string, string>();

        public FormStrategyAutoOrder()
        {
            InitializeComponent();

           GenerateGuid("FormStrategyAutoOrder");

           
        }


        private void panel2_Paint(object sender, PaintEventArgs e)
        {

        }

        protected void Init()
        {
            

            comboBox1.Items.Clear();
            this.dataGridView1.DataSource = null;
           
        }
        private void FormStrategyAutoOrder_Load(object sender, EventArgs e)
        {
            Init();
            if (delegate_GetAllStrategyName != null)
            {
                string sStrategys = delegate_GetAllStrategyName();

                string[] list = sStrategys.Split(';');

                foreach (string name in list)
                {
                    if (name != "")
                        comboBox1.Items.Add(name);
                }
            }

            Auto_InitParam();
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            int nt = comboBox1.SelectedIndex;
            if (nt == -1)
                return;

            string name = comboBox1.Items[nt].ToString();
            dataGridView1.Columns.Clear();
            //检查此策略是否存在上次未运行完毕
            FormStrategyResume frmResume = new FormStrategyResume();
            
            if (frmResume.HasResumeStrategy(name))
            {
                frmResume.delegate_GetAllStrategyName = this.delegate_GetAllStrategyName;
                if (DialogResult.Yes == frmResume.ShowDialog())
                {
                    StrategyContext oStrategy = null;
                    if (frmResume.GetResumeStrategyCase(frmResume.ResumeStrategyId, ref oStrategy))
                    {
                        m_sStrategyId = oStrategy.Uid;
                        dataGridView1.Columns.Add("col0", "参数名");
                        dataGridView1.Columns.Add("col1", "参数值");
                        dataGridView1.Columns.Add("col2", "说明");
                        dataGridView1.Columns[0].ReadOnly = true;
                        dataGridView1.Columns[2].ReadOnly = true;
                        for (int i = 0; i < oStrategy.Params.Count; i++)
                        {
                            StrategyParam _param = oStrategy.Params[i];
                            DataGridViewRow oRow = new DataGridViewRow();
                            DataGridViewCell oCell0, oCell1, oCell2;
                            if (0 == _param.ParamName.CompareTo("涨停类型"))
                            {
                                oCell0 = new DataGridViewTextBoxCell();
                                oCell0.Value = _param.ParamName;
                                DataGridViewCellStyle oCell0Style = new DataGridViewCellStyle();
                                oCell0Style.BackColor = SystemColors.Control;
                                DataGridViewComboBoxCell oCellCmb = new DataGridViewComboBoxCell();


                                oCellCmb.Items.Add("0");
                                oCellCmb.Items.Add("1");
                                oCellCmb.Value = "0";
                                
                                oCell1 = oCellCmb;

                                oCell2 = new DataGridViewTextBoxCell();
                                oCell2.Value = "0,普通涨停; 1,一字涨停";
                            }
                            else if (0 == _param.ParamName.CompareTo("退出信号为指令价或收盘价"))
                            {
                                oCell0 = new DataGridViewTextBoxCell();
                                oCell0.Value = _param.ParamName;
                                DataGridViewComboBoxCell oCellCmb = new DataGridViewComboBoxCell();
                                oCellCmb.Items.Add("0");
                                oCellCmb.Items.Add("1");
                                oCellCmb.Value = "0";
                                oCell1 = oCellCmb;

                                oCell2 = new DataGridViewTextBoxCell();
                                oCell2.Value = "0,指令价; 1,收盘价";
                                
                            }
                              
                            else
                            {
                                oCell0 = new DataGridViewTextBoxCell();
                                oCell0.Value = _param.ParamName;
                                oCell1 = new DataGridViewTextBoxCell();
                                oCell1.Value = _param.ParamValue;
                                oCell2 = new DataGridViewTextBoxCell();
                                oCell2.Value = "";

                            }

                           
                            oRow.Cells.Add(oCell0);
                            oRow.Cells.Add(oCell1);
                            oRow.Cells.Add(oCell2);

                            dataGridView1.Rows.Add(oRow);
                        }
                        

                        //resume open postion
                        for (int i = 0; i < oStrategy.Postions.Count; i++ )
                        {
                            ContractPostion oPos = oStrategy.Postions[i];
                            ListViewItem oItem = new ListViewItem(new string[] {oPos.OpenTime,
                            oPos.ContractCode, 
                            oPos.ContractName, (0 == oPos.Direction ? "买入" : "卖出"),
                            oPos.HighPrice.ToString(),
                            oPos.OpenPrice.ToString(),
                            oPos.Postion.ToString(),
                            (oPos.Postion * oPos.OpenPrice).ToString()
                            });
                            this.lstTradeRec.Items.Add(oItem);
                        }
                    }

                }
                else
                {
                    m_sStrategyId = Guid.NewGuid().ToString();
                    OnInitStrategyParams(name);
                }
                
            }
            else
            {

                m_sStrategyId = Guid.NewGuid().ToString();
                OnInitStrategyParams(name);

                
            }


            
        }


        private void btnRun_Click(object sender, EventArgs e)
        {
            this.btnApply.Enabled = true;
            this.btnRun.Enabled = false;
            //订阅合约
            string sSubcode = "subcode#";

            string code = dataGridView1.Rows[0].Cells[1].Value.ToString();

            sSubcode = sSubcode + code + ";" + "STOCK;UFX;" + "#";

            delegate_Init(sSubcode);



            //params
            string sValue = "";
            for (int i = 0; i < dataGridView1.Rows.Count; i++)
            {
                DataGridViewRow row = dataGridView1.Rows[i];

                if (row.Cells[1] != null && row.Cells[1].Value != null)
                {

                    sValue = sValue + row.Cells[1].Value.ToString() + ";";     //参数值

                }
            }
            int nt = comboBox1.SelectedIndex;
            m_sStrategyName = comboBox1.Items[nt].ToString();
            string sPostionInfo = GetResumePostionInfo();
            string sParam = m_sStrategyName
                + "#" + sValue
                + "#" + m_sFormId
                + "#" + m_sStrategyId
                + "#" + sPostionInfo
                + "#";



            string sName = delegate_AddrunStrategy(sParam);
            m_bStrategyRun = true;
            this.comboBox1.Enabled = false;
            
            

        }

        protected string GetResumePostionInfo()
        {
            //"code1;name;postion;price;dir;time$code2;name;postion;price;dir;time"
            string sPosInfo = string.Empty;
            StringBuilder oPostions = new StringBuilder();
            for (int i = 0; i < this.lstTradeRec.Items.Count; i++ )
            {
                if (i > 0)
                {
                    oPostions.Append("$");
                }
                ListViewItem oItem = this.lstTradeRec.Items[i];
                string sPos = oItem.SubItems[1].Text; //code
                sPos += ";";
                sPos += oItem.SubItems[2].Text;//name
                sPos += ";";
                sPos += oItem.SubItems[6].Text;//postion
                sPos += ";";
                sPos += oItem.SubItems[5].Text;//price
                sPos += ";";
                sPos += oItem.SubItems[3].Text;//dir
                sPos += ";";
                sPos += oItem.SubItems[0].Text;//time
                sPos += ";";
                sPos += oItem.SubItems[4].Text;//highprice
                sPos += ";";
                oPostions.Append(sPos);
                if (i == this.lstTradeRec.Items.Count - 1)
                {
                    oPostions.Append("$");
                }
            }
            //oPostions.Append("$");
            sPosInfo = oPostions.ToString();
            return sPosInfo;
        }

        protected void OnInitStrategyParams(string sStategyName)
        {
            dataGridView1.Columns.Clear();

            string list = delegate_GetParams(sStategyName);

            string[] names = list.Split(';');


            dataGridView1.Columns.Add("col0", "参数名");
            dataGridView1.Columns.Add("col1", "参数值");
            dataGridView1.Columns.Add("col2", "说明");
            dataGridView1.Columns[0].ReadOnly = true;
            dataGridView1.Columns[2].ReadOnly = true;
            foreach (string param in names)
            {
                if (param != "")                //"交易合约;交易数量;开仓价;"   会多出一行来
                {
                    DataGridViewCell oCell0, oCell2, oCell1 = null;
                    DataGridViewRow oRow = new DataGridViewRow();
                    oCell0 = new DataGridViewTextBoxCell();
                    DataGridViewCellStyle oCell0Style = new DataGridViewCellStyle();
                    oCell0Style.BackColor = SystemColors.Control;
                    
                    oCell0.Style = oCell0Style;
                    oCell0.Value = param;

                    oCell2 = new DataGridViewTextBoxCell();
                    oCell2.Value = "";

                    //进行一些初始化的操作

                    if (mapInitParam.ContainsKey(param))
                    {
                        oCell1 = new DataGridViewTextBoxCell();
                        oCell1.Value = mapInitParam[param];
                    }
                    else
                    {
                        oCell1 = new DataGridViewTextBoxCell();
                        oCell1.Value = "";
                    }


                    //特殊处理
                    if (param == "退出信号为指令价或收盘价")
                    {

                        DataGridViewComboBoxCell oCellCmb = new DataGridViewComboBoxCell();
                        oCellCmb.Items.Add("0");
                        oCellCmb.Items.Add("1");
                        oCellCmb.Value = "0";
                        oCell1 = oCellCmb;
                        oCell2.Value = "0,指令价; 1,收盘价";
                    }
                    else if (param == "是否下单")
                    {

                        DataGridViewComboBoxCell oCellCmb = new DataGridViewComboBoxCell();
                        oCellCmb.Items.Add("0");
                        oCellCmb.Items.Add("1");
                        oCellCmb.Value = "0";
                        oCell1 = oCellCmb;
                        
                        oCell2.Value = "0,否; 1,是";

                    }
                    else if (param == "涨停类型")
                    {
                        DataGridViewComboBoxCell oCellCmb = new DataGridViewComboBoxCell();
                        oCellCmb.Items.Add("0");
                        oCellCmb.Items.Add("1");
                        oCellCmb.Value = "0";
                        oCell1 = oCellCmb;
                        oCell2.Value = "0,普通涨停; 1,一字涨停";
                    }


                    //if (param == "账户")    //账户参数这里不显示
                    //   continue;

                    //....


                    oRow.Cells.Add(oCell0);
                    oRow.Cells.Add(oCell1);
                    oRow.Cells.Add(oCell2);
                    dataGridView1.Rows.Add(oRow);
                }



            }

        }


        protected void OnTradeLog(string sLog)
        {
            string[] arLog = sLog.Split('|');
            if (arLog.Length >= 2)
            {
                ListViewItem lvItem = this.lstLog.Items.Add(arLog[0]);
                lvItem.SubItems.Add(arLog[1]);
            }
            
        }

        protected void OnTradeSignal(string sTradeInfo)
        {
            //20160411 14:31:12|账户： |买开|合约：300028|金亚科技 |价格：22.0900 |数量：1800#
            string[] arTradeMsg = sTradeInfo.Split('|');
            if (arTradeMsg.Length >= 7)
            {
                ListViewItem lvItem = this.lstTradeRec.Items.Add(arTradeMsg[0]);//时间
                lvItem.SubItems.Add(arTradeMsg[3]);//code
                lvItem.SubItems.Add(arTradeMsg[4]);//name
                lvItem.SubItems.Add(arTradeMsg[2]);//交易方向
                lvItem.SubItems.Add("");//价格
                double dbPrice = double.Parse(arTradeMsg[5]);
                lvItem.SubItems.Add(arTradeMsg[5]);//成交价格
                string sVol = arTradeMsg[6];
                sVol = sVol.Trim('#');
                lvItem.SubItems.Add(sVol);//数量

                int iVol = Int32.Parse(sVol);

                double dbMoney = iVol * dbPrice;

                lvItem.SubItems.Add(dbMoney.ToString("#0.00"));//金额
                //lvItem.SubItems.Add();

            }
        }

       
        private void btnApply_Click(object sender, EventArgs e)
        {
            //params
            
            //设置参数值
            if (null != del_SetStrategyParam)
            {
                int nt = comboBox1.SelectedIndex;
                string sStrategyName = comboBox1.Items[nt].ToString();

                sStrategyName = sStrategyName + "#" + m_sStrategyId + "#";
                
                
                for (int i = 0; i < dataGridView1.Rows.Count; i++)
                {
                    DataGridViewRow row = dataGridView1.Rows[i];

                    if (row.Cells[1] != null && row.Cells[1].Value != null)
                    {
                        string sParamName = row.Cells[0].Value.ToString();
                        string sParamValue = row.Cells[1].Value.ToString();     //参数值
                        del_SetStrategyParam(m_sStrategyId, sParamName, sParamValue);

                    }
                }
                
            }

            //设置下单参数
            string type = ttype.Text;
            string time = ttime.Text;
            string jiacha = tjiacha.Text;
            del_SetStrategyParam(m_sStrategyId, "是否追单", type);
            del_SetStrategyParam(m_sStrategyId, "超时时间", time);
            del_SetStrategyParam(m_sStrategyId, "价差", jiacha);

            //获取参数值
            if (null != del_GetStrategyParam)
            {
                int nt = comboBox1.SelectedIndex;
                string sStrategyName = comboBox1.Items[nt].ToString();

                for (int i = 0; i < dataGridView1.Rows.Count; i++)
                {
                    DataGridViewRow row = dataGridView1.Rows[i];

                    if (row.Cells[0] != null && row.Cells[0].Value != null)
                    {
                        string sParamName = row.Cells[0].Value.ToString();
                        string sParamValue = del_GetStrategyParam(m_sStrategyId, sParamName);     //参数值
                        row.Cells[1].Value = sParamValue; 
                        

                    }
                }
            }

            //获取下单参数
            ttype.Text = del_GetStrategyParam(m_sStrategyId, "是否追单");
            ttime.Text = del_GetStrategyParam(m_sStrategyId, "超时时间");
            tjiacha.Text = del_GetStrategyParam(m_sStrategyId, "价差");

        }

     

        private void chkIsOrder_CheckedChanged(object sender, EventArgs e)
        {
            string sIsOrder = (chkIsOrder.Checked ? "1" : "0");

            foreach (DataGridViewRow _row in dataGridView1.Rows)
            {

                if (_row.Cells.Count >= 2 && _row.Cells[0].Value != null && 0 == _row.Cells[0].Value.ToString().CompareTo("是否下单"))
                {
                    _row.Cells[1].Value = sIsOrder;
                }
            }
            

            
        }

        protected override int OnDispatchMessage(int pamam1, int param2, string param3, string param4)
        {
            switch (pamam1)
            {
                case 1://log
                    OnTradeLog(param3);
                    break;
                case 3://hq

                    break;
                case 5://sig
                    OnTradeSignal(param3);//this.lstTradeRec.Items.Add(param3);
                    break;
            }
            return 0;
        }
        //ee9efc8f-41cf-4001-8cb5-1706a4f1dc08#200000.00;200000.00;0.08;0.15;0.08;0.05;0.05;0.08;0;;5;0.00;0;0#
        private void FormStrategyAutoOrder_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (null != del_GetStrategyContext)
            {
                if (!this.comboBox1.Enabled)
                {
                    string sContext = del_GetStrategyContext(m_sStrategyId);
                    SaveStrategyContext(sContext);

                    if (null != del_StopStrategy)
                    {
                        del_StopStrategy(m_sStrategyId);
                    }
                   

                }
                
            }
        }

        private void Auto_InitParam()
        {

            mapInitParam.Add("交易合约", "");
            mapInitParam.Add("止损率", "0.08");
            mapInitParam.Add("回调率", "0.15");
            mapInitParam.Add("日线周期", "30");
            mapInitParam.Add("新高天数", "20");
            mapInitParam.Add("震荡幅度", "0.3");
            mapInitParam.Add("止盈率", "0.2");
            mapInitParam.Add("换手率", "0.05");
            mapInitParam.Add("跌破收盘价比率1", "0.05");
            mapInitParam.Add("跌破收盘价比率2", "0.08");
            mapInitParam.Add("移动止损率", "0.08");
            mapInitParam.Add("是否追单", "-1");
            mapInitParam.Add("超时时间", "0");
            mapInitParam.Add("初始权益", "50000");
            mapInitParam.Add("初始风险", "0");
            mapInitParam.Add("交易方向", "0");
            mapInitParam.Add("开平方向", "0");
            mapInitParam.Add("同时开仓数", "5");
            mapInitParam.Add("手续费率", "0.001");
            mapInitParam.Add("是否复利", "0");
            mapInitParam.Add("入场模式", "0");
            mapInitParam.Add("是否参考20日均线", "1");
            mapInitParam.Add("创N天新高", "30");
            mapInitParam.Add("N天回调", "5");
            mapInitParam.Add("回落幅度", "0.15");
            mapInitParam.Add("单调回落", "1");


            
            
        }


        protected void SaveStrategyContext(string sContext)
        {
            string[] arContext = sContext.Split('#');
            if (arContext.Length >= 3)
            {
                string sSerialId = arContext[0];
                string sParams = arContext[1];
                string sPostion = arContext[2];

                StrategyContext oContext = new StrategyContext();
                oContext.Uid = sSerialId;

                string sParamNames = "";
                if (null != delegate_GetParams)
                {
                    sParamNames = delegate_GetParams(m_sStrategyName);
                }

                
                oContext.StrategyName = m_sStrategyName;


                StrategySerial oSerail = new StrategySerial();
                oContext.Params = oSerail.ParseParam(sParamNames, sParams);
                oContext.Postions = oSerail.ParsePostion(sPostion);
                oSerail.Save(oContext);

            }

        }
      
    }
}
