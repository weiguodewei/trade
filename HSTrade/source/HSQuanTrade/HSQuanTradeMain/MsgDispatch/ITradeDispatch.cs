using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HSQuanTradeMain.MsgDispatch
{
    //these msg type should be as the same as those in ocx
    public enum TradeOcxMsgType
    {
        E_OCX_LOG_MSG = 0,
        E_OCX_PRV_LOG_MSG,
        E_OCX_OTHER_MSG,
        E_OCX_HQ_MSG,
        E_OCX_QUERY_MSG,
        E_OCX_TRADE_SIG_MSG,
        E_OCX_ARBITRAGE_HQ_MSG,
        E_OCX_TRADE_POSTION_MSG,
        E_OCX_CODE_QUERY_DONE_MSG,
        E_OCX_UNKNOWN_MSG,//should be last postion
        
    };
    public enum TradeOcxMsgAuthType
    {
        E_OCX_PUBLIC_MSG = 0,
        E_OCX_PRIVATE_MSG,
    };
    public delegate void Delegate_RegisterOcxMsgChannel(FormMsgChannelDesc oMsgInfo);
    public interface ITradeDispatch
    {
        void Init(Delegate_RegisterOcxMsgChannel del_Method);
        void GenerateGuid(string sFormName);

        string GetModuleId();

        int DispatchMessage(int pamam1, int param2, string param3, string param4);
    }

}
