using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Collections;
using HSQuanTradeMain.MsgDispatch;
using HSQuanTradeMain.StrategyResume;
using System.Threading;

namespace HSQuanTradeMain
{
    public partial class FormArbitrage : TradeFormBase
    {
        public delegate_getstrategyparams delegate_GetParams = null;
        public StrategryManager m_oStrategyManager = null;
        public delete_init delegate_Init = null;
        public delegate_getallstrategyname delegate_GetAllStrategyName = null;
        public delegate_AddRunStrategy delegate_AddrunStrategy = null;
        public delegate_SetStrategyParam del_SetStrategyParam = null;
        public delete_getValues del_GetStrategyParam = null;
        public delegate_GetStrategyContext del_GetStrategyContext = null;
        public delegate_SetStrategyContext del_SetStrategyContext = null;
        public delete_stop del_StopStrategy = null;

        static Hashtable s_hsArbitrageCode = new Hashtable();
        static ArrayList s_arArbitrageCode = new ArrayList();

        protected string m_sStrategyName = string.Empty;

        protected string m_sStrategyId = string.Empty;

        protected bool m_bRun = false;

        public FormArbitrage()
        {
            InitializeComponent();
            GenerateGuid("FormArbitrage");

            InitStdArbitrageCodes();

             
            this.cmbFirstOpenContract.SelectedIndex = 1;
        }

        protected void InitStdArbitrageCodes()
        {
            if (0 == s_hsArbitrageCode.Count)
            {
                string sResultFile = System.Windows.Forms.Application.StartupPath;
                sResultFile += "\\arbitrage_code.dat";
                

                FileStream oFileStream = new FileStream(sResultFile, FileMode.Open, FileAccess.Read, FileShare.Write);
                if (null != oFileStream)
                {
                    StreamReader oStmReader = new StreamReader(oFileStream, Encoding.Default);
                    if (null == oStmReader)
                    {
                        return;
                    }
                    String sLine;
                    while ((sLine = oStmReader.ReadLine()) != null)
                    {

                        string[] arCells = sLine.Split(',');
                        if (arCells.Length >= 4)
                        {
                            ArbitrageItem oItem = new ArbitrageItem();
                            oItem.InstrumentID = arCells[0];
                            oItem.ExchangeID = arCells[1];
                            oItem.MinMarketOrderVolume = Int32.Parse(arCells[2]);
                            oItem.MaxMarketOrderVolume = Int32.Parse(arCells[3]);

                            if (!s_hsArbitrageCode.ContainsKey(oItem.InstrumentID))
                            {
                                s_hsArbitrageCode.Add(oItem.InstrumentID, oItem);
                                s_arArbitrageCode.Add(oItem);

                                this.cmbStdArbitrage.Items.Add(oItem.InstrumentID);
                            }

                        }
                    }

                    oFileStream.Close();

                }
                

                
            }
            else
            {
                for (int i = 0; i < s_arArbitrageCode.Count; i++ )
                {
                    ArbitrageItem oItem = (ArbitrageItem)s_arArbitrageCode[i];
                    this.cmbStdArbitrage.Items.Add(oItem.InstrumentID);
                }
               
            
            }

            for (int i = 0; i < FormSet.listAccount.Count; i++)
            {
                AccountInfo oAccount = FormSet.listAccount[i];
                this.cmbAccount.Items.Add(oAccount.account);
            }
            if (this.cmbAccount.Items.Count > 0)
            {
                this.cmbAccount.SelectedIndex = this.cmbAccount.Items.Count - 1;
            }
            
        }



        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void tabPage1_Click(object sender, EventArgs e)
        {

        }

        protected void OnTradeLog(string sLog)
        {
            string[] arLog = sLog.Split('|');
            if (arLog.Length >= 2)
            {
                ListViewItem lvItem = this.lstLog.Items.Add(arLog[0]);
                lvItem.SubItems.Add(arLog[1]);

                this.txtLastTradeInfo.Text = arLog[1];
            }

        }

        protected override int OnDispatchMessage(int pamam1, int param2, string param3, string param4)
        {
            switch (pamam1)
            {
                case 1://log
                    OnTradeLog(param3);
                    break;
                case 5://sig
                    OnTradeSignal(param3);
                    break;
                case 6:
                    OnArbitrageHq(param3);
                    break;
                case 7://E_OCX_TRADE_POSTION_MSG
                    OnPostionInfo(param3);
                    break;
            }
           
            
           return 0;
        }

        protected void OnPostionInfo(string sPosInfo)
        {
            //sprintf(szMsg, ";%s;%d;%d;%d;%.2f;%s", szCode, iOpenClose, iBuySell, iPosNum, dbAvgPrice, szOpenTime);
            sPosInfo = sPosInfo.Replace("#", "");
            string[] arInfo = sPosInfo.Split(';');
            if (arInfo.Length >= 6)
            {
                string sCode = arInfo[1];
                int iRow = -1;
                for (int i = 0; i < this.lstPostion.Items.Count; i++ )
                {
                    if (0 == sCode.CompareTo(this.lstPostion.Items[i].SubItems[1].Text))
                    {
                        iRow = i;
                        break;
                    }
                    
                }
                if (0 == arInfo[2].CompareTo("0")) //开仓
                {
                    if (-1 != iRow)
                    {
                        ListViewItem oItem = this.lstPostion.Items[iRow];
                        int iOldPos = Int32.Parse(oItem.SubItems[3].Text);
                        int iPosAdd = Int32.Parse(arInfo[4]);

                        oItem.SubItems[3].Text = (iOldPos + iPosAdd).ToString();
                        double dbOldAvg = Double.Parse(oItem.SubItems[4].Text);
                        double dbAvgAdd = Double.Parse(arInfo[5]);

                        double dbNewAvg = (dbOldAvg * iOldPos + dbAvgAdd * iPosAdd) / (double)(iOldPos + iPosAdd);

                        oItem.SubItems[4].Text = dbNewAvg.ToString();
                        oItem.SubItems[5].Text = arInfo[6];
                    }
                    else
                    {
                        ListViewItem oItem = this.lstPostion.Items.Add(arInfo[0]);
                        oItem.SubItems.Add(arInfo[1]);
                        
                        if ("0" == arInfo[3])
                        {
                            oItem.SubItems.Add("买入");
                        }
                        else
                        {
                            oItem.SubItems.Add("卖出");
                        }

                        oItem.SubItems.Add(arInfo[4]);
                        string sAvgPrice = arInfo[5];
                        sAvgPrice = sAvgPrice.Replace("#", "");
                        oItem.SubItems.Add(sAvgPrice);
                        oItem.SubItems.Add(arInfo[6]);
                    }

                }
                else //平仓
                {
                    if (-1 != iRow)
                    {
                        ListViewItem oItem = this.lstPostion.Items[iRow];
                        int iOldPos = Int32.Parse(oItem.SubItems[3].Text);
                        int iPosAdd = Int32.Parse(arInfo[4]);

                        if (iOldPos == iPosAdd)
                        {
                            this.lstPostion.Items.RemoveAt(iRow);
                        }
                        else if (iOldPos > iPosAdd)
                        {
                            int iNewPos = iOldPos - iPosAdd;
                            oItem.SubItems[3].Text = iNewPos.ToString();
                        }
                        
                    }

                }
            }
        }

        protected void OnArbitrageHq(string sHqInfo)
        {
            string[] arHq = sHqInfo.Split('|');
            int iVolMul = 0;
            if (arHq.Length >= 2)
            {
                string sParams = arHq[1];
                sParams = sParams.Replace("#", "");
                string[] arParams = sParams.Split(',');
                if (0 == arParams[0].CompareTo("0"))//标准价差合约
                {
                    this.txtLastDivPrice.Text = arParams[2];
                    iVolMul = Int32.Parse(arParams[3]);
                }
                else if (0 == arParams[0].CompareTo("1"))
                {
                    this.txtLastDivPrice.Text = arParams[2];
                    this.txtLastPrice1.Text = arParams[3];
                    this.txtLastPrice2.Text = arParams[4];
                    iVolMul = Int32.Parse(arParams[5]);
                }
            }

            //calculate profit
            if (2 == this.lstPostion.Items.Count && this.txtLastPrice1.Text != "" && this.txtLastPrice2.Text != "")
            {
                string sCode1 = string.Empty;
                string sCode2 = string.Empty;

                if (this.rdoCustom.Checked)
                {
                    sCode1 = this.txtCode1.Text;
                    sCode2 = this.txtCode2.Text;

                }
                else
                {
                    string sStdCode = this.cmbStdArbitrage.Text;
                    int nt = sStdCode.IndexOf('&');
                    if (nt > 0)
                    {
                        sCode2 = sStdCode.Substring(nt + 1, sStdCode.Length - nt - 1);
                        sCode1 = sStdCode.Substring(nt - sCode2.Length, sCode2.Length);
                    }
                }
                ListViewItem oItem = this.lstPostion.Items[0];
                
                double dbProfit = 0.0;
                double dbCost = double.Parse(oItem.SubItems[4].Text);
                double dbLastPrice = 0.0;
                if (0 == sCode1.CompareTo(oItem.SubItems[1].Text))
                {
                    dbLastPrice = double.Parse(this.txtLastPrice1.Text);
                   
                }
                else if (0 == sCode2.CompareTo(oItem.SubItems[1].Text))
                {
                    dbLastPrice = double.Parse(this.txtLastPrice2.Text);
                }
                int iCount = Int32.Parse(oItem.SubItems[3].Text);
                if ("买入" == oItem.SubItems[2].Text)//多头
                {
                    dbProfit = (dbLastPrice - dbCost) * iCount;
                }
                else if ("卖出" == oItem.SubItems[2].Text)//空头
                {
                    dbProfit = (dbCost - dbLastPrice) * iCount;
                }

                oItem = this.lstPostion.Items[1];
                dbCost = double.Parse(oItem.SubItems[4].Text);
                if (0 == sCode1.CompareTo(oItem.SubItems[1].Text))
                {
                    dbLastPrice = double.Parse(this.txtLastPrice1.Text);

                }
                else if (0 == sCode2.CompareTo(oItem.SubItems[1].Text))
                {
                    dbLastPrice = double.Parse(this.txtLastPrice2.Text);
                }
                iCount = Int32.Parse(oItem.SubItems[3].Text);
                if ("买入" == oItem.SubItems[2].Text)//多头
                {
                    dbProfit += (dbLastPrice - dbCost) * iCount;
                }
                else if ("卖出" == oItem.SubItems[2].Text)//空头
                {
                    dbProfit += (dbCost - dbLastPrice) * iCount;
                }

                dbProfit = dbProfit * iVolMul;

                this.txtFlowYK.Text = dbProfit.ToString("F");
            }
            else
            {
                this.txtFlowYK.Text = "--";
            }

        }


        protected bool CheckParams()
        {
            if (this.rdoStd.Checked && string.Empty == this.cmbStdArbitrage.Text)
            {
                MessageBox.Show("请选择标准价差合约!");
                this.cmbStdArbitrage.Focus();
                return false;
            }
            
            if (this.rdoCustom.Checked && string.Empty == this.txtCode1.Text)
            {
                MessageBox.Show("请设置价差合约!");
                this.txtCode1.Focus();
                return false;
            }

            if (this.rdoCustom.Checked && string.Empty == this.txtCode2.Text)
            {
                MessageBox.Show("请设置价差合约!");
                this.txtCode2.Focus();
                return false;
            }
            
            if (string.Empty == this.cmbCondtion.Text)
            {
                MessageBox.Show("请选择开仓条件!");
                this.cmbCondtion.Focus();
                return false;
            }

            if (string.Empty == this.txtPriceDiv.Text)
            {
                MessageBox.Show("请设置价差门限!");
                this.txtPriceDiv.Focus();
                return false;
            }
            if (string.Empty == this.txtOpenNum.Text)
            {
                MessageBox.Show("请设置开仓数量!");
                this.txtOpenNum.Focus();
                return false;
            }

            if (string.Empty == this.cmbDir.Text)
            {
                MessageBox.Show("请设置开仓方向!");
                this.cmbDir.Focus();
                return false;
            }

            if (this.chkCloseCon1.Checked && string.Empty == this.cmbPriceDivCon1.Text)
            {
                MessageBox.Show("请设置平仓价差!");
                this.txtPriceDivCon1.Focus();
                return false;
            }

            if (this.chkCloseCon2.Checked && string.Empty == this.cmbPriceDivCon2.Text)
            {
                MessageBox.Show("请设置平仓价差!");
                this.txtPriceDivCon2.Focus();
                return false;
            }

            if (this.chkStrategyOrder.Checked && string.Empty == this.cmbFirstOpenContract.Text)
            {
                MessageBox.Show("请选择优先开仓合约!");
                this.cmbFirstOpenContract.Focus();
                return false;
            }

            if (this.chkStrategyOrder.Checked && string.Empty == this.txtOpenLimit.Text)
            {
                MessageBox.Show("请设置每日开仓上限!");
                this.txtOpenLimit.Focus();
                return false;
            }

            if (string.Empty == this.cmbAccount.Text)
            {
                MessageBox.Show("请选择账户!");
                this.cmbAccount.Focus();
                return false;
            }


            if ((0 == cmbCondtion.SelectedIndex|| 1 == cmbCondtion.SelectedIndex) && (0 == cmbDir.SelectedIndex))
            {
                string sMsg = string.Format("确定要在价差大于{0}做多价差吗?", this.txtPriceDiv.Text);
                if (DialogResult.No == MessageBox.Show(sMsg, "警告", MessageBoxButtons.YesNo, MessageBoxIcon.Question))
                {
                    return false;   
                }
            }

            if ((2 == cmbCondtion.SelectedIndex || 3 == cmbCondtion.SelectedIndex) && (1 == cmbDir.SelectedIndex))
            {
                string sMsg = string.Format("确定要在价差小于{0}做空价差吗?", this.txtPriceDiv.Text);
                if (DialogResult.No == MessageBox.Show(sMsg, "警告", MessageBoxButtons.YesNo, MessageBoxIcon.Question))
                {
                    return false;
                }
            }

            return true;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (m_bRun)
            {
                ApplyParams();
            }
        }

        private void ApplyParams()
        {
            //开仓条件;价差门限;开仓数量;平仓条件一;平仓价差一;平仓条件二;平仓价差二;启用策略执行下单;优先开仓合约;每天开仓次数小于;下单账号;开仓方向
            if (null != del_SetStrategyParam)
            {
                int iFirstOpenContract = 2;
                if (this.rdoCustom.Checked)
                {
                    iFirstOpenContract = this.cmbFirstOpenContract.SelectedIndex;
                }


                del_SetStrategyParam(m_sStrategyId, "开仓条件", this.cmbCondtion.Text);
                del_SetStrategyParam(m_sStrategyId, "价差门限", this.txtPriceDiv.Text);
                del_SetStrategyParam(m_sStrategyId, "开仓数量", this.txtOpenNum.Text);
                if (this.chkCloseCon1.Checked)
                {
                    del_SetStrategyParam(m_sStrategyId, "平仓条件一", this.cmbPriceDivCon1.Text);
                    del_SetStrategyParam(m_sStrategyId, "平仓价差一", this.txtPriceDivCon1.Text);
                }
                else
                {
                    del_SetStrategyParam(m_sStrategyId, "平仓条件一", "");
                }
                if (this.chkCloseCon2.Checked)
                {
                    del_SetStrategyParam(m_sStrategyId, "平仓条件二", this.cmbPriceDivCon2.Text);
                    del_SetStrategyParam(m_sStrategyId, "平仓价差二", this.txtPriceDivCon2.Text);
                }
                else
                {
                    del_SetStrategyParam(m_sStrategyId, "平仓条件二", "");
                }
                
                del_SetStrategyParam(m_sStrategyId, "启用策略执行下单", this.chkStrategyOrder.Text);
                del_SetStrategyParam(m_sStrategyId, "优先开仓合约", iFirstOpenContract.ToString());
                del_SetStrategyParam(m_sStrategyId, "每天开仓次数小于", this.txtOpenLimit.Text);
                del_SetStrategyParam(m_sStrategyId, "下单账号", this.cmbAccount.Text);
                del_SetStrategyParam(m_sStrategyId, "开仓方向", this.cmbDir.SelectedIndex.ToString());
               
            }
         
        }

        private void cmbCondtion_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (0 == cmbCondtion.SelectedIndex || 1 == cmbCondtion.SelectedIndex)
            {
                this.cmbDir.SelectedIndex = 1;
            }
            else
            {
                this.cmbDir.SelectedIndex = 0;
            }
        }

        private void cmbDir_SelectedIndexChanged(object sender, EventArgs e)
        {
            if ((0 == cmbCondtion.SelectedIndex || 1 == cmbCondtion.SelectedIndex) && (0 == cmbDir.SelectedIndex))
            {
                string sMsg = string.Format("确定要在价差大于{0}做多价差吗?", this.txtPriceDiv.Text);
                if (DialogResult.No == MessageBox.Show(sMsg, "警告", MessageBoxButtons.YesNo, MessageBoxIcon.Question))
                {
                    cmbDir.SelectedIndex = 1;
                }
                else
                {
                    //record this dangerous choice into log, avoiding the risk of being shot by client,-_-!!!
                    sMsg = string.Format("客户设定在价差大于{0}时做多价差", this.txtPriceDiv.Text);
                    Log.WriteLog(sMsg, "Arbitrage");
                }
            }

            if ((2 == cmbCondtion.SelectedIndex || 3 == cmbCondtion.SelectedIndex) && (1 == cmbDir.SelectedIndex))
            {
                string sMsg = string.Format("确定要在价差小于{0}做空价差吗?", this.txtPriceDiv.Text);
                if (DialogResult.No == MessageBox.Show(sMsg, "警告", MessageBoxButtons.YesNo, MessageBoxIcon.Question))
                {
                    cmbDir.SelectedIndex = 0;
                }
                else
                {
                    //record this dangerous choice into log, avoiding the risk of being shot by client,-_-!!!
                    sMsg = string.Format("客户设定在价差小于{0}时做空价差", this.txtPriceDiv.Text);
                    Log.WriteLog(sMsg, "Arbitrage");
                }
            }
        }

        private void rdoCustom_CheckedChanged(object sender, EventArgs e)
        {
           
            this.cmbStdArbitrage.Enabled = !this.rdoCustom.Checked;
            this.txtCode1.Enabled = this.rdoCustom.Checked;
            this.txtCode2.Enabled = this.rdoCustom.Checked;

           
        }

        private void chkCloseCon1_CheckedChanged(object sender, EventArgs e)
        {
            this.cmbPriceDivCon1.Enabled = this.chkCloseCon1.Checked;
            this.txtPriceDivCon1.Enabled = this.chkCloseCon1.Checked;
           
           
        }

        private void chkCloseCon2_CheckedChanged(object sender, EventArgs e)
        {
            this.cmbPriceDivCon2.Enabled = this.chkCloseCon2.Checked;
            this.txtPriceDivCon2.Enabled = this.chkCloseCon2.Checked;
        }

        private void chkStrategyOrder_CheckedChanged(object sender, EventArgs e)
        {
            this.cmbFirstOpenContract.Enabled = this.chkStrategyOrder.Checked;
            this.txtOpenLimit.Enabled = this.chkStrategyOrder.Checked;
        }

        protected void OnThreadTask_SaveContext()
        {
           
        }

        private void FormArbitrage_FormClosing(object sender, FormClosingEventArgs e)
        {
            
            if (e.CloseReason == CloseReason.UserClosing)
            {
                DialogResult oResult = MessageBox.Show("是否要保存策略状态", "消息", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question);
                if (DialogResult.Yes == oResult)
                {
                    //Thread thdContext = new Thread(new ThreadStart(OnThreadTask_SaveContext));
                    //thdContext.Start();
                    if (null != del_GetStrategyContext)
                    {
                        string sContext = del_GetStrategyContext(m_sStrategyId);
                        SaveStrategyContext(sContext);

                        if (null != del_StopStrategy)
                        {
                            del_StopStrategy(m_sStrategyId);

                        }
                    }
                }
                else if (oResult == DialogResult.No)
                {
                    //remove current strategy context from xml
                    StrategySerial oSerial = new StrategySerial();
                    if (null != oSerial)
                    {
                        oSerial.RemoveStrategy(m_sStrategyId);
                    }
                }
                else
                {
                    e.Cancel = true;
                }
            }
            else if (e.CloseReason == CloseReason.MdiFormClosing)
            {
                if (null != del_GetStrategyContext)
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


                StrategyArbitrageSerial oSerail = new StrategyArbitrageSerial();
                oContext.Params = oSerail.ParseParam(sParamNames, sParams);
                oContext.Postions = oSerail.ParsePostion(sPostion);
                oSerail.Save(oContext);

            }

        }

        protected void OnTradeSignal(string sTradeInfo)
        {
            //20160411 14:31:12|账户： |买开|合约：300028|金亚科技 |价格：22.0900 |数量：1800#
            //时间, 合约,交易方向，成交价，成交量
            string[] arTradeMsg = sTradeInfo.Split('|');
            if (arTradeMsg.Length >= 7)
            {
                ListViewItem lvItem = this.lstTrade.Items.Add(arTradeMsg[0]);//时间
                lvItem.SubItems.Add(arTradeMsg[3]);//code
                lvItem.SubItems.Add(arTradeMsg[2]);//交易方向
               
                double dbPrice = double.Parse(arTradeMsg[5]);
                lvItem.SubItems.Add(arTradeMsg[5]);//成交价格
                string sVol = arTradeMsg[6];
                sVol = sVol.Trim('#');
                lvItem.SubItems.Add(sVol);//数量

                int iVol = Int32.Parse(sVol);

                double dbMoney = iVol * dbPrice;

                //lvItem.SubItems.Add(dbMoney.ToString("#0.00"));//金额
                

            }
        }

        private void label7_Click(object sender, EventArgs e)
        {

        }

        private void txtLastDivPrice_TextChanged(object sender, EventArgs e)
        {

        }

        public void ResumeStrategy(StrategyContext oContext)
        {

            this.m_sStrategyId = oContext.Uid;
            //"合约一;合约二;是否标准合约;开仓条件;价差门限;开仓数量;平仓条件一;平仓价差一;平仓条件二;平仓价差二;启用策略执行下单;优先开仓合约;每天开仓次数小于;下单账号;开仓方向";
            if (oContext.Params.Count >= 15)
            {
                StrategyParam _param = oContext.Params[2];
                if (0 == _param.ParamName.CompareTo("是否标准合约") && 0 == _param.ParamValue.CompareTo("1"))
                {
                    this.rdoStd.Checked = true;
                    this.cmbStdArbitrage.Text = oContext.Params[0].ParamValue;
                }
                else
                {
                    this.rdoCustom.Checked = true;
                    this.txtCode1.Text = oContext.Params[0].ParamValue;
                    this.txtCode2.Text = oContext.Params[1].ParamValue;
                }

                this.cmbCondtion.Text = oContext.Params[3].ParamValue;
                this.txtPriceDiv.Text = oContext.Params[4].ParamValue;
                this.txtOpenNum.Text = oContext.Params[5].ParamValue;

                if (0 == oContext.Params[6].ParamName.CompareTo("平仓条件一") && string.Empty != oContext.Params[6].ParamValue)
                {
                    this.chkCloseCon1.Checked = true;
                    this.cmbPriceDivCon1.Text = oContext.Params[6].ParamValue;
                    this.txtPriceDivCon1.Text = oContext.Params[7].ParamValue;
                }

                if (0 == oContext.Params[8].ParamName.CompareTo("平仓条件二") && string.Empty != oContext.Params[8].ParamValue)
                {
                    this.chkCloseCon2.Checked = true;
                    this.cmbPriceDivCon2.Text = oContext.Params[8].ParamValue;
                    this.txtPriceDivCon2.Text = oContext.Params[9].ParamValue;
                }

                if (0 == oContext.Params[10].ParamName.CompareTo("平仓条件二") && 0 == oContext.Params[10].ParamValue.CompareTo("1"))
                {
                    this.chkStrategyOrder.Checked = true;
                    int iFirstOpen = Int32.Parse(oContext.Params[11].ParamValue);
                    if (0 == iFirstOpen)
                    {
                        this.cmbFirstOpenContract.Text = "合约一";
                    }
                    else if (1 == iFirstOpen)
                    {
                        this.cmbFirstOpenContract.Text = "合约二";
                    }
                    else if (2 == iFirstOpen)
                    {
                        this.cmbFirstOpenContract.Text = "同时开仓";
                    }
                    
                }

                this.txtOpenLimit.Text = oContext.Params[12].ParamValue;

                this.cmbAccount.Text = oContext.Params[13].ParamValue;

                int iDir = Int32.Parse(oContext.Params[14].ParamValue);
                if (0 == iDir)
                {
                    this.cmbDir.SelectedIndex = 0;
                }
                else
                {
                    this.cmbDir.SelectedIndex = 1;
                }

            }
           

            


            //resume open postion
            for (int i = 0; i < oContext.Postions.Count; i++)
            {
                ContractPostion oPos = oContext.Postions[i];
                ListViewItem oItem = new ListViewItem(new string[] {
                            "",
                            oPos.ContractCode, 
                            (0 == oPos.Direction ? "买入" : "卖出"),
                            oPos.Postion.ToString(),
                            oPos.OpenPrice.ToString(),
                            oPos.OpenTime
                            });
                this.lstPostion.Items.Add(oItem);
            }
        }

        protected string GetPostionInfo()
        {
            string sPosInfo = string.Empty;
            //PostionInfo format: "code1;name;postion;price;dir;time$code2;name;postion;price;dir;time"
            for (int i = 0; i < lstPostion.Items.Count; i++ )
            {
                ListViewItem oItem = lstPostion.Items[i];
                sPosInfo += oItem.SubItems[1].Text;
                sPosInfo += ";";

                sPosInfo += oItem.SubItems[0].Text;
                sPosInfo += ";";

                sPosInfo += oItem.SubItems[3].Text;
                sPosInfo += ";";

                sPosInfo += oItem.SubItems[4].Text;
                sPosInfo += ";";

                sPosInfo += (oItem.SubItems[2].Text == "买入" ? "0" : "1");
                sPosInfo += ";";

                sPosInfo += oItem.SubItems[5].Text;
                sPosInfo += ";";
                sPosInfo += "$";


            }
            return sPosInfo;
        }

        private void rdoStd_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void btnStatusResume_Click(object sender, EventArgs e)
        {
            FrmArbitrageStatusResume frmStatus = new FrmArbitrageStatusResume();

            if (this.rdoCustom.Checked) 
            {
                frmStatus.Code1 = txtCode1.Text;
                frmStatus.Code2 = txtCode2.Text;

                if (2 == this.lstPostion.Items.Count)
                {
                    ListViewItem oItem = this.lstPostion.Items[0];
                    if (this.txtCode1.Text == oItem.SubItems[1].Text)
                    {
                        frmStatus.Postion1 = Int32.Parse(oItem.SubItems[3].Text);
                        frmStatus.OpenPrice1 = double.Parse(oItem.SubItems[4].Text);
                        frmStatus.OpenTime1 = oItem.SubItems[5].Text;
                    }
                    else if (this.txtCode2.Text == oItem.SubItems[1].Text)
                    {
                        frmStatus.Postion2 = Int32.Parse(oItem.SubItems[3].Text);
                        frmStatus.OpenPrice2 = double.Parse(oItem.SubItems[4].Text);
                        frmStatus.OpenTime2 = oItem.SubItems[5].Text;
                    }

                    oItem = this.lstPostion.Items[1];
                    if (this.txtCode1.Text == oItem.SubItems[1].Text)
                    {
                        frmStatus.Postion1 = Int32.Parse(oItem.SubItems[3].Text);
                        frmStatus.OpenPrice1 = double.Parse(oItem.SubItems[4].Text);
                        frmStatus.OpenTime1 = oItem.SubItems[5].Text;
                    }
                    else if (this.txtCode2.Text == oItem.SubItems[1].Text)
                    {
                        frmStatus.Postion2 = Int32.Parse(oItem.SubItems[3].Text);
                        frmStatus.OpenPrice2 = double.Parse(oItem.SubItems[4].Text);
                        frmStatus.OpenTime2 = oItem.SubItems[5].Text;
                    }

                    if (0 == cmbDir.SelectedIndex)
                    {
                        frmStatus.StrategyStatus = 1;
                    }
                    else if (1 == cmbDir.SelectedIndex)
                    {
                        frmStatus.StrategyStatus = 2;
                    }


                }
            }
            else
            {
                string sStdCode = this.cmbStdArbitrage.Text;
                int nt = sStdCode.IndexOf('&');
                if (nt > 0)
                {
                    string sCode2 = sStdCode.Substring(nt + 1, sStdCode.Length - nt - 1);
                    string sCode1 = sStdCode.Substring(nt - sCode2.Length, sCode2.Length);

                    frmStatus.Code1 = sCode1;
                    frmStatus.Code2 = sCode2;
                    if (2 == this.lstPostion.Items.Count)
                    {
                        ListViewItem oItem = this.lstPostion.Items[0];
                        if (sCode1 == oItem.SubItems[1].Text)
                        {
                            frmStatus.Postion1 = Int32.Parse(oItem.SubItems[3].Text);
                            frmStatus.OpenPrice1 = double.Parse(oItem.SubItems[4].Text);
                            frmStatus.OpenTime1 = oItem.SubItems[5].Text;
                        }
                        else if (sCode2 == oItem.SubItems[1].Text)
                        {
                            frmStatus.Postion2 = Int32.Parse(oItem.SubItems[3].Text);
                            frmStatus.OpenPrice2 = double.Parse(oItem.SubItems[4].Text);
                            frmStatus.OpenTime2 = oItem.SubItems[5].Text;
                        }

                        oItem = this.lstPostion.Items[1];
                        if (sCode1 == oItem.SubItems[1].Text)
                        {
                            frmStatus.Postion1 = Int32.Parse(oItem.SubItems[3].Text);
                            frmStatus.OpenPrice1 = double.Parse(oItem.SubItems[4].Text);
                            frmStatus.OpenTime1 = oItem.SubItems[5].Text;
                        }
                        else if (sCode2 == oItem.SubItems[1].Text)
                        {
                            frmStatus.Postion2 = Int32.Parse(oItem.SubItems[3].Text);
                            frmStatus.OpenPrice2 = double.Parse(oItem.SubItems[4].Text);
                            frmStatus.OpenTime2 = oItem.SubItems[5].Text;
                        }

                        if (0 == cmbDir.SelectedIndex)
                        {
                            frmStatus.StrategyStatus = 1;
                        }
                        else if (1 == cmbDir.SelectedIndex)
                        {
                            frmStatus.StrategyStatus = 2;
                        }
                    }
                    
                }

            }

            

            if (DialogResult.OK == frmStatus.ShowDialog())
            {
                if (DialogResult.Yes == MessageBox.Show("是否要恢复状态", "警告", MessageBoxButtons.YesNo, MessageBoxIcon.Warning))
                {
                    //PostionInfo format: "code1;name;postion;price;dir;time$code2;name;postion;price;dir;time"
                    
                    string sPostion = string.Empty;
                    if (0 == frmStatus.StrategyStatus)
                    {
                        if (null != del_SetStrategyContext)
                        {
                            del_SetStrategyContext(m_sStrategyId, "");
                        }

                        this.lstPostion.Items.Clear();
                    }
                    else
                    {

                        if (1 == frmStatus.StrategyStatus)
                        {
                            this.cmbDir.SelectedIndex = 0;
                        }
                        else if (2 == frmStatus.StrategyStatus)
                        {
                            this.cmbDir.SelectedIndex = 1;
                        }

                        //update postion list
                        if (0 == this.lstPostion.Items.Count)
                        {
                            ListViewItem oItem = new ListViewItem(new string[] {
                            "",
                            frmStatus.Code1, 
                            (1 == frmStatus.StrategyStatus ? "买入" : "卖出"),
                            frmStatus.Postion1.ToString(),
                            frmStatus.OpenPrice1.ToString(),
                            frmStatus.OpenTime1
                            });
                            this.lstPostion.Items.Add(oItem);

                           oItem = new ListViewItem(new string[] {
                            "",
                            frmStatus.Code2, 
                            (2 == frmStatus.StrategyStatus ? "买入" : "卖出"),
                            frmStatus.Postion2.ToString(),
                            frmStatus.OpenPrice2.ToString(),
                            frmStatus.OpenTime2
                            });
                            this.lstPostion.Items.Add(oItem);
                        }
                        else
                        {
                          
                            for (int i = 0; i < this.lstPostion.Items.Count; i++)
                            {
                                if (0 == frmStatus.Code1.CompareTo(this.lstPostion.Items[i].SubItems[1].Text))
                                {
                                    ListViewItem oItem = this.lstPostion.Items[i];
                                    
                                    oItem.SubItems[3].Text = frmStatus.Postion1.ToString();
                                    oItem.SubItems[4].Text = frmStatus.OpenPrice1.ToString("F");
                                    oItem.SubItems[5].Text = frmStatus.OpenTime1;
                                   
                                }
                                else if (0 == frmStatus.Code2.CompareTo(this.lstPostion.Items[i].SubItems[1].Text))
                                {
                                    ListViewItem oItem = this.lstPostion.Items[i];
                                    oItem.SubItems[3].Text = frmStatus.Postion2.ToString();
                                    oItem.SubItems[4].Text = frmStatus.OpenPrice2.ToString("F");
                                    oItem.SubItems[5].Text = frmStatus.OpenTime2;
                                }

                            }
                           
                        }


                        //first leg
                        sPostion += frmStatus.Code1;
                        sPostion += ";";

                        sPostion += ";";

                        sPostion += frmStatus.Postion1.ToString();
                        sPostion += ";";

                        sPostion += frmStatus.OpenPrice1.ToString();
                        sPostion += ";";

                        if (1 == frmStatus.StrategyStatus) //做多价差
                        {
                            sPostion += "0";
                            sPostion += ";";
                        }
                        else if (2 == frmStatus.StrategyStatus)//做空价差
                        {
                            sPostion += "1";
                            sPostion += ";";
                        }

                        sPostion += frmStatus.OpenTime1;
                        sPostion += ";";
                        sPostion += "$";

                        //second leg
                        sPostion += frmStatus.Code2;
                        sPostion += ";";

                        sPostion += ";";

                        sPostion += frmStatus.Postion2.ToString();
                        sPostion += ";";

                        sPostion += frmStatus.OpenPrice2.ToString();
                        sPostion += ";";

                        if (1 == frmStatus.StrategyStatus) //做多价差
                        {
                            sPostion += "1";
                            sPostion += ";";
                        }
                        else if (2 == frmStatus.StrategyStatus)//做空价差
                        {
                            sPostion += "0";
                            sPostion += ";";
                        }

                        sPostion += frmStatus.OpenTime2;
                        sPostion += ";";
                        sPostion += "$";

                        if (null != del_SetStrategyContext)
                        {
                            del_SetStrategyContext(m_sStrategyId, sPostion);
                        }
                    }
                    
                   
                    
                }
            }
        }

        private void btnRun_Click(object sender, EventArgs e)
        {
            if (!CheckParams())
            {
                return;
            }

            if (!m_bRun)
            {
                //订阅合约
                string sSubcode = "subcode#";

                string sCode = this.cmbStdArbitrage.Text;

                if (this.rdoCustom.Checked)
                {
                    sCode = this.txtCode1.Text + "$" + this.txtCode2.Text;
                }



                sSubcode = sSubcode + sCode + ";" + "FUTURES;CTP;" + "#";

                delegate_Init(sSubcode);

                //params

                string sValue = "";
                //"合约一;合约二;是否标准合约;开仓条件;价差门限;开仓数量;平仓条件一;平仓价差一;平仓条件二;平仓价差二;启用策略执行下单;优先开仓合约;每天开仓次数小于;下单账号";
                string sFormat = "{0};{1};{2};{3};{4};{5};{6};{7};{8};{9};{10};{11};{12};{13};{14};";
                if (this.rdoCustom.Checked)
                {
                    int iFirstOpenContract = this.cmbFirstOpenContract.SelectedIndex;


                    sValue = string.Format(sFormat, this.txtCode1.Text,
                                                      this.txtCode2.Text,
                                                      "0",
                                                      this.cmbCondtion.Text,
                                                      this.txtPriceDiv.Text,
                                                      this.txtOpenNum.Text,
                                                      this.cmbPriceDivCon1.Text,
                                                      this.txtPriceDivCon1.Text,
                                                      this.cmbPriceDivCon2.Text,
                                                      this.txtPriceDivCon2.Text,
                                                      this.chkStrategyOrder.Checked ? "1" : "0",
                                                      iFirstOpenContract.ToString(),
                                                      this.txtOpenLimit.Text,
                                                      this.cmbAccount.Text,
                                                      this.cmbDir.SelectedIndex.ToString()
                                                      );
                }
                else
                {
                    sValue = string.Format(sFormat, this.cmbStdArbitrage.Text,
                                                      this.txtCode2.Text,
                                                      "1",
                                                      this.cmbCondtion.Text,
                                                      this.txtPriceDiv.Text,
                                                      this.txtOpenNum.Text,
                                                      this.cmbPriceDivCon1.Text,
                                                      this.txtPriceDivCon1.Text,
                                                      this.cmbPriceDivCon2.Text,
                                                      this.txtPriceDivCon2.Text,
                                                      this.chkStrategyOrder.Checked ? "1" : "0",
                                                      "2",//同时开
                                                      this.txtOpenLimit.Text,
                                                      this.cmbAccount.Text,
                                                      this.cmbDir.SelectedIndex.ToString()
                                                      );
                }

                this.rdoCustom.Enabled = false;
                this.rdoStd.Enabled = false;
                this.txtCode1.Enabled = false;
                this.txtCode2.Enabled = false;
                this.cmbStdArbitrage.Enabled = false;

                if (string.Empty == m_sStrategyId)
                {
                    m_sStrategyId = Guid.NewGuid().ToString();
                }

                m_sStrategyName = "商品套利策略";
                string sPostionInfo = GetPostionInfo();
                string sParam = m_sStrategyName
                    + "#" + sValue
                    + "#" + m_sFormId
                    + "#" + m_sStrategyId
                    + "#" + sPostionInfo
                    + "#";


                string sName = delegate_AddrunStrategy(sParam);

                m_bRun = true;
                this.btnStatusResume.Enabled = true;

                this.btnApply.Enabled = true;
                this.btnStop.Enabled = true;

                this.btnRun.Enabled = false;
            }
            else
            {
                if (null != del_SetStrategyParam)
                {
                    del_SetStrategyParam(m_sStrategyId, "使能", "1");
                }

                this.btnStop.Enabled = true;
                this.btnRun.Enabled = false;
                
            }
            

        }

        private void btnStop_Click(object sender, EventArgs e)
        {
            if (!CheckParams())
            {
                return;
            }

            if (null != del_SetStrategyParam)
            {
                del_SetStrategyParam(m_sStrategyId, "使能", "0");
            }

            this.btnStop.Enabled = false;
            this.btnRun.Enabled = true;

        }

    }


    public class ArbitrageItem
    {
        string m_sInstrumentID;
        public string InstrumentID
        {
            get { return m_sInstrumentID; }
            set { m_sInstrumentID = value; }
        }
        string m_sExchangeID;
        public string ExchangeID
        {
            get { return m_sExchangeID; }
            set { m_sExchangeID = value; }
        }
        int m_iMinMarketOrderVolume;
        public int MinMarketOrderVolume
        {
            get { return m_iMinMarketOrderVolume; }
            set { m_iMinMarketOrderVolume = value; }
        }
        int m_iMaxMarketOrderVolume;
        public int MaxMarketOrderVolume
        {
            get { return m_iMaxMarketOrderVolume; }
            set { m_iMaxMarketOrderVolume = value; }
        }
    }
}
