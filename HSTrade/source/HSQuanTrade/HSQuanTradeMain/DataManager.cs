using HSQuanTradeMain.MsgDispatch;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace HSQuanTradeMain
{

    public class DataManager
    {
        public Delegate_RegisterOcxMsgChannel delegate_RegisterOcxMsgChannel = null;

        public  static List<OptData> listOpt = new List<OptData>();

        public void setOptlist(string optinfo)
        {
            string []info = optinfo.Split('#');

            listOpt.Clear();

            //foreach (string opt in info)
                for (int i = 0; i < info.Count(); i++ )
                {
                    string opt = info[i];
                    if (info[i] != "QUERYOPT")
                    {
                        string[] code = info[i].Split(';');

                        OptData optdata = new OptData();
                        optdata.sCode = code[0];
                        optdata.sName = code[1];

                        string[] str1;
                        string[] str2;

                        if (code[1].IndexOf('购') > 0)
                        {
                            str1 = code[1].Split('购');
                            str2 = str1[1].Split('月');
                            optdata.iType = 0;
                        }
                        else
                        {
                            str1 = code[1].Split('沽');
                            str2 = str1[1].Split('月');
                            optdata.iType = 1;
                        }

                        optdata.iMonth = Convert.ToInt32(str2[0]);
                        optdata.dPrice = Convert.ToDouble(str2[1]);

                        listOpt.Add(optdata);


                    }

                }
        }

        public List<OptData> getOptlist()
        {

            return null;
        }

        public bool Opt_IsEmpty()
        {

            if (listOpt.Count > 0)
                return false;
            else
                return true;
        }

        public string getName(string code)
        {
            string name = "";
            for (int i = 0; i < listOpt.Count(); i++)
            {
                if (code == listOpt[i].sCode)
                {
                    name = listOpt[i].sName;
                    break;
                }
            }

            return name;
        
        }
        

    }

    public class OptData
    {

        public string sCode;
        public string sName;
        public int iMonth;
        public double dPrice;
        public int iType;
    }

    public class OptTickData
    {
        public string sCode;
        public double lastprice;
        public double buy1Price;
        public int buy1Count;
        public double sale1Price;
        public int sale1Count;

       
    }


}
