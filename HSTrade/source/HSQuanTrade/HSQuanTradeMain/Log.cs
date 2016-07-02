using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace HSQuanTradeMain
{
    class Log
    {

        public static void WriteLog(string log,int type = 0)
        {
            if(type==0)
                WriteLog(log, "ETF");
            else
                WriteLog(log, "C#");
        
        }

        public static void WriteLog(string log, string sPrefix)
        {
            string dir = System.Windows.Forms.Application.StartupPath + "\\strlog";

            if (!Directory.Exists(dir))
            {
                Directory.CreateDirectory(dir);
            }


            string logFilePath = string.Format("{0}\\{1}_{2}.log", dir, sPrefix, DateTime.Now.ToString("yyyyMMdd")); 

            if (log != "\n\r")
                log = DateTime.Now.ToString("HH:mm:ss") + "     " + log + "\n\r";

            File.AppendAllText(logFilePath, log);
        }

    }
}
