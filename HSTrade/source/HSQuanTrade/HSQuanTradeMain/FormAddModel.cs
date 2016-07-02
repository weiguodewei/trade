using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml;
using System.Diagnostics;
using System.IO;
using HSQuanTradeMain.MsgDispatch;

namespace HSQuanTradeMain
{
    public partial class FormAddModel :TradeFormBase
    {
        public delegate_getallstrategyname getname = null;

        public delegate_getstrategyparams getparams = null;

        public delegate_AddRunStrategy addrunstrategy = null;

        public delegate_SetStrategyParam setstrategyparam = null;

        public delete_getValues getstrparamvalue = null;

        public delete_init addmodelinit = null;

        public string modelname;

        public List<modelinfo> modlist = new List<modelinfo>();

        public testExcel te;

        //map
        public Dictionary<string, string> mapInit = new Dictionary<string, string>();

        protected string m_sStrategyId = string.Empty;

        //如果是加载已有模型就不再保存，否则保存下
        bool isLoadModel = false;
        
        public FormAddModel()
        {
            InitializeComponent();
            GenerateGuid("FormAddModel");
        }

        private void FormAddModel_Load(object sender, EventArgs e)
        {
            if (getname != null)
            {
                string names = getname();

                string[] list = names.Split(';');

                foreach (string name in list)
                {
                    if( name != ""  )
                        comboBox1.Items.Add(name);
                }
            }
            string account = "";
            foreach(AccountInfo info in FormSet.listAccount)
            {
                account = info.account;
                if (account != "")
                    comboBox2.Items.Add(account);
            }

            //初始化参数值
            InitParam();

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

                    //初始化值
                    if (mapInit.ContainsKey(param))
                        row[1] = mapInit[param];

                    table.Rows.Add(row);
                }

            }


            set.Tables.Add(table);

            dataGridView1.DataSource = table.DefaultView;

          

        }

        private void button1_Click(object sender, EventArgs e)
        {
            int nt = comboBox1.SelectedIndex;

            if (nt == -1)
                return;

            if (!checkinput())
            {
                modelname = "-1";
                return;
            }

            string account = comboBox2.Text ;         //没设置account无效

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

            string param = "";

            //设置参数，添加新的模型运行----OCX中需要新建一个模型实例,
            if (addrunstrategy != null)
            {
               
                //param = name + "#" + str + "#" + value + "#";
                m_sStrategyId = Guid.NewGuid().ToString();
                param = name + "#" + value + "#"
                        + m_sFormId
                        + "#" + m_sStrategyId
                        + "#" + ""
                        + "#";

                //参数使用 策略名#参数值 传入
                modelname = addrunstrategy(param);
                modelname = m_sStrategyId;
            }

            //具体某个策略特殊的处理
            otherDeal();

            //保存
            //save(name);

            saveModel(modelname);

        }

        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {

            if (comboBox1.Text == "")
                return;

            for (int i = 0; i < dataGridView1.RowCount;i++ )
            {
                DataGridViewRow row = dataGridView1.Rows[i];

                if (row.Cells[0].Value.ToString() == "账户")
                {
                    row.Cells[1].Value = comboBox2.Text;
                    break;
                }

            }


        }

        public void save(string strname)
        {
            
            //"交易合约;交易数量;开仓价;止损率;换手率;回调率;是否追单;超时时间;账户;"

            string paramlist = getparams(strname) ;

            //-1的位置存放不再参数列表的值，后面存放策略名称
            string values = strname + "#" +modelname + "#" + getstrparamvalue(modelname, "") +"#-1#";  


            if (!FormMain.modelvalues.ContainsKey(modelname))
                FormMain.modelvalues.Add(modelname, values);

            if (!FormLoadModel.modelAll.ContainsKey(modelname))
                FormLoadModel.modelAll.Add(modelname, values);
            


        }

        public string getfilename()
        {
            string str1 = Process.GetCurrentProcess().MainModule.FileName;

            int nt = str1.LastIndexOf("\\");

            string path = str1.Substring(0, nt);

            return path + "\\model.txt";
        }

        private void rbuy_CheckedChanged(object sender, EventArgs e)
        {
            for (int i = 0; i < dataGridView1.RowCount; i++)
            {
                DataGridViewRow row = dataGridView1.Rows[i];

                if (row.Cells[0].Value.ToString() == "交易方向")
                {
                    if (rbuy.Checked)
                    {
                        row.Cells[1].Value = "0";
                        rsale.Checked = false;
                    }
                    break;
                }

            }

        }

        private void rsale_CheckedChanged(object sender, EventArgs e)
        {
            for (int i = 0; i < dataGridView1.RowCount; i++)
            {
                DataGridViewRow row = dataGridView1.Rows[i];

                if (row.Cells[0].Value.ToString() == "交易方向")
                {
                    if (rsale.Checked)
                    {
                        row.Cells[1].Value = "1";
                        rbuy.Checked = false;
                    }
                    break;
                }




            }

        }

        private void rkai_CheckedChanged(object sender, EventArgs e)
        {
            for (int i = 0; i < dataGridView1.RowCount; i++)
            {
                DataGridViewRow row = dataGridView1.Rows[i];

                if (row.Cells[0].Value.ToString() == "开平方向")
                {
                    if (rkai.Checked)
                    {
                        row.Cells[1].Value = "0";
                        rping.Checked = false;
                    }
                    break;

                }


            }

        }

        private void rping_CheckedChanged(object sender, EventArgs e)
        {
            for (int i = 0; i < dataGridView1.RowCount; i++)
            {
                DataGridViewRow row = dataGridView1.Rows[i];

                if (row.Cells[0].Value.ToString() == "开平方向")
                {
                    if (rping.Checked)
                    {
                        row.Cells[1].Value = "1";
                        rkai.Checked = false;
                    }
                    break;

                }

            }

        }

        private bool checkinput()
        {
            bool ret = true ;

            for (int i = 0; i < dataGridView1.Rows.Count-1; i++)
            {
                DataGridViewRow row = dataGridView1.Rows[i];

                if (row.Cells[1] == null || row.Cells[1].Value == null)
                {
                    MessageBox.Show("请检查数据是否正确！");
                    ret = false;
                    break;

                }

                if (row.Cells[0].Value.ToString() =="账户")
                {
                    string account = row.Cells[1].Value.ToString();

                    if (!FormLogin.loginOK.Contains(account))
                    {
                        MessageBox.Show("该账户还未登录！");
                        ret = false;
                        break;
                    }
                }

            }
            return ret;
        
        }

        private void button3_Click(object sender, EventArgs e)
        {

            //testExcel te = new testExcel();

            //te.init = new delete_init(axHSQuanTrade1.Init);

            if (te.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                if (comboBox1.Text == "抢涨停板买入卖出")
                {
                    for (int i = 0; i < dataGridView1.RowCount; i++)
                    {
                        DataGridViewRow row = dataGridView1.Rows[i];

                        if (row.Cells[0].Value.ToString() == "委托信息")
                        {
                            row.Cells[1].Value = te.orderinfo;
                            break;
                        }

                    }

                }

            }

            


        }


        private void otherDeal()
        {
            if (comboBox1.Text == "抢涨停板买入卖出")
            {
                //需要订阅131810，回购
                string sub = "subcode#" + "131810" + ";" + "STOCK;UFX;" + "#";
                addmodelinit(sub);
            
            }
            else if (comboBox1.Text == "网格交易策略")
            {
                //订阅合约
                string sub = "subcode#";

                string code = dataGridView1.Rows[0].Cells[1].Value.ToString();

                sub = sub + code + ";" + "FUTURES;CTP;" + "#";

                addmodelinit(sub);
            
            }
            else if (comboBox1.Text == "涨停板股票卖出策略")
            {
                //订阅合约
                string sub = "subcode#";

                string code = dataGridView1.Rows[0].Cells[1].Value.ToString();

                sub = sub + code + ";" + "STOCK;UFX;" + "#";

                addmodelinit(sub);
            
            }
            
        
        }

        private void cbtype_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (comboBox1.Text == "抢涨停板买入卖出")
            {
                //先初始化参数值
                for (int i = 0; i < dataGridView1.RowCount; i++)
                {
                    DataGridViewRow row = dataGridView1.Rows[i];

                    if (row.Cells[0] != null && row.Cells[0].Value != null)
                    {

                        if (row.Cells[0].Value.ToString() == "交易合约")
                        {
                            row.Cells[1].Value = "1";

                        }
                        else if (row.Cells[0].Value.ToString() == "数量")
                        {
                            row.Cells[1].Value = "1";

                        }
                        else if (row.Cells[0].Value.ToString() == "价格")
                        {
                            row.Cells[1].Value = "1";

                        }
                        else if (row.Cells[0].Value.ToString() == "交易方向")
                        {
                            row.Cells[1].Value = "0";

                        }
                        else if (row.Cells[0].Value.ToString() == "开平方向")
                        {
                            row.Cells[1].Value = "0";

                        }
                        else if (row.Cells[0].Value.ToString() == "委托信息")
                        {
                            row.Cells[1].Value = "1";

                        }
                        else if (row.Cells[0].Value.ToString() == "比例")
                        {
                            row.Cells[1].Value = "0.5";

                        }
                    }


                }


                if (cbtype.Text == "下单")
                {
                    for (int i = 0; i < dataGridView1.RowCount; i++)
                    {
                        DataGridViewRow row = dataGridView1.Rows[i];
                        if (row.Cells[0].Value.ToString() == "交易合约")
                        {
                            row.Cells[1].Value = "-6666";
                            break;
                        }
                    }
            
                }
                else if (cbtype.Text == "监控")
                {
                    for (int i = 0; i < dataGridView1.RowCount; i++)
                    {
                        DataGridViewRow row = dataGridView1.Rows[i];
                        if (row.Cells[0].Value.ToString() == "交易合约")
                        {
                            row.Cells[1].Value = "-9999";
                            break;
                        }
                    }
                }
                else if (cbtype.Text == "回购")
                {
                    for (int i = 0; i < dataGridView1.RowCount; i++)
                    {
                        DataGridViewRow row = dataGridView1.Rows[i];
                        if (row.Cells[0].Value.ToString() == "委托信息")
                        {
                            row.Cells[1].Value = "-1";
                            break;
                        }
                    }

                }

            
            }

        }

        public void InitParam()
        {
            mapInit.Add("止损率", "0.05");
            mapInit.Add("换手率", "0.01");
            mapInit.Add("回调率", "0.05");
            mapInit.Add("是否追单", "0");
            mapInit.Add("超时时间", "3");
            mapInit.Add("初始风险", "0");
            mapInit.Add("资金回撤", "0");
            mapInit.Add("最大撤单次数", "500");
            mapInit.Add("最大成交数量", "50000");
            mapInit.Add("交易方向", "0");
            mapInit.Add("开平方向", "0");
            mapInit.Add("委托信息", "1");
            mapInit.Add("报单间隔", "100");
            mapInit.Add("买一买二比", "0.5");
            mapInit.Add("持续时间", "60");

            isLoadModel = false;
        
        }

        //加载保存过的模型
        private void button4_Click(object sender, EventArgs e)
        {
            string filename = "";

            dataGridView1.DataSource = null;

            OpenFileDialog f1 = new OpenFileDialog();

            f1.Filter = "文本文件|*.ini";
            if (f1.ShowDialog() == DialogResult.OK)
            {
                filename = f1.FileName;
            }

            if (filename != "")
            {

                IniUtil ini = new IniUtil(filename);

                comboBox1.Text = ini.ReadString("strategyName", "策略名称", "请手动增加模型!");

                loadModel(ini, comboBox1.Text);
            }
        }

        private void loadModel(IniUtil ini,string strategyName)
        {
            string list = getparams(strategyName);

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

                    //读取配置文件的值
                    row[1] = ini.ReadString("params", param, "error!!!");

                    table.Rows.Add(row);
                }

            }


            set.Tables.Add(table);

            dataGridView1.DataSource = table.DefaultView;

            isLoadModel = true;

        }

        private void saveModel(string modelName)
        {
            //如果是加载已有模型就不保存
            if (isLoadModel)
                return;

            string str1 = Process.GetCurrentProcess().MainModule.FileName;

            int nt = str1.LastIndexOf("\\");

            string path = str1.Substring(0, nt);

            path = path + "\\strmodels\\" + modelName + ".ini";

            string strategyName = comboBox1.Text.ToString();


            IniUtil ini = new IniUtil(path);

            ini.WriteString("strategyName", "策略名称", strategyName);

            ini.WriteString("modelName", "模型名称", modelName);


            for (int i = 0; i < dataGridView1.Rows.Count; i++)
            {
                DataGridViewRow row = dataGridView1.Rows[i];

                if (row.Cells[1] != null && row.Cells[1].Value != null)
                {
                    string key = row.Cells[0].Value.ToString();     //参数值

                    string value = row.Cells[1].Value.ToString() ;     //参数值

                    ini.WriteString("params", key, value);
                    
                }
            }


        }

    }
}
