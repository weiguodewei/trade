using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Xml;
using System.Diagnostics;
using HSQuanTradeMain.MsgDispatch;
using System.Collections;
using HSQuanTradeMain.StrategyResume;
using System.Threading;

namespace HSQuanTradeMain
{
    public delegate string delegate_getallstrategyname();

    public delegate string delegate_getstrategyparams(string name);

    public delegate string delegate_AddRunStrategy(string name);

    public delegate string delegate_SetStrategyParam(string modelname,string paramname,string value);

    public delegate string delegate_GetStrategyStatusInfo(string name); 
 
    public delegate string delete_init(string param);

    public delegate string delete_getLog(string modelname);

    public delegate string delete_getValues(string modelname,string param);

    public delegate string delete_getSignal(string modelname);

    public delegate string delete_stop(string modelname);

    public delegate string delete_run(string modelname);

    public delegate string delete_pause(string modelname);

    public delegate bool delegate_GetBackTestStatus();

    // BSTR StragegyBackTest(BSTR name, BSTR paramlist, LONG time1, LONG time2, BSTR coderanger);
    public delegate string delegate_StragegyBackTest(string sName, string sParamList, int lTime1, int lTime2, string sCodeRange, string sCycle);

    public delegate string delegate_GetStrategyContext(string sStrategyId);
    public delegate string delegate_SetStrategyContext(string sStrategyId, string sContext);

    /// <summary>
    /// 插入一条记录到模型运行列表
    /// </summary>
    /// <param name="info"></param>
    public delegate void   delegate_insertrecord(string modelname,string info);


    public partial class FormMain : Form
    {        
        public StrategryManager stragegymanager = new StrategryManager();

        public FormStrategyRun formmodelrun = new FormStrategyRun();
        protected FormStragegyBackTest m_oFrmStrategyBackTest = null;
        FormLog logForm = new FormLog();
        public DataManager DM = new DataManager();

        List<FormOption> listOptForm = new List<FormOption>();

        //moldelname----srtname  modelname  value  other
        public static Dictionary<string, string> modelvalues = new Dictionary<string, string>();

        bool bReLogin = false;

        public FormMain()
        {
            InitializeComponent();
        }

    
        private void Form1_Load(object sender, EventArgs e)
        {

            //加载配置
            if (LoadAccountConfig() != "ok")
                return;

            DeleteConFile();

            logForm.MdiParent = this;


            logForm.Show();

#if DEBUG
#else
            formmodelrun.MdiParent = this;

            formmodelrun.Show();

            formmodelrun.getstrlog = new delete_getLog(axHSQuanTrade1.GetStragyLogInfo);    //获取策略日志

            formmodelrun.getValues = new delete_getValues(axHSQuanTrade1.GetParamValue);

            formmodelrun.addmodel = new delegate_AddRunStrategy(axHSQuanTrade1.AddRunStrategy);
#endif

            //if()
            axHSQuanTrade1.BackTestNotify +=axHSQuanTrade1_BackTestNotify;
            axHSQuanTrade1.TradeNotify +=axHSQuanTrade1_TradeNotify;
            axHSQuanTrade1.Init("system#" + FormSet.listIp["ufx"] + "#" + FormSet.listIp["ctp"] + "#" + FormSet.port + "#");            
            
            stragegymanager.getstrategystatus = new delegate_GetStrategyStatusInfo(axHSQuanTrade1.GetStrategyStatusInfo);            

            stragegymanager.insertrecord = new delegate_insertrecord(formmodelrun.insertrecord);

            stragegymanager.getstrategyparams = new delegate_getstrategyparams(axHSQuanTrade1.GetStrategyParamInfo);

            stragegymanager.getValues = new delete_getValues(axHSQuanTrade1.GetParamValue);

            stragegymanager.setValues = new delegate_SetStrategyParam(axHSQuanTrade1.SetStrategyParam);

            stragegymanager.getSignal = new delete_getSignal(axHSQuanTrade1.GetStragyTradeSignalList);

            stragegymanager.stop = new delete_stop(axHSQuanTrade1.StopStrategy);

            stragegymanager.run = new delete_run(axHSQuanTrade1.RunStrategy);

            stragegymanager.pause = new delete_pause(axHSQuanTrade1.PauseStrategy);

            formmodelrun.manager = stragegymanager;


            //登录和行情订阅
            login_sub();


            //菜单是否显示
            LoadViewConfig();

            //加载风控信息
            LoadRiskConfig("risk.xml");


           
            
            //DM.Init(this.RegisterOcxMsgChannel);

            //加载模型
            //FormLoadModel.loadmodel();

#if KGRAPH_DEBUG
            KGraph.KGraphTestFrm frmKGraph = new KGraph.KGraphTestFrm();
            frmKGraph.MdiParent = this;
            frmKGraph.Show();
#endif
           
        }


        public string LoadAccountConfig()
        {
            
            //开始需要清空list
            FormSet.listAccount.Clear();

            string fileConfig = FormSet.getfilename();

            if (fileConfig == "")
            {
                MessageBox.Show("account.xml配置文件不存在,无法登录！");
                return "";
            }

            XmlDocument doc = new XmlDocument();

            doc.Load(fileConfig);

            XmlNode root = doc.LastChild;

            XmlNodeList list = root.ChildNodes;

            for (int i = 0; i < list.Count; i++)
            {
                XmlNodeList sublist = list[i].ChildNodes;

                AccountInfo data = new AccountInfo();

                data.account = sublist[0].InnerText.Trim();

                data.pwd = sublist[1].InnerText.Trim();

                data.serverip = sublist[2].InnerText.Trim();

                data.apitype = sublist[3].InnerText.Trim();

                data.brokeid = sublist[4].InnerText.Trim();

                FormSet.listAccount.Add(data);

            }


            //加载合约

            FormSet.listCodeInfo.Clear();

            FormSet.listIp.Clear();

            fileConfig = FormSet.getcodefilename();

            if (fileConfig == "")
            {
                MessageBox.Show("code.xml配置文件不存在，无法连接行情！");
                return "";
            }

            doc.Load(fileConfig);


            list = doc.SelectNodes("/ranges/codeinfo");

            //list = doc.SelectSingleNode("/ranges").ChildNodes;

            for (int i = 0; i < list.Count; i++)
            {
                        
                XmlNodeList sublist = list[i].ChildNodes;

                CodeInfo data = new CodeInfo();

                string s = sublist[0].Name;

                data.code = sublist[0].InnerText.Trim();

                data.name = sublist[1].InnerText.Trim();

                data.subtype = sublist[2].InnerText.Trim();


                FormSet.listCodeInfo.Add(data);

            }

            list = doc.SelectNodes("/ranges/ip");

            for (int i = 0; i < list.Count; i++)
            {
                XmlNodeList sublist = list[i].ChildNodes;

                foreach(XmlNode xn in sublist)
                {
                    if(xn.Name=="ufx")
                    {
                        FormSet.listIp["ufx"] = xn.InnerText.Trim();
                    }
                    else if(xn.Name=="ctp")
                    {
                        FormSet.listIp["ctp"] = xn.InnerText.Trim();
                    }
                
                }


            }

            list = doc.SelectNodes("/ranges/port");

            for (int i = 0; i < list.Count; i++)
            {
                XmlNodeList sublist = list[i].ChildNodes;

                FormSet.port = sublist[0].InnerText.Trim();


            }

            if (FormSet.listIp.Count == 0)
            {
                FormSet.listIp["ufx"] = "";
                FormSet.listIp["ctp"] = "";
                MessageBox.Show("尚未配置行情地址！");
            }

            return "ok";
        
        }

        private void login_sub()
        {
            if (FormSet.listAccount.Count == 0)
            {
                MessageBox.Show("尚未添加用户,未登录状态！");

            }

            /*
            foreach (AccountInfo info in FormSet.listAccount)
            {
                string login = "account#";

                login = login + info.account + ";" + info.pwd + ";" + info.serverip + ";" + info.apitype + ";" + info.brokeid + ";#";

                axHSQuanTrade1.Init(login);

            }
            */

            foreach (CodeInfo info in FormSet.listCodeInfo)
            {
                string sub = "subcode#";

                sub = sub + info.code + ";" + "FUTURES;" + info.subtype + ";#";

                if (FormSet.listIp[info.subtype]!="")
                    axHSQuanTrade1.Init(sub);
            }

        
        }

        private void 策略运行窗口ToolStripMenuItem_Click(object sender, EventArgs e)
        {
           
        }

        private void 新建模型运行ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            formmodelrun.timer1.Enabled = false; 

            FormAddModel model = new FormAddModel();

            model.getname = new delegate_getallstrategyname(GetAllStrategy);

            model.getparams = new delegate_getstrategyparams(axHSQuanTrade1.GetStrategyParamInfo);

            model.setstrategyparam = new delegate_SetStrategyParam(axHSQuanTrade1.SetStrategyParam);

            model.addrunstrategy = new delegate_AddRunStrategy(axHSQuanTrade1.AddRunStrategy);

            model.addmodelinit = new delete_init(axHSQuanTrade1.Init);

            model.getstrparamvalue = new delete_getValues(axHSQuanTrade1.GetParamValue);

            model.te = new testExcel();

            model.te.init = new delete_init(axHSQuanTrade1.Init);

            if (model.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                if ( model.modelname != "-1")
                {
                    stragegymanager.addmodel(model.modelname);

                }
            }

            formmodelrun.timer1.Enabled = true; 
        }

        public string GetAllStrategy()
        {
            return axHSQuanTrade1.GetAllStrategy(0, "");
        }

        private void 设置ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            formmodelrun.timer1.Enabled = false; 

            FormSet set = new FormSet();

            set.init = new delete_init(axHSQuanTrade1.Init);

            set.fsr = formmodelrun;

            if (set.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {

            }

            formmodelrun.timer1.Enabled = true; 
        }

        private void 平铺ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            LayoutMdi(MdiLayout.TileHorizontal);
        }

        private void 层叠ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            LayoutMdi(MdiLayout.Cascade);
        }

        public void showlog(string msg)
        {
            logForm.addlog(msg);
        }

        private void axHSQuanTrade1_TradeNotify(object sender, AxHSQuanTradeLib._DHSQuanTradeEvents_TradeNotifyEvent e)
        {
            if (0 == e.pamam1)
            {
                showlog(e.param3);
            }
            else if (2 == e.pamam1)
            {
                string sFileName = e.param3;
                FormBackTestResult frmBackResult = new FormBackTestResult();
                frmBackResult.MdiParent = this;
                frmBackResult.ResultFile = sFileName;
                frmBackResult.ResultType = e.param2;
                if (0 == e.param2)
                {
                    frmBackResult.Text = "回测记录表";
                }
                else if (1 == e.param2)
                {
                    frmBackResult.Text = "回测权益表";
                }

                frmBackResult.Show();
            }
            else if (8 == e.pamam1)
            {
                this.timer1.Enabled = true;
            }
            else
            {
                if (4 == e.pamam1)
                {
                    showlog(e.param3);
                    DM.setOptlist(e.param3);
                }
                System.Collections.IDictionaryEnumerator itDispath = m_hsMsgDispatch.GetEnumerator();
                while (itDispath.MoveNext())
                {
                    FormMsgChannelDesc oDesc = (FormMsgChannelDesc)itDispath.Value;
                    ITradeDispatch oForm = oDesc.TradeFormInt;
                    if (null != oForm)
                    {

                        if (e.pamam1 < (int)TradeOcxMsgType.E_OCX_UNKNOWN_MSG && e.pamam1 >= (int)TradeOcxMsgType.E_OCX_LOG_MSG)
                        {
                            if (TradeOcxMsgAuthType.E_OCX_PUBLIC_MSG == oDesc.MsgAuth[e.pamam1])
                            {
                                oForm.DispatchMessage(e.pamam1, e.param2, e.param3, e.param4);
                            }
                            else if (e.param4 != null && oForm.GetModuleId().Equals(e.param4))
                            {
                                oForm.DispatchMessage(e.pamam1, e.param2, e.param3, e.param4);
                            }

                        }
                        else //unkown message type dispath directly
                        {
                            oForm.DispatchMessage(e.pamam1, e.param2, e.param3, e.param4);
                        }

                    }

                }
            }
            

/*
            switch (e.pamam1)
            {
                case 0:
                    {
                        showlog(e.param3);
                    }
                    
                    break;
                case 1:
                    {
                        formmodelrun.addstrlog(e.param3);
                    }
                    
                    break;
                case 2://策略统计结果
                    {
                        string sFileName = e.param3;
                        FormBackTestResult frmBackResult = new FormBackTestResult();
                        frmBackResult.MdiParent = this;
                        frmBackResult.ResultFile = sFileName;
                        frmBackResult.ResultType = e.param2;
                        if (0 == e.param2)
                        {
                            frmBackResult.Text = "回测记录表";
                        }
                        else if (1 == e.param2)
                        {
                            frmBackResult.Text = "回测权益表";
                        }

                        frmBackResult.Show();
                    }
                    break;
                case 3://行情
                    {

                        for (int i = 0; i < listOptForm.Count; i++)
                        {
                            listOptForm[i].Update(e.param3);
                        
                        }
                    //showlog(e.param3);
                    
                    }
                    break;
                case 4://功能请求返回
                    {
                        showlog(e.param3);
                        DM.setOptlist(e.param3);
                    }
                    break;

            }
 * 
 */

        }

        private void 加载模型ToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            formmodelrun.timer1.Enabled = false;           

            FormLoadModel lm = new FormLoadModel();

            if(lm.sm==null)
                lm.sm = stragegymanager;

            lm.loadstrategy = new delegate_AddRunStrategy(axHSQuanTrade1.AddRunStrategy);

            lm.trans = new delete_init(axHSQuanTrade1.Init);

            if (lm.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {

            }



            formmodelrun.timer1.Enabled = true;
        }

        private void FormMain_FormClosing(object sender, FormClosingEventArgs e)
        {

            formmodelrun.timer1.Enabled = false;

            //string strpath = FormLoadModel.getfilename();

            //StreamWriter sw = new StreamWriter(strpath, false, Encoding.Unicode);

            //Dictionary<string, string>.ValueCollection valueColAll = FormLoadModel.modelAll.Values;

            //foreach (string value in valueColAll)
            //{
            //    sw.WriteLine(value);
            //}


            //sw.Flush();

            //sw.Close();

            //这里来个循环删除pt
            //formmodelrun.stopAll();
            
            axHSQuanTrade1.Exit();  


        }

        private void button1_Click(object sender, EventArgs e)
        {
            //axHSQuanTrade1.Init("order,600570,100,0,0,70,0,0,5,10000013,");

            testorder order = new testorder();

            order.init = new delete_init(axHSQuanTrade1.Init);

            order.Show();

        }

        private void button2_Click(object sender, EventArgs e)
        {
            testExcel te = new testExcel();

            te.init = new delete_init(axHSQuanTrade1.Init);

            te.Show();

        }


        private void 模型测试ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (null == m_oFrmStrategyBackTest)
            {

                m_oFrmStrategyBackTest = new FormStragegyBackTest();
                m_oFrmStrategyBackTest.getname = new delegate_getallstrategyname(GetAllStrategy);

                m_oFrmStrategyBackTest.getparams = new delegate_getstrategyparams(axHSQuanTrade1.GetStrategyParamInfo);

                m_oFrmStrategyBackTest.stragegyBackTest = new delegate_StragegyBackTest(axHSQuanTrade1.StragegyBackTest);

                m_oFrmStrategyBackTest.getBackTestStatus = new delegate_GetBackTestStatus(this.GetBackTestStatus);

            }

                
            m_oFrmStrategyBackTest.ShowDialog();
        

         
        }
		
		
        private void toolStripMenuItem1_Click(object sender, EventArgs e)
        {
            FormLogin LgForm = new FormLogin();
            
            LgForm.InitLogin = new delete_init(axHSQuanTrade1.Init);

            LgForm.Show();

        }

        private void testorder_Click(object sender, EventArgs e)
        {
            testorder order = new testorder();

            order.init = new delete_init(axHSQuanTrade1.Init);

            order.Show();
        }

        private void testExcel_Click(object sender, EventArgs e)
        {
            testExcel te = new testExcel();

            te.init = new delete_init(axHSQuanTrade1.Init);

            te.Show();
        }

        private void axHSQuanTrade1_BackTestNotify(object sender, AxHSQuanTradeLib._DHSQuanTradeEvents_BackTestNotifyEvent e)
        {
            if (null != m_oFrmStrategyBackTest)
            {
                m_oFrmStrategyBackTest.UpdateTestProgress(e.sCode, e.lParam1, e.lParam2, e.lParam3, e.lParam4);
            }
        }


        public bool GetBackTestStatus()
        {
            bool bRunning = true;
            if (0 == axHSQuanTrade1.BackTestStatus)
            {
                bRunning = false;
            }
            return bRunning;
        }

        public void DeleteConFile()
        {
            string str1 = Process.GetCurrentProcess().MainModule.FileName;

            int nt = str1.LastIndexOf("\\");

            string path = str1.Substring(0, nt);

            if (File.Exists(path + "\\Private.con")) 
            {
                File.Delete(path + "\\Private.con");
            }

        }

        private void 监听ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            
        }

        private void 证券策略化交易ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FormStrategyAutoOrder frmAutoOrder = new FormStrategyAutoOrder();
            //frmAutoOrder.delegate_RegisterOcxMsgChannel = new Delegate_RegisterOcxMsgChannel(this.RegisterOcxMsgChannel);
            frmAutoOrder.delegate_Init = new delete_init(this.axHSQuanTrade1.Init);
            frmAutoOrder.delegate_GetParams = new delegate_getstrategyparams(this.axHSQuanTrade1.GetStrategyParamInfo);
            frmAutoOrder.delegate_GetAllStrategyName = new delegate_getallstrategyname(GetAllStrategy);
            frmAutoOrder.delegate_AddrunStrategy = new delegate_AddRunStrategy(axHSQuanTrade1.AddRunStrategy);
            frmAutoOrder.del_SetStrategyParam = new delegate_SetStrategyParam(axHSQuanTrade1.SetStrategyParam);
            frmAutoOrder.del_GetStrategyParam = new delete_getValues(axHSQuanTrade1.GetParamValue);
            frmAutoOrder.m_oStrategyManager = this.stragegymanager;
            frmAutoOrder.del_GetStrategyContext = new delegate_GetStrategyContext(axHSQuanTrade1.GetStrategyContext);
            frmAutoOrder.del_StopStrategy = new delete_stop(axHSQuanTrade1.StopStrategy);
            frmAutoOrder.Init(RegisterOcxMsgChannel);
            frmAutoOrder.MdiParent = this;
            frmAutoOrder.Show();
        }

        Hashtable m_hsMsgDispatch = new Hashtable();
        protected void RegisterOcxMsgChannel(FormMsgChannelDesc oMsgInfo)
        {
            string sModuleId = oMsgInfo.TradeFormInt.GetModuleId();
            m_hsMsgDispatch[sModuleId] = oMsgInfo;
        }

        private void ResumeArbitrageForm()
        {
            FormStrategyResume frmResume = new FormStrategyResume();

            if (frmResume.HasResumeStrategy("商品套利策略"))
            {
                frmResume.delegate_GetAllStrategyName = new delegate_getallstrategyname(GetAllStrategy);

                List<StrategyContext> arContext = frmResume.GetStrategyContext("商品套利策略");
                foreach (StrategyContext oStrategy in arContext)
                {
                    FormArbitrage frmArbitrage = new FormArbitrage();
                    frmArbitrage.delegate_Init = new delete_init(this.axHSQuanTrade1.Init);
                    frmArbitrage.delegate_GetParams = new delegate_getstrategyparams(this.axHSQuanTrade1.GetStrategyParamInfo);
                    frmArbitrage.delegate_GetAllStrategyName = new delegate_getallstrategyname(GetAllStrategy);
                    frmArbitrage.delegate_AddrunStrategy = new delegate_AddRunStrategy(axHSQuanTrade1.AddRunStrategy);
                    frmArbitrage.del_SetStrategyParam = new delegate_SetStrategyParam(axHSQuanTrade1.SetStrategyParam);
                    frmArbitrage.del_GetStrategyParam = new delete_getValues(axHSQuanTrade1.GetParamValue);
                    frmArbitrage.m_oStrategyManager = this.stragegymanager;
                    frmArbitrage.del_GetStrategyContext = new delegate_GetStrategyContext(axHSQuanTrade1.GetStrategyContext);
                    frmArbitrage.del_SetStrategyContext = new delegate_SetStrategyContext(axHSQuanTrade1.SetStrategyContext);
                    frmArbitrage.del_StopStrategy = new delete_stop(axHSQuanTrade1.StopStrategy);
                    frmArbitrage.Init(RegisterOcxMsgChannel);
                    frmArbitrage.MdiParent = this;

                    frmArbitrage.ResumeStrategy(oStrategy);
                    frmArbitrage.Show();
                }


               
            }
        }

        private void 套利策略ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FormArbitrage frmArbitrage = new FormArbitrage();
            frmArbitrage.delegate_Init = new delete_init(this.axHSQuanTrade1.Init);
            frmArbitrage.delegate_GetParams = new delegate_getstrategyparams(this.axHSQuanTrade1.GetStrategyParamInfo);
            frmArbitrage.delegate_GetAllStrategyName = new delegate_getallstrategyname(GetAllStrategy);
            frmArbitrage.delegate_AddrunStrategy = new delegate_AddRunStrategy(axHSQuanTrade1.AddRunStrategy);
            frmArbitrage.del_SetStrategyParam = new delegate_SetStrategyParam(axHSQuanTrade1.SetStrategyParam);
            frmArbitrage.del_GetStrategyParam = new delete_getValues(axHSQuanTrade1.GetParamValue);
            frmArbitrage.m_oStrategyManager = this.stragegymanager;
            frmArbitrage.del_GetStrategyContext = new delegate_GetStrategyContext(axHSQuanTrade1.GetStrategyContext);
            frmArbitrage.del_SetStrategyContext = new delegate_SetStrategyContext(axHSQuanTrade1.SetStrategyContext);
            frmArbitrage.del_StopStrategy = new delete_stop(axHSQuanTrade1.StopStrategy);
            frmArbitrage.Init(RegisterOcxMsgChannel);
            frmArbitrage.MdiParent = this;

            //FormStrategyResume frmResume = new FormStrategyResume();

            //if (frmResume.HasResumeStrategy("商品套利策略"))
            //{
            //    frmResume.delegate_GetAllStrategyName = new delegate_getallstrategyname(GetAllStrategy);
            //    if (DialogResult.Yes == frmResume.ShowDialog())
            //    {
            //         StrategyContext oStrategy = null;
            //         if (frmResume.GetResumeStrategyCase(frmResume.ResumeStrategyId, ref oStrategy))
            //         {
            //             frmArbitrage.ResumeStrategy(oStrategy);
            //         }
            //    }
            //}

            frmArbitrage.Show();
        }

        private void 执行选股ToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void 股票期权套利监控ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FormOption opt = new FormOption();

            opt.MdiParent = this;

            opt.init = new delete_init(axHSQuanTrade1.Init);

            //opt.delegate_RegisterOcxMsgChannel = new Delegate_RegisterOcxMsgChannel(this.RegisterOcxMsgChannel);
            opt.Init(RegisterOcxMsgChannel);

            opt.Show();

            listOptForm.Add(opt);
        }

        private void LoadViewConfig()
        {
            List<ToolStripMenuItem> menus = new List<ToolStripMenuItem>();
            menus.Add(SysMS);
            menus.Add(TradeMS);
            menus.Add(FormMS);
            menus.Add(HelpMS);

            string islogin = "0"; 

            string menuitem = "";
            try
            {
                string filepath = FormSet.GetFilePath("Menu.xml");
                if (filepath == "")
                    return;
                XmlDocument XmlDoc = new XmlDocument();
                XmlDoc.Load(filepath);

                XmlNode xmlContext = XmlDoc.SelectSingleNode("Menu");
                XmlNodeList xmlChildLst = xmlContext.ChildNodes;

                foreach (XmlNode oXmlNode in xmlChildLst)
                {
                    //visible
                    if (oXmlNode.Name == "visible")
                    {
                        XmlNodeList items = oXmlNode.ChildNodes;
                        foreach (XmlNode item in items)
                        {
                            if(item.Name == "SysMS")
                            {
                                islogin = item.Attributes["login"].Value;
                            }
                            menuitem += item.InnerText;
                        }
                    }
             
                }


            }
            catch (System.Exception ex)
            {
                Log.WriteLog("LoadViewConfig" + ex.ToString() ,1);
            }

            //显示哪些菜单
            for(int i=0; i< menus.Count;i++)
            {
                foreach (ToolStripItem item in menus[i].DropDownItems)
                {
                    if (menuitem.IndexOf(item.Name) >= 0)
                    {
                        item.Visible = true;
                    }
                    else
                    {
                        item.Visible = false;
                    }

                }
            }

            //是否立即登录
            if (islogin=="1")
            {
                foreach (AccountInfo info in FormSet.listAccount)
                {
                    string login = "account#";

                    login = login + info.account + ";" + info.pwd + ";" + info.serverip + ";" + info.apitype + ";" + info.brokeid + ";#";

                    axHSQuanTrade1.Init(login);

                    //延迟100ms
                    testExcel.timeDelay(100);

                }
            }
        
        }


        //加载风控配置
        private void LoadRiskConfig(string filename)
        {

            string path = FormSet.GetFilePath(filename);
            if (path == "")
                return;
            try
            {
                XmlDocument XmlDoc = new XmlDocument();
                XmlDoc.Load(path);

                XmlNode xmlContext = XmlDoc.SelectSingleNode("risk");
                XmlNodeList xmlChildLst = xmlContext.ChildNodes;

                string account = "";
                string open = "";
                string margin = "";
                string withdraw = "";
                string pl = "";
                foreach (XmlNode oXmlNode in xmlChildLst)
                {
                    //riskinfo
                    if (oXmlNode.Name == "riskinfo")
                    {
                        account = oXmlNode.Attributes["account"].Value;
                        open = oXmlNode.Attributes["open"].Value;
                        XmlNodeList info = oXmlNode.ChildNodes;
                        margin = info[0].InnerText.Trim();
                        withdraw = info[1].InnerText.Trim();
                        pl = info[2].InnerText.Trim();

                        //风控信息
                        string riskinfo = "risk;";
                        riskinfo += account + ";" + margin + ";" + withdraw + ";" + pl + ";" + open + ";";
                        axHSQuanTrade1.Init(riskinfo);

                    }

                }


            }
            catch (System.Exception ex)
            {
                Log.WriteLog("LoadRiskConfig" + ex.ToString(), 1);
            }
        
        }
       		
		
 		private void timer1_Tick(object sender, EventArgs e)
        {
            this.timer1.Enabled = false;
            ResumeArbitrageForm();
		}

        private void timerLogin2_Tick(object sender, EventArgs e)
        {
            int h = DateTime.Now.Hour;
            int m = DateTime.Now.Minute;
            int s = DateTime.Now.Second;

            //reset
            if (h == 10 && m == 30)
            {
                bReLogin = false;
            }

            if (h == 22 && m == 30)
            {
                bReLogin = false;
            }


            if (h == 8 && m == 40 && !bReLogin)
            {
                string loginfo = "logout";

                axHSQuanTrade1.Init(loginfo);

                testExcel.timeDelay(5000);

                loginfo = "login";

                axHSQuanTrade1.Init(loginfo);

                bReLogin = true;
            }

            if (h == 20 && m == 40 && !bReLogin)
            {
                string loginfo = "logout";

                axHSQuanTrade1.Init(loginfo);

                testExcel.timeDelay(5000);

                loginfo = "login";

                axHSQuanTrade1.Init(loginfo);

                bReLogin = true;
            }

            
        }
       
    }
}
