using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace HSQuanTradeMain.MsgDispatch
{
    public partial class TradeFormBase : Form, ITradeDispatch
    {
        protected string m_sFormId;
        public Delegate_RegisterOcxMsgChannel delegate_RegisterOcxMsgChannel = null;

        protected virtual int OnDispatchMessage(int pamam1, int param2, string param3, string param4)
        {
            return 0;
        }
        public TradeFormBase()
        {
            InitializeComponent();
        }

        public void Init(Delegate_RegisterOcxMsgChannel del_Method)
        {
            delegate_RegisterOcxMsgChannel = del_Method;
            FormMsgChannelDesc oMsgInfo = new FormMsgChannelDesc();

            FormMsgChannelDesc.StandardFormat(ref oMsgInfo);

            oMsgInfo.TradeFormInt = this;
            if (null != delegate_RegisterOcxMsgChannel)
            {

                delegate_RegisterOcxMsgChannel(oMsgInfo);
            }
        }

        public void GenerateGuid(string sFormName)
        {
            m_sFormId = FormIdManager.GenerateGuid(sFormName);
        }

        public string GetModuleId()
        {
            return m_sFormId;
        }

        public int DispatchMessage(int pamam1, int param2, string param3, string param4)
        {
            return OnDispatchMessage(pamam1, param2, param3, param4);
        }
    }
}
