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

namespace HSQuanTradeMain
{
    public partial class FormBackTestResult : Form
    {

        private string m_sResultFile;
        public string ResultFile
        {
            get { return m_sResultFile; }
            set { m_sResultFile = value; }
        }

        private string m_sExportTitle;
        private int m_iResultType;
        public int ResultType
        {
            get { return m_iResultType; }
            set { 
                m_iResultType = value; 
                switch(m_iResultType)
                {
                    case 0:
                        m_sExportTitle = "回测记录表";
                        break;
                    case 1:
                        m_sExportTitle = "回测权益表";
                        break;
                    default:
                        m_sExportTitle = "回测记录表";
                        break;
                }
       
            }
        }

        protected int[] m_arColWidth = {110/*序号*/, 
                                           110,/*合约*/ 
                                           100/*日期*/,
                                           100, /*开仓时间*/                                  
                                           60, /*数量*/
                                           60, /*价格*/
                                           60,/*方向*/
                                           60,/*开平*/
                                           95,/*盈亏*/
                                           60,/*手续费*/
                                           100/*权益*/
                                       };
        public FormBackTestResult()
        {
            InitializeComponent();
        }

        private void FormBackTestResult_Load(object sender, EventArgs e)
        {
            
            LoadTestResult();
        }

        private void LoadTestResult()
        {
            if (m_sResultFile != string.Empty)
            {
                StreamReader oStmReader = new StreamReader(m_sResultFile, Encoding.Default);
                String sLine;
                bool bColAdded = false;
                while ((sLine = oStmReader.ReadLine()) != null)
                {
                  
                    string [] arCells = sLine.Split(',');

                   if (!bColAdded)
                   {
                       if (0 == m_iResultType)
                       {
                           if (arCells.Count() < 12)
                           {
                               continue;
                           }
                       }
                      
                       for (int i = 0; i < arCells.Count(); i++)
                       {
                           string sCell = arCells[i];
                           if (sCell != string.Empty)
                           {
                               this.listView1.Columns.Add(sCell, m_arColWidth[i]);
                           }
                         
                       }
                    
                      
                       bColAdded = true;
                       continue;

                   }
                   ListViewItem oItem = new ListViewItem();
                    for(int i = 0; i < arCells.Count(); i++)
                    {
       
                        string sCell = arCells[i];
                        
                        if (0 == i)
                        {
                            oItem.Text = sCell;
                        }
                        else
                        {
                            oItem.SubItems.Add(sCell);
                        }
                        
                       
                    }
                    this.listView1.Items.Add(oItem);
                }
            }
        }


        private void 导出ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string sFilePath = m_sResultFile;
            int nt = sFilePath.LastIndexOf("\\");
            sFilePath = sFilePath.Substring(0, nt);
            sFilePath += "\\";
            DateTime tNow = DateTime.Now;
            string sFileName = string.Format("{0}_{1}.csv", m_sExportTitle, tNow.ToString("yyyyMMdd HH_mm_ss"));

            bool bRewrite = true;
            System.IO.File.Copy(m_sResultFile, sFilePath + sFileName, bRewrite);

            string sResult = string.Format("文件({0})已导出在({1}), 是否要打开", sFileName, sFilePath);
            if (DialogResult.Yes == MessageBox.Show(sResult, "消息", MessageBoxButtons.YesNo, MessageBoxIcon.Question))
            {
                System.Diagnostics.Process.Start(sFileName); 
            }
            

            
        }
        
        
    }
}
