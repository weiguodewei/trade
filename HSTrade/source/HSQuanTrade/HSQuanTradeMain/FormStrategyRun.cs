using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace HSQuanTradeMain
{
    public partial class FormStrategyRun : Form
    {
        public  List<string> listmodelname = new List<string>();   

        public StrategryManager manager = null;

        public delete_getLog getstrlog = null;

        public delete_getValues getValues = null;

        public delegate_AddRunStrategy addmodel = null;

        public string selModelName = "";
 


        public FormStrategyRun()
        {
            InitializeComponent();
        }

        public void insertrecord(string modelname,string info)
        {
            string[] list = info.Split(';');

            if (list.Length < 1)
                return;

            ListViewItem item = new ListViewItem();

            listmodelname.Add(modelname);

            item.Text = list[0];

            for(int i=1;i<list.Length;i++)
            {
                item.SubItems.Add(list[i]);
            }

            listView1.Items.Add(item);
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            for(int i=0;i<listmodelname.Count();i++)
            {
                if (manager != null)
                {
                    string status =  manager.getstrategystatus(listmodelname[i]);

                    ListViewItem item = listView1.Items[i];

                    //更新列表内容
                    string[] list = status.Split(';');

                    if (list.Length < 1)
                        continue;

                    item.SubItems.Clear();

                    item.Text = list[0];

                    for (int  j= 1; j < list.Length; j++)
                    {
                        item.SubItems.Add(list[j]);
                    }   
                    /*
                    ////将最高价保存起来
                    string strValues = FormMain.modelvalues[listmodelname[i]];
                    string[] s0 = strValues.Split('#');
                    if (s0[3] == "-1")
                    {
                        string s1 = list[10];
                        string[] s2 = s1.Split('(');
                        string highestprice = s2[0];

                        string sNew = s0[0] + "#" + s0[1] + "#" + s0[2] + "#" + highestprice + "#";
                        //更新
                        FormMain.modelvalues[listmodelname[i]] = sNew;

                        FormLoadModel.modelAll[listmodelname[i]] = sNew;

                    }
                    */
                }
            }
        }

        private void 暂停ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string strName = listView1.SelectedItems[0].SubItems[0].Text;      //策略名

            string code = listView1.SelectedItems[0].SubItems[1].Text;

            string modelName = listView1.SelectedItems[0].SubItems[12].Text;                                 //模型名

            manager.pause(modelName);

        }

        private void 激活ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string strName = listView1.SelectedItems[0].SubItems[0].Text;      //策略名

            string code = listView1.SelectedItems[0].SubItems[1].Text;

            string modelName = listView1.SelectedItems[0].SubItems[12].Text;                                //模型名

            manager.run(modelName);

        }

        private void 停止ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            timer1.Enabled = false;

            MessageBoxButtons messButton = MessageBoxButtons.OKCancel;

            DialogResult dr = MessageBox.Show("确定要删除模型?", "对话框", messButton);

            if (dr != DialogResult.OK)
            {
                return;
            }


            string modelName = listView1.SelectedItems[0].SubItems[12].Text;   //模型名

            manager.stop(modelName);

            selModelName = "";

            dataGridView1.DataSource = null;

            int Index = 0;

            if (this.listView1.SelectedItems.Count > 0)//判断listview有被选中项
            {
                Index = this.listView1.SelectedItems[0].Index;//取当前选中项的index,SelectedItems[0]这必须为0

                listView1.Items[Index].Remove();
            }

            listmodelname.Remove(modelName);           //删除列表的值

            FormMain.modelvalues.Remove(modelName);

            manager.listmodelrun.Remove(modelName);

            FormLoadModel.modelAll.Remove(modelName);

            timer1.Enabled = true;
        }



        public void addstrlog(string log)
        {
            if (log == "")
                return;

            string[] info = log.Split('|');
            
            //DateTime tm = DateTime.Now;

            //string time = tm.Date.ToShortDateString() + " " + tm.Hour.ToString() + ":" + tm.Minute.ToString() + ":" + tm.Second.ToString();

            ListViewItem item = listView2.Items.Add(info[0]);

            item.SubItems.Add(info[1]);
        }

        
        private void menu1_Opening(object sender, CancelEventArgs e)
        {
            if (listView1.SelectedItems.Count == 0)
            {
                e.Cancel = true;
            }
        }

        private void listView1_Click(object sender, EventArgs e)
        {

        }

        private void listView1_DoubleClick(object sender, EventArgs e)
        {

            //参数

            if (listView1.SelectedItems.Count == 0)
                return;

            string strName = listView1.SelectedItems[0].SubItems[0].Text;      //策略名

            string code = listView1.SelectedItems[0].SubItems[1].Text;

            string modelName = listView1.SelectedItems[0].SubItems[12].Text;                                 //模型名

            string[] str = manager.getParamsByModelName(strName);

            string[] value = manager.getValueStr(modelName, ""); 


            if (value.Length< 5)
                return;

            //显示在表格里面
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

            string param = "";
            string val = "";

            for (int i = 0; i < str.Length;i++ )
            {
                param = str[i];
                
                if (param != "")                //"交易合约;交易数量;开仓价;"   会多出一行来
                {
                    val = value[i];

                    //不显示这几个参数，在下单管理里面显示
                    if ( (param == "是否追单") || (param == "超时时间") || (param == "账户") )
                        continue;

                    DataRow row = table.NewRow();

                    row[0] = param;

                    row[1] = val;

                    table.Rows.Add(row);
                }

            }

            set.Tables.Add(table);

            dataGridView1.DataSource = table.DefaultView;
            
            //需要保存modelname
            selModelName = modelName;

            //下单控制

            string[] zhuidan = manager.getValueStr(modelName, "是否追单");

            string[] timeout = manager.getValueStr(modelName, "超时时间");

            string[] jiacha = manager.getValueStr(modelName, "价差");

            string[] account = manager.getValueStr(modelName, "账户"); //得到a,b,c,之类的

            string straccount = account[0];

            if (zhuidan[0] == "0")
                radioButton1.Checked = true;
            else if (zhuidan[0] == "1")
                radioButton2.Checked = true;
            else
            {
                radioButton1.Checked = false;
                radioButton2.Checked = false;

            }

            textBox2.Text = timeout[0];

            textBox3.Text = jiacha[0];
            
            //账户
            for (int i = 0; i < checkedListBox1.Items.Count; i++)
            {
                if (straccount.IndexOf(checkedListBox1.Items[i].ToString()) >= 0)
                {
                    checkedListBox1.SetItemChecked(i, true);
                }
                textBox1.Text = straccount;

            }



            //信号记录
            string signalInfo = manager.getSignal(modelName);

            listView3.Items.Clear();

            List<string> listSig = new List<string>(signalInfo.Split('#'));

            foreach (string t in listSig)
            {
                addsiglog(t);
            }

            //日志
            string log = getstrlog(modelName);

            listView2.Items.Clear();

            List<string> list = new List<string>(log.Split('#'));

            foreach (string t in list)
            {
                addstrlog(t);
            }

        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (selModelName == "")
            {
                MessageBox.Show("未选中任何模型");
                return;
            }

            string key ="";

            string value = "";

            //string fundaccount = "";

            //if (fundaccount == "")
            //    fundaccount = "*,";

            for (int i = 0; i < dataGridView1.Rows.Count; i++)
            {
                DataGridViewRow row = dataGridView1.Rows[i];

                if (row.Cells[1] != null && row.Cells[1].Value != null)
                {

                    //value = value + row.Cells[1].Value.ToString() + ";";
                    key = row.Cells[0].Value.ToString();

                    value = row.Cells[1].Value.ToString();

                    manager.setValues(selModelName, key, value);                    

                }
            }

            timer1.Enabled = false; 

            string values = getValues(selModelName,"");/*
            if (FormMain.modelvalues.ContainsKey(selModelName))
            {

                //更新内存里面的值
                string v = FormMain.modelvalues[selModelName];

                string[] v0 = v.Split('#');

                if (v0.Length > 2)
                {

                    FormMain.modelvalues[selModelName] = v0[0] + "#" + v0[1] + "#" + values + "#" + v0[3] + "#";

                    FormLoadModel.modelAll[selModelName] = v0[0] + "#" + v0[1] + "#" + values + "#" + v0[3] + "#";
                    
                }
            
            }*/
            timer1.Enabled = true ;

            selModelName = "";

        }

        private void checkedListBox1_Click(object sender, EventArgs e)
        {
            checkedListBox1_SelectedIndexChanged(sender, e);
        }

        private void checkedListBox1_DoubleClick(object sender, EventArgs e)
        {
            checkedListBox1_SelectedIndexChanged(sender, e);
        }

        private void checkedListBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            
            textBox1.Text = "";

            string tt = textBox1.Text;


            if (checkedListBox1.SelectedIndex==0)                   //单击全选
            {

                if (checkedListBox1.GetItemChecked(0))               //全选是否被选中
                {
                    tt = "";
                    for (int i = 1; i < checkedListBox1.Items.Count; i++)
                    {
                        checkedListBox1.SetItemChecked(i, true);
                        tt = checkedListBox1.Items[i].ToString() + ",";
                    }
                }
                else
                {
                    tt = "";
                    for (int i = 1; i < checkedListBox1.Items.Count; i++)
                    {
                        checkedListBox1.SetItemChecked(i, false);
                    }
                }

            }
            else     //不是单击全选
            {
                int index = checkedListBox1.SelectedIndex;

                bool isAll = true;

                if(checkedListBox1.GetItemChecked(0))
                {
                    if(!checkedListBox1.GetItemChecked(index))
                    {
                        checkedListBox1.SetItemChecked(0, false); 
                    }
                }
                
                for (int i = 1; i < checkedListBox1.Items.Count; i++)
                {
                    if (!checkedListBox1.GetItemChecked(i))
                        isAll = false;

                    if (checkedListBox1.GetItemChecked(i))
                        tt = tt + checkedListBox1.GetItemText(checkedListBox1.Items[i]) + ",";
                    
                }

               checkedListBox1.SetItemChecked(0, isAll);
            }

            textBox1.Text = tt;
            
        }

        public void addFundaccount(string s)
        {
            checkedListBox1.Items.Add(s);
        
        }
        public void clearFundaccount()     //初始化
        {
            checkedListBox1.Items.Clear();
            checkedListBox1.Items.Add("全选");
        }

        private void button2_Click(object sender, EventArgs e)
        {
            
        }


        public void addsiglog(string signal)
        {
            if (signal == "")
                return;

            //DateTime tm = DateTime.Now;

            //string time = tm.Date.ToShortDateString() + " " + tm.Hour.ToString() + ":" + tm.Minute.ToString() + ":" + tm.Second.ToString();

            //ListViewItem item = listView2.Items.Add(time);

            //item.SubItems.Add(log);

            List<string> listSig = new List<string>(signal.Split('|'));

            ListViewItem item = listView3.Items.Add(listSig[0]);

            item.SubItems.Add(listSig[1]);

            item.SubItems.Add(listSig[2]);

            item.SubItems.Add(listSig[3]);

            //List<string>::iterator it;

            //for (int i = 1; i < listSig.get;i++ )
            //{

            //    item.SubItems.Add(listSig[i]);
            //}

        }

        private void button4_Click(object sender, EventArgs e)
        {
            if (selModelName == "")
            {
                MessageBox.Show("未选中任何模型");
                return;
            }

            if (radioButton1.Checked)
            {
                manager.setValues(selModelName, "是否追单", "0");
            }
            else if (radioButton2.Checked)
            {
                manager.setValues(selModelName, "是否追单", "1");
            }

            if(textBox2.Text!="")
                manager.setValues(selModelName, "超时时间", textBox2.Text);

            if(textBox3.Text!="")
                manager.setValues(selModelName, "价差", textBox3.Text);

            if(textBox1.Text!="")
                manager.setValues(selModelName, "账户", textBox1.Text);

            timer1.Enabled = false;

            string values = getValues(selModelName, "");/*
            if (FormMain.modelvalues.ContainsKey(selModelName))
            {

                //更新内存里面的值
                string v = FormMain.modelvalues[selModelName];

                string[] v0 = v.Split('#');

                if (v0.Length > 2)
                {

                    FormMain.modelvalues[selModelName] = v0[0] + "#" + v0[1] + "#" + values + "#" + v0[3] + "#";

                    FormLoadModel.modelAll[selModelName] = v0[0] + "#" + v0[1] + "#" + values + "#" + v0[3] + "#";

                }

            }*/
            timer1.Enabled = true;

        }

        private void button3_Click(object sender, EventArgs e)
        {

        }

        private void FormStrategyRun_FormClosing(object sender, FormClosingEventArgs e)
        {
            MessageBoxButtons messButton = MessageBoxButtons.OKCancel;

            DialogResult dr = MessageBox.Show("确定要关闭策略运行窗口", "对话框", messButton);

            if (dr != DialogResult.OK)
            {
                e.Cancel = true;    //事件悔改
            }
            else
            {
                e.Cancel = false;    //事件悔改
            }


        }

        private void button5_Click(object sender, EventArgs e)
        {

            timer1.Enabled = false;

            if (dataGridView1.Rows.Count <= 2)
            {
                timer1.Enabled = true;
                return;
            }

            if (listView1.SelectedItems.Count == 0)
                return;

            string strName = listView1.SelectedItems[0].SubItems[0].Text;      //策略名
            string modelName = listView1.SelectedItems[0].SubItems[12].Text;  

            string value = "";

            string param = "";

            string newModelName = "-1";

            for (int i = 0; i < dataGridView1.Rows.Count; i++)
            {
                DataGridViewRow row = dataGridView1.Rows[i];

                if (row.Cells[1] != null && row.Cells[1].Value != null  && row.Cells[1].Value.ToString() != "")
                {
                    value = value + row.Cells[1].Value.ToString() + ";";

                }
            }

            //string[] zhuidan = manager.getValueStr(modelName, "是否追单");

            //string[] timeout = manager.getValueStr(modelName, "超时时间");

            //string[] jiacha = manager.getValueStr(modelName, "价差");

            //value = value + ";" + zhuidan[0] + ";" + timeout[0] + ";";

            string[] zhanghu = manager.getValueStr(modelName, "账户");

            value = value + zhanghu[0] + ";";

            param = strName + "#" + value + "#";

            newModelName = addmodel(param);

            if (newModelName!="-1")
                manager.addmodel(newModelName);

            timer1.Enabled = true;

        }

        public void stopAll()
        {

            timer1.Enabled = false;

            for(int i = 0; i < listView1.Items.Count; i++)
            {
                string modelName = listView1.Items[i].SubItems[12].Text;   //模型名

                manager.stop(modelName);
            
            }

            selModelName = "";

            dataGridView1.DataSource = null;

            listView1.Items.Clear();

              
        }


    }
}
