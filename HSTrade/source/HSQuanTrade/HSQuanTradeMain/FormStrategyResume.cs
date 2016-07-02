using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;
using System.IO;
using System.Xml;
using System.Collections;
using HSQuanTradeMain.StrategyResume;

namespace HSQuanTradeMain
{
    public partial class FormStrategyResume : Form
    {
        public delegate_getallstrategyname delegate_GetAllStrategyName = null;

        protected List<StrategyContext> m_arStrategyCase = new List<StrategyContext>();
	    
        protected string m_sCurStrategyName;

        protected string m_sResumeStrategyId;
        public string ResumeStrategyId
        {
            get { return m_sResumeStrategyId; }
        }
        
        public FormStrategyResume()
        {
            InitializeComponent();

            
        }

        public bool HasResumeStrategy(string sStagtegyName)
        {
            
            if (!LoadStrategyXml())
            {
                return false;
            }
            foreach (StrategyContext oStrategy in m_arStrategyCase)
            {
                if (0 == oStrategy.StrategyName.CompareTo(sStagtegyName))
                {
                    m_sCurStrategyName = sStagtegyName;
                    return true;
                }
            }
           
            return false;
        }

        public List<StrategyContext> GetStrategyContext(string sStagtegyName)
        {
            List<StrategyContext> arContext = new List<StrategyContext>();
            foreach (StrategyContext oStrategy in m_arStrategyCase)
            {
                if (0 == oStrategy.StrategyName.CompareTo(sStagtegyName))
                {
                    arContext.Add(oStrategy);
                }
            }


            return arContext;
        }

        
        private void FormStrategyResume_Load(object sender, EventArgs e)
        {
            if (delegate_GetAllStrategyName != null)
            {
                string sStrategys = delegate_GetAllStrategyName();

                string[] list = sStrategys.Split(';');

                foreach (string name in list)
                {
                    if (name != "")
                        cmbStrategy.Items.Add(name);
                }
                cmbStrategy.Items.Add("所有");

            }

            for (int i = 0; i < cmbStrategy.Items.Count; i++ )
            {
                if (0 == m_sCurStrategyName.CompareTo(cmbStrategy.Items[i].ToString()))
                {
                    cmbStrategy.SelectedIndex = i;
                    break;
                }
                
            }

            
        }

        private void cmbStrategy_SelectedIndexChanged(object sender, EventArgs e)
        {
            int nt = cmbStrategy.SelectedIndex;
            if (nt == -1)
                return;

            lstStrategy.Items.Clear();
            lstStrategyInfo.Items.Clear();
            string sName = cmbStrategy.Items[nt].ToString();
            
            if (0 == sName.CompareTo("所有"))
            {
                foreach (StrategyContext oStrategy in m_arStrategyCase)
                {
                    ListViewItem oItem = lstStrategy.Items.Add(oStrategy.StrategyName);
                    oItem.SubItems.Add(oStrategy.Uid);
                    
                }
            }
            else
            {
                foreach (StrategyContext oStrategy in m_arStrategyCase)
                {
                    if (0 == oStrategy.StrategyName.CompareTo(sName))
                    {
                        ListViewItem oItem = lstStrategy.Items.Add(oStrategy.StrategyName);
                        oItem.SubItems.Add(oStrategy.Uid);
                    }
                   

                }
            }


        }

        protected bool LoadStrategyXml()
        {
            StrategySerial oSerail = new StrategySerial();
            oSerail.Load(ref m_arStrategyCase);

            //string strPath = Process.GetCurrentProcess().MainModule.FileName;

            //int nt = strPath.LastIndexOf("\\");

            //strPath = strPath.Substring(0, nt);
            //strPath += "\\StrageDLL\\strategy.xml";
            //if (!File.Exists(strPath))
            //    return false;

            //XmlDocument oXmlDoc = new XmlDocument();
            //oXmlDoc.Load(strPath);

            //XmlNodeList xmlChildLst = oXmlDoc.ChildNodes;
            //foreach (XmlNode oChildNode in xmlChildLst)
            //{
            //    if (0 == oChildNode.Name.CompareTo("strategy_bak"))
            //    {
            //        XmlNodeList xmlNodeLst = oChildNode.ChildNodes;
            //        foreach (XmlNode oXmlNode in xmlNodeLst)
            //        {
            //            ParseStrategyCase(oXmlNode);
            //        }
            //    }
            //}

                                
            return true;
        }

        //protected void ParseStrategyCase(XmlNode oXmlNode)
        //{
        //    if (0 == oXmlNode.Name.CompareTo("strategy_case"))
        //    {
        //        string sStatus = oXmlNode.Attributes["status"].Value;
        //        if( sStatus != null)
        //        {
        //            if (0 == sStatus.CompareTo("0"))//未加载
        //            {
        //                StrategyContext oStragtegySer = new StrategyContext();
        //                XmlNodeList xmlNodeLst = oXmlNode.ChildNodes;
        //                foreach (XmlNode oChildNode in xmlNodeLst)
        //                {
        //                    if (0 == oChildNode.Name.CompareTo("id"))
        //                    {
        //                        oStragtegySer.Uid = oChildNode.FirstChild.Value;
        //                    }
        //                    else if (0 == oChildNode.Name.CompareTo("name"))
        //                    {
        //                        oStragtegySer.StrategyName = oChildNode.FirstChild.Value;
        //                    }
        //                    else if (0 == oChildNode.Name.CompareTo("params"))
        //                    {
        //                        XmlNodeList oParamLst = oChildNode.ChildNodes;
        //                        foreach (XmlNode oParamNode in oParamLst)
        //                        {
        //                            StrategyParam oParam = new StrategyParam();
        //                            oParam.ParamName = oParamNode.Attributes["name"].Value;
        //                            oParam.ParamValue = oParamNode.Attributes["value"].Value;
        //                            oStragtegySer.Params.Add(oParam);
        //                        }
        //                    }
        //                    else if (0 == oChildNode.Name.CompareTo("contract_postion"))
        //                    {
        //                        XmlNodeList oPostionList = oChildNode.ChildNodes;
        //                        foreach (XmlNode oPostionNode in oPostionList)
        //                        {
        //                            ContractPostion oPostion = new ContractPostion();
        //                            oPostion.ContractCode = oPostionNode.Attributes["code"].Value;
        //                            oPostion.ContractName = oPostionNode.Attributes["name"].Value;
        //                            oPostion.Postion = Int32.Parse(oPostionNode.Attributes["postion"].Value);
        //                            oPostion.Direction = Int32.Parse(oPostionNode.Attributes["dir"].Value);
        //                            oPostion.OpenPrice = double.Parse(oPostionNode.Attributes["open_price"].Value);
        //                            oPostion.OpenTime = oPostionNode.Attributes["open_time"].Value;
        //                            oStragtegySer.Postions.Add(oPostion);
        //                        }
        //                    }
                           
        //                }

        //                m_arStrategyCase.Add(oStragtegySer);
        //            }
        //        }

                
        //    }
        //}

        private void lstStrategy_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (lstStrategy.SelectedItems.Count > 0)
            {
                lstStrategyInfo.Items.Clear();
                string sUid = lstStrategy.SelectedItems[0].SubItems[1].Text;
                foreach (StrategyContext oStrategy in m_arStrategyCase)
                {
                    if (0 == oStrategy.Uid.CompareTo(sUid))
                    {
                        ListViewGroup oGrpBasic = lstStrategyInfo.Groups.Add("Basic", "基本信息");
                        lstStrategyInfo.Groups.Add(oGrpBasic);
                        ListViewItem _item = new ListViewItem(new string[] { "Uid", oStrategy.Uid.ToString() }, oGrpBasic);
                         
                        _item = new ListViewItem(new string[]{"Name", oStrategy.StrategyName});
                        lstStrategyInfo.Items.Add(_item);
                        

                        ListViewGroup oGrpParam = lstStrategyInfo.Groups.Add("Param", "参数");
                        lstStrategyInfo.Groups.Add(oGrpParam);
                        foreach (StrategyParam oParam in oStrategy.Params)
                        {
                            _item = new ListViewItem(new string[] { oParam.ParamName, oParam.ParamValue }, oGrpParam);
                            lstStrategyInfo.Items.Add(_item);
                        }
                       

                        ListViewGroup oGrpPostion = lstStrategyInfo.Groups.Add("Postion", "仓位");
                        lstStrategyInfo.Groups.Add(oGrpPostion);
                        foreach (ContractPostion oPostion in oStrategy.Postions)
                        {
                            _item = new ListViewItem(new string[] { oPostion.ContractCode, 
                                oPostion.Postion.ToString(), 
                                oPostion.Direction.ToString(),
                                oPostion.OpenPrice.ToString(),
                                oPostion.OpenTime
                            }, oGrpPostion);
                            lstStrategyInfo.Items.Add(_item);
                        }
                        

                    }


                }
            }
            


        }

        private void btnResume_Click(object sender, EventArgs e)
        {
            
            if (DialogResult.Yes == MessageBox.Show("是否恢复运行当前策略", "策略恢复", MessageBoxButtons.YesNo, MessageBoxIcon.Question))
            {
                
                if (this.lstStrategy.SelectedItems.Count > 0)
                {
                    m_sResumeStrategyId = this.lstStrategy.SelectedItems[0].SubItems[1].Text;
                }
                
                this.DialogResult = DialogResult.Yes;
            }
            else
            {
                this.DialogResult = DialogResult.No;
            }
            
        }


        public bool GetResumeStrategyCase(string sStategyId, ref StrategyContext oStategy)
        {
            bool bFound = false;
            for (int i = 0; i < m_arStrategyCase.Count; i++ )
            {
                oStategy = m_arStrategyCase[i];
                if (0 == oStategy.Uid.CompareTo(sStategyId))
                {
                    bFound = true;
                    break;
                }
            }

            return bFound;
        }

        
        private void btnRemove_Click(object sender, EventArgs e)
        {
            if (this.lstStrategy.SelectedItems.Count > 0)
            {
                string sStrategyId = this.lstStrategy.SelectedItems[0].SubItems[1].Text;

                StrategySerial oSerial = new StrategySerial();
                if (null != oSerial)
                {
                    oSerial.RemoveStrategy(sStrategyId);

                    this.lstStrategyInfo.Clear();

                    this.lstStrategy.Items.RemoveAt(this.lstStrategy.SelectedItems[0].Index);
                }
            }
            
        }

        private void lstStrategy_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            if (DialogResult.Yes == MessageBox.Show("是否恢复运行当前策略", "策略恢复", MessageBoxButtons.YesNo, MessageBoxIcon.Question))
            {

                if (this.lstStrategy.SelectedItems.Count > 0)
                {
                    m_sResumeStrategyId = this.lstStrategy.SelectedItems[0].SubItems[1].Text;
                }

                this.DialogResult = DialogResult.Yes;
            }
            else
            {
                this.DialogResult = DialogResult.No;
            }
        }
    }
}
