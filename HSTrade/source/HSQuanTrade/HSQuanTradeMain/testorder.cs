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
    public partial class testorder : Form
    {
        int grpid = 0;

        public delete_init init = null;

        public testorder()
        {
            InitializeComponent();
        }

        private void button2_Click(object sender, EventArgs e)
        {

            string account, code, bs, kp, price, amount,type,time,jiacha;

            account = textBox1.Text;

            code = textBox2.Text;

            bs = textBox3.Text;

            kp = textBox4.Text;

            price = textBox5.Text;

            amount = textBox6.Text;

            type = textBox8.Text;

            time = textBox9.Text;

            jiacha = textBox10.Text;

            string orderinfo = "order" + ";" + code + ";" + amount + ";" + bs + ";" + kp + ";" + price + ";" + type + ";" + time + ";" +jiacha + ";" + account + ";";

            //init("order,600570,100,0,0,70,0,0,5,10000013,");

            string subinfo = "subcode#" + code + ";" + "FUTURES;UFX;" + "#";

            string subinfo1 = "subcode#" + code + ";" + "FUTURES;CTP;" + "#";

            init(subinfo);

            init(orderinfo);

            init(subinfo1);

            //this.Close();


        }

        private void button3_Click(object sender, EventArgs e)
        {

            string entrustno = textBox7.Text;
            string account = textBox1.Text;
            string code = textBox2.Text;

            string orderinfo = "withdraw" + ";" + account + ";" + entrustno + ";" + code + ";" ;

            init(orderinfo);

            //this.Close();

        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void button4_Click(object sender, EventArgs e)
        {

            string entrustno = textBox7.Text;
            string account = textBox1.Text;

            string orderinfo = "withdraw" + ";" + account + ";" + entrustno + ";";

            init(orderinfo);

            this.Close();

        }

        private void button5_Click(object sender, EventArgs e)
        {
            string orderinfo = "test";

            init(orderinfo);

            this.Close();

        }

        private void button6_Click(object sender, EventArgs e)
        {
            string account, code, bs, kp, price, amount, type, time, jiacha;

            account = textBox1.Text;

            code = textBox2.Text;

            bs = textBox3.Text;

            kp = textBox4.Text;

            price = textBox5.Text;

            amount = textBox6.Text;

            type = textBox8.Text;

            time = textBox9.Text;

            jiacha = textBox10.Text;

            string grpid = textBox7.Text;

            string orderinfo = "grpxiadan" + ";" + code + ";" + amount + ";" + bs + ";" + kp + ";" + price + ";" + type + ";" + time + ";" + jiacha + ";" + account + ";" + grpid + ";";

            //init("order,600570,100,0,0,70,0,0,5,10000013,");

            string subinfo = "subcode#" + code + ";" + "STOCK;UFX;" + "#";

            string subinfo1 = "subcode#" + code + ";" + "FUTURES;CTP;" + "#";

            init(subinfo);

            init(orderinfo);

            init(subinfo1);
        }

        private void button7_Click(object sender, EventArgs e)
        {
            string loginfo = "logout";

            init(loginfo);

            testExcel.timeDelay(5000);

            loginfo = "login";

            init(loginfo);
        }

        private void button8_Click(object sender, EventArgs e)
        {
            string loginfo = "login";

            init(loginfo);
        }

        private void button9_Click(object sender, EventArgs e)
        {
            string p1 = textBox1.Text;

            string p2 = textBox2.Text;

            string p3 = textBox3.Text;

            string p4 = textBox4.Text;


            string amount = textBox6.Text;

            string type = textBox8.Text;

            string time = textBox9.Text;

            string jiacha = textBox10.Text;

            //string grpid = textBox7.Text;

            string code1 = "rb1610";

            string code2 = "rb1701";

            string code3 = "ru1609";

            string code4 = "ru1701";


            string j1 = textBox11.Text;

            string j2 = textBox12.Text;

            string j3 = textBox13.Text;


            string orderinfo = "grpxiadan" + ";" + code1 + ";" + amount + ";" + "0" + ";" + "0" + ";" + p1 + ";" + type + ";" + time + ";" + jiacha + ";" + "041249" + ";" + grpid + ";";        

            string subinfo1 = "subcode#" + code1 + ";" + "FUTURES;CTP;" + "#";

            init(orderinfo);

            init(subinfo1);


            orderinfo = "grpxiadan" + ";" + code2 + ";" + amount + ";" + "0" + ";" + "0" + ";" + p2 + ";" + type + ";" + time + ";" + j1 + ";" + "041249" + ";" + grpid + ";";

            subinfo1 = "subcode#" + code2 + ";" + "FUTURES;CTP;" + "#";

            init(orderinfo);

            init(subinfo1);


            orderinfo = "grpxiadan" + ";" + code3 + ";" + amount + ";" + "0" + ";" + "0" + ";" + p3 + ";" + type + ";" + time + ";" + j2 + ";" + "041249" + ";" + grpid + ";";

            subinfo1 = "subcode#" + code3 + ";" + "FUTURES;CTP;" + "#";

            init(orderinfo);

            init(subinfo1);

            if (radioButton1.Checked == true)
                testExcel.timeDelay(2000);

            orderinfo = "grpxiadan" + ";" + code4 + ";" + amount + ";" + "0" + ";" + "0" + ";" + p4 + ";" + type + ";" + time + ";" + j3 + ";" + "041249" + ";" + grpid + ";";

            subinfo1 = "subcode#" + code4 + ";" + "FUTURES;CTP;" + "#";

            init(orderinfo);

            init(subinfo1);

            grpid++;
        }

    }
}
