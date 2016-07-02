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
    public partial class FormOptModi : Form
    {
        public static string RZp1 = "0";
        public static string RZp2 = "0";
        public static string RQp1 = "0";
        public static string RQp2 = "0";
        public static string condition = "0";

        public FormOptModi()
        {
            InitializeComponent();
        }

        public FormOptModi(string p1,string p2,string p3,string p4,string p5)
        {
            InitializeComponent();

            t1.Text = p1;
            t2.Text = p2;
            t3.Text = p3;
            t4.Text = p4;
            t5.Text = p5;

            RZp1 = p1;
            RZp2 = p2;
            RQp1 = p3;
            RQp2 = p4;
            condition = p5;
        }

        private void b2_Click(object sender, EventArgs e)
        {
            RZp1 = t1.Text;
            RZp2 = t2.Text;
            RQp1 = t3.Text;
            RQp2 = t4.Text;
            condition = t5.Text;

        }

    }
}
