using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace HSQuanTradeMain
{
    public partial class FormStragegyBackTest : Form
    {
        public delegate_getallstrategyname getname = null;

        public delegate_getstrategyparams getparams = null;



        public delegate_StragegyBackTest stragegyBackTest = null;

        public delegate_GetBackTestStatus getBackTestStatus = null;
        public string m_sModelName;

        public FormStragegyBackTest()
        {
            InitializeComponent();
        }


        protected void Init()
        {
            comboBox1.Items.Clear();
            this.dataGridView1.DataSource = null;
            this.lstCode.Items.Clear();
            this.progTotal.Value = 0;
            this.progSub.Value = 0;
            this.txtCode.Text = "";
            this.lblSub.Text = "";
            this.lblTotal.Text = "";
            this.btnStartTest.Enabled = true;
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            int nt = comboBox1.SelectedIndex;
            if (nt == -1)
                return;

            string name = comboBox1.Items[nt].ToString();

            string list = getparams(name);

            string[] names = list.Split(';');

            DataSet set = new DataSet();

            DataTable table = new DataTable();

            table.TableName = "param";

            DataColumn col = table.Columns.Add();

            col.ColumnName = "参数名";

            col.Caption = "参数名";

            col.DataType = System.Type.GetType("System.String");

            DataColumn col2 = table.Columns.Add();

            col2.ColumnName = "参数值";

            col2.Caption = "参数值";

            col2.DataType = System.Type.GetType("System.String");

            foreach (string param in names)
            {
                if (param != "")                //"交易合约;交易数量;开仓价;"   会多出一行来
                {
                    DataRow row = table.NewRow();

                    row[0] = param;

                    row[1] = "";

                    //进行一些初始化的操作
                    if (param == "止损率")
                        row[1] = "0.08";
                    else if (param == "回调率")
                        row[1] = "0.15";
                    else if (param == "日线周期")
                        row[1] = "30";
                    else if (param == "新高天数")
                        row[1] = "20";
                    else if (param == "震荡幅度")
                        row[1] = "0.3";
                    else if (param == "同时开仓数")
                        row[1] = "5";
                    else if (param == "止盈率")
                        row[1] = "0.20";
                    else if (param == "换手率")
                        row[1] = "0.05";
                    else if (param == "跌破收盘价比率1")
                        row[1] = "0.05";
                    else if (param == "跌破收盘价比率2")
                        row[1] = "0.08";
                    else if (param == "涨停类型")
                        row[1] = "0"; //0, 单日涨停， 1，一字涨停
                    else if (param == "移动止损率")
                        row[1] = "0.08";
                    else if (param == "是否追单")
                        row[1] = "0";
                    else if (param == "超时时间")
                        row[1] = "3";
                    else if (param == "初始风险")
                        row[1] = "0";
                    else if (param == "资金回撤")
                        row[1] = "0";
                    else if (param == "最大撤单次数")
                        row[1] = "500";
                    else if (param == "最大成交数量")
                        row[1] = "50000";
                    else if (param == "初始权益")
                        row[1] = "200000";
                    else if (param == "交易方向")
                        row[1] = "0";
                    else if (param == "开平方向")
                        row[1] = "0";
                    else if (param == "委托信息")
                        row[1] = "1";
                    else if (param == "报单间隔")
                        row[1] = "500";
                    else if (param == "是否复利")
                        row[1] = "0";
                    else if (param == "入场模式")
                        row[1] = "0";
                    else if (param == "是否下单")
                        row[1] = "0";
                    else if (param == "同时开仓数")
                        row[1] = "5";
                    else if (param == "手续费率")
                        row[1] = "0.001";
                    else if (param == "退出信号为指令价或收盘价")
                        row[1] = "0";
                    else if (param == "创N天新高")
                        row[1] = "0";
                    else if (param == "N天回调")
                        row[1] = "5";
                    else if (param == "回落幅度")
                        row[1] = "0.30";
                    else if (param == "单调回落")
                        row[1] = "1";
                   
                    //if (param == "账户")    //账户参数这里不显示
                    //   continue;

                    //....


                    table.Rows.Add(row);
                }

            }


            set.Tables.Add(table);

            dataGridView1.DataSource = table.DefaultView;
        }

        private void FormStragegyBackTest_Load(object sender, EventArgs e)
        {
            Init();
            if (getname != null)
            {
                string names = getname();

                string[] list = names.Split(';');

                foreach (string name in list)
                {
                    if (name != "")
                        comboBox1.Items.Add(name);
                }
            }

            this.progTotal.Step = 1;
            this.progTotal.Maximum = 100;
            this.progTotal.Minimum = 0;
            this.progSub.Step = 1;
            this.progSub.Maximum = 100;
            this.progSub.Minimum = 0;

            this.cmbCycle.SelectedIndex = 4;
            
        }
        private bool checkinput()
        {
            bool ret = true;

            for (int i = 0; i < dataGridView1.Rows.Count - 1; i++)
            {
                DataGridViewRow row = dataGridView1.Rows[i];

                if (row.Cells[1] == null || row.Cells[1].Value == null)
                {
                    MessageBox.Show("请检查数据是否正确！");
                    ret = false;
                    break;

                }
            }
            return ret;

        }
        private void btnOk_Click(object sender, EventArgs e)
        {
            
        }

        private void btnSelCode_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.InitialDirectory = Assembly.GetExecutingAssembly().Location;
            openFileDialog.Filter = "文本文件|*.txt|所有文件|*.*";
            openFileDialog.RestoreDirectory = true;
            openFileDialog.Multiselect = true;
            openFileDialog.FilterIndex = 1;
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                
                string[] arFiles = openFileDialog.FileNames;
                foreach (string sFile in arFiles)
                {
                    int nt = sFile.LastIndexOf("\\");
                    string sCode = sFile.Substring(nt + 1, sFile.Length - nt - 1 - 4 /*.txt*/);
                    
                    this.lstCode.Items.Add(sCode);
         
                }
              
            }

        }

        private void rdoStock_CheckedChanged(object sender, EventArgs e)
        {
            
            this.pnStock.Visible = this.rdoStock.Checked;
            
        }

        private void rdoFuture_CheckedChanged(object sender, EventArgs e)
        {
            
        }

       

        public void UpdateTestProgress(string sCode, int iTotalCount, int iTotalStep, int iSubCount, int iSubStep)
        {
            this.txtCode.Text = sCode;

            string sValue = "";
           
            if (0xffff == iTotalCount && 0xffff == iTotalStep)
            {
                progTotal.Value = 100;
                sValue = "100%";
                this.lblTotal.Text = sValue;
            }
            else if (iTotalCount > 0)
            {
                progTotal.Value = iTotalStep * 100 / iTotalCount;
                sValue = string.Format("{0}%", progTotal.Value);
                this.lblTotal.Text = sValue;
            }

            if (0xffff == iSubCount && 0xffff == iSubStep)
            {
                progSub.Value = 100;
                sValue = "100%";
                this.lblSub.Text = sValue;

                this.DialogResult = DialogResult.OK;
            }
            else if (iSubCount > 0)
            {
                progSub.Value = iSubStep * 100 / iSubCount;
                sValue = string.Format("{0}%", progSub.Value);
                this.lblSub.Text = sValue;
            }
           
        }

        private void btnStartTest_Click(object sender, EventArgs e)
        {
            this.btnStartTest.Enabled = false;
            int nt = comboBox1.SelectedIndex;

            if (nt == -1)
                return;

            if (!checkinput())
            {
                m_sModelName = "-1";
                return;
            }



            string name = comboBox1.Items[nt].ToString();

            string value = "";


            for (int i = 0; i < dataGridView1.Rows.Count; i++)
            {
                DataGridViewRow row = dataGridView1.Rows[i];

                if (row.Cells[1] != null && row.Cells[1].Value != null)
                {
                    value = value + row.Cells[1].Value.ToString() + ";";     //参数值

                }
            }

            //string param = "";

            int iTime1 = (int)(this.dateTimePicker1.Value - new DateTime(1970, 1, 1).ToLocalTime()).TotalSeconds;

            int iTime2 = (int)(this.dateTimePicker2.Value - new DateTime(1970, 1, 1).ToLocalTime()).TotalSeconds;

            string sCodeRange = "";
            for (int i = 0; i < this.lstCode.Items.Count; i++)
            {
                sCodeRange += this.lstCode.Items[i];
                sCodeRange += "$";
            }
           
            string sCycle = this.cmbCycle.Text;
            
            //设置参数，添加新的模型运行----OCX中需要新建一个模型实例,
            if (stragegyBackTest != null)
            {

                //参数使用 策略名#参数值 传入

                m_sModelName = stragegyBackTest(name, value, iTime1, iTime2, sCodeRange, sCycle);

            }
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            if (getBackTestStatus())
            {
                if (DialogResult.Yes == MessageBox.Show("当前回测没有完成，是否终止并退出?", "消息", MessageBoxButtons.YesNo, MessageBoxIcon.Question))
                {
                    this.DialogResult = DialogResult.Cancel;
                }
            }
            else
            {
                this.DialogResult = DialogResult.Cancel;
            }
        }

    }
}
