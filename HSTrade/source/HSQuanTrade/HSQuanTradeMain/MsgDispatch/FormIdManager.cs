using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections;

namespace HSQuanTradeMain.MsgDispatch
{
    public class FormIdManager
    {
        static Hashtable s_hsFormId = new Hashtable();
        public static string GenerateGuid(string sFormName)
        {
            string sFormId;
            int iAcc;
            if (s_hsFormId.ContainsKey(sFormName))
            {
                iAcc = (int)s_hsFormId[sFormName];
                iAcc++;
                s_hsFormId[sFormName] = iAcc;
                
            }
            else
            {
                s_hsFormId[sFormName] = 0;
                iAcc = 0;
            }

            sFormId = string.Format("{0}::{1}", sFormName, iAcc);
            return sFormId;
        }

   
    }
}
