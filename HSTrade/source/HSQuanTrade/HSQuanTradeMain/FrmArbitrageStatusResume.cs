using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Globalization;

namespace HSQuanTradeMain
{
    public partial class FrmArbitrageStatusResume : Form
    {

        private int m_iStrategyStatus;
        public int StrategyStatus
        {
            get { return m_iStrategyStatus; }
            set { m_iStrategyStatus = value; }
        }

        private string m_strCode1;
        public string Code1
        {
            get { return m_strCode1; }
            set { m_strCode1 = value; }
        }
        private string m_strCode2;
        public string Code2
        {
            get { return m_strCode2; }
            set { m_strCode2 = value; }
        }

        private int m_iPostion1;
        public int Postion1
        {
            get { return m_iPostion1; }
            set { m_iPostion1 = value; }
        }
        private int m_iPostion2;
        public int Postion2
        {
            get { return m_iPostion2; }
            set { m_iPostion2 = value; }
        }

        private string m_strOpenTime1;
        public string OpenTime1
        {
            get { return m_strOpenTime1; }
            set { m_strOpenTime1 = value; }
        }
        private string m_strOpenTime2;
        public string OpenTime2
        {
            get { return m_strOpenTime2; }
            set { m_strOpenTime2 = value; }
        }

        private int m_iDir1;
        public int Dir1
        {
            get { return m_iDir1; }
            set { m_iDir1 = value; }
        }
        private int m_iDir2;
        public int Dir2
        {
            get { return m_iDir2; }
            set { m_iDir2 = value; }
        }
        private double m_dbOpenPrice1;
        public double OpenPrice1
        {
            get { return m_dbOpenPrice1; }
            set { m_dbOpenPrice1 = value; }
        }
        
        private double m_dbOpenPrice2;
        public double OpenPrice2
        {
            get { return m_dbOpenPrice2; }
            set { m_dbOpenPrice2 = value; }
        }
        


        public FrmArbitrageStatusResume()
        {
            InitializeComponent();
        }

        private bool CheckValues()
        {
            bool bErr = false;

           
           

            if (!this.rdoInit.Checked)
            {
                if (string.Empty == this.txtCode1.Text)
                {
                    MessageBox.Show("合约一为空");
                    txtCode1.Focus();
                    return false;
                }

                if (string.Empty == this.txtCode2.Text)
                {
                    MessageBox.Show("合约二为空");
                    txtCode2.Focus();
                    return false;
                }

                if (string.Empty == this.txtPrice1.Text)
                {
                    MessageBox.Show("合约一开仓价格为空");
                    return false;
                }

                if (string.Empty == this.txtPrice2.Text)
                {
                    MessageBox.Show("合约二开仓价格为空");
                    return false;
                }


                try
                {
                    m_dbOpenPrice1 = double.Parse(this.txtPrice1.Text);
                }
                catch (System.Exception ex)
                {
                    MessageBox.Show("合约一开仓价格不正确");
                    this.txtPrice1.Focus();
                    bErr = true;
                }
                if (bErr)
                {
                    return false;
                }
                try
                {
                    m_dbOpenPrice2 = double.Parse(this.txtPrice2.Text);
                }
                catch (System.Exception ex)
                {
                    MessageBox.Show("合约二开仓价格不正确");
                    this.txtPrice2.Focus();
                    bErr = true;
                }
                if (bErr)
                {
                    return false;
                }

                try
                {
                    m_iPostion1 = Int32.Parse(this.txtPos1.Text);
                }
                catch
                {
                    MessageBox.Show("合约一持仓量不正确");
                    this.txtPos1.Focus();
                    bErr = true;
                }
                if (bErr)
                {
                    return false;
                }

                try
                {
                    m_iPostion2 = Int32.Parse(this.txtPos2.Text);
                }
                catch
                {
                    MessageBox.Show("合约二持仓量不正确");
                    this.txtPos2.Focus();
                    bErr = true;
                }
                if (bErr)
                {
                    return false;
                }
            }




            return true;
            
        }

        private void btnOk_Click(object sender, EventArgs e)
        {

            if (!CheckValues())
            {
                return;
            }

            m_strCode1 = this.txtCode1.Text;
            m_strCode2 = this.txtCode2.Text;


            m_strOpenTime1 = this.dtOpen1.Value.ToString("yyyyMMdd HH:mm:ss");
            m_strOpenTime2 = this.dtOpen2.Value.ToString("yyyyMMdd HH:mm:ss");


            if (this.rdoInit.Checked)
            {
                m_iStrategyStatus = 0;
            }
            else if (this.rdoBK.Checked)
            {
                m_iStrategyStatus = 1;
            }
            else if (this.rdoSK.Checked)
            {
                m_iStrategyStatus = 2;
            }


           
            this.DialogResult = DialogResult.OK;
        }

        private void rdoBK_CheckedChanged(object sender, EventArgs e)
        {
            this.txtCode1.Enabled = !this.rdoInit.Checked;
            this.txtCode2.Enabled = !this.rdoInit.Checked;

            this.txtPos1.Enabled = !this.rdoInit.Checked;
            this.txtPos2.Enabled = !this.rdoInit.Checked;

            this.dtOpen1.Enabled = !this.rdoInit.Checked;
            this.dtOpen2.Enabled = !this.rdoInit.Checked;

            this.txtPrice1.Enabled = !this.rdoInit.Checked;
            this.txtPrice2.Enabled = !this.rdoInit.Checked;
        }

        private void rdoSK_CheckedChanged(object sender, EventArgs e)
        {
            this.txtCode1.Enabled = !this.rdoInit.Checked;
            this.txtCode2.Enabled = !this.rdoInit.Checked;

            this.txtPos1.Enabled = !this.rdoInit.Checked;
            this.txtPos2.Enabled = !this.rdoInit.Checked;

            this.dtOpen1.Enabled = !this.rdoInit.Checked;
            this.dtOpen2.Enabled = !this.rdoInit.Checked;

            this.txtPrice1.Enabled = !this.rdoInit.Checked;
            this.txtPrice2.Enabled = !this.rdoInit.Checked;
        }

        private void rdoInit_CheckedChanged(object sender, EventArgs e)
        {
            this.txtCode1.Enabled = !this.rdoInit.Checked;
            this.txtCode2.Enabled = !this.rdoInit.Checked;

            this.txtPos1.Enabled = !this.rdoInit.Checked;
            this.txtPos2.Enabled = !this.rdoInit.Checked;

            this.dtOpen1.Enabled = !this.rdoInit.Checked;
            this.dtOpen2.Enabled = !this.rdoInit.Checked;

            this.txtPrice1.Enabled = !this.rdoInit.Checked;
            this.txtPrice2.Enabled = !this.rdoInit.Checked;
        }

        private void FrmArbitrageStatusResume_Load(object sender, EventArgs e)
        {
            txtCode1.Text = m_strCode1;
            txtCode2.Text = m_strCode2;

            if (0 == m_iStrategyStatus)
            {
                this.rdoInit.Checked = true;
            }
            else if (1 == m_iStrategyStatus)
            {
                this.rdoBK.Checked = true;
                
                this.txtPos1.Text = m_iPostion1.ToString();
                this.txtPos2.Text = m_iPostion2.ToString();

                this.txtPrice1.Text = this.OpenPrice1.ToString();
                this.txtPrice2.Text = this.OpenPrice2.ToString();

                this.dtOpen1.Value = DateTime.ParseExact(OpenTime1, "yyyyMMdd HH:mm:ss", System.Globalization.CultureInfo.CurrentCulture);
                this.dtOpen2.Value = DateTime.ParseExact(OpenTime2, "yyyyMMdd HH:mm:ss", System.Globalization.CultureInfo.CurrentCulture);
            }
            else if (2 == m_iStrategyStatus)
            {
                this.rdoSK.Checked = true;
                this.txtPos1.Text = m_iPostion1.ToString();
                this.txtPos2.Text = m_iPostion2.ToString();

                this.txtPrice1.Text = this.OpenPrice1.ToString();
                this.txtPrice2.Text = this.OpenPrice2.ToString();

                this.dtOpen1.Value = DateTime.ParseExact(OpenTime1, "yyyyMMdd HH:mm:ss", System.Globalization.CultureInfo.CurrentCulture);
                this.dtOpen2.Value = DateTime.ParseExact(OpenTime2, "yyyyMMdd HH:mm:ss", System.Globalization.CultureInfo.CurrentCulture);
                
            }

            
        }
    }
}
