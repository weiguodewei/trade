using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace HSQuanTradeMain
{
    public partial class Login : Form
    {

        public delete_init InitLogin = null;

        public static List<string> loginOK = new List<string>();

        public Login()
        {
            InitializeComponent();
        }


        private void Login_Load(object sender, EventArgs e)
        {

            this.StartPosition = FormStartPosition.CenterParent;

            foreach (AccountInfo info in FormSet.listAccount)
            {
                //string login = "account#";

                //login = login + info.account + ";" + info.pwd + ";" + info.serverip + ";" + info.apitype + ";" + info.brokeid + ";#";

                //axHSQuanTrade1.Init(login);

                loginlist.Items.Add(info.account);

            }
        }

        private void cancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void OK_Click(object sender, EventArgs e)
        {
            string tt = "";
            for (int i = 0; i < loginlist.Items.Count; i++)
            {
                if (loginlist.GetItemChecked(i))
                {
                    tt = loginlist.Items[i].ToString();
                }

                if (tt == "")
                    continue;

                foreach (AccountInfo info in FormSet.listAccount)
                {
                    if ( (tt == info.account) && (!loginOK.Contains(tt)) )
                    {

                        string login = "account#";

                        login = login + info.account + ";" + info.pwd + ";" + info.serverip + ";" + info.apitype + ";" + info.brokeid + ";#";

                        InitLogin(login);

                        loginOK.Add(tt);

                        tt = "";
                 
                    }

                }
                
            }

            this.Close();

        }


    }
}
