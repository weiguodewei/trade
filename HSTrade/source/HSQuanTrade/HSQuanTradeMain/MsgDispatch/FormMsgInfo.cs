using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HSQuanTradeMain.MsgDispatch
{
    public class FormMsgChannelDesc
    {

        ITradeDispatch m_oTradeFormInt;
        public HSQuanTradeMain.MsgDispatch.ITradeDispatch TradeFormInt
        {
            get { return m_oTradeFormInt; }
            set { m_oTradeFormInt = value; }
        }

        TradeOcxMsgAuthType[] m_eMsgAuth;
        public TradeOcxMsgAuthType[] MsgAuth
        {
            get { return m_eMsgAuth; }
            set { m_eMsgAuth = value; }
        }

        public static void StandardFormat(ref FormMsgChannelDesc oDesc)
        {
            oDesc.MsgAuth = new TradeOcxMsgAuthType[(int)TradeOcxMsgType.E_OCX_UNKNOWN_MSG];
            oDesc.MsgAuth[(int)TradeOcxMsgType.E_OCX_PRV_LOG_MSG] = TradeOcxMsgAuthType.E_OCX_PRIVATE_MSG;
            oDesc.MsgAuth[(int)TradeOcxMsgType.E_OCX_HQ_MSG] = TradeOcxMsgAuthType.E_OCX_PUBLIC_MSG;
            oDesc.MsgAuth[(int)TradeOcxMsgType.E_OCX_QUERY_MSG] = TradeOcxMsgAuthType.E_OCX_PRIVATE_MSG;
            oDesc.MsgAuth[(int)TradeOcxMsgType.E_OCX_OTHER_MSG] = TradeOcxMsgAuthType.E_OCX_PRIVATE_MSG;
            oDesc.MsgAuth[(int)TradeOcxMsgType.E_OCX_TRADE_SIG_MSG] = TradeOcxMsgAuthType.E_OCX_PRIVATE_MSG;
            oDesc.MsgAuth[(int)TradeOcxMsgType.E_OCX_ARBITRAGE_HQ_MSG] = TradeOcxMsgAuthType.E_OCX_PRIVATE_MSG;
            oDesc.MsgAuth[(int)TradeOcxMsgType.E_OCX_TRADE_POSTION_MSG] = TradeOcxMsgAuthType.E_OCX_PRIVATE_MSG;
        }
        
    }
}
