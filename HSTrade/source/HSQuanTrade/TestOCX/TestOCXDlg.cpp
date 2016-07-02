
// TestOCXDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestOCX.h"
#include "TestOCXDlg.h"
#include "afxdialogex.h"
#include<string>
#include <Windows.h>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTestOCXDlg 对话框



CTestOCXDlg::CTestOCXDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestOCXDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestOCXDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HSQUANTRADECTRL1, m_trade);
	DDX_Control(pDX, IDC_LIST1, m_list);
}

BEGIN_MESSAGE_MAP(CTestOCXDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CTestOCXDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CTestOCXDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTestOCXDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDCANCEL, &CTestOCXDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON3, &CTestOCXDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CTestOCXDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CTestOCXDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CTestOCXDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CTestOCXDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CTestOCXDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CTestOCXDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CTestOCXDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CTestOCXDlg::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &CTestOCXDlg::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON13, &CTestOCXDlg::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON15, &CTestOCXDlg::OnBnClickedButton15)
	ON_BN_CLICKED(IDC_BUTTON14, &CTestOCXDlg::OnBnClickedButton14)
END_MESSAGE_MAP()


// CTestOCXDlg 消息处理程序

BOOL CTestOCXDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_list.InsertColumn(0,"msg",0,600);


	GetDlgItem(IDC_EDIT1)->SetWindowTextA("IF1512");
	GetDlgItem(IDC_EDIT2)->SetWindowTextA("3660");
	GetDlgItem(IDC_EDIT3)->SetWindowTextA("1");
	GetDlgItem(IDC_EDIT4)->SetWindowTextA("1");
	GetDlgItem(IDC_EDIT5)->SetWindowTextA("1");
	GetDlgItem(IDC_EDIT6)->SetWindowTextA("101300595");

	GetDlgItem(IDC_EDIT9)->SetWindowTextA("1");
	GetDlgItem(IDC_EDIT7)->SetWindowTextA("5");
	GetDlgItem(IDC_EDIT8)->SetWindowTextA("5");


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTestOCXDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTestOCXDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTestOCXDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTestOCXDlg::OnBnClickedOk()
{
	

	//m_trade.Init("account#101300936;888888;tcp://180.166.103.34:41205;UFX;20000;#");
	//m_trade.Init("account#101300595;111111;tcp://180.166.103.34:41205;UFX;20000;#");
	//m_trade.Init("account#10000013;111111;60.191.25.162:4445;UFX_S;44;#");

	m_trade.Init("account#041249;032179WC;tcp://180.168.146.187:10000;CTP;9999;#");

	
}
BEGIN_EVENTSINK_MAP(CTestOCXDlg, CDialogEx)
	ON_EVENT(CTestOCXDlg, IDC_HSQUANTRADECTRL1, 1, CTestOCXDlg::OnTradenotifyHsquantradectrl1, VTS_I4 VTS_I4 VTS_BSTR VTS_BSTR)
	ON_EVENT(CTestOCXDlg, IDC_TESTOCXCTRL3, DISPID_DBLCLICK, CTestOCXDlg::DblClickTestocxctrl3, VTS_NONE)
END_EVENTSINK_MAP()


void CTestOCXDlg::OnTradenotifyHsquantradectrl1(long pamam1, long param2, LPCTSTR param3, LPCTSTR param4)
{
	if( pamam1 == 0 )
	{
		char * pt = (char*)param3;

		m_list.InsertItem(0,pt);
	}




}


void CTestOCXDlg::OnBnClickedButton1()
{
	
	//m_trade.Init("subcode#600783;;;#");

	//m_trade.Init("subcode#IF1511;FUTURES;CTP;#");

	m_trade.Init("subcode#TF1512;FUTURES;CTP;#");
}


void CTestOCXDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_trade.Init("system#115.238.53.140:9006#tcp://180.168.212.228:412413#");
	
	//m_trade.Init("system###");

	//char exeFullPath[MAX_PATH];
	//string StrPath="";

	/*
	GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	StrPath = (string)exeFullPath;
	int pos = StrPath.find_last_of("\\",StrPath.length());
	string s = StrPath.substr(0,pos);	*/
}


void CTestOCXDlg::DblClickTestocxctrl3()
{
	// TODO: 在此处添加消息处理程序代码
}


void CTestOCXDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CString code,price,amount,sb,kp,account,type,time,jiacha;

	//"600570",100,'1','0',75,0,3,2,"10000013"


	GetDlgItemText(IDC_EDIT1,code);
	GetDlgItemText(IDC_EDIT2,price);
	GetDlgItemText(IDC_EDIT3,amount);
	GetDlgItemText(IDC_EDIT4,sb);
	GetDlgItemText(IDC_EDIT5,kp);
	GetDlgItemText(IDC_EDIT6,account);

	//978
	GetDlgItemText(IDC_EDIT9,type);
	GetDlgItemText(IDC_EDIT7,time);
	GetDlgItemText(IDC_EDIT8,jiacha);
	



	CString order;
	order.Format("order,%s,%s,%s,%s,%s,%s,%s,%s,%s,",code,amount,sb,kp,price,type,time,jiacha,account);	

	//order = "order,IF1511,1,1,1,3680,0,10,5,101300595,";

	//"600570",100,'1','0',75,0,3,2,"10000013"
	
	m_trade.Init(order);
	
}


void CTestOCXDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CTestOCXDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_trade.AddRunStrategy("涨停板股票卖出策略#IF1511;1;3720;0.05;0.01;0.001;0;3;101300595,;#");

	//m_trade.AddRunStrategy("网格交易策略#ru1605;10;10;10000;5;1;0;0;100;100;0;0;041249;#");

	m_trade.AddRunStrategy("网格交易策略#ru1605;10;10;10000;5;1;1;1;100;100;0;0;041249;#");

	//m_trade.RunStrategy("网格交易策略TF1512做多");

	//string s = m_trade.GetStrategyStatusInfo("网格交易策略TF1512做多");
}


void CTestOCXDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	CString s = m_trade.GetStrategyLog("涨停板股票卖出策略600570");
	//int a = 1 + 1;
	//for(int i=0;i<2;)
	//	a =a +1;
}


void CTestOCXDlg::OnBnClickedButton6()
{
	m_trade.GetStrategyLog("交易合约;交易数量;开仓价;止损率;回调率;#600570;10;40;0.01;0.02;#");
	// TODO: 在此添加控件通知处理程序代码
}


void CTestOCXDlg::OnBnClickedButton7()
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	CString str ;
	str.Format("%d",st.wYear);
	// TODO: 在此添加控件通知处理程序代码
}


void CTestOCXDlg::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	m_trade.GetParamValue("涨停板股票卖出策略600570","");
}


void CTestOCXDlg::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	m_trade.GetParamValue("涨停板股票卖出策略600570","超时时间");

	m_trade.GetParamValue("涨停板股票卖出策略600570","账户");
}


void CTestOCXDlg::OnBnClickedButton10()
{
	// TODO: 在此添加控件通知处理程序代码
	m_trade.Init("withdraw");
}


void CTestOCXDlg::OnBnClickedButton11()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_trade.Init("account#10000013;111111;192.168.78.151:19013;UFX_S;44;#");

	//simnow
	//m_trade.Init("account#041249;032179WC;tcp://180.168.146.187:10000;CTP;9999;#");

	//东证期货
	m_trade.Init("account#66609557;080022;tcp://180.166.103.34:41205;CTP;6666;#");
}


void CTestOCXDlg::OnBnClickedButton12()
{
	// TODO: 在此添加控件通知处理程序代码
	m_trade.Init("account#101300595;111111;115.238.53.140:9006;UFX_F;44;#");
	m_trade.Init("account#10000013;111111;192.168.78.151:19013;UFX_S;44;#");
}


void CTestOCXDlg::OnBnClickedButton13()
{
	// TODO: 在此添加控件通知处理程序代码
	//CString	order = "order,rb1601,1,0,0,1680,0,0,5,041249,";

	//CString	order = "order,TF1512,1,0,0,100,0,0,5,041249,";

	//东证期货
	CString	order = "order,TF1512,1,0,0,100,0,0,5,66609557,";

	//"600570",100,'1','0',75,0,3,2,"10000013"
	
	m_trade.Init(order);
}


void CTestOCXDlg::OnBnClickedButton15()
{
	// TODO: 在此添加控件通知处理程序代码
	double d = 37.69;
	float f;
	f = d;

}


void CTestOCXDlg::OnBnClickedButton14()
{
	// TODO: 在此添加控件通知处理程序代码
	m_trade.StopStrategy("");
}
