using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;


namespace HSQuanTradeMain.ctrls
{
    public partial class TextBoxEx : TextBox
    {
        protected const int WM_CONTEXTMENU = 0x007b;//右键菜单消息
        protected const int WM_CHAR = 0x0102; //输入字符消息（键盘输入的，输入法输入的好像不是这个消息）
        protected const int WM_CUT = 0x0300; //程序发送此消息给一个编辑框或combobox来删除当前选择的文本
        protected const int WM_COPY = 0x0301; //程序发送此消息给一个编辑框或combobox来复制当前选择的文本到剪贴板
        protected const int WM_PASTE = 0x0302; //程序发送此消息给editcontrol或combobox从剪贴板中得到数据
        protected const int WM_CLEAR = 0x0303; //程序发送此消息给editcontrol或combobox清除当前选择的内容；
        protected const int WM_UNDO = 0x0304; //程序发送此消息给editcontrol或combobox撤消最后一次操作


        protected string m_sNumberString = "012345678";

        [Browsable(true), Category("Custom")]
        public string NumberFilter
        {
            get
            {
                return m_sNumberString;

            }

            set
            {
                this.m_sNumberString = value;
            }
        }


        public TextBoxEx()
        {
            InitializeComponent();
        }

        protected override void WndProc(ref Message m)
        {
            switch (m.Msg)
            {
                case WM_CHAR:
                    System.Console.WriteLine(m.WParam);
                    bool isSign = ((int)m.WParam == 45);
                    bool isNum = ((int)m.WParam >= 48) && ((int)m.WParam <= 57);
                    bool isBack = (int)m.WParam == (int)Keys.Back;
                    bool isDelete = (int)m.WParam == (int)Keys.Delete;//实际上这是一个"."键
                    bool isCtr = ((int)m.WParam == 24) || ((int)m.WParam == 22) || ((int)m.WParam == 26) || ((int)m.WParam == 3);

                    if (isNum || isBack || isCtr)
                    {
                        base.WndProc(ref m);
                    }
                    if (isSign)
                    {
                        if (this.SelectionStart != 0)
                        {
                            break;
                        }
                        base.WndProc(ref m);
                        break;
                    }
                    if (isDelete)
                    {
                        if (this.m_sNumberString.IndexOf(".") > 0)
                        {
                            if (this.Text.IndexOf(".") < 0)
                            {
                                base.WndProc(ref m);
                            }
                        }
                        
                    }
                    if ((int)m.WParam == 1)
                    {
                        this.SelectAll();
                    }
                    break;
                case WM_PASTE:
                    IDataObject iData = Clipboard.GetDataObject();//取剪贴板对象

                    if (iData.GetDataPresent(DataFormats.Text)) //判断是否是Text
                    {
                        string str = (string)iData.GetData(DataFormats.Text);//取数据
                        if (MatchNumber(str))
                        {
                            base.WndProc(ref m);
                            break;
                        }
                    }
                    m.Result = (IntPtr)0;//不可以粘贴
                    break;
                default:
                    base.WndProc(ref m);
                    break;
            }
        }

        private bool MatchNumber(string ClipboardText)
        {
            int index = 0;
            //string strNum = "-0.123456789";
            string strNum = m_sNumberString;

            index = ClipboardText.IndexOf(strNum[0]);
            if (index >= 0)
            {
                if (index > 0)
                {
                    return false;
                }
                index = this.SelectionStart;
                if (index > 0)
                {
                    return false;
                }
            }

            index = ClipboardText.IndexOf(strNum[2]);
            if (index != -1)
            {
                index = this.Text.IndexOf(strNum[2]);
                if (index != -1)
                {
                    return false;
                }
            }

            for (int i = 0; i < ClipboardText.Length; i++)
            {
                index = strNum.IndexOf(ClipboardText[i]);
                if (index < 0)
                {
                    return false;
                }
            }
            return true;
        }

    }
}
