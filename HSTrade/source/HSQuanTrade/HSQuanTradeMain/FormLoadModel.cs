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

namespace HSQuanTradeMain
{
    public struct modelinfo 
    {
        public string strName;
        public string code;
        public string amount;
        public string openprice;
        public string zhisunrate;
        public string changerate;
        public string huitiaorate;
        public string iszhuidan;
        public string time;
        public string account;
        public string highestprice;
        
        //....
    }


    public partial class FormLoadModel : Form
    {

        public static List<string> modInfo = new List<string>(); //存放结构体操作，不是很方便，改为字符串 

        public List<string> currModel = new List<string>();

        public static Dictionary<string, string> modelAll = new Dictionary<string, string>();

        public StrategryManager sm = null;

        public delegate_AddRunStrategy loadstrategy = null;

        public delete_init trans = null;

        public FormLoadModel()
        {
            InitializeComponent();
        }

        private void FormLoadModel_Load(object sender, EventArgs e)
        {
            //根据map里面是否有值来判断是否第一次加载
            if (modelAll.Count == 0)
            {
                
            }
            else
            {
                Dictionary<string, string>.ValueCollection valueColAll = modelAll.Values;
                foreach (string info in valueColAll)
                {
                    string[] v = info.Split('#');

                    if (!comboBox1.Items.Contains(v[0]))
                    {
                        comboBox1.Items.Add(v[0]);
                    }
                }
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            foreach (ListViewItem item in listView1.SelectedItems)
            {

                //string model = comboBox1.Text + item.Text;
                string modelOld = item.Text;

                if (modelAll.Keys.Contains(modelOld))
                {
                    string[] v = modelAll[modelOld].Split('#');

                    string info = comboBox1.Text + "#" + v[2] + "#";

                    string modelname = loadstrategy(info);

                    if (modelname == "-1")
                        return;

                    //需要用modelname更新掉原来的值
                    string valueNew1 = modelAll[modelOld];
                    string[] ssnew = valueNew1.Split('#');
                    string valueNew = ssnew[0] + "#" + modelname + "#" + ssnew[2] + "#" + ssnew[3] + "#";
                    modelAll.Remove(modelOld);
                    modelAll.Add(modelname, valueNew);


                    //加载成功需要存到map里面
                    FormMain.modelvalues.Add(modelname, valueNew);

                    sm.addmodel(modelname);

                    //订阅合约
                    string sub = "subcode#";

                    string[] s1 = info.Split('#');

                    string[] s2 = s1[1].Split(';');

                    string code = s2[0];

                    sub = sub + code + ";" + "FUTURES;CTP" + ";#";

                    trans(sub);

                    //save();

                }
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {

            //涨停板股票卖出策略#600570;1;1;0.03;0.05;0.05;0;3;1,;#

            for (int i = 0; i < currModel.Count; i++)
            {
                string[] info = currModel[i].Split('|'); ;

                string modelname = loadstrategy(info[0]);              //创建策略实例

                if (modelname == "-1")
                    return;

                //需要用modelname更新掉原来的值
                string valueNew1 = modelAll[info[1]];
                string[] ssnew = valueNew1.Split('#');
                string valueNew = ssnew[0] + "#" + modelname + "#" + ssnew[2] + "#" + ssnew[3] + "#";
                modelAll.Remove(info[1]);
                modelAll.Add(modelname, valueNew);


                //加载成功需要存到map里面
                FormMain.modelvalues.Add(modelname, valueNew);

                sm.addmodel(modelname);

                string ss = info[0];

                //订阅合约
                string sub = "subcode#";

                string[] s1 = ss.Split('#');

                string[] s2 = s1[1].Split(';');

                string code = s2[0];

                sub = sub + code + ";" + "FUTURES;CTP" + ";#";

                trans(sub);                       //订阅合约

                //save();
            
            }

        }


        private void button3_Click(object sender, EventArgs e)
        {

        }

        public static  void loadmodel()
        {

            string strpath = FormLoadModel.getfilename();

            string info = "";

            StreamReader sr = new StreamReader(strpath, Encoding.Unicode, true);

            //涨停板股票卖出策略#涨停板股票卖出策略1#1;222;222.0000;22.00;22.00;222.00;0;3;1,;#0.0000#

            while (!sr.EndOfStream)
            {

                info = sr.ReadLine();

                if (info != "")
                {

                    string[] v = info.Split('#');

                    if (v.Length < 3)
                        continue;

                    modelAll.Add(v[1], info);    

                }

            }

            sr.Close();             

        }

        public void insertlist(string modelname,string data)
        {

            //涨停板股票卖出策略#600570;1;1;0.03;0.05;0.05;0;3;1,;#

            string []s1 = data.Split(';');

            if (listView1.Columns.Count == s1.Length-1)
            {

                ListViewItem item = listView1.Items.Add(modelname);

                for (int i = 0; i < s1.Length; i++)
                {

                    item.SubItems.Add(s1[i]);

                    
                }

                currModel.Add(comboBox1.Text + "#" + data + "#" + "|" + modelname+"|");
            }

        }


        public static string getfilename()
        {
            string str1 = Process.GetCurrentProcess().MainModule.FileName;

            int nt = str1.LastIndexOf("\\");

            string path = str1.Substring(0, nt);

            return path + "\\model.txt";
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            int nt = comboBox1.SelectedIndex;
            if (nt == -1)
                return;

            listView1.Columns.Clear();

            listView1.Items.Clear();

            currModel.Clear();

            string name = comboBox1.Items[nt].ToString();

            string list = sm.getstrategyparams(name);

            string[] names = list.Split(';');


            //先增加一列模型名称
            ColumnHeader ch1 = new ColumnHeader();

            ch1.Text = "模型名称";   //设置列标题 

            ch1.Width = 90;    //设置列宽度 

            ch1.TextAlign = HorizontalAlignment.Left;   //设置列的对齐方式 

            listView1.Columns.Add(ch1);    //将列头添加到ListView控件。


            for (int i = 0; i < names.Length; i++)
            {
                if (names[i] != "")
                {
                    ColumnHeader ch = new ColumnHeader();

                    ch.Text = names[i];   //设置列标题 

                    ch.Width = 90;    //设置列宽度 

                    ch.TextAlign = HorizontalAlignment.Left;   //设置列的对齐方式 

                    listView1.Columns.Add(ch);    //将列头添加到ListView控件。
                }
            
            }


            //Dictionary<string,string>.ValueCollection valueCol=FormMain.modelvalues.Values;

            Dictionary<string, string>.ValueCollection valueColAll = modelAll.Values;

            foreach (string value in valueColAll)
            {
                string[] v = value.Split('#');

                string strName = v[0];

                if (strName.CompareTo(name) == 0)
                {
                    string values = v[2];

                    insertlist(v[1],values);
                }
            }

            
        }

        public void save()
        {

            string strpath = FormLoadModel.getfilename();

            StreamWriter sw = new StreamWriter(strpath, false, Encoding.Unicode);

            Dictionary<string, string>.ValueCollection valueColAll = modelAll.Values;

            foreach (string value in valueColAll)
            {
                sw.WriteLine(value);
            }


            sw.Flush();

            sw.Close();

        
        }


    }
}
