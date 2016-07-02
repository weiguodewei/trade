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
    public partial class testExcel : Form
    {
        public delete_init init = null;

        public string orderinfo = "-1";

        public testExcel()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string filename = "";

            dg1.DataSource = null;

            OpenFileDialog f1 = new OpenFileDialog();

            f1.Filter = "文本文件|*.xlsx";
            if (f1.ShowDialog() == DialogResult.OK)
            {
                filename = f1.FileName;
            }

            if (filename != "")
            {

                ExcelUtil ex = new ExcelUtil();

                DataTable dt = ex.GetTradeData(filename, filename);

                dg1.DataSource = dt.DefaultView;

            }



        }

        private void button2_Click(object sender, EventArgs e)
        {
            //init("order,600570,100,0,0,70,0,0,5,10000013,");
            string account = textBox1.Text;
            for (int i = 0; i < dg1.Rows.Count; i++)
            {
                DataGridViewRow row = dg1.Rows[i];

                if (row.Cells[0] != null && row.Cells[1].Value != null)
                {
                    string code = row.Cells[2].Value.ToString();
                    //code = "600570";
                    string price = row.Cells[5].Value.ToString();
                    string amount = row.Cells[6].Value.ToString();

                    string orderinfo = "order" + ";" + code + ";" + amount + ";" + "0;0;" + price + ";" + "0;0;5;" + account +";";

                    init(orderinfo);

                    //System.Threading.Thread.Sleep(2000);
                    timeDelay(2000);

                }
            }

            this.Close();

        }

        public static void timeDelay(int iInterval)
        {
            DateTime now = DateTime.Now;
            while (now.AddMilliseconds(iInterval) > DateTime.Now)
            {
                Application.DoEvents();
            }
            return;
        }

        private void button4_Click(object sender, EventArgs e)
        {
            orderinfo = "";

            string code, price, count,sub;

            for (int i = 0; i < dg1.Rows.Count-1;i++ )
            {
                DataGridViewRow row = dg1.Rows[i];

                code = row.Cells[2].Value.ToString();
                price = row.Cells[5].Value.ToString();
                count = row.Cells[6].Value.ToString();

                orderinfo = orderinfo + code + "," + count + "," + price + ",|";

                //订阅行情
                sub = "subcode#" + code + ";" + "STOCK;UFX;" + "#";
                init(sub);
            }

            //需要订阅回购131810的行情
            //sub = "subcode#" + "131810" + ";" + "STOCK;UFX;" + "#";
            //init(sub);

            this.Close();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            this.Close();
        }


    }
}
