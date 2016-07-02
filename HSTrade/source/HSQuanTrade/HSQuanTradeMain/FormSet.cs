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
using System.Xml;
using System.IO;

namespace HSQuanTradeMain
{
    public struct AccountInfo
    {
        public string account;
        public string pwd;
        public string brokeid;
        public string serverip;
        public string apitype;
    };

    public struct CodeInfo
    {
        public string code;
        public string name;
        public string subtype;
    };

    public partial class FormSet : Form
    {
        public static List<AccountInfo> listAccount = new List<AccountInfo>();               //定义为static

        public List<AccountInfo> newAccount = new List<AccountInfo>();  

        public static List<CodeInfo> listCodeInfo = new List<CodeInfo>();

        public static Dictionary<string, string> listIp = new Dictionary<string, string>();

        public static string port = "";

        public delete_init init = null;

        public FormStrategyRun fsr = null;

        public FormSet()
        {
            InitializeComponent();

        }

        private void FormSet_Load(object sender, EventArgs e)
        {
            //读取配置
            LoadConfig();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (textBox1.Text == "" || textBox2.Text == "" || textBox3.Text == "" || comboBox1.SelectedIndex == -1)
            {
                MessageBox.Show("没有填写完整！");
                return;
            }


            AccountInfo info = new AccountInfo();            

            info.account = textBox1.Text;

            info.pwd = textBox2.Text;

            info.serverip = textBox3.Text;

            info.brokeid = textBox4.Text;

            info.apitype = comboBox1.Items[comboBox1.SelectedIndex].ToString();
            
            listAccount.Add(info);

            //新增加的账户
            newAccount.Add(info);


            InsertList(info);

        }

        public void InsertList(AccountInfo info)
        {
            ListViewItem item = listView1.Items.Add(info.account);

            item.SubItems.Add(info.serverip);

            item.SubItems.Add(info.brokeid);

            //item.SubItems.Add(info.apitype);
        }

        public void InsertCode(CodeInfo code)
        {
            ListViewItem item = listView2.Items.Add(code.code);

            item.SubItems.Add(code.name);

            item.SubItems.Add(code.subtype);


        }

        private void button2_Click(object sender, EventArgs e)
        {
            foreach(ListViewItem item in  listView1.SelectedItems)
            {               
                foreach (AccountInfo info in listAccount)
                {
                    string ip =  item.SubItems[1].Text;
                    if (info.account == item.Text && info.serverip == ip )
                    {
                        listAccount.Remove(info);
                        break;
                    }
                }

                foreach (AccountInfo info in newAccount)
                {
                    string ip = item.SubItems[1].Text;
                    if (info.account == item.Text && info.serverip == ip)
                    {
                        newAccount.Remove(info);
                        break;
                    }
                }
                    
               listView1.Items.Remove(item);

            }
        }

     
        private void button4_Click(object sender, EventArgs e)
        {
            //存文件
            SaveConfig();

            //登录帐号
            LoginAccount();

        }

        private void button5_Click(object sender, EventArgs e)
        {

        }

        public void LoadConfig()
        {

            //开始需要清空list
            listAccount.Clear();

            fsr.clearFundaccount();

            string fileConfig = getfilename();

            XmlDocument doc = new XmlDocument();

            doc.Load(fileConfig);

            XmlNode root = doc.LastChild;

            XmlNodeList list = root.ChildNodes;

            for (int i = 0; i < list.Count; i++)
            {
                XmlNodeList sublist = list[i].ChildNodes;
                
                AccountInfo data = new AccountInfo();

                data.account = sublist[0].InnerText.Trim();

                cbbAccount.Items.Add(data.account);

                //加到checkedlist里面
                fsr.addFundaccount(data.account);

                data.pwd = sublist[1].InnerText.Trim();

                data.serverip = sublist[2].InnerText.Trim();

                data.apitype = sublist[3].InnerText.Trim();

                data.brokeid = sublist[4].InnerText.Trim();

                InsertList(data);

                listAccount.Add(data);
                
            }


            //加载合约

            FormSet.listCodeInfo.Clear();

            listIp.Clear();

            fileConfig = FormSet.getcodefilename();

            doc.Load(fileConfig);

            list = doc.SelectNodes("/ranges/codeinfo");

            for (int i = 0; i < list.Count; i++)
            {

                XmlNodeList sublist = list[i].ChildNodes;

                CodeInfo data = new CodeInfo();

                data.code = sublist[0].InnerText.Trim();

                data.name = sublist[1].InnerText.Trim();

                data.subtype = sublist[2].InnerText.Trim();


                FormSet.listCodeInfo.Add(data);

                InsertCode(data);

            }

            list = doc.SelectNodes("/ranges/ip");

            for (int i = 0; i < list.Count; i++)
            {
                XmlNodeList sublist = list[i].ChildNodes;

                foreach (XmlNode xn in sublist)
                {
                    if (xn.Name == "ufx")
                    {
                        FormSet.listIp["ufx"] = xn.InnerText.Trim();
                    }
                    else if (xn.Name == "ctp")
                    {
                        FormSet.listIp["ctp"] = xn.InnerText.Trim();
                    }

                }


            }

            if (FormSet.listIp.Count == 0)
            {
                FormSet.listIp["ufx"] = "";
                FormSet.listIp["ctp"] = "";
            }

        }

        public void SaveConfig()
        {
            string fileConfig = getfilename();

            XmlDocument doc = new XmlDocument();

            doc.Load(fileConfig);

            XmlNode root = doc.LastChild;

            XmlNodeList list = root.ChildNodes;

            root.RemoveAll();

            for (int i = 0; i < listAccount.Count; i++)
            {
                AccountInfo info = listAccount[i];

                XmlElement elemChild = doc.CreateElement("accountinfo");

                root.AppendChild(elemChild);

                XmlElement elem = doc.CreateElement("account");

                elem.InnerText = info.account;

                elemChild.AppendChild(elem);

                //////////////////////////////////
                elem = doc.CreateElement("pwd");

                elem.InnerText = info.pwd;

                elemChild.AppendChild(elem);

                /////////////////////////////////
                elem = doc.CreateElement("serverip");

                elem.InnerText = info.serverip;
                
                elemChild.AppendChild(elem);


                /////////////////////////////////
                elem = doc.CreateElement("apitype");

                elem.InnerText = info.apitype;

                elemChild.AppendChild(elem);


                /////////////////////////////////
                elem = doc.CreateElement("brokeid");

                elem.InnerText = info.brokeid;

                elemChild.AppendChild(elem);

            }
            doc.Save(fileConfig);
        }

        //保存合约
        public void SaveCodeInfo()
        {
            string fileConfig = getcodefilename();

            XmlDocument doc = new XmlDocument();

            doc.Load(fileConfig);

            XmlNode root = doc.LastChild;

            XmlNodeList list = root.ChildNodes;

            root.RemoveAll();

            for (int i = 0; i < listCodeInfo.Count; i++)
            {
                CodeInfo codeinfo = listCodeInfo[i];

                XmlElement elemChild = doc.CreateElement("codeinfo");

                root.AppendChild(elemChild);

                XmlElement elem = doc.CreateElement("code");

                elem.InnerText = codeinfo.code;

                elemChild.AppendChild(elem);

                //////////////////////////////////
                elem = doc.CreateElement("name");

                elem.InnerText = codeinfo.name;

                elemChild.AppendChild(elem);

                /////////////////////////////////
                elem = doc.CreateElement("subtype");

                elem.InnerText = codeinfo.subtype;

                elemChild.AppendChild(elem);

            }


            //只有一个ip
            XmlElement elemChildip = doc.CreateElement("ip");

            root.AppendChild(elemChildip);

            if (listIp.ContainsKey("ufx"))
            {
                //////////////////////////////////
                XmlElement e_ufx = doc.CreateElement("ufx");

                e_ufx.InnerText = listIp["ufx"];

                elemChildip.AppendChild(e_ufx);
 
            }

            if (listIp.ContainsKey("ctp"))
            {
                //////////////////////////////////
                XmlElement e_ctp = doc.CreateElement("ctp");

                e_ctp.InnerText = listIp["ctp"];

                elemChildip.AppendChild(e_ctp); 
            
            }



            doc.Save(fileConfig);
        
        }

        public static string getfilename()
        {
            string str1 = Process.GetCurrentProcess().MainModule.FileName;

            int nt = str1.LastIndexOf("\\");

            string path = str1.Substring(0, nt);

            if (!File.Exists(path + "\\account.xml"))            
                return "";

            return path + "\\account.xml";
        }

        public static string getConfigPath()
        {
            string str1 = Process.GetCurrentProcess().MainModule.FileName;

            int nt = str1.LastIndexOf("\\");

            string path = str1.Substring(0, nt);

            if (!File.Exists(path + "\\strlog\\Menu.xml"))
                return "";

            return path + "\\strlog\\Menu.xml";
        }

        //获取code文件名
        public static string getcodefilename()
        {
            string str1 = Process.GetCurrentProcess().MainModule.FileName;

            int nt = str1.LastIndexOf("\\");

            string path = str1.Substring(0,nt);

            if (!File.Exists(path + "\\code.xml"))            
                return "";

            return path + "\\code.xml";
        
        }

        public static string GetFilePath(string filename)
        {

            string str1 = Process.GetCurrentProcess().MainModule.FileName;

            int nt = str1.LastIndexOf("\\");

            string path = str1.Substring(0, nt);

            if (!File.Exists(path + "\\" + filename))
                return "";

            return path + "\\" + filename;
        
        }

        private void LoginAccount()
        {

            //foreach (AccountInfo info in listAccount)
            //{
            //    string login = "account#";

            //    login = login + info.account + ";" + info.pwd + ";" + info.serverip + ";" + info.apitype + ";" + info.brokeid + ";#";

            //    init(login);

            //}

            //新增加的才登录
            foreach (AccountInfo info in newAccount)
            {
                string login = "account#";

                login = login + info.account + ";" + info.pwd + ";" + info.serverip + ";" + info.apitype + ";" + info.brokeid + ";#";

                init(login);

                //延迟100ms
                testExcel.timeDelay(100);

            }

            newAccount.Clear();
        }

        //订阅合约
        private void SubCode()
        {
            foreach (CodeInfo info in listCodeInfo)
            {
                string sub = "subcode#";

                sub = sub + info.code + ";"+"FUTURES;"+info.subtype+";#";

                if (FormSet.listIp[info.subtype] != "")
                    init(sub);

                
            }
        
        }

        private void bAdd_Click(object sender, EventArgs e)
        {
            if (tCode.Text == "" || tName.Text == "" || comboBox2.SelectedIndex==-1 || tIp.Text=="")
            {
                MessageBox.Show("没有填写完整！");
                return;
            }

            CodeInfo codeinfo = new CodeInfo();
            codeinfo.code = tCode.Text;
            codeinfo.name = tName.Text;
            codeinfo.subtype = comboBox2.Items[comboBox2.SelectedIndex].ToString();

            //要注意的是如果新加了ip地址进来，必须先建立连接才能订阅合约，否则崩掉
            if (listIp[comboBox2.Text] == "")
            {
                string sub = "newip#";

                sub = sub + comboBox2.Text + "#" + tIp.Text+"#";

                init(sub);
            }
            else if (listIp[comboBox2.Text] != tIp.Text)
            {
                //MessageBox.Show("行情连接地址更新需重启程序！");
                string sub = "newip#";

                sub = sub + comboBox2.Text + "#" + tIp.Text + "#";

                init(sub);
            }

            listIp[comboBox2.Text] = tIp.Text;
            listCodeInfo.Add(codeinfo);
            InsertCode(codeinfo);

            

        }

        private void bDelete_Click(object sender, EventArgs e)
        {
            foreach (ListViewItem item in listView2.SelectedItems)
            {
                foreach (CodeInfo code in listCodeInfo)
                {
                    string name = item.SubItems[1].Text;
                    string c = item.SubItems[0].Text;
                    if (code.code == item.Text /*&& code.name == name*/)
                    {
                        listCodeInfo.Remove(code);
                        break;
                    }
                }

                listView2.Items.Remove(item);

            }
        }

        private void bCancel_Click(object sender, EventArgs e)
        {

        }

        private void bOk_Click(object sender, EventArgs e)
        {
            SaveCodeInfo();

            SubCode();
        }

        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {

            tIp.Text = FormSet.listIp[comboBox2.Text];
            

        }

        private void button3_Click(object sender, EventArgs e)
        {
            string path = GetFilePath("risk.xml");
            if (path == "")
            {
                path = "risk.xml";
            }
           
            MakesureXmlExist(path);

            XmlDocument XmlDoc;

            try 
            {
                XmlDoc = new XmlDocument();
                XmlDoc.Load(path); 
            }
            catch (System.Exception ex)
            {
                Log.WriteLog("risk.xml" + ex.ToString(), 1);
                //xml format is not correct, remake it
                File.Delete(path);
                MakesureXmlExist(path);

                XmlDoc = new XmlDocument();
                XmlDoc.Load(path);
            }

            string margin = tMargin.Text;
            string chedan = tCheDan.Text;
            string pl = tPL.Text;
            string account = cbbAccount.Text;
            string open = "0";
            if(rdOpen.Checked)
                open = "1";


            XmlNode xmlcontext = XmlDoc.SelectSingleNode("risk");
            XmlNodeList xmlChildLst = xmlcontext.ChildNodes;
            //先删除已经存在的
            foreach (XmlNode oXmlNode in xmlChildLst)
            {
                string acc = oXmlNode.Attributes["account"].Value;
                if (string.Empty != acc && 0 == acc.CompareTo(account))
                {
                    xmlcontext.RemoveChild(oXmlNode);
                    break;
                }
            }

            XmlElement riskinfo = XmlDoc.CreateElement("riskinfo");
            riskinfo.SetAttribute("account", account);
            riskinfo.SetAttribute("open", open);
            xmlcontext.AppendChild(riskinfo);

            XmlElement xmlmargin = XmlDoc.CreateElement("margin");
            xmlmargin.InnerText = margin;
            riskinfo.AppendChild(xmlmargin);

            XmlElement xmlwithdraw = XmlDoc.CreateElement("withdraw");
            xmlwithdraw.InnerText = chedan;
            riskinfo.AppendChild(xmlwithdraw);

            XmlElement xmlPL = XmlDoc.CreateElement("PL");
            xmlPL.InnerText = pl;
            riskinfo.AppendChild(xmlPL);

            XmlDoc.Save(path);

            //风控信息
            string info = "risk;";
            info += account + ";" + margin + ";" + chedan + ";" + pl + ";" + open + ";";
            init(info);
         
        }

        protected void MakesureXmlExist(string filepath)
        {
            if (!File.Exists(filepath))
            {
                XmlDocument XmlDoc = new XmlDocument();
                if (null != XmlDoc)
                {
                    XmlDeclaration xmlDec = XmlDoc.CreateXmlDeclaration("1.0", "gb2312", null);
                    XmlDoc.AppendChild(xmlDec);

                    XmlElement xmlRoot = XmlDoc.CreateElement("risk");
                    XmlDoc.AppendChild(xmlRoot);
                    XmlDoc.Save(filepath);
                }

            }
        }

        private void cbbAccount_SelectedIndexChanged(object sender, EventArgs e)
        {
            LoadRiskInfo(cbbAccount.Text);
        }

        private void LoadRiskInfo(string account)
        {
            string path = GetFilePath("risk.xml");

            if (path != "")
            {
                XmlDocument XmlDoc = new XmlDocument();
                XmlDoc.Load(path);

                XmlNode xmlcontext = XmlDoc.SelectSingleNode("risk");
                XmlNodeList xmlChildLst = xmlcontext.ChildNodes;

                foreach (XmlNode oXmlNode in xmlChildLst)
                {
                    string acc = oXmlNode.Attributes["account"].Value;
                    string open = oXmlNode.Attributes["open"].Value;
                    if (string.Empty != open && 0 == open.CompareTo("1"))
                    {
                        rdOpen.Checked = true;
                        rdClose.Checked = false;
                    }
                    else
                    {
                        rdOpen.Checked = false;
                        rdClose.Checked = true;
                    }

                    if (string.Empty != acc && 0 == acc.CompareTo(account))
                    {
                        foreach (XmlNode node in oXmlNode.ChildNodes)
                        {
                            if (0 == node.Name.CompareTo("margin"))
                            {
                                tMargin.Text = node.InnerText;
                            }
                            if (0 == node.Name.CompareTo("withdraw"))
                            {
                                tCheDan.Text = node.InnerText;
                            }
                            if (0 == node.Name.CompareTo("PL"))
                            {
                                tPL.Text = node.InnerText;
                            }                                                                             
                        }

                        break;
                    }
                }


            }

        }

        private void listView1_DoubleClick(object sender, EventArgs e)
        {

            string account = listView1.SelectedItems[0].Text;

            foreach (AccountInfo acc in listAccount)
            {
                if (account == acc.account)
                {
                    textBox1.Text = account;
                    textBox2.Text = acc.pwd;
                    textBox3.Text = acc.serverip;
                    textBox4.Text = acc.brokeid;
                    comboBox1.Text = acc.apitype;
                    button7.Enabled = true;

                    break;
                }
            
            }


        }

        private void button7_Click(object sender, EventArgs e)
        {
            if (textBox1.Text == "" || textBox2.Text == "" || textBox3.Text == "" || comboBox1.SelectedIndex == -1)
            {
                MessageBox.Show("没有填写完整！");         
                return;
            }

            foreach (AccountInfo acc in listAccount)
            {
                if (textBox1.Text == acc.account)
                {
                    listAccount.Remove(acc);
                    break;
                }

            }



            AccountInfo info = new AccountInfo();

            info.account = textBox1.Text;

            info.pwd = textBox2.Text;

            info.serverip = textBox3.Text;

            info.brokeid = textBox4.Text;

            info.apitype = comboBox1.Items[comboBox1.SelectedIndex].ToString();

            listAccount.Add(info);



            ListViewItem item = listView1.SelectedItems[0];

            string a = item.SubItems[0].Text;
            a = item.SubItems[2].Text;
            int s = item.SubItems.Count;

            item.SubItems[0].Text = textBox1.Text;

            item.SubItems[1].Text = textBox3.Text;

            item.SubItems[2].Text = textBox4.Text;

            //item.SubItems[3].Text = comboBox1.Text;


            SaveConfig();

            MessageBox.Show("账户信息修改成功!");

            button7.Enabled = false;
            

        }


    }
}
