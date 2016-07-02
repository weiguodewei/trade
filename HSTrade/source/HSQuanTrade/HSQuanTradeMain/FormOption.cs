using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Media;
using HSQuanTradeMain.MsgDispatch;
using System.Xml;
using System.IO;
using System.Diagnostics;


namespace HSQuanTradeMain
{
    public partial class FormOption : TradeFormBase
    {
        public class RZRQ
        {
            public double RZ;
            public double RQ;
        };

        //记录上一次融资融券信息
        public class LastRZRQ
        {
            public double price_rz;    //最小融资值对应执行价
            public double rz;          //融资价格
            public double price_rq;    //最大融券值对应执行价
            public double rq;          //融券价格
        };


        LastRZRQ preRERQ;

        public DataManager DM;

        public delete_init init = null;
    
        //当前月份所有期权合约
        //认购   执行价  期权信息
        Dictionary<double, OptData> tempMap1 = new Dictionary<double, OptData>();
        //认沽
        Dictionary<double, OptData> tempMap2 = new Dictionary<double, OptData>();

        //当面页面存放更新合约的位置信息
        // code  对应list位置
        Dictionary<string, int> tempMap3 = new Dictionary<string, int>();

        //缓存订阅的行情
        Dictionary<string,OptTickData> priceMap = new Dictionary<string,OptTickData>();

        //缓存每一行的融资融券数据,执行价最为key值
        Dictionary<double, RZRQ> RzRqMap = new Dictionary<double, RZRQ>();
        
        //上一次播放时间
        double lastPlay = 0;

        System.DateTime startTime = TimeZone.CurrentTimeZone.ToLocalTime(new System.DateTime(1970, 1, 1));

        //已经交易的次数
        int tradeTime = 0;
        //报单组号
        int grpId = 0;

        //上一次委托时间，避免连续多次短时间开仓
        double lastEntrust = 0;

        string m_sFilePath = System.Windows.Forms.Application.StartupPath + "\\strlog\\ETF_trade.xml";

        public FormOption()
        {
            InitializeComponent();
            GenerateGuid("FormOption");
            DM = new DataManager();
            tradeTime = 0;
            grpId = 0;

        }


        public void QueryOptCode()
        {

            string orderinfo = "REQ#queryopt";

            if (DataManager.listOpt.Count > 0)
            {

            }
            else
            {
                init(orderinfo);
            }
        }

        private void FormOption_Load(object sender, EventArgs e)
        {
            
            QueryOptCode();

            //订阅50ETF
            string sub = "subcode#" + "510050" + ";" + "STOCK;UFX;" + "#";
            init(sub);

            Init();

        }

        private void comboBox1_DropDown(object sender, EventArgs e)
        {
            //if (combOpt.Items.Count == 0)
            {
                for (int i = 0; i < DataManager.listOpt.Count; i++)
                {
                    OptData opt = DataManager.listOpt[i];

                    if ( !combOpt.Items.Contains(opt.iMonth))
                    {
                        combOpt.Items.Add(opt.iMonth);
                  
                    }
                }
            
            }
        }

        private void combOpt_SelectedIndexChanged(object sender, EventArgs e)
        {
            //先取消订阅
            CancelSubCode();

            tempMap1.Clear();

            tempMap2.Clear();

            tempMap3.Clear();

            RzRqMap.Clear();

            lvOpt.Items.Clear();

            for (int i = 0; i < DataManager.listOpt.Count; i++ )
            {
                OptData opt = DataManager.listOpt[i];

                if (opt.iMonth != Convert.ToInt32(combOpt.SelectedItem.ToString()) )
                    continue;

                if ( opt.iType == 0 && !tempMap1.ContainsKey(opt.dPrice) )
                {
                    tempMap1.Add(opt.dPrice, opt);
                }
                else if ( opt.iType == 1 && !tempMap2.ContainsKey(opt.dPrice) )
                {
                    tempMap2.Add(opt.dPrice, opt);
                }

            }

            int j = 0;

            lvOpt.BeginUpdate();

            foreach (double key in tempMap1.Keys)
            {
                if (tempMap2.ContainsKey(key))
                {

                    ListViewItem item = new ListViewItem();

                    item.Text = "0";

                    item.SubItems.Add("0");

                    item.SubItems.Add("0");

                    item.SubItems.Add("0");

                    item.SubItems.Add(tempMap1[key].sCode);

                    item.SubItems.Add(Convert.ToString(key));
             
                    item.SubItems.Add(tempMap2[key].sCode);

                    item.SubItems.Add("0");

                    item.SubItems.Add("0");

                    item.SubItems.Add("0");

                    item.SubItems.Add("0");

                    item.SubItems.Add("0");

                    item.SubItems.Add("0");

                    item.SubItems.Add("0");

                    item.SubItems.Add("0");

                    lvOpt.Items.Add(item);

                    tempMap3.Add(tempMap1[key].sCode,j);

                    tempMap3.Add(tempMap2[key].sCode, j);

                    j++;

                }

            }

            lvOpt.EndUpdate();

            SubCode();

            Log.WriteLog("开始监控" + combOpt.SelectedItem.ToString() + "月...");

            lsthold.Items.Clear();

            //加载委托数据
            LoadOrder(combOpt.SelectedItem.ToString());

            //Notice4();

        }


        public void SubCode()
        {

            foreach (OptData value in tempMap1.Values)
            {

                //订阅行情
                string sub = "subcode#" + value.sCode + ";" + "STOCK;UFX;" + "#";
                init(sub);
            }

            foreach (OptData value in tempMap2.Values)
            {
                //订阅行情
                string sub = "subcode#" + value.sCode + ";" + "STOCK;UFX;" + "#";
                init(sub);
            }

        }

        //取消订阅
        public void CancelSubCode()
        {
            foreach (OptData value in tempMap1.Values)
            {

                //取消订阅
                string sub = "unsub#" + value.sCode + ";" + "STOCK;UFX;" + "#";
                init(sub);
            }

            foreach (OptData value in tempMap2.Values)
            {
                //取消订阅
                string sub = "unsub#" + value.sCode + ";" + "STOCK;UFX;" + "#";
                init(sub);
            }
        
        }

        //数据格式:"TICK#合约;最新价;买一价;买一量;卖一价;卖一量"
        public void Update(string tickinfo)
        {
            int h = DateTime.Now.Hour;
            int min = DateTime.Now.Minute;
            int second = DateTime.Now.Second;

            if ((h == 9) && (min == 30) && (second <= 5) )
                return;

            if (h >= 15)
                return;

            if ( (h == 14) && (min >= 55) )
                return;


            string[] temp1 = tickinfo.Split('#');
            string[] temp2 = temp1[1].Split(';');
            string code = temp2[0];
            string lastprice = temp2[1];
            string buy1price = temp2[2];
            string buy1count = temp2[3];
            string sale1price = temp2[4];
            string sale1count = temp2[5];
            string ticktime = temp2[6];


            //先需要判断是否是当前页面的合约
            //510050是每个页面都需要的
            if ( (!tempMap3.ContainsKey(code)) && (code!="510050") )
                return;

            //排除集合竞价的干扰
            //double chazhi = Math.Abs(double.Parse(buy1price) - double.Parse(sale1price));
            //if (chazhi < 0.0000001)
            //    return;

            //缓存行情数据
            if (priceMap.ContainsKey(code))
            {
                priceMap[code].buy1Price = Convert.ToDouble(buy1price);
                priceMap[code].buy1Count = Convert.ToInt32(buy1count);
                priceMap[code].sale1Price = Convert.ToDouble(sale1price);
                priceMap[code].sale1Count = Convert.ToInt32(sale1count);
                priceMap[code].lastprice = Convert.ToDouble(lastprice);
            }
            else
            {
                OptTickData opt = new OptTickData();
                opt.sCode = code;
                opt.lastprice = Convert.ToDouble(lastprice);
                opt.buy1Price = Convert.ToDouble(buy1price);
                opt.buy1Count = Convert.ToInt32(buy1count);
                opt.sale1Price = Convert.ToDouble(sale1price);
                opt.sale1Count = Convert.ToInt32(sale1count);

                priceMap[code] = opt;

            }

            //更新界面
            if (code != "510050")
            {
                FlashLV(code, buy1count, sale1count, buy1price, sale1price);
            }
            else
            {
                //CalByEtf();
            }

            //更新盈亏
            CalPL();
                        
        }


        public void FlashLV(string code,string buy1count,string sale1count,string buy1price,string saleprice1)
        {
            if (!tempMap3.ContainsKey(code))
                return;

            //找到对应行数
            int row = tempMap3[code];

            double price = Convert.ToDouble(lvOpt.Items[row].SubItems[5].Text);

            string ccode = tempMap1[price].sCode;

            string pcode = tempMap2[price].sCode;

            //如果还没有行情就跳过
            if (!priceMap.ContainsKey(ccode) || !priceMap.ContainsKey(pcode) /*|| !priceMap.ContainsKey("510050")*/ )
            {
                return;
            }


            double etfsaleprice = priceMap["510050"].sale1Price;     //50etf  卖一价

            double etfbuyprice = priceMap["510050"].buy1Price;       //50etf  买一价

            double cprice = priceMap[ccode].buy1Price;               //认购   买一   

            double cprice1 = priceMap[ccode].sale1Price;

            double pprice = priceMap[pcode].sale1Price;              //认沽   卖一

            double pprice1 = priceMap[pcode].buy1Price;

            int bcount = priceMap[ccode].buy1Count;

            int bcount1 = priceMap[ccode].sale1Count;

            int pcount = priceMap[pcode].buy1Count;

            int pcount1 = priceMap[pcode].sale1Count;


            if ((etfsaleprice < 0.000001) || (etfbuyprice < 0.000001) || (cprice < 0.000001) || (pprice < 0.000001) || (cprice1 < 0.000001) || (pprice1 < 0.000001))
                return;

            if ((bcount == 0) || (bcount1 == 0) || (pcount == 0) || (pcount1 == 0))
                return;

            //融券价格 = 行权价  +  认购买一  -  认沽卖一  - 手续费
            double rong = (price / 1000) + cprice - pprice - 0.0006;

            //融资价格 = 行权价  +  认购卖一  -  认沽买一  - 手续费
            double rongzi = (price / 1000) + cprice1 - pprice1 + 0.0006;


            //排除集合竞价
            bool cal = true;
            if (Math.Abs(cprice - cprice1) < 0.000001 || Math.Abs(pprice - pprice1) < 0.000001)
                cal = false;

            //排除价格为0的合约
            if ((cprice > 0.00001) && (cprice1 > 0.00001) && (pprice > 0.00001) && (pprice1 > 0.00001) && cal)
            {
                //缓存行情数据
                if (RzRqMap.ContainsKey(price))
                {
                    RzRqMap[price].RZ = rongzi;
                    RzRqMap[price].RQ = rong;

                }
                else
                {
                    RZRQ rr = new RZRQ();
                    rr.RZ = rongzi;
                    rr.RQ = rong;
                    RzRqMap[price] = rr;
                }
            }
           
            Notice4();

            lvOpt.BeginUpdate();

            if (ccode == code)
            {
                lvOpt.Items[row].SubItems[0].Text = buy1price;
                lvOpt.Items[row].SubItems[1].Text = buy1count;
                lvOpt.Items[row].SubItems[2].Text = saleprice1;
                lvOpt.Items[row].SubItems[3].Text = sale1count;
            }
            else if (pcode == code)
            {
                lvOpt.Items[row].SubItems[7].Text = buy1price;
                lvOpt.Items[row].SubItems[8].Text = buy1count;
                lvOpt.Items[row].SubItems[9].Text = saleprice1;
                lvOpt.Items[row].SubItems[10].Text = sale1count;
            }

                      
            double diff = rong - etfsaleprice;

            double diff2 = etfbuyprice - (price / 1000) - cprice1 + pprice1 - 0.0006;

            string diffstr = Convert.ToDouble(diff).ToString("0.0000");

            string diffstr2 = Convert.ToDouble(diff2).ToString("0.0000");

            lvOpt.Items[row].SubItems[11].Text = Convert.ToString(rong);

            lvOpt.Items[row].SubItems[12].Text = Convert.ToString(rongzi); 

            lvOpt.Items[row].SubItems[13].Text = diffstr;

            lvOpt.Items[row].SubItems[14].Text = diffstr2;

            lvOpt.EndUpdate();

           
        }


        public void CalByEtf()
        {
            //CMediaAux::PlayWavSound();
            lvOpt.BeginUpdate();

            for (int i = 0; i < lvOpt.Items.Count;i++ )
            {
                string ccode = lvOpt.Items[i].SubItems[4].Text;
                string pcode = lvOpt.Items[i].SubItems[6].Text;
                double price = Convert.ToDouble(lvOpt.Items[i].SubItems[5].Text);

                //如果还没有行情就跳过
                if (!priceMap.ContainsKey(ccode) || !priceMap.ContainsKey(pcode))
                    continue ;


                double etfsaleprice = priceMap["510050"].sale1Price;     //50etf  卖一价

                double etfbuyprice = priceMap["510050"].buy1Price;       //50etf  买一价

                double cprice = priceMap[ccode].buy1Price;               //认购   买一   

                double cprice1 = priceMap[ccode].sale1Price;

                double pprice = priceMap[pcode].sale1Price;              //认沽   卖一

                double pprice1 = priceMap[pcode].buy1Price;

                int bcount = priceMap[ccode].buy1Count;

                int bcount1 = priceMap[ccode].sale1Count;

                int pcount = priceMap[pcode].buy1Count;

                int pcount1 = priceMap[pcode].sale1Count;


                if ((etfsaleprice < 0.000001) || (etfbuyprice < 0.000001) || (cprice < 0.000001) || (pprice < 0.000001) || (cprice1 < 0.000001) || (pprice1 < 0.000001))
                    return;

                if ((bcount == 0) || (bcount1 == 0) || (pcount == 0) || (pcount1 == 0))
                    return;

                //融券价格 = 行权价  +  认购买一  -  认沽卖一  - 手续费
                double rong = (price / 1000) + cprice - pprice - 0.0006;

                double diff = rong - etfsaleprice;

                //反过来
                double diff2 = etfbuyprice - (price / 1000) - cprice1 + pprice1 - 0.0006;

                string diffstr = Convert.ToDouble(diff).ToString("0.0000");

                string diffstr2 = Convert.ToDouble(diff2).ToString("0.0000");


                lvOpt.Items[i].SubItems[11].Text = Convert.ToString(rong);

                lvOpt.Items[i].SubItems[13].Text = diffstr;

                lvOpt.Items[i].SubItems[14].Text = diffstr2;

                bool temp = false;
                if (temp)
                {

                    //日志记录
                    string cname = tempMap1[price].sName;
                    string pname = tempMap2[price].sName;

                    string month = combOpt.SelectedItem.ToString() + "月 " + "执行价: " + Convert.ToString(price);

                    string code1 = cname + "    买一价: " + cprice.ToString("f4");

                    string code2 = pname + "    卖一价: " + pprice.ToString("f4");

                    string etf = "  融券价: " + rong.ToString("f4") + "     ETF50: " + etfsaleprice.ToString("f3") + "   差值1: " + diffstr + "  差值2: " + diffstr2;

                    string log = month + "  " + code1 + "   " + code2 + "   " + etf;


                }

            }

            lvOpt.EndUpdate();
     
        }


        public void playsound()
        {
            double now = (DateTime.Now - startTime).TotalMilliseconds;

            if (now - lastPlay < 1000) 
                return;

            SoundPlayer player = new SoundPlayer();
            player.SoundLocation = "msg.wav";
            player.Load();
            player.Play();

            lastPlay = (DateTime.Now - startTime).TotalMilliseconds;
        }
     
        //在整个list寻找
        public void Notice3()
        {
            string log = "";
            string log1 = "";

            double min = 9999.0;
            double max = -9999.0;
            double pMin = 0.0;
            double pMax = 0.0;

            foreach (ListViewItem item in lvOpt.Items)
            {

                string price = item.SubItems[5].Text;
                string code1 = item.SubItems[4].Text;
                string buy1 = item.SubItems[0].Text;
                string sale1 = item.SubItems[2].Text;

                string code2 = item.SubItems[6].Text;
                string buy2 = item.SubItems[7].Text;
                string sale2 = item.SubItems[9].Text;

                string rongquan = item.SubItems[11].Text;
                string rongzi = item.SubItems[12].Text;

                double tempMax = Convert.ToDouble(rongquan);
                if (tempMax > max)
                {
                    max = tempMax;
                    pMax = Convert.ToDouble(price);
                }

                double tempMin = Convert.ToDouble(rongzi);

                if (tempMin < min)
                {
                    min = tempMin;
                    pMin = Convert.ToDouble(price);
                }

                log = log + "执行价: " + price + "   融资: " + rongzi + "  融券: " + rongquan + "   " + DM.getName(code1) + "   买一: " + buy1 + "    卖一: " + sale1 + "   " + DM.getName(code2) + "   买一: " + buy2 + "    卖一: " + sale2 + "\n\r";

                    
            }

           if (max > min)
           {
               log1 = "融资执行价:  " + pMin + " " + min + "   融券执行价: " + pMax + " " + max + "   差值: " + (min-max);
               Log.WriteLog(log1);
               Log.WriteLog(log);
               Log.WriteLog("\n\r");
               Log.WriteLog("\n\r");

               //下单
               //OrderInsert(pMin,pMax);
                
           }

        }


        public void Notice4()
        {
            string log = "";
            string log1 = "";

            double min = 9999.0;
            double max = -9999.0;
            double pMin = 0.0;
            double pMax = 0.0;

            string temp = DateTime.Now.ToString("HH:mm:ss") + "     ";

            foreach(double price in RzRqMap.Keys)
            {
              
                string code1 = tempMap1[price].sCode;
                double buy1 = priceMap[code1].buy1Price;
                double sale1 = priceMap[code1].sale1Price;

                string code2 = tempMap2[price].sCode;
                double buy2 = priceMap[code2].buy1Price;
                double sale2 = priceMap[code2].sale1Price;

                //排除价格为0的干扰
                if (buy1 < 0.000001 || sale1 < 0.000001 || buy2 < 0.000001 || sale2 < 0.000001)
                    continue;

                //排除集合竞价
                if (Math.Abs(buy1 - sale1) < 0.000001 || Math.Abs(buy2 - sale2) < 0.000001)
                    continue;

                double rongquan = RzRqMap[price].RQ;
                double rongzi = RzRqMap[price].RZ;
               
                if (rongquan > max)
                {
                    max = rongquan;
                    pMax = price;
                }
             
                if (rongzi < min)
                {
                    min = rongzi;
                    pMin = price;
                }

                log = log + "执行价: " + price.ToString("f4") + "   融资: " + rongzi.ToString("f4") + "  融券: " + rongquan.ToString("f4") + "   " + DM.getName(code1) + "   买一: " + buy1.ToString("f4") + "    卖一: " + sale1.ToString("f4") + "   " + DM.getName(code2) + "   买一: " + buy2.ToString("f4") + "    卖一: " + sale2.ToString("f4") + "\n\r" + temp;
                //Log.WriteLog(log);
            }


            if ((max - min) > 0.00001)
            {

                //如果和上一次信息都一致就过滤掉
                if (max == preRERQ.rq && min == preRERQ.rz && pMax == preRERQ.price_rq && pMin == preRERQ.price_rz)
                    return;
                else
                {
                    preRERQ.rq = max;
                    preRERQ.rz = min;
                    preRERQ.price_rq = pMax;
                    preRERQ.price_rz = pMin;

                }

                //下单
                double diff =  Convert.ToDouble(tdiff.Text);
                if ((max - min) >= diff)
                {
                    if (chktest.Checked)
                    {
                        //测试
                        test();
                    }
                    else
                    {
                        OrderInsert(pMin, pMax);
                    }
                }
                log1 = "融资执行价:  " + pMin.ToString("f4") + "     " + min.ToString("f4") + "   融券执行价: " + pMax.ToString("f4") + "     " + max.ToString("f4") + "   差值: " + (max - min).ToString("f4");
                Log.WriteLog(log1);
                Log.WriteLog(log);
                Log.WriteLog("\n\r");
                Log.WriteLog("\n\r");
             
            }
        
        }


        public void Init()
        {
            string account = "";
            foreach (AccountInfo info in FormSet.listAccount)
            {
                account = info.account;
                if (account != "")
                    comboBox4.Items.Add(account);
            }


            preRERQ = new LastRZRQ();
            preRERQ.price_rq = 0;
            preRERQ.price_rz = 0;
            preRERQ.rq = 0;
            preRERQ.rz = 0;

        }


        public void OrderInsert(double rz,double rq)
        {
            string RZcode = tempMap1[rz].sCode;   //认购
            string RZcode1 = tempMap2[rz].sCode;  //认沽

            string RQcode = tempMap1[rq].sCode;
            string RQcode1 = tempMap2[rq].sCode;

            string jiacha = tjiacha.Text;
            if(jiacha=="")
                jiacha = "0.0015";

            //校验
            if (!CheckOrder(RZcode, RZcode1, RQcode, RQcode1))
                return;

            string account = comboBox4.Text;

            string orderinfo = "";

            //融资  买认购  卖认沽
            double RZBuy = priceMap[RZcode].sale1Price;
            //价格加或减0.0001 利于成交
            RZBuy = RZBuy + 0.0001;
            if (!chkNotOrder.Checked)
            {
                orderinfo = "grpxiadan" + ";" + RZcode + ";1" + ";0;0;" + RZBuy + ";1;0;" + jiacha + ";" + comboBox4.Text + ";" + grpId + ";";
                init(orderinfo);
            }
            string loginfo = "账户:   " + comboBox4.Text + "  合约: " + DM.getName(RZcode) + " 买开   " + "数量:   1" + " 价格: " + RZBuy;
            Log.WriteLog(loginfo);

            double RZSale = priceMap[RZcode1].buy1Price;
            //价格加或减0.0001 利于成交
            RZSale = RZSale - 0.0001;
            if (!chkNotOrder.Checked)
            {
                orderinfo = "grpxiadan" + ";" + RZcode1 + ";1" + ";1;0;" + RZSale + ";1;0;" + jiacha + ";" + comboBox4.Text + ";" + grpId + ";";
                init(orderinfo);
            }
            loginfo = "账户:   " + comboBox4.Text + "  合约: " + DM.getName(RZcode1) + " 卖开   " + "数量:   1" + " 价格: " + RZSale;
            Log.WriteLog(loginfo);

            //融券  卖认购  买认沽
            double RQSale = priceMap[RQcode].buy1Price;
            //价格加或减0.0001 利于成交
            RQSale = RQSale - 0.0001;
            if (!chkNotOrder.Checked)
            {
                orderinfo = "grpxiadan" + ";" + RQcode + ";1" + ";1;0;" + RQSale + ";1;0;" + jiacha + ";" + comboBox4.Text + ";" + grpId + ";";
                init(orderinfo);
            }
            loginfo = "账户:   " + comboBox4.Text + "  合约: " + DM.getName(RQcode) + " 卖开   " + "数量:   1" + " 价格: " + RQSale;
            Log.WriteLog(loginfo);

            double RQBuy = priceMap[RQcode1].sale1Price;
            //价格加或减0.0001 利于成交
            RQBuy = RQBuy + 0.0001;
            if (!chkNotOrder.Checked)
            {
                orderinfo = "grpxiadan" + ";" + RQcode1 + ";1" + ";0;0;" + RQBuy + ";1;0;" + jiacha + ";" + comboBox4.Text + ";" + grpId + ";";
                init(orderinfo);
            }
            loginfo = "账户:   " + comboBox4.Text + "  合约: " + DM.getName(RQcode1) + " 买开   " + "数量:   1" + " 价格: " + RQBuy;
            Log.WriteLog(loginfo);

            Log.WriteLog("\n\r");


            tradeTime++;
            grpId++;
            lastEntrust = (DateTime.Now - startTime).TotalMilliseconds;    //记录委托时间
            //init(orderinfo);

            //插入持仓
            if (!chkNotOrder.Checked)
            {
                InsertHold(RZcode, RZcode1, RQcode, RQcode1, RZBuy, RZSale, RQSale, RQBuy);
            }


        }


        public bool CheckOrder(string code1,string code2,string code3,string code4)
        {
            bool ret = true;

            if (comboBox4.Text == "")
                ret = false;

            string log = "";

            if (!chktest.Checked)
            {

                if (priceMap[code1].buy1Price == priceMap[code1].sale1Price)
                {
                    log = "熔断    " + code1 + "买一价: " + priceMap[code1].buy1Price + "   卖一价:    " + priceMap[code1].sale1Price;
                    Log.WriteLog(log);
                    ret = false;
                }

                if (priceMap[code2].buy1Price == priceMap[code2].sale1Price)
                {
                    log = "熔断    " + code2 + "买一价: " + priceMap[code2].buy1Price + "   卖一价:    " + priceMap[code2].sale1Price;
                    Log.WriteLog(log);
                    ret = false;
                }

                if (priceMap[code3].buy1Price == priceMap[code3].sale1Price)
                {
                    log = "熔断    " + code3 + "买一价: " + priceMap[code3].buy1Price + "   卖一价:    " + priceMap[code3].sale1Price;
                    Log.WriteLog(log);
                    ret = false;
                }

                if (priceMap[code4].buy1Price == priceMap[code4].sale1Price)
                {
                    log = "熔断    " + code4 + "买一价: " + priceMap[code4].buy1Price + "   卖一价:    " + priceMap[code4].sale1Price;
                    Log.WriteLog(log);
                    ret = false;
                }

            }

            int count = Convert.ToInt32(tCount.Text);

            if (tradeTime >= count)
            {
                string str = "已经交易次数:   " + tradeTime + "   交易上限:   " + count;
                Log.WriteLog(str);
                ret = false;
            }

            //防止短时间多次交易      
            double now = (DateTime.Now - startTime).TotalMilliseconds;
            if (now - lastEntrust < 1000)
            { 
                string str = "当前时间:  " + now + "上次交易时间: " + lastEntrust;
                Log.WriteLog(str);
                ret = false;
            }

            return ret;
        
        }


        public void InsertHold(string code1, string code2, string code3, string code4,double p1,double p2,double p3,double p4)
        {

            ListViewItem item = new ListViewItem();

            item.Text = "0";

            string time = DateTime.Now.ToString("HH:mm:ss");

            item.SubItems.Add(time);

            item.SubItems.Add(comboBox4.Text);

            item.SubItems.Add(code1);

            item.SubItems.Add(Convert.ToString(p1));

            item.SubItems.Add(code2);

            item.SubItems.Add(Convert.ToString(p2));

            item.SubItems.Add(code3);

            item.SubItems.Add(Convert.ToString(p3));

            item.SubItems.Add(code4);

            item.SubItems.Add(Convert.ToString(p4));

            item.SubItems.Add("0");

            item.SubItems.Add("0");

            item.SubItems.Add("999999");


            lsthold.Items.Add(item);


        }


        protected override int OnDispatchMessage(int pamam1, int param2, string param3, string param4)
        {
            this.Update(param3);
            return 0;
        }


        private void lsthold_DoubleClick(object sender, EventArgs e)
        {

            if (lsthold.SelectedItems.Count == 0)
                return;


            MessageBoxButtons messButton = MessageBoxButtons.OKCancel;

            DialogResult dr = MessageBox.Show("确定要平仓吗?", "对话框", messButton);

            if (dr != DialogResult.OK)
            {
                return;
            }


            string account = lsthold.SelectedItems[0].SubItems[2].Text;

            if (!chktest.Checked)
            {
                
                string RZcode = lsthold.SelectedItems[0].SubItems[3].Text;
                string RZcode1 = lsthold.SelectedItems[0].SubItems[5].Text;

                string RQcode = lsthold.SelectedItems[0].SubItems[7].Text;
                string RQcode1 = lsthold.SelectedItems[0].SubItems[9].Text;

                double RZP1 = priceMap[RZcode].buy1Price;
                double RZP2 = priceMap[RZcode1].sale1Price;
                double RQP1 = priceMap[RQcode].sale1Price;
                double RQP2 = priceMap[RQcode1].buy1Price;

                pingcang(RZcode, RZcode1, RQcode, RQcode1, RZP1, RZP2, RQP1, RQP2, account,0);

            }
            else
            {
                pingcang("11001031", "11001036", "11001061", "11001062", 0.001, 0.4039, 0.3148, 0.001, account,0);
            }


            int Index = 0;
            if (lsthold.SelectedItems.Count > 0)//判断listview有被选中项
            {
                Index = lsthold.SelectedItems[0].Index;//取当前选中项的index,SelectedItems[0]这必须为0

                lsthold.Items[Index].Remove();
            }


        }


        public void pingcang(string code1, string code2, string code3, string code4, double p1, double p2, double p3, double p4, string account,int type)
        {
            //平仓
            string orderinfo = "";
            string loginfo = "";
 

            //融资 卖平认购
            if (type == 0)
            {
                orderinfo = "order" + ";" + code1 + ";1" + ";1;1;" + p1 + ";1;0;0.0001;" + account + ";";
                init(orderinfo);
                loginfo = "账户:   " + account + "  合约: " + DM.getName(code1) + "  " + code1 +  " 卖平   " + "数量:   1" + " 价格: " + p1;
                Log.WriteLog(loginfo);
            }
            //融资 卖开认购
            else if (type == 1)
            {
                orderinfo = "order" + ";" + code1 + ";1" + ";1;0;" + p1 + ";1;0;0.0001;" + account + ";";
                init(orderinfo);
                loginfo = "账户:   " + account + "  合约: " + DM.getName(code1) + "  " + code1 + " 卖开   " + "数量:   1" + " 价格: " + p1;
                Log.WriteLog(loginfo);
            }

            //融资 买平认沽
            orderinfo = "order" + ";" + code2 + ";1" + ";0;1;" + p2 + ";1;0;0.0001;" + account + ";";
            init(orderinfo);
            loginfo = "账户:   " + account + "  合约: " + DM.getName(code2) + "  " + code2 + " 买平   " + "数量:   1" + " 价格: " + p2;
            Log.WriteLog(loginfo);

            //融券 买平认购
            orderinfo = "order" + ";" + code3 + ";1" + ";0;1;" + p3 + ";1;0;0.0001;" + account + ";";
            init(orderinfo);
            loginfo = "账户:   " + account + "  合约: " + DM.getName(code3) + "  " + code3 + " 买平   " + "数量:   1" + " 价格: " + p3;
            Log.WriteLog(loginfo);

            //融券 卖平认沽
            if (type == 0)
            {
                orderinfo = "order" + ";" + code4 + ";1" + ";1;1;" + p4 + ";1;0;0.0001;" + account + ";";
                init(orderinfo);
                loginfo = "账户:   " + account + "  合约: " + DM.getName(code4) + "  " + code4 +  " 卖平   " + "数量:   1" + " 价格: " + p4;
                Log.WriteLog(loginfo);
            }
            //融券 卖开认沽
            else if (type == 1)
            {
                orderinfo = "order" + ";" + code4 + ";1" + ";1;0;" + p4 + ";1;0;0.0001;" + account + ";";
                init(orderinfo);
                loginfo = "账户:   " + account + "  合约: " + DM.getName(code4) + "  " + code4 + " 卖开   " + "数量:   1" + " 价格: " + p4;
                Log.WriteLog(loginfo);
            }

            Log.WriteLog("\n\r");
        
        }


        public void CalPL()
        {
            foreach (ListViewItem item in lsthold.Items)
            {
                string RZcode1 = item.SubItems[3].Text;
                string RZcode2 = item.SubItems[5].Text;
                string RQcode1 = item.SubItems[7].Text; 
                string RQcode2 = item.SubItems[9].Text;

                if (!priceMap.ContainsKey(RZcode1) || !priceMap.ContainsKey(RZcode2) || !priceMap.ContainsKey(RQcode1) || !priceMap.ContainsKey(RQcode2))
                    return;

                double condition = Convert.ToDouble(item.SubItems[13].Text);
                string account = item.SubItems[2].Text;

                //成本价格
                double p1 = double.Parse(item.SubItems[4].Text);    //买
                double p2 = double.Parse(item.SubItems[6].Text);    //卖
                double p3 = double.Parse(item.SubItems[8].Text);    //卖
                double p4 = double.Parse(item.SubItems[10].Text);   //买

                //最新价盈亏
                double PL1_RZcode1 = priceMap[RZcode1].lastprice - p1;
                double PL1_RZcode2 = p2 - priceMap[RZcode2].lastprice;
                double PL1_RQcode1 = p3 - priceMap[RQcode1].lastprice;
                double PL1_RQcode2 = priceMap[RQcode2].lastprice - p4;

                double PL1 = PL1_RZcode1 + PL1_RZcode2 + PL1_RQcode1 + PL1_RQcode2;

                //对价盈亏
                double PL2_RZcode1 = priceMap[RZcode1].buy1Price - p1;
                double PL2_RZcode2 = p2 - priceMap[RZcode2].sale1Price;
                double PL2_RQcode1 = p3 - priceMap[RQcode1].sale1Price;
                double PL2_RQcode2 = priceMap[RQcode2].buy1Price - p4;

                double PL2 = PL2_RZcode1 + PL2_RZcode2 + PL2_RQcode1 + PL2_RQcode2;

                //如果对价盈亏满足条件就平仓
                if (PL2 > condition)
                {
                    pingcang(RZcode1, RZcode2, RQcode1, RQcode2, priceMap[RZcode1].buy1Price, priceMap[RZcode2].sale1Price, priceMap[RQcode1].sale1Price, priceMap[RQcode2].buy1Price, account,0);
                    lsthold.Items.Remove(item);
                    break;
                }

                //排队价盈亏
                double PL3_RZcode1 = priceMap[RZcode1].sale1Price - p1;
                double PL3_RZcode2 = p2 - priceMap[RZcode2].buy1Price;
                double PL3_RQcode1 = p3 - priceMap[RQcode1].buy1Price;
                double PL3_RQcode2 = priceMap[RQcode2].sale1Price - p4;

                double PL3 = PL3_RZcode1 + PL3_RZcode2 + PL3_RQcode1 + PL3_RQcode2;

                item.SubItems[11].Text = Convert.ToDouble(PL2).ToString("0.0000");
                item.SubItems[12].Text = Convert.ToDouble(PL3).ToString("0.0000") + "/" + Convert.ToDouble(PL1).ToString("0.0000");

            }
        
        }

        //测试下单程序
        public void test()
        {
            string RZcode = "11001031";  //认购
            string RZcode1 = "11001036";  //认沽

            string RQcode = "11001061";
            string RQcode1 = "11001062";

            double RZBuy = 0.3402;
            double RZSale = 0.0001;
            double RQSale = 0.0001;
            double RQBuy = 0.4914;

            if (!CheckOrder(RZcode, RZcode1, RQcode, RQcode1))
                return;


            if (!chkNotOrder.Checked)
            {
                string orderinfo = "";
                orderinfo = "order" + ";" + RZcode + ";1" + ";0;0;" + RZBuy + ";1;0;0.0001;" + comboBox4.Text + ";";
                init(orderinfo);

                orderinfo = "order" + ";" + RZcode1 + ";1" + ";1;0;" + RZSale + ";1;0;0.0001;" + comboBox4.Text + ";";
                init(orderinfo);

                orderinfo = "order" + ";" + RQcode + ";1" + ";1;0;" + RQSale + ";1;0;0.0001;" + comboBox4.Text + ";";
                init(orderinfo);

                orderinfo = "order" + ";" + RQcode1 + ";1" + ";0;0;" + RQBuy + ";1;0;0.0001;" + comboBox4.Text + ";";
                init(orderinfo);

                tradeTime++;

                InsertHold(RZcode, RZcode1, RQcode, RQcode1,0,0,0,0);

            }


        }


        private void SaveOrder(string month)
        {
            MakesureXmlExist();
            XmlDocument XmlDoc;
            try
            {
                XmlDoc = new XmlDocument();
                XmlDoc.Load(m_sFilePath);
            }
            catch (System.Exception ex)
            {
                //xml format is not correct, remake it
                Log.WriteLog("SaveOrder" + ex.ToString(), 1);
                File.Delete(m_sFilePath);
                MakesureXmlExist();

                XmlDoc = new XmlDocument();
                XmlDoc.Load(m_sFilePath);
            }

            
            XmlNode xmlContext = XmlDoc.SelectSingleNode("etf_bak");

            XmlNodeList xmlChildLst = xmlContext.ChildNodes;

            foreach (XmlNode oXmlNode in xmlChildLst)
            {
                string sId = oXmlNode.Attributes["month"].Value;
                if (string.Empty != sId && 0 == sId.CompareTo(month))
                {
                    xmlContext.RemoveChild(oXmlNode);
                    break;
                }
            }

            XmlElement xmlEtf = XmlDoc.CreateElement("etf_case");
            xmlEtf.SetAttribute("month", month);
            xmlContext.AppendChild(xmlEtf);

            //order_info
            XmlElement xmlOrderInfo = XmlDoc.CreateElement("order_info");
            xmlEtf.AppendChild(xmlOrderInfo);
            foreach (ListViewItem item in lsthold.Items)
            {
                XmlElement xmlOrder = XmlDoc.CreateElement("order");
                xmlOrder.SetAttribute("no", item.SubItems[0].Text);
                xmlOrder.SetAttribute("time", item.SubItems[1].Text);
                xmlOrder.SetAttribute("account", item.SubItems[2].Text);
                xmlOrder.SetAttribute("code1", item.SubItems[3].Text);
                xmlOrder.SetAttribute("price1", item.SubItems[4].Text);
                xmlOrder.SetAttribute("code2", item.SubItems[5].Text);
                xmlOrder.SetAttribute("price2", item.SubItems[6].Text);
                xmlOrder.SetAttribute("code3", item.SubItems[7].Text);
                xmlOrder.SetAttribute("price3", item.SubItems[8].Text);
                xmlOrder.SetAttribute("code4", item.SubItems[9].Text);
                xmlOrder.SetAttribute("price4", item.SubItems[10].Text);
                xmlOrder.SetAttribute("duijia", item.SubItems[11].Text);
                xmlOrder.SetAttribute("paidui", item.SubItems[12].Text);
                xmlOrder.SetAttribute("condition", item.SubItems[13].Text);
                xmlOrderInfo.AppendChild(xmlOrder);
            }

            XmlDoc.Save(m_sFilePath);

        }


        private void LoadOrder(string month)
        {
            MakesureXmlExist();
            try
            {
                XmlDocument XmlDoc = new XmlDocument();
                XmlDoc.Load(m_sFilePath);

                XmlNode xmlContext = XmlDoc.SelectSingleNode("etf_bak");
                XmlNodeList xmlChildLst = xmlContext.ChildNodes;

                foreach (XmlNode oXmlNode in xmlChildLst)
                {
                    string id = oXmlNode.Attributes["month"].Value;

                    if (id == month)
                    {
                        XmlNode orderinfo = oXmlNode.SelectSingleNode("order_info");

                        foreach (XmlNode order in orderinfo.ChildNodes)
                        {
                            /*
                            ListViewItem item = new ListViewItem(new string[]{
                                    order.Attributes["no"].Value,
                                    order.Attributes["time"].Value,
                                    order.Attributes["account"].Value,
                                    order.Attributes["code1"].Value,
                                    order.Attributes["price1"].Value,
                                    order.Attributes["code2"].Value,
                                    order.Attributes["price2"].Value,
                                    order.Attributes["code3"].Value,
                                    order.Attributes["price3"].Value,
                                    order.Attributes["code4"].Value,
                                    order.Attributes["price4"].Value,
                                    order.Attributes["duijia"].Value,
                                    order.Attributes["paidui"].Value,
                                    order.Attributes["condition"].Value

                                    });
                            */
                            //string[] temp = new string[lsthold.Columns.Count];
                            List<string> temp = new List<string>();
                            foreach (XmlAttribute att in order.Attributes)
                            {
                                temp.Add(att.Value);                   
                            }

                            //防止数据不全
                            for (int i = 0; i < (lsthold.Columns.Count - temp.Count); i++)
                            {
                                temp.Add("99999");
                            }
                            
                            ListViewItem item = new ListViewItem(temp.ToArray<string>());


                            lsthold.Items.Add(item);

                        }
                    }
                }


            }
            catch (System.Exception ex)
            {
                Log.WriteLog("LoadOrder" + ex.ToString(), 1);
            }
            
        
        }


        protected void MakesureXmlExist()
        {
            if (!File.Exists(m_sFilePath))
            {
                XmlDocument XmlDoc = new XmlDocument();
                if (null != XmlDoc)
                {
                    XmlDeclaration xmlDec = XmlDoc.CreateXmlDeclaration("1.0", "gb2312", null);
                    XmlDoc.AppendChild(xmlDec);

                    XmlElement xmlRoot = XmlDoc.CreateElement("etf_bak");
                    XmlDoc.AppendChild(xmlRoot);
                    XmlDoc.Save(m_sFilePath);
                }

            }
        }


        private void FormOption_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (combOpt.SelectedIndex >=0 )
            {
                SaveOrder(combOpt.SelectedItem.ToString());
            }
        }

        private void 对价全平ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (lsthold.SelectedItems.Count == 0)
                return;


            MessageBoxButtons messButton = MessageBoxButtons.OKCancel;

            DialogResult dr = MessageBox.Show("确定要以对价全部平仓吗?", "对话框", messButton);

            if (dr != DialogResult.OK)
            {
                return;
            }


            string account = lsthold.SelectedItems[0].SubItems[2].Text;

            if (!chktest.Checked)
            {

                string RZcode = lsthold.SelectedItems[0].SubItems[3].Text;
                string RZcode1 = lsthold.SelectedItems[0].SubItems[5].Text;

                string RQcode = lsthold.SelectedItems[0].SubItems[7].Text;
                string RQcode1 = lsthold.SelectedItems[0].SubItems[9].Text;

                double RZP1 = priceMap[RZcode].buy1Price;
                double RZP2 = priceMap[RZcode1].sale1Price;
                double RQP1 = priceMap[RQcode].sale1Price;
                double RQP2 = priceMap[RQcode1].buy1Price;

                pingcang(RZcode, RZcode1, RQcode, RQcode1, RZP1, RZP2, RQP1, RQP2, account,0);

            }

            int Index = 0;
            if (lsthold.SelectedItems.Count > 0)//判断listview有被选中项
            {
                Index = lsthold.SelectedItems[0].Index;//取当前选中项的index,SelectedItems[0]这必须为0

                lsthold.Items[Index].Remove();
            }

        }

        private void 对价开平ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (lsthold.SelectedItems.Count == 0)
                return;


            MessageBoxButtons messButton = MessageBoxButtons.OKCancel;

            DialogResult dr = MessageBox.Show("确定要平仓吗?", "对话框", messButton);

            if (dr != DialogResult.OK)
            {
                return;
            }


            string account = lsthold.SelectedItems[0].SubItems[2].Text;

            if (!chktest.Checked)
            {

                string RZcode = lsthold.SelectedItems[0].SubItems[3].Text;
                string RZcode1 = lsthold.SelectedItems[0].SubItems[5].Text;

                string RQcode = lsthold.SelectedItems[0].SubItems[7].Text;
                string RQcode1 = lsthold.SelectedItems[0].SubItems[9].Text;

                double RZP1 = priceMap[RZcode].buy1Price;
                double RZP2 = priceMap[RZcode1].sale1Price;
                double RQP1 = priceMap[RQcode].sale1Price;
                double RQP2 = priceMap[RQcode1].buy1Price;

                pingcang(RZcode, RZcode1, RQcode, RQcode1, RZP1, RZP2, RQP1, RQP2, account,1);

            }


            int Index = 0;
            if (lsthold.SelectedItems.Count > 0)//判断listview有被选中项
            {
                Index = lsthold.SelectedItems[0].Index;//取当前选中项的index,SelectedItems[0]这必须为0

                lsthold.Items[Index].Remove();
            }

        }

        private void 数据修改ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (lsthold.SelectedItems.Count == 0)
                return;

            string p1 = lsthold.SelectedItems[0].SubItems[4].Text;
            string p2 = lsthold.SelectedItems[0].SubItems[6].Text;
            string p3 = lsthold.SelectedItems[0].SubItems[8].Text;
            string p4 = lsthold.SelectedItems[0].SubItems[10].Text;

            string p5 = "";
            //int i = lsthold.SelectedItems[0].SubItems.Count; 
            if(lsthold.SelectedItems[0].SubItems[13] !=null)
                 p5 = lsthold.SelectedItems[0].SubItems[13].Text;

            FormOptModi modi = new FormOptModi(p1,p2,p3,p4,p5);

            if (DialogResult.OK == modi.ShowDialog())
            {
                lsthold.SelectedItems[0].SubItems[4].Text = FormOptModi.RZp1;
                lsthold.SelectedItems[0].SubItems[6].Text = FormOptModi.RZp2;
                lsthold.SelectedItems[0].SubItems[8].Text = FormOptModi.RQp1;
                lsthold.SelectedItems[0].SubItems[10].Text = FormOptModi.RQp2;
                lsthold.SelectedItems[0].SubItems[13].Text = FormOptModi.condition;
            }
            else
            { 
            
            }


        }


    }
}
