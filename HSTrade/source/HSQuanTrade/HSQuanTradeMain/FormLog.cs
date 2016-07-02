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
    public partial class FormLog : Form
    {
        public FormLog()
        {
            InitializeComponent();
        }

        public void addlog(string log)
        {
            DateTime tm = DateTime.Now;

            string time = tm.Date.ToShortDateString() + " " + tm.Hour.ToString() + ":" + tm.Minute.ToString() + ":" + tm.Second.ToString();

            ListViewItem item = listView1.Items.Add(time);

            item.SubItems.Add(log);
        }

        private void FormLog_FormClosing(object sender, FormClosingEventArgs e)
        {
            //MessageBoxButtons messButton = MessageBoxButtons.OKCancel;

            //DialogResult dr = MessageBox.Show("确定要关闭日志窗口?", "对话框", messButton);

            //if (dr != DialogResult.OK)
            //{
            //    e.Cancel = true;    //事件悔改
            //}
            //else
            //{
            //    e.Cancel = false;    //事件悔改
            //}

        }
    }
}
